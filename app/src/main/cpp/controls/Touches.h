#ifndef ILLEGAL_RACER_TOUCHES_H
#define ILLEGAL_RACER_TOUCHES_H

#include <utility>
#include <queue>
#include <vector>
#include <pthread.h>
#include <chrono>

//#define _touch_DEBUG

namespace touch
{
    struct Event
    {
        struct pressAction
        {
            float posX;
            float posY;
        };
        struct releaseAction
        {
            float posX;
            float posY;
            std::chrono::milliseconds elapsedSincePress;
        };
        struct moveAction
        {
            float primordialX;
            float primordialY;
            float deltaX;
            float deltaY;
        };
        struct zoomAction
        {
            float centerX;
            float centerY;
            float deltaDist;
        };

        enum EventType
        {
            Nope = 0, Press, Release, Move, Zoom
        } type = Nope;

        union
        {
            pressAction PressEvent;
            releaseAction ReleaseEvent;
            moveAction MoveEvent;
            zoomAction ZoomEvent;
        };
    };

    class Touches
    {
    public:
        constexpr static const char *const TAG = "Touches";

        Touches();
        ~Touches();

        void flush();
        void setThreshold(float value);
        void setHandlerFrequency(float hz);

        void registerActionDown(int id, float x, float y);
        void registerActionUp(int id, float x, float y);
        void registerActionMove(int id, float x, float y);

        bool pollEvent(Event& e);

        void start();
        void stop();

    private:
        float mThreshold;
        int mHandlerNanosDelay;

        struct Touch
        {
            int id;
            // Для того, чтобы клиент мог определить, к какой области применять перемещение
            std::pair<float, float> primordialPosition;
            std::pair<float, float> position;
            std::chrono::milliseconds startTime;
            bool moveFlag;
            Touch() :
                    id(0),
                    primordialPosition({0.f, 0.f}),
                    position({0.f, 0.f}),
                    startTime(0),
                    moveFlag(false)
            {}
            Touch(float x, float y) :
                    id(0),
                    primordialPosition({x, y}),
                    position({x, y}),
                    startTime(0),
                    moveFlag(false)
            {}
        };

        struct PostEvent
        {
            enum types
            {
                Nope = 0, Down, Up, Move
            } type = Nope;

            int touchId;
            std::pair<float, float> arg;

            PostEvent() : type(Nope) {}
            PostEvent(types type, int touchId) : type(type), touchId(touchId) {}
        };

        enum RenderThreadMessage {
            MSG_NOPE = 0,
            MSG_HANDLER_LOOP_EXIT
        };
        pthread_t mThreadId;
        pthread_mutex_t mMutex;
        RenderThreadMessage mMsg;
        void handlerLoop();
        // Вспомогательный метод для запуска потока обработки касаний
        static void* threadStartCallback(void *myself);

        std::queue<PostEvent> mPostEvents;
        std::queue<Event> mEvents;
        std::vector<Touch> mTouches;

        bool pollPostEvent(PostEvent& e);
    };
}

#endif
