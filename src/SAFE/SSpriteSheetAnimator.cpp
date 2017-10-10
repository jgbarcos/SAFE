#include "SAFE/SSpriteSheetAnimator.h"

#include "SAFE/CSheetAnimation.h"
#include "SAFE/CSprite.h"
#include "SAFE/CTransform.h"

namespace safe {

void SSpriteSheetAnimator::Update(float delta, std::vector<Entity*>& entities) {
    for (auto&& e : entities) {
        // Preconditions
        auto pSheet = e->Get<CSheetAnimation>();
        if (!pSheet) continue;

        auto pSprite = e->Get<CSprite>();
        if (!pSprite) continue;

        auto pTransform = e->Get<CTransform>();
        if (!pTransform) continue;

        // Initializations
        if (!pSheet->mIsInit) {
            pSheet->mIsInit = true;

            pSprite->mClip = pSheet->GetCurrentFrame().first;
        }

        // Update Logic             
        auto anim = pSheet->mCurrentAnimation;

        bool should_play = pSheet->mIsPlaying;
        bool should_change = should_play && anim != pSheet->mCurrentAnimation;

        if (should_change) {
            pSheet->ChangeAnimation(anim);
        }

        if (should_play) {
            // Update Sheet
            if (pSheet->mIsReset) pSheet->ResetFrame();
            if (pSheet->mIsPlaying) pSheet->AdvanceTime(delta);

            // Update Sprite
            pSprite->mClip = pSheet->GetCurrentFrame().first;
        }
    }
}



} // namespace safe

