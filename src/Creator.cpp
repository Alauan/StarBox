#include "../include/main.hpp"


PlanetCreator::PlanetCreator(Camera *camera, PlanetManager *planet_manager)
    : camera(camera), planet_manager(planet_manager)
{
    icon_path = "assets/star.png";
    state = IDLE;
    current_planet = Planet();
    input_handler = InputHandler::getInstance();
    timer = Timer::getInstance();
    painter = Painter::getInstance();
    growth_speed = 30;
}

void PlanetCreator::update()
{
    std::random_device rd;
    SDL_Point pos_diff;
    switch (state)
    {
    case JUST_STARTED:
        current_planet.setPosition(camera->screenToWorld(input_handler->getMousePosition()));
        timer->setTimeWarp(0.1);
        
        state = GROWING;
        break;

    case GROWING:
        current_planet.setRadius(current_planet.getRadius() + growth_speed * timer->getRealDtSeconds() * camera->getZoom());

        pos_diff = input_handler->getMousePositionDiff();
        if (pos_diff.x != 0 || pos_diff.y != 0)
            state = AIMING;
            
        if(input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_RELEASED))
            state = JUST_CREATED;
        break;

    case AIMING:
        current_planet.setVelocity(current_planet.getPosition() - camera->screenToWorld(input_handler->getMousePosition()));

        if (input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_RELEASED))
            state = JUST_CREATED;
        break;

    case JUST_CREATED:
        timer->setTimeWarp(1);
        planet_manager->addPlanet(new Planet(current_planet));

        state = IDLE;
        break;

    case IDLE:
        if (input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_PRESSED))
        {
            current_planet = Planet();
            state = JUST_STARTED;
        }
        break;

    default:
        break;
    }
}

void PlanetCreator::render()
{
    switch (state)
    {
    case JUST_STARTED:
        painter->drawPlanet(&current_planet);
        break;

    case GROWING:
        painter->drawPlanet(&current_planet);
        break;

    case AIMING:
        painter->drawPlanet(&current_planet);
        painter->drawLine(camera->worldToScreen(current_planet.getPosition()), input_handler->getMousePosition(), current_planet.getColor());
        break;

    default:
        break;
    }
}



GalaxyCreator::GalaxyCreator(Camera *camera, PlanetManager *planet_manager)
    : camera(camera), planet_manager(planet_manager)
{
    icon_path = "assets/galaxy.png";
    input_handler = InputHandler::getInstance();
    timer = Timer::getInstance();
    painter = Painter::getInstance();

    state = IDLE;
    add_new_star_speed = 500;
    radius_growth_speed = 80;
    reset();
}

