#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include <functional>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "SAFE/Event.h"

namespace safe {

class EventDispatcher
{
public:
    typedef std::function< bool (const Event&) > Func;
    typedef int ObserverID;
    
    void PropagateEvents(){
        for( auto&& e : mEventList){
            Send(*e);
        }
        mEventList.clear();
    }
    
    void Subscribe(ObserverID id, const Event::Type& descriptor, Func&& func){
        mObserverSubs[id].insert(descriptor);
        mEventsHandlers[descriptor].push_back( std::pair< int, Func >(id, func) );
    }
    
    ObserverID Subscribe(const Event::Type& descriptor, Func&& func){
        auto id = GetNextID();
        Subscribe(id, descriptor, std::move(func));
        return id;
    }
    
    void Unsubscribe(ObserverID id, const Event::Type& descriptor){
        auto l = std::vector<ObserverID>();
        l.push_back(id);
        Unsubscribe(l, descriptor);
    }
    
    void Unsubscribe(std::vector<ObserverID>& list, const Event::Type& descriptor){
        mEventsHandlers[descriptor].erase(
            std::remove_if(
                mEventsHandlers[descriptor].begin(),
                mEventsHandlers[descriptor].end(),
                [&](std::pair< int, Func> elem) -> bool {
                    return std::find(list.begin(), list.end(), elem.first) != list.end();
                }
            ),
            mEventsHandlers[descriptor].end()
        );
            
        for(auto id : list){
            mObserverSubs[id].erase(descriptor);
        }
    }
    
    /**
     * Post an event that will be later propagated to subscribers.
     * @param pEvent takes pointer ownership
     */
    void Post( std::unique_ptr<Event>& pEvent ){
        if(mDebugLog){
            std::cout << "[EventDispatcher]" << "(Event posted)" << pEvent->type() 
                << ": " << pEvent->toString()
            << std::endl;
        }
        
        mEventList.push_back(std::move(pEvent));
    }
    void Post( Event* pEvent ){
        auto p = std::unique_ptr<Event>(pEvent);
        Post(p);
    }
    
    /**
     * Send an event now. The event will be received and treated 
     * in this call by all the subscribes of the type of event.
     * @param event
     */
    void Send( const Event& event ) 
    {     
        if(mDebugLog){
            std::cout << "[EventDispatcher]" << "(Event send)" << event.type() 
                << ": " << event.toString()
            << std::endl;
        }
        
        std::vector< ObserverID > removeList;
        
        Event::Type const type = event.type();

        // Ignore events for which we do not have an observer (yet).
        if( mEventsHandlers.find( type ) == mEventsHandlers.end() )
            return;

        auto&& eventObservers = mEventsHandlers.at( type );

        for( auto&& obv : eventObservers ){
            bool valid = true;
            if(bool(obv.second)){
                valid = obv.second( event );
            }
            
            if(!valid){
                removeList.push_back(obv.first);
            }
        }
        
        Unsubscribe(removeList, type);
    }
    
    ObserverID GetNextID(){
        return ++mCurrentID;
    }
    
public: // for testing
    ObserverID mCurrentID;
    std::unordered_map<Event::Type, std::vector< std::pair< ObserverID, Func > > > mEventsHandlers;
    std::unordered_map<int, std::set<Event::Type> > mObserverSubs;
    std::vector< std::unique_ptr<Event> > mEventList;
    bool mDebugLog = false;
};

} // namespace safe

#endif /* EVENTDISPATCHER_H */

