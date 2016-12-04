#ifndef SPRITESHEETANIMATOR_SYSTEM_H
#define SPRITESHEETANIMATOR_SYSTEM_H

#include "SAFE/System.h"
#include "SAFE/CSheetAnimation.h"
#include "SAFE/CSprite.h"

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

            // Update Logic
            if( pSheet->GetCurrentAnimation().size() > 0){
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

