#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SDL_ttf.h>

#include <memory>

#include <SAFE/GameObject.h>
#include <SAFE/Texture.h>

namespace safe {

class TextBox : public GameObject
{
    public:
        TextBox(TTF_Font* font, Texture texture, float updateRate = 0.0f)
            : mFont(font), mTexture(texture), mUpdateRate(updateRate) {}


        virtual void Update(float delta){
            mTimeToUpdate -= delta;
            if(mLastText != mText && mFont != nullptr && mTimeToUpdate <= 0){
                mLastText = mText;
                mTimeToUpdate = mUpdateRate;

                mTexture.FillRect(Rect(0,0,mTexture.GetWidth(), mTexture.GetHeight()), Color(0,0,0,0));
                mTexture.PlotText(mFont, mText, Vector2(0), Color(255,255,255,255));
            }
        }

        virtual void Draw(safe::Camera camera){
            mTexture.Render(camera, mTransform.mPosition, mTransform.mScale);
        }

        Texture mTexture;
        float mUpdateRate = 0.0f;
        std::string mText = "";
    private:
        TTF_Font* mFont = nullptr;
        float mTimeToUpdate = 0.0f;
        std::string mLastText = "";

};

} // namespace safe

#endif // TEXTBOX_H
