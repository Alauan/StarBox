#include "../include/main.hpp"

Planet::Planet(double mass, Vector2 position, Vector2 velocity, Vector2 acceleration)
    : mass(mass), position(position), velocity(velocity), acceleration(acceleration)
{
    radius = cbrt(mass);
    color = getColorFromMass(mass);
}

Planet::Planet(const Planet& other)
{
    mass = other.mass;
    radius = other.radius;
    position = other.position;
    velocity = other.velocity;
    acceleration = other.acceleration;
    color = other.color;
}

double Planet::getMass() const
{
    return mass;
}

double Planet::getRadius() const
{
    return radius;
}

Vector2 Planet::getPosition() const
{
    return position;
}

Vector2 Planet::getVelocity() const
{
    return velocity;
}

Vector2 Planet::getAcceleration() const
{
    return acceleration;
}

SDL_Color Planet::getColor() const
{
    return color;
}


void Planet::setMass(double mass)
{
    this->mass = mass;
    this->radius = cbrt(mass);
    this->color = getColorFromMass(mass);
}

void Planet::setRadius(double radius)
{
    this->radius = radius;
    this->mass = pow(radius, 3);
    this->color = getColorFromMass(mass);
}

void Planet::setPosition(Vector2 position)
{
    this->position = position;
}

void Planet::setVelocity(Vector2 velocity)
{
    this->velocity = velocity;
}

void Planet::setAcceleration(Vector2 acceleration)
{
    this->acceleration = acceleration;
}


void Planet::addAccelerationTowards(const Planet& other)
{
    addAccelerationTowards(other.mass, other.position);
}

void Planet::addAccelerationTowards(double _mass, Vector2 center_of_mass)
{
    Vector2 distance = center_of_mass - position;
    acceleration += distance * GRAV_CONST * _mass / pow(distance.magnitude(), 3);
}

bool Planet::isCollidingWith(const Planet& other) const
{
    return (position - other.position).magnitude() <= radius + other.radius;
}

bool Planet::isCollidingWith(Vector2 position) const
{
    return (this->position - position).magnitude() <= radius;
}


Planet Planet::operator+(const Planet& other) const
{
    double new_mass = mass + other.mass;
    Vector2 new_position = (position*mass + other.position*other.mass)/new_mass;
    Vector2 new_velocity = (velocity*mass + other.velocity*other.mass)/new_mass;
    return {new_mass, new_position, new_velocity, {0, 0}};
}

Planet& Planet::operator=(const Planet& other)
{
    position = other.position;
    velocity = other.velocity;
    acceleration = other.acceleration;
    mass = other.mass;
    radius = other.radius;
    color = other.color;
    return *this;
}

void Planet::update(double dt)
{
    velocity += acceleration*dt;
    position += velocity*dt;
    acceleration = Vector2(0, 0);
}


SDL_Color Planet::getColorFromMass(double mass)
{
    return COLOR::interpolateColors(COLOR::star_colors, log10(mass)/log10(1e8));
}


SDL_Color COLOR::interpolateColors(const std::vector<SDL_Color>& colors, double value)
{
    if (value >= 1)
        return colors.back();
    if (value <= 0)
        return colors.front();

    double color_index = value*(colors.size()-1);
    const SDL_Color& color1 = colors.at(floor(color_index));
    const SDL_Color& color2 = colors.at(ceil(color_index));

    double color1_weight = ceil(color_index) - color_index;

    return {
        (Uint8)(color1.r*color1_weight + color2.r*(1-color1_weight)),
        (Uint8)(color1.g*color1_weight + color2.g*(1-color1_weight)),
        (Uint8)(color1.b*color1_weight + color2.b*(1-color1_weight)),
        (Uint8)(color1.a*color1_weight + color2.a*(1-color1_weight))
    };
}