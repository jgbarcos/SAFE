#ifndef ACTION_H
#define ACTION_H

namespace safe {

class Action {
public:

    virtual ~Action() { }

    virtual void Update(float delta) {
        mHasFinished = true;
    };
    bool mHasFinished = false;
};

} // namespace safe

#endif /* ACTION_H */

