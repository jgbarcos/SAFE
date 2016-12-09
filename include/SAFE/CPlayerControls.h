#ifndef CPLAYERCONTROLS_H
#define CPLAYERCONTROLS_H

namespace safe {

class CPlayerControls : public Component {
public:
    CPlayerControls()
    {}
    
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

