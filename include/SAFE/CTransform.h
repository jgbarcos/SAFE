#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "SAFE/Vector3.h"
#include "SAFE/Component.h"

namespace safe {

class CTransform : public Component
{
    public:
        CTransform() :
            mPosition(Vector3(0,0,0)),
            mVelocity(Vector3(0,0,0)),
            mScale(Vector3(1,1,1))
        {}

        // Optional
        Vector3 mPosition;
        Vector3 mVelocity;
        Vector3 mScale;
};

} // namespace safe

#endif // TRANSFORM_COMPONENT_H
