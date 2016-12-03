#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

#include <string>

#include "SAFE/GameObject.h"
#include "SAFE/Texture.h"

namespace safe {

    class TextureObject : public GameObject
    {
        public:

            TextureObject(Texture texture)
                : mTexture(texture) {};

            void Draw(Camera camera) override {
                mTexture.Render(camera, Vector3(mTransform.mPosition), Vector3(mTransform.mScale));
            }
            void Update(float delta) override { /* do nothing */ };


            Texture mTexture;
    };
}

#endif // TEXTUREOBJECT_H
