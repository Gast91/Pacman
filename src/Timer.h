#pragma once
#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

class Timer
{
private:
    TimePoint startTime;
    TimePoint endTime;

    bool running = false;
    bool paused = false;
public:
    bool isPaused()  const { return paused; }
    bool isRunning() const { return running; }

    void startTimer()
    {
        startTime = std::chrono::steady_clock::now();
        running = true;
    }

    void stopTimer()
    {
        endTime = std::chrono::steady_clock::now();
        running = false;
    }

    void pauseTimer()
    {
        endTime = std::chrono::steady_clock::now();
        paused = true;
        running = false;
    }

    void resumeTimer()
    {
        paused = false;
        running = true;
    }

    auto msEllapsed() const // if not initialized?
    {
        TimePoint currentTime = running ? std::chrono::steady_clock::now() : endTime;
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
    }
};