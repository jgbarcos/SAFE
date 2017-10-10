#ifndef COMPONENT_H
#define COMPONENT_H

#include <sol.hpp>

namespace safe {

/**
 * Component in the ECS system. All components inherits from this.
 */
class Component {
public:

    virtual ~Component() { };

    /**
     * Called once, it allows to initialize and set lua bindings.
     * @param lua the lua state.
     */
    virtual std::string PrepareLua(sol::state_view& lua) {
        return "";
    };

    /**
     * Loads member data from lua table.
     * @param luaT lua table with the component contents.
     */
    virtual void FromLua(sol::table luaT) { };

    std::string mComponentName;
};

} // namespace safe

#endif // COMPONENT_H
