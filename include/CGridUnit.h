#ifndef CGRIDUNIT_H
#define CGRIDUNIT_H

#include "SAFE/Component.h"
#include "SAFE/Vector2.h"

using namespace safe;

class CGridUnit : public Component
{
public:
    CGridUnit() :
        mX(-1), mY(-1)
    {   mComponentName = "GridUnitComponent"; }    
    
    void FromLua(sol::table luaT) override {
        mX = luaT.get_or("x", mX);
        mY = luaT.get_or("y", mY);
    }
    
    // Required
    int mX; 
    int mY;
};



#endif /* CGRIDUNIT_H */

