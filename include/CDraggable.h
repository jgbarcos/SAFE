#ifndef CDRAGGABLE_H
#define CDRAGGABLE_H

#include "SAFE/Component.h"

using namespace safe;

class CDraggable : public Component
{
public:
    CDraggable() :
        mIsDraggable(true),
        mBeingDragged(false)
    {   mComponentName = "DraggableComponent"; }    
    
    void FromLua(sol::table luaT) override {
        mIsDraggable = luaT.get_or<bool>("is_draggable", mIsDraggable);
    }
    
    // Optional
    bool mIsDraggable;
    
    // Generated
    bool mBeingDragged;
    
    
};


#endif /* CDRAGGABLE_H */

