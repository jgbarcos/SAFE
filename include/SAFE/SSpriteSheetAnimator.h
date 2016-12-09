#ifndef SPRITESHEETANIMATOR_SYSTEM_H
#define SPRITESHEETANIMATOR_SYSTEM_H

#include "SAFE/System.h"
#include "SAFE/CSheetAnimation.h"
#include "SAFE/CSprite.h"
#include "CTransform.h"

namespace safe {

class SSpriteSheetAnimator : public System {
public:    
    void Update(float delta, std::vector<std::unique_ptr<Entity>>&entities) override {
        for (auto&& e : entities) {
            // Preconditions
            auto pSheet = e->Get<CSheetAnimation>();
            if (!pSheet) continue;
            
            auto pSprite = e->Get<CSprite>();
            if (!pSprite) continue;
            
            auto pTransform = e->Get<CTransform>();
            if (!pTransform) continue; 

            // Update Logic 
            int anim = pSheet->mCurrentAnimation;
            if( pTransform->mVelocity.y < 0) anim = 0;
            else if(pTransform->mVelocity.y > 0) anim = 1;
            else if(pTransform->mVelocity.x < 0) anim = 2;
            else if(pTransform->mVelocity.x > 0) anim = 3;
            else anim = -1;
            if( anim != -1 && pSheet->mCurrentAnimation != anim){
                pSheet->ChangeAnimation(anim);
            }
            
            if( anim != -1 && pSheet->GetCurrentAnimation().size() > 0){
                // Update Sheet
                if( pSheet->mIsReset) pSheet->ResetFrame();
                if( pSheet->mIsPlaying) pSheet->AdvanceTime(delta);

                // Update Sprite
                pSprite->mClip = pSheet->GetCurrentFrame().first;
            }
        }
    }
    
};

} // namespace safe

#endif // SPRITESHEETANIMATOR_SYSTEM_H

