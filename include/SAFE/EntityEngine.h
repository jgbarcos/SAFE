#ifndef ENTITYENGINE_H
#define ENTITYENGINE_H

#include <memory>
#include <vector>
#include <map>
#include <iostream>

#include <sol.hpp>
#include "SAFE/Component.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "SAFE/CCollider.h"
#include "SAFE/CPlayerControls.h"
#include "SAFE/CSheetAnimation.h"
#include "SAFE/CSprite.h"
#include "SAFE/CTransform.h"

namespace safe {

class EntityEngine
{
    public:        
        void AddSystem(std::unique_ptr<System> system)
        {
            mSystems.push_back(std::move(system));
        }


        Entity* CreateEntity(){
            mEntities.push_back(std::unique_ptr<Entity>(new Entity));
            return mEntities.back().get();
        }
        
        Entity* LoadEntity(sol::table luaT){
            /* This helps with debugging
            sol::state_view view(luaT.lua_state());
            view["sprite"] = luaT.get<sol::object>("SpriteComponent");
            view["collider"] = luaT.get<sol::object>("ColliderComponent");
            
            view.script(R"(
                function pp(o)
                    print( '{' )
                    if type(o) == 'table' then
                        for k, v in pairs( o ) do
                            print(k, v)
                        end
                    end
                    print( '}' )
                end

                pprint = pp
            )");
            */
            
            mEntities.push_back(std::unique_ptr<Entity>(new Entity));
            Entity* pEntity = mEntities.back().get();
            
            sol::table res = luaT.get<sol::table>("SpriteComponent");
            if(res.valid()){
                auto pComp = new CSprite(res);
                pEntity->Add<CSprite>(std::unique_ptr<Component>(pComp));
            }
            
            res = luaT.get<sol::table>("TransformComponent");
            if(res.valid()){
                auto pComp = new CTransform(res);
                pEntity->Add<CTransform>(std::unique_ptr<Component>(pComp));
            }
            
            res = luaT.get<sol::table>("SheetAnimationComponent");
            if(res.valid()){
                auto pComp = new CSheetAnimation(res);
                pEntity->Add<CSheetAnimation>(std::unique_ptr<Component>(pComp));
            }
            
            res = luaT.get<sol::table>("PlayerControlsComponent");
            if(res.valid()){
                auto pComp = new CPlayerControls(res);
                pEntity->Add<CPlayerControls>(std::unique_ptr<CPlayerControls>(pComp));
            }
            
            res = luaT.get<sol::table>("ColliderComponent");
            if(res.valid()){
                auto pComp = new CCollider(res);
                pEntity->Add<CCollider>(std::unique_ptr<CCollider>(pComp));
            }
            
            return pEntity;
        }

        void Update (float delta){
            for(auto&& e : mSystems){
                e->Update(delta, mEntities);
            }
        }

        std::vector<std::unique_ptr<Entity>> mEntities;
        std::vector<std::unique_ptr<System>> mSystems;

};

} // namespace safe

#endif // ENTITYENGINE_H