void GalaxyCreator::update()
{
    std::random_device rd;
    SDL_Point pos_diff;
    Vector2 pos, vel;
    int stars_to_add;
    double mass;
    double random;
    Vector2 distance;
    std::vector<Planet *> planets_to_remove;
    Planet vacuum = Planet();
    

    switch (state)
    {
    case JUST_STARTED:
        reset();
        galaxy_center = new Planet(0, camera->screenToWorld(input_handler->getMousePosition()));
        timer->setTimeWarp(0.1);
        
        state = GROWING;
        break;

    case GROWING:
        stars_to_add = add_new_star_speed * timer->getRealDtSeconds();
        current_radius += radius_growth_speed * timer->getRealDtSeconds() * camera->getZoom();
        galaxy_center->setRadius(current_radius/8);

        vacuum.setPosition(galaxy_center->getPosition());
        vacuum.setRadius(galaxy_center->getRadius()*1.7);

        for(Planet *planet : current_galaxy)
        {
            if(planet->isCollidingWith(vacuum))
            {
                planets_to_remove.push_back(planet);
            }
        }
        for(Planet *planet : planets_to_remove)
        {
            current_galaxy.erase(planet);
            delete planet;
        }

        for (int i = 0; i < stars_to_add + planets_to_remove.size(); i++)
        {
            mass = pow(((rand()%20)+1)/10.0 * camera->getZoom(), 3);

            random = (double)rand()/RAND_MAX;
            pos = Vector2(random * (current_radius - galaxy_center->getRadius()) + cbrt(mass) + galaxy_center->getRadius(), 0);
            random = (double)rand()/RAND_MAX;
            pos = pos.rotate(random * 2*M_PI);
            pos += galaxy_center->getPosition();

            current_galaxy.insert(new Planet(mass, pos));
        }

        planets_to_remove.clear();

        pos_diff = input_handler->getMousePositionDiff();
        if (pos_diff.x != 0 || pos_diff.y != 0)
            state = AIMING;
            
        if(input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_RELEASED))
            state = JUST_CREATED;
        break;

    case AIMING:
        if (input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_RELEASED))
            state = JUST_CREATED;
        break;

    case JUST_CREATED:
        timer->setTimeWarp(1);
        galaxy_center->setVelocity(galaxy_center->getPosition() - camera->screenToWorld(input_handler->getMousePosition()));
        planet_manager->addPlanet(galaxy_center);

        for(Planet *planet : current_galaxy)
        {
            planet->setVelocity((galaxy_center->getPosition() - camera->screenToWorld(input_handler->getMousePosition())));
            distance = planet->getPosition() - galaxy_center->getPosition();
            if(distance.magnitude() != 0)
            {
                planet->setVelocity(planet->getVelocity() + distance.normalized().rotate90() * sqrt(GRAV_CONST*galaxy_center->getMass()/distance.magnitude()));
            }
            planet_manager->addPlanet(planet);
        }
        reset();

        state = IDLE;
        break;

    case IDLE:
        if (input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_PRESSED))
            state = JUST_STARTED;
        break;

    default:
        break;
    }
}

void GalaxyCreator::reset()
{
    current_galaxy.clear();
    galaxy_center = nullptr;
    current_radius = 0;
}

void GalaxyCreator::render()
{
    if(galaxy_center)
        painter->drawPlanet(galaxy_center);
    for(Planet *planet : current_galaxy)
    {
        painter->drawPlanet(planet);
    }
    if(state == AIMING)
    {
        painter->drawLine(camera->worldToScreen(galaxy_center->getPosition()), input_handler->getMousePosition(), COLOR::GOLDEN_STAR);
    }
}



CosmicDustCreator::CosmicDustCreator(Camera* camera, PlanetManager *planet_manager)
    : camera(camera), planet_manager(planet_manager)
{
    icon_path = "assets/cosmic_dust.png";
    input_handler = InputHandler::getInstance();
    painter = Painter::getInstance();

    state = IDLE;
}

void CosmicDustCreator::update()
{
    switch (state)
    {
    case JUST_CREATED:
        for(int i = 0; i < camera->getWindowWidth(); i+= 15)
        {
            for(int j = 0; j < camera->getWindowHeight(); j+= 15)
            {
                planet_manager->addPlanet(new Planet(pow(camera->getZoom(), 3), camera->screenToWorld({i, j})));
            }
        }

        state = IDLE;
        break;
    
    case IDLE:
        if(input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_PRESSED))
        {
            state = JUST_CREATED;
        }
        break;

    default:
        break;
    }
}

void CosmicDustCreator::render(){}



Eraser::Eraser(Camera *camera, PlanetManager *planet_manager)
    :planet_manager(planet_manager), camera(camera)
{
    icon_path = "assets/trash.png";
    state = IDLE;
    input_handler = InputHandler::getInstance();
    painter = Painter::getInstance();
    radius = 100;
}

void Eraser::update()
{
    switch (state)
    {
    case IDLE:
        if(input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_PRESSED))
        state = ERASING;
        break;

    case ERASING:
        planet_manager->removePlanetsThatCollideWith(camera->screenToWorld(input_handler->getMousePosition()), radius*camera->getZoom());
        if (input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::JUST_RELEASED))
            state = IDLE;
        break;

    default:
        break;
    }
}

void Eraser::render()
{
    if(state == ERASING)
    {
        painter->drawFilledCircle(input_handler->getMousePosition(), radius, COLOR::BLACK);
        painter->drawCircle(input_handler->getMousePosition(), radius, COLOR::WHITE);
    }
}