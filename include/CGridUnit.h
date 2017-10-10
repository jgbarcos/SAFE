#ifndef CGRIDUNIT_H
#define CGRIDUNIT_H

#include "SAFE/Component.h"
#include "SAFE/Vector2.h"

using namespace safe;

class CGridUnit : public Component {
public:

    CGridUnit() :
        mX(-1), 
        mY(-1), 
        mTeam(0), 
        mCanMove(true) 
    {
        mComponentName = "GridUnitComponent";
    }

    void FromLua(sol::table luaT) override {
        mX = luaT.get_or("x", mX);
        mY = luaT.get_or("y", mY);
        mTeam = luaT.get_or("team", mTeam);
    }

    // Required
    int mX;
    int mY;
    int mTeam;

    // Generated
    bool mCanMove;
};



#endif /* CGRIDUNIT_H */

