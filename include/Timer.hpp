#pragma once
#include "main.hpp"


class Timer
{
public:
    Timer(Timer const&) = delete;
    void operator=(Timer const&) = delete;

    static Timer *getInstance();

    Uint64 getWorldTimeInMillisseconds();
    double getDtSeconds();
    double getRealDtSeconds();

    double getTimeWarp();
    void setTimeWarp(double time_warp);

    int getFps();

    void update();
private:
    Timer();
    static Timer *instance;

    Uint64 world_time_in_milliseconds;
    double real_dt_seconds;
    double time_warp;
};