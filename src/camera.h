#ifndef CAMERA_H
#define CAMERA_H

#include "vector_util.h"
#include <SFML/System.hpp>

class Camera {
public:
    Camera(const sf::Vector2f& windowSize, const sf::Vector2f& viewCenterWorld,
           float worldUnitsPerPixel = 1.f)
        : mWindowSize{windowSize},
          mViewCenterWorld{viewCenterWorld},
          mWorldUnitsPerPixel{worldUnitsPerPixel} {}


    const sf::Vector2f windowToWorld(const sf::Vector2f& pWindow) {
        sf::Vector2f offsetPixels = pWindow - mWindowSize / 2.0f;  // mWorldSize / 2 gives center in window units
        return mViewCenterWorld + (offsetPixels * mWorldUnitsPerPixel);
    };

    const sf::Vector2f worldToWindow(const sf::Vector2f& pWorld) {
        sf::Vector2 offsetWorld = pWorld - mViewCenterWorld;
        return (mWindowSize / 2.0f) + (offsetWorld / mWorldUnitsPerPixel);
    };

    const sf::Vector2f windowPercentToWorld(const sf::Vector2f& pWindowPercent) {
        // Converts a point in normalized window coordinates (i.e., expressed as percentages of the window size) to a point in world coordinates.
        sf::Vector2f windowPoint = mWindowSize * pWindowPercent;
        return windowToWorld(windowPoint);
    };

    const sf::Vector2f worldToWindowPercent(const sf::Vector2f& pWorld) {
        // Converts a point in world coordinates to normalized window coordinates
        sf::Vector2f windowPoint = worldToWindow(pWorld);
        return (windowPoint / mWindowSize);
    };

    void translateWorld(const sf::Vector2f& deltaWorld) {
        // Translates the camera by deltaWorld world units.
        mViewCenterWorld += deltaWorld;
    };

    void translateWindow(const sf::Vector2f& deltaScreen) {
        // Translates the camera by deltaScreen window units.
        mViewCenterWorld += (deltaScreen * mWorldUnitsPerPixel);
    };

    void translateWindowPercent(const sf::Vector2f& deltaScreenPercent) {
        // Translates the camera by deltaScreenPercent normalized window units.
        translateWindow(deltaScreenPercent * mWindowSize);
    };

    void setCamera(const sf::Vector2f& pWorld, const sf::Vector2f& pWindow) {
        // Given a point in world coordinates and a point in window coordinates,
        // maintains the camera's scale and translates the camera such that the
        // given screen point now maps to the given world point.
        sf::Vector2f offset = pWorld - windowToWorld(pWindow);
        translateWorld(offset);
    };

    void setCameraPercent(const sf::Vector2f& pWorld, const sf::Vector2f& pWindowPercent) {
        // Takes a screen point given as a percentage of the total window size from
        // the top-left (instead of a pixel distance from the top-left).
        setCamera(pWorld, pWindowPercent * mWindowSize);  // convert percent to point and just call setCamera
    };

    void setCameraCenter(const sf::Vector2f& pWorld) {
        // sets the camera such that the center of the window-coordinates rectangle maps to the given world point.
        mViewCenterWorld = pWorld;
    };

    sf::Vector2f getCameraCenter() const {
        // returns the current camera view center in world coordinates.
        return mViewCenterWorld;
    };

private:
    sf::Vector2f mWindowSize;       // size of the window in screen pixels (i.e. window coords)
    sf::Vector2f mViewCenterWorld;  // world coords point corresponding to the center of our window.
    float mWorldUnitsPerPixel;      // distance in world coords covered by one pixel
};

#endif  // CAMERA_H
