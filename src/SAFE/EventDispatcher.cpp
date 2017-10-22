#include "SAFE/EventDispatcher.h"

#include <functional>
#include <iostream>
#include <algorithm>

namespace safe {

void EventDispatcher::PropagateEvents() {
    for (auto&& e : mEventList) {
        DispatchEvent<Event,EventHandler>(e->type(), *e, mEventHandler);
    }
    
    for (auto&& e : mEventList) {        
        const EventLua& luaEvent = static_cast<const EventLua&> (*e);
        DispatchEvent<sol::object,EventHandlerLua>(luaEvent.type(), luaEvent.mPayload, mEventHandlerLua);
    }
    
    
    mEventList.clear();
}


void EventDispatcher::Subscribe(EventDispatcher::ObserverID id, const Event::Type& descriptor, Func&& func) {
    mObserverSubs[id].insert(descriptor);
    mEventHandler[descriptor].push_back(std::pair< int, Func >(id, func));
}

EventDispatcher::ObserverID EventDispatcher::Subscribe(const Event::Type& descriptor, Func&& func) {
    auto id = GetNextID();
    Subscribe(id, descriptor, std::move(func));
    return id;
}

EventDispatcher::ObserverID EventDispatcher::SubscribeLua(const Event::Type& descriptor, FuncLua&& func) {
    auto id = GetNextID();
    mObserverSubs[id].insert(descriptor);
    mEventHandlerLua[descriptor].push_back(std::pair< int, FuncLua >(id, func));
    return id;
}

void EventDispatcher::Unsubscribe(EventDispatcher::ObserverID id, const Event::Type& descriptor) {
    auto l = std::vector<EventDispatcher::ObserverID>();
    l.push_back(id);
    Unsubscribe(l, descriptor);
}

void EventDispatcher::Unsubscribe(std::vector<EventDispatcher::ObserverID>& list, const Event::Type& descriptor) {
    EraseHandlers<EventHandler>(mEventHandler, list, descriptor);
    EraseHandlers<EventHandlerLua>(mEventHandlerLua, list, descriptor);
    
    for (auto id : list) {
        mObserverSubs[id].erase(descriptor);
    }
}

void EventDispatcher::Post(std::unique_ptr<Event>& pEvent) {
    if (mDebugLog) {
        std::cout << "[EventDispatcher]" << "(Event posted)" << pEvent->type()
                << ": " << pEvent->toString()
                << std::endl;
    }

    mEventList.push_back(std::move(pEvent));
}

void EventDispatcher::Post(Event* pEvent) {
    auto p = std::unique_ptr<Event>(pEvent);
    Post(p);
}

void EventDispatcher::PostLua(std::string type, sol::object payload){
    std::unique_ptr<Event> p = std::make_unique<EventLua>(type, payload);
    Post(p);
}

void EventDispatcher::Send(const Event& event){
    DispatchEvent<Event,EventHandler>(event.type(), event, mEventHandler);
    const EventLua& luaEvent = static_cast<const EventLua&> (event);
    DispatchEvent<sol::object,EventHandlerLua>(luaEvent.type(), luaEvent.mPayload, mEventHandlerLua);
}

EventDispatcher::ObserverID EventDispatcher::GetNextID() {
    return ++mCurrentID;
}

} // namespace safe
