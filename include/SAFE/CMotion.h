#ifndef CMOTION_H
#define CMOTION_H

#include "SAFE/Component.h"

namespace safe {

class CMotion : public Component 
{
public:

    CMotion() :
        mVelocity(Vector3(0, 0, 0)),
        mAcceleration(Vector3(0, 0, 0)) 
    {}

    Vector3 mVelocity;
    Vector3 mAcceleration;
};

} // namespace safe

#endif /* CMOTION_H */

