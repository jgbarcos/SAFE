#ifndef EDRAGUNIT_H
#define EDRAGUNIT_H

#include "SAFE/Event.h"
#include "SAFE/Vector3.h"

class EDragUnit : public safe::Event {
public:

    EDragUnit()
    : mUnit(""), mIsPicked(false), mIsDropped(false) { }

    EDragUnit(safe::EntityEngine::EntityID id, bool picked, bool dropped)
    : mUnit(id), mIsPicked(picked), mIsDropped(dropped) { }

    safe::EntityEngine::EntityID mUnit;
    bool mIsPicked;
    bool mIsDropped;

    safe::Vector3 mDroppedPosition;

    std::string toString() const override {
        if (mIsPicked) {
            return mUnit + " " + "is picked";
        }
        else if (mIsDropped) {
            return mUnit + " " + "is dropped";
        }
        else {
            return "nothing happened";
        }
    }

    virtual Type type() const override {
        return "DragUnitEvent";
    }
};


#endif /* EDRAGUNIT_H */

