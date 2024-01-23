#pragma once
#include "main.hpp"

class Vector2
{
public:
    Vector2(double x = 0, double y = 0);
    Vector2(const Vector2& v);
    Vector2(const SDL_Point& point);

    void operator=(const Vector2& v);
    void operator=(const SDL_Point& point);

    Vector2 operator+(const Vector2& v) const;
    Vector2 operator-(const Vector2& v) const;
    Vector2 operator*(double k) const;
    Vector2 operator/(double k) const;

    Vector2& operator+=(const Vector2& v);
    Vector2& operator-=(const Vector2& v);

    Vector2& operator*=(double k);
    Vector2& operator/=(double k);

    bool operator==(const Vector2& v) const;
    bool operator!=(const Vector2& v) const;

    double magnitude() const;
    Vector2 normalized() const;

    double operator*(const Vector2& v) const;
    Vector2 vectorProduct(const Vector2& v) const;

    Vector2 projection(const Vector2& v) const;
    Vector2 rotate90() const;
    Vector2 rotate(double angle) const;

    double x;
    double y;

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
};

std::ostream& operator<<(std::ostream& os, const Vector2& v);