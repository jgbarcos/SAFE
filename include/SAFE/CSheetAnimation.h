#ifndef SHEET_ANIMATION_COMPONENT_H
#define SHEET_ANIMATION_COMPONENT_H

#include <vector>
#include <map>

#include "SAFE/Rect.h"
#include "SAFE/Vector3.h"
#include "SAFE/Component.h"

namespace safe {

class CSheetAnimation : public Component
{
    public:
        typedef std::pair<Rect, float> Frame;
        typedef std::vector < Frame > Animation;
        typedef int AnimIndex;
        
        CSheetAnimation() :
            mCurrentFrame(0),
            mIsPlaying(true),
            mIsReset(true)
        {}

        // Required
        std::map< AnimIndex, Animation > mAnimations;
        AnimIndex mCurrentAnimation;
        
        // Generated
        int mCurrentFrame;
        float mTimeRemaining;
        bool mIsPlaying;
        bool mIsReset;
        

        // Functions (Const functions should not modify the component)
        const Animation GetCurrentAnimation() const {
            return mAnimations.at(mCurrentAnimation);
        }
        
        const Frame GetCurrentFrame() const {
            return GetCurrentAnimation()[mCurrentFrame];
        }
        
        void ChangeAnimation(AnimIndex index) {
            mCurrentAnimation = index;
            mCurrentFrame = 0;
            ResetFrame();
        }
        
        void ResetFrame(){
            mTimeRemaining = GetCurrentFrame().second;
            mIsReset = false;
        }
        
        void AdvanceTime(float delta){
            mTimeRemaining -= delta;
            if( mTimeRemaining < 0){
                mCurrentFrame = (mCurrentFrame+1) % GetCurrentAnimation().size();
                ResetFrame();
            }
        }

};

} // namespace safe

#endif // SHEET_ANIMATION_COMPONENT_H
