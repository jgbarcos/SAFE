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
    typedef std::string AnimIndex;

    CSheetAnimation() :
        mCurrentAnimation(),
        mCurrentFrame(0),
        mIsPlaying(true),
        mIsReset(true),
        mPlayOnce(false)
    {   mComponentName = "SheetAnimationComponent"; }
    
    std::string PrepareLua(sol::state_view& lua) override {
        lua.new_usertype<CSheetAnimation>(
            "SheetAnimationComponent", 
            "change_animation", &CSheetAnimation::ChangeAnimation,
            "is_playing", &CSheetAnimation::mIsPlaying,
            "play_once", &CSheetAnimation::mPlayOnce
        );
        
        return "get_sheet_animation";
    }

    void FromLua(sol::table luaT) override {
        sol::table t = luaT.get<sol::table>("animations");
        if(t.valid()){
            auto fx = [&](sol::object key, sol::object value){
                createAnimation( key.as<AnimIndex>(), value.as<sol::table>() );
            };
            t.for_each(fx);
        }
        auto index = luaT.get_or("start_anim", mAnimations.begin()->first);
        index = ErrorCheckAnimationExists(index);
        
        mCurrentAnimation = index;
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
    bool mPlayOnce;


    // Functions (Const functions should not modify the component)
    const Animation GetCurrentAnimation() const {
        return mAnimations.at(mCurrentAnimation);
    }

    const Frame GetCurrentFrame() const {
        return GetCurrentAnimation().at(mCurrentFrame);
    }

    void ChangeAnimation(AnimIndex index) {
        index = ErrorCheckAnimationExists(index);
        
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
            int numFrames = GetCurrentAnimation().size();
            
            mIsPlaying = !(mPlayOnce && mCurrentFrame == numFrames-1);
            
            mCurrentFrame = (mCurrentFrame+1) % numFrames;
            ResetFrame();
        }
    }
    
    AnimIndex ErrorCheckAnimationExists(AnimIndex index){
        if(mAnimations.find(index) == mAnimations.end()){
            std::cout << "[CSheetAnimation]" << "(Error) " 
                << "animation \"" << index << "\" does not exist"
            << std::endl;
            
            // Use valid animation
            index = mAnimations.begin()->first;
        }
        return index;
    }

};

} // namespace safe

#endif // SHEET_ANIMATION_COMPONENT_H
