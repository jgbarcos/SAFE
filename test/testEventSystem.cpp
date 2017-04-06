#include "gtest/gtest.h"

#include "SAFE/Event.h"
#include "SAFE/EventDispatcher.h"


class DemoEvent : public safe::Event
{
public:
    DemoEvent(){}
    DemoEvent(std::string payload) : mPayload(payload) {}
    
    std::string mPayload;

    virtual Type type() const override
    {
        return "DemoEvent";
    }
};


class ClassObserver
{
public:
    std::string mReceivedPayload;
    
    bool handle( const safe::Event& e )
    {
        if( e.type() == DemoEvent().type() )
        {
          const DemoEvent& demoEvent = static_cast<const DemoEvent&>( e );          
          mReceivedPayload = demoEvent.mPayload;
        }
        
        return true; // function is still valid as a handler
    }
};


/* A payload is delivered correctly using a bound class */
TEST(EventSystem, PayloadBind) {
    ClassObserver receiver;
    safe::EventDispatcher dispatcher;

    auto id = dispatcher.GetNextID();
    
    dispatcher.Subscribe( id, DemoEvent().type(),
                          std::bind( &ClassObserver::handle, &receiver, std::placeholders::_1 ) );

    DemoEvent e("payload");
    dispatcher.Send( e );
    
    EXPECT_EQ(receiver.mReceivedPayload, e.mPayload);
}

/* A payload is delivered correctly using a Lambda function */
TEST(EventSystem, PayloadLambda) {
    std::string receivedPayload = "";
    
    safe::EventDispatcher dispatcher;

    auto id = dispatcher.GetNextID();
    
    auto f = [&](const safe::Event& e) -> bool { 
        if( e.type() == DemoEvent().type() )
        {
          const DemoEvent& demoEvent = static_cast<const DemoEvent&>( e );          
          receivedPayload = demoEvent.mPayload;
        }
        return true;
    };
    
    dispatcher.Subscribe( id, DemoEvent().type(), f);

    DemoEvent e("payload");
    dispatcher.Send( e );
    
    EXPECT_EQ(receivedPayload, e.mPayload);
}

/* Unsubscribing removes observer from listen an event */
TEST(EventSystem, Unsubscribe){
    ClassObserver receiver;
    safe::EventDispatcher dispatcher;
    
    auto id = dispatcher.GetNextID();
    
    dispatcher.Subscribe( id, DemoEvent().type(),
                          std::bind( &ClassObserver::handle, &receiver, std::placeholders::_1 ) );
    
    DemoEvent e = DemoEvent("payload");
    dispatcher.Send( e );
    
    EXPECT_EQ(receiver.mReceivedPayload, e.mPayload);
    
    dispatcher.Unsubscribe( id, DemoEvent().type() );
    
    DemoEvent e2("payload2");
    dispatcher.Send( e2 );
    
    EXPECT_EQ(receiver.mReceivedPayload, e.mPayload);
}

/* Dispatcher auto-removes dangling functions */
TEST(EventSystem, DanglingFunction){
    auto pReceiver = std::make_shared<ClassObserver>();
    safe::EventDispatcher dispatcher;
    
    auto secureWrapper = [&](const safe::Event& e) -> bool { 
        if (pReceiver) pReceiver->handle(e);
        return (bool)pReceiver;
    };
        
    auto id = dispatcher.Subscribe(DemoEvent().type(), secureWrapper);
    
    DemoEvent e;
    e.mPayload = "payload";
    dispatcher.Send(e);
    
    EXPECT_EQ(pReceiver->mReceivedPayload, e.mPayload);
    
    pReceiver.reset();
    
    DemoEvent e2("payload2");
    dispatcher.Send(e2);
    
    EXPECT_EQ(dispatcher.mEventsHandlers[e2.type()].size(), (unsigned int)0);
    EXPECT_EQ(dispatcher.mObserverSubs[id].size(), (unsigned int)0);
}
