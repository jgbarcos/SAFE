#ifndef ACTIONLISTMANAGER_H
#define ACTIONLISTMANAGER_H

#include <list>
#include <queue>
#include <vector>

#include "Action.h"
#include "ActionLua.h"

namespace safe {
/**
 * Manages lists of Action, a set of actions that are needed to run sequentially.
 */
class ActionListManager
{
public:    
    typedef std::queue< std::shared_ptr<Action> > ActionList;
    
    void Update(float delta){
        for(ActionList& actions : mActiveActionLists){
            if(!actions.empty()){
                actions.front()->Update(delta);

                if(actions.front()->mHasFinished){
                    actions.pop();
                }
            }
        }
        // Remove empty action lists
        mActiveActionLists.remove_if([](ActionList l){ return l.empty(); });
    }
    
    sol::table NewAction(sol::table t, const std::string& name){
        sol::table self = t[name];
        return self["new"](self);
    }
    
    sol::table NewAction(sol::table t, const std::string& name, sol::table params){
        sol::table self = t[name];
        return self["new"](self, params);
    }
    
    void Add(std::shared_ptr<Action> action){
        ActionList actionList;
        actionList.push(action);
        mActiveActionLists.push_back(actionList);
    }
    
    void Add(ActionList actionList){
        mActiveActionLists.push_back(actionList);
    }
    
    void Add(sol::table actionTable){
        auto ptr = new ActionLua();
        ptr->FromLua( actionTable );
        Add(std::shared_ptr<Action>(ptr));
    }
    
    void Add(std::vector<sol::table> actionListTable){
        ActionList actionList;
        
        for(auto t : actionListTable){
            auto ptr = new ActionLua();
            ptr->FromLua( t );
            actionList.push( std::shared_ptr<Action>(ptr) );
        }

        Add(actionList);
    }
    
    void LuaAdd(sol::table actionListTable){
        ActionList actionList;
        auto fx = [&](sol::object key, sol::object value){
            auto ptr = new ActionLua();
            ptr->FromLua( value.as<sol::table>() );
            actionList.push( std::shared_ptr<Action>(ptr) );
        };
        actionListTable.for_each(fx);
        
        Add(actionList);
    }
    
    
    
    std::list< ActionList > mActiveActionLists;
};

} // namespace safe

#endif /* ACTIONLISTMANAGER_H */

