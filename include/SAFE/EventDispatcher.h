#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "SAFE/Event.h"
#include "SAFE/EventLua.h"

namespace safe {

class EventDispatcher {
public:
    typedef std::function< bool (const Event&) > Func;
    typedef std::function< bool (sol::object) > FuncLua;
    typedef int ObserverID;
    typedef std::unordered_map<Event::Type, std::vector< std::pair< ObserverID, Func > > > EventHandler;
    typedef std::unordered_map<Event::Type, std::vector< std::pair< ObserverID, FuncLua > > > EventHandlerLua;

    void PropagateEvents();

    void Subscribe(ObserverID id, const Event::Type& descriptor, Func&& func);
    ObserverID Subscribe(const Event::Type& descriptor, Func&& func);
    ObserverID SubscribeLua(const Event::Type& descriptor, FuncLua&& func);

    void Unsubscribe(ObserverID id, const Event::Type& descriptor);
    void Unsubscribe(std::vector<ObserverID>& list, const Event::Type& descriptor);

    /**
     * Post an event that will be later propagated to subscribers.
     * @param pEvent takes pointer ownership
     */
    void Post(std::unique_ptr<Event>& pEvent);
    void Post(Event* pEvent);
    void PostLua(std::string type, sol::object payload);

    /**
     * Send an event now. The event will be received and treated 
     * in this call by all the subscribes of the type of event.
     * @param event
     */
    void Send(const Event& event);
    

    ObserverID GetNextID();

public: // for testing
    ObserverID mCurrentID;
    EventHandler mEventHandler;
    EventHandlerLua mEventHandlerLua;
    std::unordered_map<int, std::set<Event::Type> > mObserverSubs;
    std::vector< std::unique_ptr<Event> > mEventList;
    bool mDebugLog = false;
    
private:
    template <typename E, typename T>
    void DispatchEvent(Event::Type const type, const E& payload, T& eventHandler){

        std::vector< EventDispatcher::ObserverID > removeList;

        // Ignore events for which we dont have any observer.
        if (eventHandler.find(type) != eventHandler.end()) {
            auto&& eventObservers = eventHandler.at(type);

            for (auto&& obv : eventObservers) {
                if (bool(obv.second)) {
                    bool valid = obv.second(payload);

                    if(!valid){
                        removeList.push_back(obv.first);
                    }
                }
            }

            Unsubscribe(removeList, type);
        }
    }
    
    template <typename T>
    void EraseHandlers(T& eventHandler, 
                       std::vector<EventDispatcher::ObserverID>& list, const Event::Type& descriptor){
        eventHandler[descriptor].erase(
            std::remove_if(
                eventHandler[descriptor].begin(),
                eventHandler[descriptor].end(),
                [&](std::pair< int, auto> elem) -> bool
                {
                    return std::find(list.begin(), list.end(), elem.first) != list.end();
                }
            ),
            eventHandler[descriptor].end()
        );
    }
};

} // namespace safe

#endif /* EVENTDISPATCHER_H */

