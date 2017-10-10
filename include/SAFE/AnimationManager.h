#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <vector>

namespace safe {

template <typename T>
class AnimationManager {
public:

    AnimationManager() {
        mCurrentItem = 0;
    }

    void AddFrame(T obj, float time) {
        mFrames.push_back({obj, time});

        if (mFrames.size() == 1) {
            mCurrentItem = 0;
            mTimeRemaining = time;
        }
    }

    int GetSize() {
        return mFrames.size();
    }

    T GetCurrent() {
        return mFrames[mCurrentItem].first;
    }

    void SetCurrent(int frame) {
        mCurrentItem = frame % mFrames.size();
        mTimeRemaining = mFrames[mCurrentItem].second;
    }

    void AdvanceTime(float delta) {
        if (mFrames.size() > 0) {
            mTimeRemaining -= delta;

            if (mTimeRemaining < 0) {
                SetCurrent(mCurrentItem + 1);
            }
        }
    }
private:
    std::vector<std::pair<T, float>> mFrames;
    float mTimeRemaining;
    int mCurrentItem;

};

} // namespace safe

#endif // ANIMATIONMANAGER_H
