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
            mCurrentAnimation(0),
            mCurrentFrame(0),
            mIsPlaying(true),
            mIsReset(true)
        {}
        
        CSheetAnimation(sol::table luaT) : CSheetAnimation() {
            sol::table t = luaT.get<sol::table>("animations");
            if(t.valid()){
                auto fx = [&](sol::object key, sol::object value){
                    createAnimation( key.as<AnimIndex>(), value.as<sol::table>() );
                };
                t.for_each(fx);
            }
            mCurrentAnimation = luaT.get_or("start_anim", mAnimations.begin()->first);
        }
        
        
        void createAnimation(AnimIndex key, sol::table luaT){
            mAnimations[key] = std::vector<Frame>();

            for(size_t i=0; i<luaT.size(); i++){
                sol::table t = luaT.get<sol::table>(i+1);
                if(t.valid()) {
                    std::pair<Rect, float> frame = std::pair<Rect, float> (Rect( t.get<sol::table>("rect") ), t["time"]); 
                    mAnimations[key].push_back(frame);
                }
            }
        }


        // Required
        std::map< AnimIndex, Animation > mAnimations;
        AnimIndex mCurrentAnimation;
        
        // Generated
        bool mIsInit = false;
        int mCurrentFrame;
        float mTimeRemaining;
        bool mIsPlaying;
        bool mIsReset;
        

        // Functions (Const functions should not modify the component)
        const Animation GetCurrentAnimation() const {
            return mAnimations.at(mCurrentAnimation);
        }
        
        const Frame GetCurrentFrame() const {
            return GetCurrentAnimation().at(mCurrentFrame);
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
