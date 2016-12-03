#ifndef CPLAYERCONTROLS_H
#define CPLAYERCONTROLS_H

namespace safe {

class CPlayerControls : public Component {
public:
    CPlayerControls()
    {}
    
    Uint8 mKeyMoveUp;
    Uint8 mKeyMoveDown;
    Uint8 mKeyMoveLeft;
    Uint8 mKeyMoveRight;
};

} // namespace safe
#endif /* CPLAYERCONTROLS_H */

