#ifndef ILLEGAL_RACER_TOUCHES_H
#define ILLEGAL_RACER_TOUCHES_H

#include <utility>
#include <queue>
#include <map>
#include <pthread.h>

//#define _touch_DEBUG

namespace touch
{
    struct Event
    {
    private:
        struct clickAction
        {
            std::pair<float, float> position = {0.f, 0.f};
        };
        struct moveAction
        {
            std::pair<float, float> startPosition = {0.f, 0.f};
            std::pair<float, float> deltaDist = {0.f, 0.f};
        };
        struct zoomAction
        {
            std::pair<float, float> firstPointer = {0.f, 0.f};
            std::pair<float, float> secondPointer = {0.f, 0.f};
            float dDist = 0.f;
        };

    public:
        enum types
        {
            Nope = 0, Click, Move, Zoom
        } Type = Nope;

        union action
        {
            clickAction Click;
            moveAction Move;
            zoomAction Zoom;
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
            std::pair<float, float> primordialPosition;
            std::pair<float, float> position;
            Touch() : primordialPosition({0.f, 0.f}), position({0.f, 0.f}) {}
            Touch(float x, float y) : primordialPosition({x, y}), position({x, y}) {}
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
        std::map<int, Touch> mTouches;

        bool pollPostEvent(PostEvent& e);
    };
}

#endif
