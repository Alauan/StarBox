#pragma once
#include "main.hpp"


class PlanetManager{
public:
    typedef std::unordered_set<Planet*>::iterator Planet_it;

    PlanetManager(int n_threads=std::thread::hardware_concurrency());
    ~PlanetManager();

    void update();
    void render() const;

    Planet* addPlanet(Planet *planet);
    void followPlanet(Planet *planet);
    void removePlanet(Planet *planet);
    void removePlanetsThatCollideWith(Vector2 position, double radius);
    void mergePlanets(const std::vector<Planet*>& planets);

    bool isPlanetInSet(Planet *planet) const;
    
    Planet *getPlanetAtPosition(Vector2 position) const;

    //std::vector<Vector2>* getPrediction(const Planet *Planet, int steps, double dt);

    bool isFollowingPlanet() const;
    Planet* getFollowedPlanet();
    void addPlanetInRelationToFollowedPlanet(Planet* planet);

    std::unordered_set<Planet*>* getPlanetsToRenderSet();
private:
    void handleCalculations(double dt);
    QuadTree::Node *createQuadTree();
    void pushAccelerationsToPool();
    void pushCollisionsToPool();
    bool didCalculationsFinnished();
    void updatePlanetsVector();
    void mergeCollisionsFromFutures();
    void copyPlanetsForRendering();

    void drawSimulationFPS() const;
    void drawPlanets() const;
    void drawPlanetCounter() const;

    std::unordered_set<Planet*> planets;
    std::vector<std::future<std::unique_ptr<std::unordered_set<Planet*>>>> collision_futures;
    QuadTree::Node *root;
    double calculation_dt;
    double simulation_fps;

    std::vector<Planet*> planets_to_insert;
    std::vector<Planet*> planets_to_remove;
    std::unordered_set<Planet*> planets_to_render;

    Planet *followed_planet;
    ctpl::thread_pool thread_pool;

    Painter *painter;
    Timer *timer;
};

