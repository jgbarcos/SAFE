#ifndef SCAMERA_H
#define SCAMERA_H

#include <SDL.h>

#include <math.h>

#include "SAFE/Vector2.h"
#include "SAFE/Vector3.h"
#include "SAFE/Transform.h"

namespace safe {

class Camera
{
    public:
        Camera(SDL_Renderer* renderer)
            : mRenderer(renderer) {}

        Vector3 World2Camera(const Vector3& position) const{
            float x = position.x - mTransform.mPosition.x;
            float y = position.y - mTransform.mPosition.y;
            float z = position.z - mTransform.mPosition.z;

            return Vector3(x,y,z);
        }

        // 3/4 like projection
        Vector2 Camera2Screen(const Vector3& position) const {
            // return SVector2(position.x, position.y + 0.75 * position.z);
            return Vector2(position.x, position.y);
        }

        Vector2 World2Screen(const Vector3& position) const {
            return Camera2Screen(World2Camera(position));
        }

        SDL_Renderer* getSDLRenderer() const { return mRenderer; } // TODO: not really constness, use it carefully

        Transform mTransform;
    private:
        SDL_Renderer* mRenderer;
};

} // namespace safe

#endif // SCAMERA_H
