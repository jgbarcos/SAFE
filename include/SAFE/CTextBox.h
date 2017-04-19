#ifndef CTEXTBOX_H
#define CTEXTBOX_H

#include "SAFE/Component.h"
#include "SAFE/Texture.h"
#include "SAFE/Vector2.h"
#include "SAFE/Color.h"

namespace safe {

class CTextBox : public Component
{
public:
    CTextBox() :
        mText(""),
        mAutoResize(true),
        mSize(1,1),
        mCenter(0.5,0.5),  // Middle point
        mTextColor(0,0,0,255)
    {   mComponentName = "TextBoxComponent"; }
        
    void FromLua(sol::table luaT) override {
        mText = luaT.get<std::string>("text");
        mAutoResize = luaT.get_or("auto_resize", true);
        sol::table t = luaT.get<sol::table>("size");
        if (t.valid()) mSize = Vector2(t);
        t =  luaT.get<sol::table>("center") ;
        if (t.valid()) mCenter = Vector2(t);
    }
    
    // Returns the Size of the sprite with clipping applied in local coordinates (no zoom involved)
    Rect GetLocalRect(Vector2 scale = Vector2(1,1)){
        int w = mpTexture->GetWidth();
        int h = mpTexture->GetHeight();
        return Rect(-w*scale.x*mCenter.x, -h*scale.y*mCenter.y, w*scale.x, h*scale.y);
    }

    // Required
    std::string mText;

    // Optional
    bool mAutoResize;
    Vector2 mSize;
    Vector2 mCenter;
    Color mTextColor;
    bool mRender = true;

    // Generated
    std::string mLastText;
    bool mIsLoaded = false;
    std::shared_ptr<Texture> mpTexture;
};

} // namespace safe


#endif /* CTEXTBOX_H */

