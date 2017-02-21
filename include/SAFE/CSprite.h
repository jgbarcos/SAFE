#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <memory>

#include "SAFE/Component.h"
#include "SAFE/Texture.h"
#include "SAFE/Vector2.h"

namespace safe {

class CSprite : public Component
{
public:
    CSprite() :
        mFilename(""),
        mCenter(Vector2(0.5,0.5)),  // Middle point
        mClip(Rect(0,0,1,1))       // Whole texture
    {   mComponentName = "SpriteComponent"; }
        
    void FromLua(sol::table luaT) override {
        mFilename = luaT.get<std::string>("filename");

        sol::table t =  luaT.get<sol::table>("center") ;
        if (t.valid()) mCenter = Vector2(t);

        mIsVertical = luaT.get_or<bool>("is_vertical", mIsVertical);
    }

    // Returns the clip over the texture measured in pixels
    Rect GetPixelClip(){
        int width = mpTexture->GetWidth();
        int height = mpTexture->GetHeight();
        return Rect(mClip.x*width, mClip.y*height, mClip.width*width, mClip.height*height);
    }

    // Returns the Size of the sprite with clipping applied in local coordinates (no zoom involved)
    Rect GetLocalRect(Vector2 scale = Vector2(1,1)){
        Rect pclip = GetPixelClip();
        pclip.width *= scale.x;
        pclip.height *= scale.y;
        return Rect(-pclip.width*mCenter.x, -pclip.height*mCenter.y, pclip.width, pclip.height);
    }



    // Required
    std::string mFilename;

    // Optional
    Vector2 mCenter;
    Rect mClip;
    bool mIsVertical = true; // true: plane(x,z) like a wall or character
                             // false: plane(x,y) like a floor tile
    bool mRender = true;

    // Generated
    bool mIsLoaded = false;
    std::shared_ptr<Texture> mpTexture;
};

} // namespace safe

#endif // SPRITE_COMPONENT_H
