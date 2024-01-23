#include "../include/main.hpp"


QuadTree::Node::Node(Boundaries _boundaries)
{
    planet = nullptr;
    boundaries = _boundaries;
    center_of_mass = 0;
    mass = 0;
    max_radius = 0;
    NE = nullptr;
    NW = nullptr;
    SE = nullptr;
    SW = nullptr;
}

QuadTree::Node::~Node()
{
    delete NE;
    delete NW;
    delete SE;
    delete SW;
}

bool QuadTree::Node::isInValidSpace(Vector2 position)
{
    return position.x >= boundaries.top_left.x && position.x <= boundaries.top_left.x + boundaries.size && 
           position.y >= boundaries.top_left.y && position.y <= boundaries.top_left.y + boundaries.size;
}

void QuadTree::Node::insertPlanetInTheRightQuadrant(Planet *_planet)
{
    double midX = boundaries.top_left.x + boundaries.size/2;
    double midY = boundaries.top_left.y + boundaries.size/2;
    if(_planet->getPosition().x <= midX)
    {
        if(_planet->getPosition().y <= midY)
        {
            if(NW == nullptr)
                NW = new Node({boundaries.top_left, boundaries.size/2});
            NW->insertPlanet(_planet);
        }
        else
        {
            if(SW == nullptr)
                SW = new Node({{boundaries.top_left.x, midY}, boundaries.size/2});
            SW->insertPlanet(_planet);
        }
    }
    else
    {
        if(_planet->getPosition().y <= midY)
        {
            if(NE == nullptr)
                NE = new Node({{midX, boundaries.top_left.y}, boundaries.size/2});
            NE->insertPlanet(_planet);
        }
        else
        {
            if(SE == nullptr)
                SE = new Node({{midX, midY}, boundaries.size/2});
            SE->insertPlanet(_planet);
        }
    }
}

void QuadTree::Node::insertPlanet(Planet* _planet)
{
    if(_planet == nullptr)
    {
        throw std::runtime_error("Planet is nullptr");
    }

    if(_planet->getMass() == 0)
    {
        throw std::runtime_error("Planet mass is 0");
    }


    if(_planet->getRadius() > max_radius)
        max_radius = _planet->getRadius();
    double total_mass = _planet->getMass() + mass;
    center_of_mass = (center_of_mass*mass + _planet->getPosition()*_planet->getMass())/total_mass;
    mass = total_mass;

    if(mass == _planet->getMass())
    {
        planet = _planet;
        return;
    }

    if(planet == nullptr)
    {
        insertPlanetInTheRightQuadrant(_planet);
        return;
    }

    if(_planet->getPosition() == planet->getPosition())
    {
        //std::cout << "Planet getPosition() is the same, planet not inserted!" << std::endl;
        return;
    }

    insertPlanetInTheRightQuadrant(planet);
    insertPlanetInTheRightQuadrant(_planet);
    planet = nullptr;
}

void QuadTree::Node::calculateAcceleration(Planet *_planet, double precision)
{
    if( boundaries.size / (center_of_mass - _planet->getPosition()).magnitude() < precision || planet && planet != _planet)
    {
        _planet->addAccelerationTowards(mass, center_of_mass);
        return;
    }

    if(NW != nullptr)
        NW->calculateAcceleration(_planet, precision);

    if(NE != nullptr)
        NE->calculateAcceleration(_planet, precision);

    if(SW != nullptr)
        SW->calculateAcceleration(_planet, precision);

    if(SE != nullptr)
        SE->calculateAcceleration(_planet, precision);

}


std::unique_ptr<std::unordered_set<Planet*>> QuadTree::Node::calculateCollision(Planet *_planet)
{
    std::unique_ptr<std::unordered_set<Planet*>> collisions = std::make_unique<std::unordered_set<Planet*>>();
    if(planet && _planet != planet)
    {
        if((_planet->getPosition() - planet->getPosition()).magnitude() <= _planet->getRadius() + planet->getRadius())
        {
            collisions->insert(planet);
            return collisions;
        }
        return nullptr;
    }

    Boundaries collision_boundaries = {boundaries.top_left - Vector2(max_radius+_planet->getRadius(), max_radius+_planet->getRadius()), boundaries.size + 2*(max_radius+_planet->getRadius())};
    Node collision_node = Node(collision_boundaries);
    if(!collision_node.isInValidSpace(_planet->getPosition()))
    {
        return nullptr;
    }

    if(NW != nullptr)
    {
        auto NW_collisions = NW->calculateCollision(_planet);
        if(NW_collisions != nullptr)
            collisions->insert(NW_collisions->begin(), NW_collisions->end());
    }
    
    if(NE != nullptr)
    {
        auto NE_collisions = NE->calculateCollision(_planet);
        if(NE_collisions != nullptr)
            collisions->insert(NE_collisions->begin(), NE_collisions->end());
    }
    
    if(SW != nullptr)
    {
        auto SW_collisions = SW->calculateCollision(_planet);
        if(SW_collisions != nullptr)
            collisions->insert(SW_collisions->begin(), SW_collisions->end());
    }

    if(SE != nullptr)
    {
        auto SE_collisions = SE->calculateCollision(_planet);
        if(SE_collisions != nullptr)
            collisions->insert(SE_collisions->begin(), SE_collisions->end());
    }

    return collisions;
}