#ifndef SCAMERA_H
#define SCAMERA_H

#include <SDL.h>

#include <math.h>

#include "SAFE/Vector2.h"
#include "SAFE/Vector3.h"
#include "SAFE/CTransform.h"

namespace safe {

class Camera {
public:

    Camera(SDL_Renderer* renderer, int width, int height)
    : mWidth(width), mHeight(height), mpRenderer(renderer) { }

    Vector3 World2Camera(const Vector3 position) const {
        return position - mTransform.mPosition;
    }

    Vector3 Camera2World(const Vector3 camPos) const {
        return camPos + mTransform.mPosition;
    }

    Vector2 Camera2Screen(const Vector3 position) const {
        return Vector2::Reduce(position * mTransform.mScale) + GetSize() / 2.0;
    }

    Vector3 Screen2Camera(const Vector2 screenPos) const {
        Vector2 v = (screenPos - GetSize() / 2.0) / Vector2::Reduce(mTransform.mScale);
        return Vector2::Extend(v, mTransform.mPosition.z);
    }

    Vector2 World2Screen(const Vector3 position) const {
        return Camera2Screen(World2Camera(position));
    }

    Vector3 Screen2World(const Vector2 position) const {
        return Camera2World(Screen2Camera(position));
    }

    Vector2 Percentage2Pixel(Vector2 p) const {
        return Vector2(p.x*mWidth, p.y * mHeight);
    }

    SDL_Renderer* getSDLRenderer() const {
        return mpRenderer;
    } // TODO: not really constness, use it carefully

    CTransform mTransform;
    int mWidth;
    int mHeight;
private:

    Vector2 GetSize() const {
        return Vector2(mWidth, mHeight);
    }
    SDL_Renderer* mpRenderer;
};

} // namespace safe

#endif // SCAMERA_H
