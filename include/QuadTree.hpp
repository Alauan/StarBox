#pragma once
#include "main.hpp"

struct Boundaries
{
    Vector2 top_left;
    double size;
};

namespace QuadTree
{
    enum Quadrant
    {
        NW,
        NE,
        SW,
        SE,
        NONE
    };


    struct Node
    {
        Node(Boundaries boundaries);
        ~Node();

        Planet *planet;
        Boundaries boundaries;

        Vector2 center_of_mass;
        double mass;
        double max_radius;

        Node *NW;
        Node *NE;
        Node *SW;
        Node *SE;

        void insertPlanet(Planet* planet);
        void calculateAcceleration(Planet *planet, double precision=0.5);
        std::unique_ptr<std::unordered_set<Planet*>> calculateCollision(Planet *planet);

    private:
        bool isInValidSpace(Vector2 position);
        void insertPlanetInTheRightQuadrant(Planet *planet);
    };
}

