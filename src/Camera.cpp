#include "../include/main.hpp"


Camera::Camera(int window_width, int window_height, double zoom)
{
    position = Vector2(0, 0);
    this->zoom = zoom;
    windowWidth = window_width;
    windowHeight = window_height;
}

void Camera::setZoom(double new_zoom)
{
    setZoomOnPoint(new_zoom, SDL_Point{windowWidth / 2, windowHeight / 2});
}

void Camera::setZoomOnPoint(double new_zoom, SDL_Point screenPosition)
{
    if(new_zoom == getZoom())
        return;
    Vector2 position_in_world = screenToWorld(screenPosition);
    Vector2 position_in_screen = screenPosition;
    zoom = new_zoom;
    position = position_in_world - position_in_screen * zoom;
}

void Camera::zoomOnPoint(double zoom_val, SDL_Point screenPosition)
{
    setZoomOnPoint(getZoom() + getZoom()*zoom_val, screenPosition);
}

void Camera::setPosition(Vector2 new_position)
{
    position = new_position - Vector2(windowWidth, windowHeight) * zoom / 2;
}

void Camera::move(Vector2 offset)
{
    position += offset;
}

double Camera::getZoom() const
{
    return zoom;
}

Vector2 Camera::getPosition() const
{
    return position + Vector2(windowWidth, windowHeight) * zoom / 2;
}

Vector2 Camera::screenToWorld(SDL_Point position_in_screen) const
{
    return position + Vector2(position_in_screen) * zoom;
}

SDL_Point Camera::worldToScreen(Vector2 position_in_world) const
{
    Vector2 position_in_screen = (position_in_world - position) / zoom;
    return SDL_Point{static_cast<int>(position_in_screen.x), static_cast<int>(position_in_screen.y)};
}

int Camera::getWindowWidth() const
{
    return windowWidth;
}

int Camera::getWindowHeight() const
{
    return windowHeight;
}