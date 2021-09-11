#include "Touches.h"
#include "../LogHelper.h"

#include <unistd.h>
#include <chrono>

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
    std::queue<PostEvent> emptyPE;
    std::swap(mPostEvents, emptyPE);
    std::queue<Event> emptyE;
    std::swap(mEvents, emptyE);
    std::map<int, Touch> emptyT;
    std::swap(mTouches, emptyT);
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
        delay = delay > 0 ? delay : 1000000; // 1мс

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
