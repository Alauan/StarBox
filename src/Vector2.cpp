#include "../include/main.hpp"


Vector2::Vector2(double x, double y)
    : x(x), y(y){}

Vector2::Vector2(const SDL_Point& point)
    : x(point.x), y(point.y){}

Vector2::Vector2(const Vector2& other)
{
    x = other.x;
    y = other.y;
}

void Vector2::operator=(const Vector2& other)
{
    x = other.x;
    y = other.y;
}

void Vector2::operator=(const SDL_Point& point)
{
    x = point.x;
    y = point.y;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(double k) const
{
    return Vector2(x * k, y * k);
}

double Vector2::operator*(const Vector2& other) const
{
    return x * other.x + y * other.y;
}

Vector2 Vector2::operator/(double k) const
{
    return Vector2(x / k, y / k);
}

Vector2& Vector2::operator+=(const Vector2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2& Vector2::operator*=(double k)
{
    x *= k;
    y *= k;
    return *this;
}

Vector2& Vector2::operator/=(double k)
{
    x /= k;
    y /= k;
    return *this;
}

bool Vector2::operator==(const Vector2& other) const
{
    return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const
{
    return x != other.x || y != other.y;
}

double Vector2::magnitude() const
{
    return sqrt(x*x + y*y);
}

Vector2 Vector2::normalized() const
{
    return *this / magnitude();
}

Vector2 Vector2::vectorProduct(const Vector2& other) const
{
    return Vector2(x * other.y - y * other.x, x * other.y - y * other.x);
}

Vector2 Vector2::projection(const Vector2& other) const
{
    return other * ((*this * other) / (other * other));
}

Vector2 Vector2::rotate90() const
{
    return Vector2(-y, x);
}

Vector2 Vector2::rotate(double angle) const
{
    return Vector2(x * cos(angle) - y * sin(angle), x * sin(angle) + y * cos(angle));
}

std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}
