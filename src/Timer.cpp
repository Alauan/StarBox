#include "../include/main.hpp"


Timer *Timer::instance = nullptr;

Timer *Timer::getInstance()
{
    if (!instance)
    {
        instance = new Timer();
    }
    return instance;
}

Timer::Timer()
{
    world_time_in_milliseconds = 0;
    time_warp = 1.0;
}

Uint64 Timer::getWorldTimeInMillisseconds()
{
    return world_time_in_milliseconds;
}

double Timer::getDtSeconds()
{
    return real_dt_seconds * time_warp;
}

double Timer::getRealDtSeconds()
{
    return real_dt_seconds;
}

double Timer::getTimeWarp()
{
    return time_warp;
}

void Timer::setTimeWarp(double time_warp)
{
    this->time_warp = time_warp;
}

int Timer::getFps()
{
    return (int)(1.0 / real_dt_seconds);
}

void Timer::update()
{
    static Uint64 time_since_update = UINT64_MAX;

    if(time_since_update == UINT64_MAX)
        real_dt_seconds = 0;
    else
        real_dt_seconds = (double)(SDL_GetPerformanceCounter() - time_since_update)/(double)SDL_GetPerformanceFrequency();

    time_since_update = SDL_GetPerformanceCounter();
}