#ifndef ACTIONLUA_H
#define ACTIONLUA_H

#include "Action.h"

namespace safe {

class ActionLua : public Action {
public:

    virtual void Update(float delta) override {
        if (mFirstUpdate) {
            mFirstUpdate = false;
            mOnEnter(mSelf);
        }
        mHasFinished = mOnUpdate(mSelf, delta);
        if (mHasFinished) {
            mFirstUpdate = true;
            mOnExit(mSelf);
        }
    };

    void FromLua(sol::table luaT) {
        mOnUpdate = luaT.get < std::function<bool(sol::table, float) >>("on_update");
        mOnEnter = luaT.get < std::function<void(sol::table)>>("on_enter");
        mOnExit = luaT.get < std::function<void(sol::table)>>("on_exit");
        mSelf = luaT;
    }

private:
    bool mFirstUpdate = true;

    std::function<bool(sol::table, float) > mOnUpdate;
    std::function<void(sol::table) > mOnEnter;
    std::function<void(sol::table) > mOnExit;
    sol::table mSelf;
};

} // namespace safe

#endif /* ACTIONLUA_H */

