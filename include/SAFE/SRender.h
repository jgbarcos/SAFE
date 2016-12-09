#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <memory>

#include "SAFE/CSprite.h"
#include "SAFE/System.h"
#include "SAFE/TextureManager.h"
#include "CTransform.h"

namespace safe {
    
typedef std::pair<float, Entity*> Depth;
struct DepthOrder
{
    bool operator()(Depth const& a, Depth const& b) const
    {
        return a.first > b.first;
    }
};

class SRender : public System
{
    public:
        SRender(TextureManager* textureMgr, Camera* camera) :
            mpTextureManager(textureMgr),
            mpCamera(camera)
        {}

        void Update(float delta, std::vector<std::unique_ptr<Entity>>& entities) override {
            if(mpTextureManager && mpCamera){
                std::priority_queue<Depth, std::vector<Depth>, DepthOrder > renderOrder;
                
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
                    
                    // Sort by depth
                    renderOrder.push(Depth(GetDepth(pTransform, pSprite), e.get()));
                }
                
                // Render accordingly to depth
                while(!renderOrder.empty()){
                    Entity* e = renderOrder.top().second;
                    renderOrder.pop();
                    
                    auto pSprite = e->Get<CSprite>();
                    auto pTransform = e->Get<CTransform>();
                    
                    auto pTex = pSprite->mpTexture.get();
                    int width = pTex->GetWidth();
                    int height = pTex->GetHeight();
                    
                    Vector2 scale (pTransform->mScale.x, pTransform->mScale.y);
                    float angle = 0;
                    
                    // Clip texture
                    Rect clip = pSprite->mClip;
                    Rect pixelclip (clip.mX*width, clip.mY*height, clip.mWidth*width, clip.mHeight*height);

                    
                    Vector2 screenPos = mpCamera->World2Screen(pTransform->mPosition);
                    
                    pTex->Render(*mpCamera, screenPos, scale, angle, pSprite->mCenter, pixelclip);
                }
            }
        }
        
        float GetDepth(CTransform* pTransform, CSprite* pSprite){
            float vert = 0.0; // 0.0 is pick the top y
            if(pSprite->mIsVertical) vert = 1.0; // 1.0 pick the bottom y
            float y = pTransform->mPosition.y;
            float center = pSprite->mCenter.y;

            float top = y - center * pSprite->mClip.mHeight * pTransform->mScale.y;
            float height = pSprite->mpTexture->GetHeight() * pSprite->mClip.mHeight * pTransform->mScale.y;
            return top + height * vert;    
        }

    private:
        TextureManager* mpTextureManager;
        Camera* mpCamera;
};

} // namespace safe

#endif // RENDER_SYSTEM_H
