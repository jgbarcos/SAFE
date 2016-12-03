#ifndef TILE_H
#define TILE_H

#include "SAFE/GameObject.h"
#include "SAFE/Texture.h"

namespace safe {

class Tile : public safe::GameObject
{
    public:
        Tile(Texture texture, Vector3 position, int x, int y, int width, int height)
        : mTexture(texture), mX(x), mY(y), mWidth(width), mHeight(height) {
            mTransform.mPosition = position;
        }

        void Draw(Camera camera) override {
            mTexture.Render(camera, Vector3(mTransform.mPosition), Vector3(mTransform.mScale)
                , 0, Vector2(0.5,0.0));
        }
        void Update(float delta) override { /* do nothing */ };

        int mX;
        int mY;
        int mWidth;
        int mHeight;
        Texture mTexture;
};

} // namespace safe

#endif // TILE_H
