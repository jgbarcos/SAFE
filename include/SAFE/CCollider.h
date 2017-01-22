#ifndef CCOLLIDER_H
#define CCOLLIDER_H

#include "Box2D/Box2D.h"

#include "SAFE/Component.h"
#include "SAFE/Vector2.h"

namespace safe {

class CCollider : public Component 
{
public:
    enum class Shape { CIRCLE, RECTANGLE };
    enum class Type { DYNAMIC, STATIC };    

    CCollider() :
        mCenter(Vector2(0,0)),
        mSize(Vector2(4,4)),
        mAngle(0.0),
        mShape(Shape::CIRCLE),
        mType(Type::DYNAMIC)
    {}
    
    CCollider(sol::table luaT) : CCollider() {
        sol::table t = luaT.get<sol::table>("center");
        if (t.valid()) mCenter = Vector2(t);
        
        t = luaT.get<sol::table>("size");
        if (t.valid()) mSize = Vector2(t);
        
        mAngle = luaT.get_or<float>("angle", mAngle);
        
        auto shape = luaT.get<std::string>("shape");
        if(shape=="circle")         mShape = Shape::CIRCLE;
        else if(shape=="rectangle") mShape = Shape::RECTANGLE;
        
        auto type = luaT.get<std::string>("type");
        if(type == "static")       mType = Type::STATIC;
        else if(type == "dynamic") mType = Type::DYNAMIC;
    }

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

