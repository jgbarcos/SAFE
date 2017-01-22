#ifndef CPLAYERCONTROLS_H
#define CPLAYERCONTROLS_H

#include "SAFE/Input.h"
#include "SAFE/Vector2.h"

#include <iostream>

namespace safe {

class CPlayerControls : public Component {
public:
    CPlayerControls()
    {}
    
    CPlayerControls(sol::table luaT) : CPlayerControls() {
        mKeyMoveUp = Input::KeyFromName( luaT.get<std::string>("up") );
        mKeyMoveDown = Input::KeyFromName( luaT.get<std::string>("down") );
        mKeyMoveLeft = Input::KeyFromName( luaT.get<std::string>("left") );
        mKeyMoveRight = Input::KeyFromName( luaT.get<std::string>("right") );
    }
    
    // Required
    Uint8 mKeyMoveUp;
    Uint8 mKeyMoveDown;
    Uint8 mKeyMoveLeft;
    Uint8 mKeyMoveRight;
    
    // Provided
    Vector2 mDirection;
    
    // Debug
    bool dFollowMouse = false;
};

} // namespace safe
#endif /* CPLAYERCONTROLS_H */

