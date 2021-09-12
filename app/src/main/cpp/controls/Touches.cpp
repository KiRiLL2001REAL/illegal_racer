#include "Touches.h"
#include "../LogHelper.h"

#include <unistd.h>

touch::Touches::Touches() :
        mThreadId(0),
        mMutex(),
        mMsg(MSG_NOPE),
        mThreshold(10.f),
        mHandlerNanosDelay(0),
        mPostEvents(),
        mEvents(),
        mTouches()
{
    pthread_mutex_init(&mMutex, nullptr);
}

touch::Touches::~Touches()
{
    pthread_mutex_destroy(&mMutex);
    flush();
}

void touch::Touches::flush()
{
    pthread_mutex_lock(&mMutex);

    std::queue<PostEvent> emptyPE;
    std::swap(mPostEvents, emptyPE);
    std::queue<Event> emptyE;
    std::swap(mEvents, emptyE);
    std::vector<Touch> emptyT;
    std::swap(mTouches, emptyT);

    pthread_mutex_unlock(&mMutex);
}

void touch::Touches::setThreshold(float value)
{
    mThreshold = value;
}

void touch::Touches::setHandlerFrequency(float hz)
{
    mHandlerNanosDelay = int(1000000000.f / (float)hz);
}

bool touch::Touches::pollEvent(touch::Event &e)
{
    if (mEvents.empty())
        return false;
    e = mEvents.front();
    mEvents.pop();
    return true;
}

void touch::Touches::registerActionDown(int id, float x, float y)
{
    PostEvent e(PostEvent::Down, id);
    e.arg = {x, y};
    mPostEvents.push(e);
}

void touch::Touches::registerActionUp(int id, float x, float y)
{
    PostEvent e(PostEvent::Up, id);
    e.arg = {x, y};
    mPostEvents.push(e);
}

void touch::Touches::registerActionMove(int id, float x, float y)
{
    PostEvent e(PostEvent::Move, id);
    e.arg = {x, y};
    mPostEvents.push(e);
}

void touch::Touches::start()
{
    LOGI(TAG, "Creating touch handler thread");
    pthread_create(&mThreadId, nullptr, threadStartCallback, this);
}

void touch::Touches::stop()
{
    LOGI(TAG, "Stopping touch handler thread");
    pthread_mutex_lock(&mMutex);
    mMsg = MSG_HANDLER_LOOP_EXIT;
    pthread_mutex_unlock(&mMutex);

    pthread_join(mThreadId, nullptr);
    LOGI(TAG, "Touch handler thread stopped");
}

