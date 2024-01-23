#pragma once
#include "main.hpp"


class Creator
{
public:
    virtual void render() = 0;
    virtual void update() = 0;

    std::string icon_path; 
};


class PlanetCreator : public Creator
{
public:
    PlanetCreator(Camera *camera, PlanetManager *planet_manager);
    void update();
    void render();
private:
    enum State
    {
        JUST_STARTED,
        GROWING,
        AIMING,
        JUST_CREATED,
        IDLE
    };

    State state;
    Planet current_planet;
    InputHandler *input_handler;
    Timer *timer;
    Camera *camera;
    Painter *painter;
    PlanetManager *planet_manager;
    double growth_speed;
};


class GalaxyCreator : public Creator
{
public:
    GalaxyCreator(Camera *camera, PlanetManager *planet_manager);
    void update();
    void render();
private:
    void reset();
    enum State
    {
        JUST_STARTED,
        GROWING,
        AIMING,
        JUST_CREATED,
        IDLE
    };
    std::unordered_set<Planet *> current_galaxy;
    Planet *galaxy_center;
    State state;
    int add_new_star_speed;
    double current_radius;
    double radius_growth_speed;

    InputHandler *input_handler;
    Timer *timer;
    Camera *camera;
    Painter *painter;
    PlanetManager *planet_manager;
};


class CosmicDustCreator : public Creator
{
public:
    CosmicDustCreator(Camera *camera, PlanetManager *planet_manager);
    void update();
    void render();

private:
    enum State
    {
        JUST_CREATED,
        IDLE
    };

    State state;
    InputHandler *input_handler;
    Painter *painter;
    PlanetManager *planet_manager;
    Camera *camera;
};


class Eraser : public Creator
{
public:
    Eraser(Camera* camera, PlanetManager *planet_manager);
    void update();
    void render();

private:
    enum State
    {
        IDLE,
        ERASING
    };
    Painter *painter;
    PlanetManager *planet_manager;
    InputHandler *input_handler;
    Camera *camera;
    State state;
    double radius;
};


