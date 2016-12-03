#ifndef STRANSFORM_H
#define STRANSFORM_H

#include "SAFE/Vector3.h"

namespace safe {

class Transform
{
    public:
        Transform();
        virtual ~Transform();

        Vector3 mPosition = Vector3(0);
        Vector3 mScale = Vector3(1);
        Vector3 mRotation = Vector3(0);
    protected:
    private:
};

} // namespace safe

#endif // STRANSFORM_H
