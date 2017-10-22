#ifndef EVENTLUA_H
#define EVENTLUA_H

#include <string>

#include "sol.hpp"

#include "SAFE/Event.h"

namespace safe {

class EventLua : public Event {
public:
    EventLua() { }

    EventLua(std::string type, sol::object payload) : mPayload(payload), mType(type) { mIsLuaEvent = true; }
    
    virtual Type type() const override {
        return mType.c_str();
    }
    
    virtual std::string toString() const override {
        if (mPayload.is<std::string>()) return mPayload.as<std::string>();
        else return "?";
    }
    
    sol::object mPayload;
    std::string mType;
};

} // namespace safe

#endif /* EVENTLUA_H */

