#pragma once
#include "main.hpp"



namespace COLOR
{
    const SDL_Color RED = {255, 100, 100, 255};
    const SDL_Color BLUE = {100, 100, 255, 255};
    const SDL_Color GREEN = {100, 255, 100, 255};
    const SDL_Color YELLOW = {255, 255, 100, 255};
    const SDL_Color PURPLE = {255, 100, 255, 255};
    const SDL_Color CYAN = {100, 255, 255, 255};
    const SDL_Color WHITE = {255, 255, 255, 255};
    const SDL_Color ORANGE = {255, 150, 100, 255};
    const SDL_Color BLACK = {0, 0, 0, 255};

    const SDL_Color BROWN_DWARF = {118, 14, 0, 255};
    const SDL_Color RED_STAR = {255, 61, 0, 255};
    const SDL_Color ORANGE_STAR = {255, 159, 0, 255};
    const SDL_Color GOLDEN_STAR = {255, 210, 0, 255};
    const SDL_Color YELLOW_STAR = {255, 249, 128, 255};
    const SDL_Color YELLOW_WHITE_STAR = {255, 251, 189, 255};
    const SDL_Color WHITE_STAR = {207, 245, 255, 255};
    const SDL_Color BLUE_STAR = {124, 230, 255, 255};
    const std::vector<SDL_Color> star_colors = {BROWN_DWARF, RED_STAR, ORANGE_STAR, GOLDEN_STAR, YELLOW_STAR, YELLOW_WHITE_STAR, WHITE_STAR, BLUE_STAR};

    SDL_Color interpolateColors(const std::vector<SDL_Color>& colors, double value);
}


class Planet
{
private:
    double mass;
    double radius;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    SDL_Color color;

public:
    Planet(double mass=0, Vector2 position={0,0}, Vector2 velocity={0,0}, Vector2 acceleration={0,0});
    Planet(const Planet& other);

    void update(double dt);

    SDL_Color getColor() const;
    double getMass() const;
    double getRadius() const;
    Vector2 getPosition() const;
    Vector2 getVelocity() const;
    Vector2 getAcceleration() const;

    void setMass(double mass);
    void setRadius(double radius);
    void setPosition(Vector2 position);
    void setVelocity(Vector2 velocity);
    void setAcceleration(Vector2 acceleration);

    void addAccelerationTowards(const Planet& other);
    void addAccelerationTowards(double mass, Vector2 position);

    bool isCollidingWith(const Planet& other) const;

    Planet operator+(const Planet& p) const;
    Planet &operator=(const Planet& p);

private:
    static SDL_Color getColorFromMass(double mass);
};