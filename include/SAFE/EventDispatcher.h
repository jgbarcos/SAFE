#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "SAFE/Event.h"

namespace safe {

class EventDispatcher {
public:
    typedef std::function< bool (const Event&) > Func;
    typedef int ObserverID;

    void PropagateEvents();

    void Subscribe(ObserverID id, const Event::Type& descriptor, Func&& func);
    ObserverID Subscribe(const Event::Type& descriptor, Func&& func);

    void Unsubscribe(ObserverID id, const Event::Type& descriptor);
    void Unsubscribe(std::vector<ObserverID>& list, const Event::Type& descriptor);

    /**
     * Post an event that will be later propagated to subscribers.
     * @param pEvent takes pointer ownership
     */
    void Post(std::unique_ptr<Event>& pEvent);
    void Post(Event* pEvent);

    /**
     * Send an event now. The event will be received and treated 
     * in this call by all the subscribes of the type of event.
     * @param event
     */
    void Send(const Event& event);

    ObserverID GetNextID();

public: // for testing
    ObserverID mCurrentID;
    std::unordered_map<Event::Type, std::vector< std::pair< ObserverID, Func > > > mEventsHandlers;
    std::unordered_map<int, std::set<Event::Type> > mObserverSubs;
    std::vector< std::unique_ptr<Event> > mEventList;
    bool mDebugLog = false;
};

} // namespace safe

#endif /* EVENTDISPATCHER_H */

