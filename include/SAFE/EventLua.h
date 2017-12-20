#ifndef EVENTLUA_H
#define EVENTLUA_H

#include <string>

#include "sol.hpp"

#include "SAFE/Event.h"

namespace safe {

class EventLua : public Event {
public:
    EventLua() { }

    EventLua(sol::table event) : mEvent(event) { mIsLuaEvent = true; }
    
    virtual Type type() const override {
        return mEvent.get<std::string>("type");
    }
    
    virtual std::string toString() const override {
        sol::object payload = mEvent["payload"];
        if (payload.is<std::string>()){
            return mEvent.get<std::string>("payload");
        }
        else return "?";
    }
    
    sol::table mEvent;
};

} // namespace safe

#endif /* EVENTLUA_H */

