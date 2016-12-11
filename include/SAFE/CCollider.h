#ifndef CCOLLIDER_H
#define CCOLLIDER_H

#include "Box2D/Box2D.h"
#include "SAFE/Component.h"

namespace safe {

class CCollider : public Component 
{
public:
    enum class Shape { CIRCLE, RECTANGLE };
    enum class Type { DYNAMIC, STATIC };

    CCollider() :
        mCenter(Vector2(0,0)),
        mSize(4),
        mAngle(0.0),
        mShape(Shape::CIRCLE),
        mType(Type::DYNAMIC)
    {}

    // Required
    Vector2 mCenter;
    Vector2 mSize;
    float mAngle;
    Shape mShape;
    Type mType;
    
    // Provided
    b2Body* mpBody = nullptr;
    
};

} // namespace safe

#endif /* CCOLLIDER_H */

