#include "../include/main.hpp"


PlanetManager::PlanetManager(int n_threads)
    :thread_pool(n_threads), followed_planet(nullptr), calculation_dt(0), simulation_fps(0), root(nullptr)
{
    painter = Painter::getInstance();
    timer = Timer::getInstance();
}

PlanetManager::~PlanetManager()
{
    thread_pool.stop(true);
    for(Planet *planet: planets)
        delete planet;
    planets.clear();

    for(Planet *planet: planets_to_render)
        delete planet;
    planets_to_render.clear();

    for(Planet *planet: planets_to_insert)
        delete planet;
    planets_to_insert.clear();

    if(root) delete root;
}


Planet* PlanetManager::addPlanet(Planet *planet){
    if(planet == nullptr)
        throw std::invalid_argument("PlanetManager::addPlanet: planet is nullptr");

    if(isPlanetInSet(planet))
        return planet;
    
    planets_to_insert.push_back(planet);
    return planet;
}

void PlanetManager::followPlanet(Planet *planet){
    if(isPlanetInSet(planet)){
        followed_planet = planet;
    }
    else
    {
        throw std::invalid_argument("PlanetManager::followPlanet: planet is not in set");
    }
}

void PlanetManager::removePlanet(Planet *planet){
    if(isPlanetInSet(planet))
        planets_to_remove.push_back(planet);
}

void PlanetManager::removePlanetsThatCollideWith(Vector2 pos, double radius){
    for(Planet *planet : planets)
    {
        if((planet->getPosition()-pos).magnitude() <= radius + planet->getRadius())
            removePlanet(planet);
    }
}

void PlanetManager::mergePlanets(const std::vector<Planet*>& planets){
    if(planets.size() <= 1)
        return;

    Planet *new_planet = new Planet();
    for(Planet *planet : planets)
    {
        if(isPlanetInSet(planet))
        {
            if(planet == followed_planet) 
                followed_planet = new_planet;

            *new_planet = *new_planet + *planet;
            removePlanet(planet);
        }
        
    }
    
    if(new_planet->getMass() > 0)
        addPlanet(new_planet);
    else
        delete new_planet;
}

bool PlanetManager::isPlanetInSet(Planet *planet) const{
    return planets.find(planet) != planets.end();
}

Planet *PlanetManager::getFollowedPlanet(){
    return followed_planet;
}

void PlanetManager::update(){
    calculation_dt += timer->getDtSeconds();
    
    if(didCalculationsFinnished())
    {
        simulation_fps = (int)(1/calculation_dt);
        updatePlanetsVector();
        calculation_dt = 0;
        copyPlanetsForRendering();
        thread_pool.push([this](int) { handleCalculations(calculation_dt); });
    }
    else
    {
        for(Planet *planet : planets_to_render)
            planet->update(timer->getDtSeconds());
    }
}

bool PlanetManager::didCalculationsFinnished(){
    return thread_pool.size() == thread_pool.n_idle();
}

void PlanetManager::updatePlanetsVector(){
    mergeCollisionsFromFutures();
    for(Planet *planet : planets_to_insert)
        planets.insert(planet);
    planets_to_insert.clear();

    for(Planet *planet : planets_to_remove)
    {
        removePlanet(planet);
        if(isPlanetInSet(planet)){
            if(followed_planet == planet){
                followed_planet = nullptr;
            }
            delete planet;
        }
        planets.erase(planet);
    }
    planets_to_remove.clear();

    for(Planet *planet : planets)
        planet->update(calculation_dt);
}

void PlanetManager::mergeCollisionsFromFutures(){
    std::vector<std::unique_ptr<std::unordered_set<Planet*>>> collisions;
    std::unordered_set<Planet*> planets_that_collided;
    for(auto &future : collision_futures)
    {
        auto collision = future.get();
        if(collision)
            collisions.push_back(std::move(collision));
    }
    
    std::sort(collisions.begin(), collisions.end(), [](const auto& a, const auto& b) {
        return a->size() > b->size();
    });

    for(auto& collision: collisions)
    {
        if(planets_that_collided.find(*(collision->begin())) == planets_that_collided.end())
        {
            mergePlanets(std::vector<Planet*>(collision->begin(), collision->end()));
            planets_that_collided.insert(collision->begin(), collision->end());
        }
    }

    collision_futures.clear();
}

