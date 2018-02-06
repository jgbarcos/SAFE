#ifndef SYSTEMLUA_H
#define SYSTEMLUA_H

#include "SAFE/System.h"

namespace safe {

/**
 * System in the ECS system. Performs changes on entities.
 */
class SystemLua : public System {
private:
    typedef std::function<void(sol::table)> SelfFunc;
    typedef std::function<void(sol::table, std::vector<Entity*>& entities)> InitFunc;
    typedef std::function<void(sol::table, float, std::vector<Entity*>& entities)> UpdateFunc;
    
public:
    
    SystemLua(sol::table luaT){
        mSelf = luaT;
        mName = luaT["name"];
        mInit = luaT.get < InitFunc >("init");
        mUpdate = luaT.get < UpdateFunc >("update");
        mOnEnable = luaT.get< SelfFunc >("on_enable");
        mOnDisable = luaT.get< SelfFunc >("on_disable");
        luaT["set_active"] = [&](bool active){ mActive = active; };
        luaT["get_active"] = [&](){ return mActive; };
    }

    void Init(std::vector<Entity*>& entities) {
        mInit(mSelf, entities);
    }

    void Update(float delta, std::vector<Entity*>& entities){
        mUpdate(mSelf, delta, entities);
    }
    
    void OnEnable() { mOnEnable(mSelf);  }
    void OnDisable(){ mOnDisable(mSelf); }
    
private:
    sol::table mSelf;
    InitFunc mInit;
    UpdateFunc mUpdate;
    SelfFunc mOnEnable;
    SelfFunc mOnDisable;
};

} // namespace safe

#endif // SYSTEM_H
