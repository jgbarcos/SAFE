#ifndef COMPONENT_H
#define COMPONENT_H

#include <sol.hpp>

namespace safe {

/**
 * Component in the ECS system. All components inherits from this.
 */
class Component
{
    public:
        virtual ~Component() {};  
        
        /**
         * Loads member data from lua table.
         * @param luaT lua table with the component contents.
         */
        virtual void FromLua(sol::table luaT) {};
        
        std::string mComponentName;
};

} // namespace safe

#endif // COMPONENT_H
