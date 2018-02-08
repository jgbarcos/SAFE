#ifndef CDRAGGABLE_H
#define CDRAGGABLE_H

#include "SAFE/Component.h"

using namespace safe;

class CDraggable : public Component {
public:

    CDraggable() :
        mIsDraggable(true),
        mTurnHorizontal(false),
        mBeingDragged(false)
    {
        mComponentName = "DraggableComponent";
    }

    void FromLua(sol::table luaT) override {
        mIsDraggable = luaT.get_or<bool>("is_draggable", mIsDraggable);
        mTurnHorizontal = luaT.get_or<bool>("turn_horizontal", mTurnHorizontal);
    }

    // Optional
    bool mIsDraggable;
    bool mTurnHorizontal;

    // Generated
    bool mBeingDragged;


};


#endif /* CDRAGGABLE_H */

