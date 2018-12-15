#ifndef SYSTEMLUA_H
#define SYSTEMLUA_H

#include "SAFE/System.h"

namespace safe {

/**
 * System in the ECS system. Performs changes on entities.
 */
class SystemLua : public System {
private:
    typedef std::function<void(sol::table)> InitFunc;
    typedef std::function<void(sol::table, EntitySpace& space)> EnableFunc;
    typedef std::function<void(sol::table, float, EntitySpace& space)> UpdateFunc;
    
public:
    
    SystemLua(sol::table luaT){
        mSelf = luaT;
        mName = luaT["name"];
        mInit = luaT.get < InitFunc >("init");
        mUpdate = luaT.get < UpdateFunc >("update");
        mOnEnable = luaT.get< EnableFunc >("on_enable");
        mOnDisable = luaT.get< EnableFunc >("on_disable");
    }

    void Init() {
        mInit(mSelf);
    }

    void Update(float delta, EntitySpace& space){
        mUpdate(mSelf, delta, space);
    }
    
    void OnEnable(safe::EntitySpace& space) { mOnEnable(mSelf, space);  }
    void OnDisable(safe::EntitySpace& space){ mOnDisable(mSelf, space); }
    
private:
    sol::table mSelf;
    InitFunc mInit;
    UpdateFunc mUpdate;
    EnableFunc mOnEnable;
    EnableFunc mOnDisable;
};

} // namespace safe

#endif // SYSTEM_H
