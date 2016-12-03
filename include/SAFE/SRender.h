#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <memory>

#include "SAFE/CSprite.h"
#include "SAFE/System.h"
#include "SAFE/TextureManager.h"

namespace safe {

class SRender : public System
{
    public:
        SRender(TextureManager* textureMgr, Camera* camera) :
            mpTextureManager(textureMgr),
            mpCamera(camera)
        {}

        void Update(float delta, std::vector<std::unique_ptr<Entity>>& entities) override {
            if(mpTextureManager && mpCamera){
                for(auto&& e : entities){
                    // Preconditions
                    auto pSprite = e->Get<CSprite>();
                    if(!pSprite) continue;

                    auto pTransform = e->Get<CTransform>();
                    if(!pTransform) continue;


                    // Update Logic
                    if(!pSprite->mIsLoaded){
                        pSprite->mpTexture = mpTextureManager->LoadFromFile(pSprite->mFilename);
                        pSprite->mIsLoaded = true;
                    }

                    auto pTex = pSprite->mpTexture.get();
                    int width = pTex->GetWidth();
                    int height = pTex->GetHeight();
                    
                    Vector2 scale (pTransform->mScale.x, pTransform->mScale.y);
                    float angle = 0;
                    
                    Rect clip = pSprite->mClip;
                    Rect pixelclip (clip.mX*width, clip.mY*height, clip.mWidth*width, clip.mHeight*height);

                    Vector2 screenPos = mpCamera->World2Screen(pTransform->mPosition);

                    pTex->Render(*mpCamera, screenPos, scale, angle, pSprite->mCenter, pixelclip);
                }
            }
        }

    private:
        TextureManager* mpTextureManager;
        Camera* mpCamera;
};

} // namespace safe

#endif // RENDER_SYSTEM_H
