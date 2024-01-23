#pragma once
#include "main.hpp"


class Camera
{
public:
    Camera();

    void setPosition(Vector2 position);
    void move(Vector2 offset);

    void setZoom(double zoom);
    void setZoomOnPoint(double zoom, SDL_Point screenPosition);
    void zoomOnPoint(double zoom_val, SDL_Point screenPosition);

    Vector2 getPosition() const;
    double getZoom() const;

    Vector2 screenToWorld(SDL_Point screenPosition) const;
    SDL_Point worldToScreen(Vector2 worldPosition) const;

private:
    Vector2 position;
    double zoom;
};