void PlanetManager::copyPlanetsForRendering(){
    for(Planet *planet : planets_to_render)
        delete planet;
    planets_to_render.clear();

    for(Planet *planet : planets){
        planets_to_render.insert(new Planet(*planet));
    }
}

void PlanetManager::handleCalculations(double dt){
    delete root;
    root = createQuadTree();

    pushAccelerationsToPool();
    pushCollisionsToPool();
}

void PlanetManager::pushAccelerationsToPool(){
    for(Planet *planet : planets){
        thread_pool.push([planet, this](int) { root->calculateAcceleration(planet, 0.5); });
    }
}

void PlanetManager::pushCollisionsToPool(){
    int i = 0;
    for(Planet *planet : planets){
        collision_futures.push_back(thread_pool.push([i, this, planet](int)
        { 
            auto collision = root->calculateCollision(planet);
            if(collision && collision->size() > 0){ 
                collision->insert(planet);
                return collision;
            }
            return std::unique_ptr<std::unordered_set<Planet*>>(nullptr);
        }));
        i++;
    }
}

QuadTree::Node *PlanetManager::createQuadTree(){
    using namespace QuadTree;

    double topY = INFINITY, botY = -INFINITY, leftX = INFINITY, rightX = -INFINITY;
    for(Planet *planet : planets){

        if(planet->getPosition().x <= leftX)
            leftX = std::nextafter(planet->getPosition().x, -INFINITY);

        if(planet->getPosition().x >= rightX)
            rightX = std::nextafter(planet->getPosition().x, INFINITY);

        if(planet->getPosition().y <= topY)
            topY = std::nextafter(planet->getPosition().y, -INFINITY);
        
        if(planet->getPosition().y >= botY)
            botY = std::nextafter(planet->getPosition().y, INFINITY);
    }

    Node *root = new Node({Vector2(leftX, topY), std::max(rightX - leftX, botY - topY)});
    for(Planet *planet : planets){
        root->insertPlanet(planet);
    }

    return root;
}

std::unordered_set<Planet*>* PlanetManager::getPlanetsToRenderSet(){
    return &planets_to_render;
}

void PlanetManager::render() const
{
    drawPlanets();
    drawSimulationFPS();
    drawPlanetCounter();
}

void PlanetManager::drawSimulationFPS() const
{
    std::string fps_text = "Simulation FPS: " + std::to_string((int)simulation_fps);
    painter->drawText({20, 20}, fps_text, COLOR::WHITE);
}

void PlanetManager::drawPlanetCounter() const
{
    std::string planets_text = "Planets: " + std::to_string(planets_to_render.size());
    painter->drawText({20, 60}, planets_text, COLOR::WHITE);
}

void PlanetManager::drawPlanets() const
{
    for(Planet *planet: planets_to_render)
    {
        painter->drawPlanet(planet);
    }
}





/*
std::vector<Vector2>* PlanetManager::getPrediction(const Planet *planet, int steps, double dt){
    
    int initial_thread_amount = thread_pool.size();
    thread_pool.resize(2);
    
    std::vector<Vector2> *prediction = new std::vector<Vector2>();
    {
        PlanetManager predictionManager = PlanetManager(initial_thread_amount-2);
        Planet* prediction_planet = predictionManager.addPlanet(new Planet(*planet));
        for(Planet *planet : planets){
            predictionManager.addPlanet(new Planet(*planet));
        }

        for(int i = steps-2; i < steps; i++){
            while(!predictionManager.didCalculationsFinnished()){};
            prediction->push_back(prediction_planet->getPosition());
            predictionManager.update(dt);
        }
    }
    thread_pool.resize(initial_thread_amount);

    return prediction;
    
}
*/