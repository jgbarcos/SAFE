#ifndef SYSTEMLUA_H
#define SYSTEMLUA_H

#include "SAFE/System.h"

namespace safe {

/**
 * System in the ECS system. Performs changes on entities.
 */
class SystemLua : public System {
private:
    typedef std::function<void(sol::table, std::vector<Entity*>& entities)> InitFunc;
    typedef std::function<void(sol::table, float, std::vector<Entity*>& entities)> UpdateFunc;
    
public:
    
    SystemLua(sol::table luaT){
        mSelf = luaT;
        mInit = luaT.get < InitFunc >("init");
        mUpdate = luaT.get < UpdateFunc >("update");
        luaT["set_active"] = [&](bool active){ mActive = active; };
        luaT["get_active"] = [&](){ return mActive; };
    }

    void Init(std::vector<Entity*>& entities) {
        mInit(mSelf, entities);
    }

    void Update(float delta, std::vector<Entity*>& entities){
        mUpdate(mSelf, delta, entities);
    }
    
private:
    sol::table mSelf;
    InitFunc mInit;
    UpdateFunc mUpdate;
};

} // namespace safe

#endif // SYSTEM_H
