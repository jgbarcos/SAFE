#include "SAFE/EventDispatcher.h"

#include <functional>
#include <iostream>
#include <algorithm>

namespace safe {

void EventDispatcher::PropagateEvents() {
    for (auto&& e : mEventList) {
        Send(*e);
    }
    mEventList.clear();
}

void EventDispatcher::Subscribe(EventDispatcher::ObserverID id, const Event::Type& descriptor, Func&& func) {
    mObserverSubs[id].insert(descriptor);
    mEventsHandlers[descriptor].push_back(std::pair< int, Func >(id, func));
}

EventDispatcher::EventDispatcher::ObserverID EventDispatcher::Subscribe(const Event::Type& descriptor, Func&& func) {
    auto id = GetNextID();
    Subscribe(id, descriptor, std::move(func));
    return id;
}

void EventDispatcher::Unsubscribe(EventDispatcher::ObserverID id, const Event::Type& descriptor) {
    auto l = std::vector<EventDispatcher::ObserverID>();
    l.push_back(id);
    Unsubscribe(l, descriptor);
}

void EventDispatcher::Unsubscribe(std::vector<EventDispatcher::ObserverID>& list, const Event::Type& descriptor) {
    mEventsHandlers[descriptor].erase(
        std::remove_if(
                       mEventsHandlers[descriptor].begin(),
                       mEventsHandlers[descriptor].end(),
                       [&](std::pair< int, Func> elem) -> bool
                       {
                           return std::find(list.begin(), list.end(), elem.first) != list.end();
                       }
                       ),
        mEventsHandlers[descriptor].end()
        );

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

void EventDispatcher::Send(const Event& event) {
    if (mDebugLog) {
        std::cout << "[EventDispatcher]" << "(Event send)" << event.type()
                << ": " << event.toString()
                << std::endl;
    }

    std::vector< EventDispatcher::ObserverID > removeList;

    Event::Type const type = event.type();

    // Ignore events for which we do not have an observer (yet).
    if (mEventsHandlers.find(type) == mEventsHandlers.end())
        return;

    auto&& eventObservers = mEventsHandlers.at(type);

    for (auto&& obv : eventObservers) {
        bool valid = true;
        if (bool(obv.second)) {
            valid = obv.second(event);
        }

        if (!valid) {
            removeList.push_back(obv.first);
        }
    }

    Unsubscribe(removeList, type);
}

EventDispatcher::ObserverID EventDispatcher::GetNextID() {
    return ++mCurrentID;
}

} // namespace safe
