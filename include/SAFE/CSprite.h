#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <memory>

#include "SAFE/Component.h"
#include "SAFE/TextureWrapper.h"
#include "SAFE/Vector2.h"

namespace safe {

class CSprite : public Component
{
    public:
        CSprite() :
            mFilename(""),
            mCenter(Vector2(0.5,0.5)),  // Middle point
            mClip(Rect(0,0,1,1))        // Whole texture
        {}

        // Required
        std::string mFilename;

        // Optional
        Vector2 mCenter;
        Rect mClip;
        bool mIsVertical = true; // true: plane(x,z) like a wall or character
                                  // false: plane(x,y) like a floor tile

        // Generated
        bool mIsLoaded = false;
        std::shared_ptr<TextureWrapper> mpTexture;

};

} // namespace safe

#endif // SPRITE_COMPONENT_H
