#include "SAFE/Texture.h"

namespace safe {
  
void Texture::Render(const Camera& camera, const Vector3& worldPos, const Vector3& scale,
    float angle, const Vector2& center, const Rect& clip) const {
        Vector2 screenPos = camera.World2Screen(worldPos);
        Vector2 screenScale(scale.getX(), scale.getY());

        mpTexture->Render(camera, screenPos, screenScale, angle, center, clip);
}

void Texture::FillRect(Rect rect, Color color){
    int x = rect.getX();
    int y = rect.getY();
    int size_x = rect.getWidth();
    int size_y = rect.getHeight();

    if(x < 0){
        size_x -= x;
        x = 0;
    }

    if(y < 0){
        size_y -= y;
        y = 0;
    }

    Uint32 c = color.to32BE();

    for(int i=x; i<size_x+x && i < mpTexture->GetWidth(); i++){
        for(int j=y; j<size_y-y && j < mpTexture->GetHeight(); j++){
            mpTexture->PutPixel32(i,j,c);
        }
    }
}
  
} // namespace safe