void touch::Touches::handlerLoop()
{
    LOGI(TAG, "Touch handler loop entry");
    bool handlingEnabled = true;
    timespec sleepTime{};

#ifdef _touch_DEBUG
    int handledTouches = 0;
    int hps = 0;
    auto hpsTimerStart = std::chrono::steady_clock::now();
#endif

    while (handlingEnabled)
    {
        auto timeStart = std::chrono::steady_clock::now();

        pthread_mutex_lock(&mMutex);

        // Обработка входящих сообщений
        switch (mMsg)
        {
            case MSG_HANDLER_LOOP_EXIT:
                handlingEnabled = false;
                break;
            default:
                break;
        }
        mMsg = MSG_NOPE;

        // Обработка отложенных событий
#ifdef _touch_DEBUG
        handledTouches += mPostEvents.size();
#endif
        PostEvent e;
        while (pollPostEvent(e))
        {
            using namespace std::chrono;

            switch (e.type)
            {
                case PostEvent::Down:
                {
                    auto it = mTouches.begin();
                    while (it != mTouches.end() && it->id != e.touchId) it++;
                    if (it != mTouches.end())
                        LOGW(TAG, "TouchDown with id %d is already exists", e.touchId);
                    Touch t(e.arg.first, e.arg.second);
                    t.startTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch());
                    if (it != mTouches.end())
                        *it = t;
                    else mTouches.push_back(t);
                    // Генерация события PressEvent
                    Event ev;
                    ev.type = Event::Press;
                    ev.PressEvent.posX = e.arg.first;
                    ev.PressEvent.posY = e.arg.second;
                    mEvents.push(ev);
                    // конец
                    break;
                }
                case PostEvent::Up:
                {
                    auto it = mTouches.begin();
                    while (it != mTouches.end() && it->id != e.touchId) it++;
                    if (it == mTouches.end())
                    {
                        LOGW(TAG, "TouchUp with id %d isn't exists. Skipping.", e.touchId);
                        break;
                    }
                    Touch t = *it;
                    // Генерация события ReleaseEvent
                    Event ev;
                    ev.type = Event::Press;
                    ev.ReleaseEvent.posX = e.arg.first;
                    ev.ReleaseEvent.posX = e.arg.second;
                    milliseconds ms = duration_cast<milliseconds>(steady_clock::now().time_since_epoch());
                    ev.ReleaseEvent.elapsedSincePress = ms - t.startTime;
                    mEvents.push(ev);
                    // конец

                    mTouches.erase(it);
                    break;
                }
                case PostEvent::Move:
                {
                    auto it = mTouches.begin();
                    while (it != mTouches.end() && it->id != e.touchId) it++;
                    /* Проверка также предусматривает выход из обработки при отсутствии зарегис-
                     * трированных касаний
                     */
                    if (it == mTouches.end())
                    {
                        LOGW(TAG, "TouchUp with id %d isn't exists. Skipping.", e.touchId);
                        break;
                    }
                    /*
                     * Случаи: (прежде всего, до обработки касание должно преодолеть пороговое
                     * смещение)
                     * 1) Зарегистрировано 1 касание:
                     *      1.1) Генерируется событие MoveEvent.
                     *      1.2) Положение касания изменяется на то, что записано во внешнем
                     *      событии.
                     *      1.3) Взводится флаг движения (касание преодолело пороговое смещение)
                     * 2) Зарегистрировано 2 и более касаний:
                     *      2.1) Внешнее событие пришло от первого или последнего зарегистриро-
                     *      ванного касания:
                     *          2.1.1) Генерируется событие ZoomEvent с центром приближения в
                     *          противоположном касании.
                     *          2.1.2) Положение касания изменяется на то, что записано во внеш-
                     *          нем событии.
                     *      2.2) Внешнее событие пришло не от первого и не от последнего зареги-
                     *      стрированного касания:
                     *          2.2.1) Выполняем пункт 1
                     */
                    float deltaX = e.arg.first - it->primordialPosition.first;
                    float deltaY = e.arg.second - it->primordialPosition.second;
                    if (abs(deltaX) >= mThreshold && abs(deltaY) >= mThreshold)
                    {
                        if (mTouches.size() < 2 &&
                            (it->id != mTouches.front().id || it->id != mTouches.back().id))
                        {
                            // Генерация события MoveEvent
                            Event ev;
                            ev.type = Event::Move;
                            ev.MoveEvent.primordialX = it->primordialPosition.first;
                            ev.MoveEvent.primordialY = it->primordialPosition.second;
                            ev.MoveEvent.deltaX = deltaX;
                            ev.MoveEvent.deltaY = deltaY;
                            mEvents.push(ev);
                            it->position = e.arg;
                            it->moveFlag = true;
                            // конец
                        }
                        else if (it->moveFlag)
                        {
                            // Генерация события ZoomEvent
                            Event ev;
                            ev.type = Event::Zoom;
                            if (it->id == mTouches.front().id)
                            {
                                ev.ZoomEvent.centerX = mTouches.back().position.first;
                                ev.ZoomEvent.centerY = mTouches.back().position.second;
                            }
                            else
                            {
                                ev.ZoomEvent.centerX = mTouches.front().position.first;
                                ev.ZoomEvent.centerY = mTouches.front().position.second;
                            }
                            float distanceOld =
                                    sqrt(
                                            pow(mTouches.front().position.first -
                                                mTouches.back().position.first,
                                                2.f) +
                                            pow(mTouches.front().position.second -
                                                mTouches.back().position.second,
                                                2.f));
                            deltaX = e.arg.first - it->position.first;
                            deltaX = e.arg.second - it->position.second;
                            it->position.first += deltaX;
                            it->position.second += deltaX;
                            float distanceCurrent =
                                    sqrt(
                                            pow(mTouches.front().position.first -
                                                mTouches.back().position.first,
                                                2.f) +
                                            pow(mTouches.front().position.second -
                                                mTouches.back().position.second,
                                                2.f));
                            ev.ZoomEvent.deltaDist = distanceCurrent - distanceOld;
                            mEvents.push(ev);
                            // конец
                        }
                    }
                    break;
                }
                case PostEvent::Nope:
                default:
                    break;
            }
        }

        pthread_mutex_unlock(&mMutex);

        auto timeEnd = std::chrono::steady_clock::now();
        auto elapsedNanos =
                std::chrono::duration_cast<std::chrono::nanoseconds>(timeEnd - timeStart);

#ifdef _touch_DEBUG
        hps++;
        auto hpsTimerCurrent = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(hpsTimerCurrent - hpsTimerStart)
                    .count() >= 1000)
        {
            LOGI(TAG, "\nHandler looped %d times per second "
                      "and handled %d touch events", hps, handledTouches);
            handledTouches = 0;
            hps = 0;
            hpsTimerStart = std::chrono::steady_clock::now();
        }
#endif

        // Приостанавливаем поток на некоторое время
        long delay = long(mHandlerNanosDelay - elapsedNanos.count());
        delay = delay > 0 ? delay : 10000000; // 1мс

        sleepTime.tv_sec = 0;
        sleepTime.tv_nsec = delay;
        nanosleep(&sleepTime, nullptr);
    }

    LOGI(TAG, "Touch handler loop exits");
}

void *touch::Touches::threadStartCallback(void *myself)
{
    auto *handler = (touch::Touches *)myself;
    handler->handlerLoop();
    pthread_exit(nullptr);
}

bool touch::Touches::pollPostEvent(touch::Touches::PostEvent &e)
{
    if (mPostEvents.empty())
        return false;
    e = mPostEvents.front();
    mPostEvents.pop();
    return true;
}
