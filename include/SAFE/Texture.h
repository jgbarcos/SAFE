#ifndef STEXTURE_H
#define STEXTURE_H

#include <string>
#include <memory>
#include <bitset>

#include "SAFE/Camera.h"
#include "SAFE/Color.h"
#include "SAFE/Rect.h"
#include "SAFE/Vector2.h"
#include "SAFE/Vector3.h"
#include "SAFE/TextureWrapper.h"

namespace safe {

class Texture
{
    public:
        // Initializes variables
        Texture( std::shared_ptr<TextureWrapper> ptexture, std::string const id):
            mID(id), mpTexture(ptexture)
        {}

        bool isLoaded() const{
            return (bool)mpTexture;
        }

        void Render(const Camera& camera, const Vector3& worldPos, const Vector3& scale = Vector3(1),
            float angle = 0, const Vector2& center = Vector2(0.5,0.5), const Rect& clip = Rect()) const;

        // Gets image dimensions
        int GetWidth() const { return mpTexture->GetWidth(); };
        int GetHeight() const { return mpTexture->GetHeight(); };

        void FillRect(Rect rect, Color color);

        void PlotText(TTF_Font* font, const std::string& text, const Vector2& origin = Vector2(0), Color color = Color(0,0,0,255)){
            mpTexture->PlotText(font, text, origin, color);
        }

    private:
        std::string mID = "NO_ID";
        std::shared_ptr< TextureWrapper > mpTexture;

};

} // namespace safe
#endif // STEXTURE_H
