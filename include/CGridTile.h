#ifndef CGRIDTILE_H
#define CGRIDTILE_H

#include "SAFE/Component.h"
#include "SAFE/Vector2.h"

using namespace safe;

class CGridTile : public Component {
public:

    CGridTile() :
        mX(-1), 
        mY(-1) 
    {
        mComponentName = "GridTileComponent";
    }

    void FromLua(sol::table luaT) override {
        mX = luaT.get_or("x", mX);
        mY = luaT.get_or("y", mY);
    }

    // Required
    int mX;
    int mY;

};


#endif /* CGRIDTILE_H */

