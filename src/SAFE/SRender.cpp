#include "SAFE/SRender.h"

#include <queue>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "SAFE/CCollider.h"
#include "SAFE/Input.h"
#include "SAFE/CTextBox.h"

namespace safe{

typedef std::pair<float, Entity*> Depth;
struct DepthOrder
{
    bool operator()(Depth const& a, Depth const& b) const
    {
        return a.first > b.first;
    }
};

void SRender::Update(float delta, std::vector<Entity*>& entities) {
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
            if(pSprite->mRender){
                renderOrder.push(Depth(GetDepth(pTransform, pSprite), e));
            }
        }

        // Render accordingly to depth
        while(!renderOrder.empty()){
            Entity* e = renderOrder.top().second;
            renderOrder.pop();

            auto pSprite = e->Get<CSprite>();
            auto pTransform = e->Get<CTransform>();

            Vector2 scale (pTransform->mScale.x, pTransform->mScale.y);
            float angle = 0;

            Vector2 screenPos = mpCamera->World2Screen(pTransform->mPosition);

            pSprite->mpTexture->Render(*mpCamera, screenPos, scale, angle, pSprite->mCenter, pSprite->GetPixelClip());
        }
        
        for(auto&& e : entities){
            // Preconditions
            auto pTextBox = e->Get<CTextBox>();
            if(!pTextBox) continue;

            auto pTransform = e->Get<CTransform>();
            if(!pTransform) continue;

            // Update Logic
            if(!pTextBox->mIsLoaded){
                auto size = pTextBox->mSize;
                pTextBox->mpTexture = mpTextureManager->CreateEmpty(size.x, size.y, pTextBox->mComponentName+"_"+e->GetName());
                pTextBox->mIsLoaded = true;
            }

            Vector2 scale (pTransform->mScale.x, pTransform->mScale.y);
            float angle = 0;

            Vector2 screenPos = mpCamera->World2Screen(pTransform->mPosition);
            
            if(pTextBox->mText != pTextBox->mLastText){
                pTextBox->mpTexture->FillRect(Rect(0,0,pTextBox->mpTexture->GetWidth(), pTextBox->mpTexture->GetHeight()), Color(0,0,0,0));
                pTextBox->mpTexture->PlotText(nullptr, pTextBox->mText, Vector2(0,0), pTextBox->mTextColor, pTextBox->mAutoResize);
                pTextBox->mLastText = pTextBox->mText;
            }
            pTextBox->mpTexture->Render(*mpCamera, screenPos, scale, angle, pTextBox->mCenter, Rect());
        }
        
        // Render sprite borders
        if(dRenderSpriteRect){
            for(auto&& e:entities){
                auto pSprite = e->Get<CSprite>();
                auto pTextBox = e->Get<CTextBox>();

                auto pTransform = e->Get<CTransform>();
                if(!pTransform) continue;

                Vector3 pos = pTransform->mPosition;

                auto renderer = mpCamera->getSDLRenderer();
                Vector2 screenPos = mpCamera->World2Screen(pos);

                if(pSprite){
                    Rect rect = pSprite->GetLocalRect(Vector2::Reduce(pTransform->mScale));
                    
                    Vector2 init = mpCamera->World2Screen(pos + Vector3(rect.x, rect.y, 0));
                    Vector2 end = mpCamera->World2Screen(pos + Vector3( rect.getU(), rect.getV(), 0 ));

                    Color red(155,0,0,255);
                    rectangleColor(renderer, init.x, init.y, end.x, end.y, red.toRGBA());

                    Color bright_red(255,0,0,255);
                    pixelColor(renderer, screenPos.x, screenPos.y, bright_red.toRGBA ());
                }
                if(pTextBox){
                    Rect rect = pTextBox->GetLocalRect(Vector2::Reduce(pTransform->mScale));
                    
                    Vector2 init = mpCamera->World2Screen(pos + Vector3(rect.x, rect.y, 0));
                    Vector2 end = mpCamera->World2Screen(pos + Vector3( rect.getU(), rect.getV(), 0 ));

                    Color red(155,0,0,255);
                    rectangleColor(renderer, init.x, init.y, end.x, end.y, red.toRGBA());

                    Color bright_red(255,0,0,255);
                    pixelColor(renderer, screenPos.x, screenPos.y, bright_red.toRGBA ());
                }
            }
        }


        // Render Debugging images
        if(dRenderPhysics){
            for(auto&& e : entities){
                // Preconditions
                auto pCollider = e->Get<CCollider>();
                if(!pCollider) continue;

                auto pTransform = e->Get<CTransform>();
                if(!pTransform) continue;

                Vector2 size = pCollider->mSize;
                Vector3 pos = pTransform->mPosition;
                pos.x += pCollider->mCenter.x;
                pos.y += pCollider->mCenter.y;

                Vector2 screenPos = mpCamera->World2Screen(pos);

                Color green(0,255,0,255);

                auto renderer = mpCamera->getSDLRenderer();
                if( pCollider->mShape == CCollider::Shape::CIRCLE ){
                    circleColor(renderer, screenPos.x, screenPos.y, size.x, green.toRGBA());
                }
                else if( pCollider->mShape == CCollider::Shape::RECTANGLE  )
                {
                    Vector2 init = screenPos + size;
                    Vector2 end = screenPos - size;
                    rectangleColor(renderer, init.x, init.y, end.x, end.y, green.toRGBA());
                }                   
            }
        }
    }
}
        
float SRender::GetDepth(CTransform* pTransform, CSprite* pSprite){
    Rect clip = pSprite->GetLocalRect() + Vector2::Reduce(pTransform->mPosition);

    float vert = 0.0; // 0.0 is pick the top y
    if(pSprite->mIsVertical) vert = 1.0; // 1.0 pick the bottom y
    
    if(Input::IsPressed(SDL_SCANCODE_F2)){
        std::cout << pTransform->mPosition.z << "," << std::endl;
    }
    
    if(pTransform->mPosition.z != pTransform->mPosition.z){
        std::cout << "error with:" << pTransform->mPosition << std::endl;
    }
    

    return pTransform->mPosition.z + 1000.0*(clip.y + clip.height * pTransform->mScale.y * vert); 
}

  
} // namespace safe

