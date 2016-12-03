#ifndef SSPRITESHEET_H
#define SSPRITESHEET_H

#include <SDL.h>

#include "SAFE/Texture.h"

namespace safe {

class SpriteSheet
{
    public:
        SpriteSheet(Texture texture, int width, int height);

        SpriteSheet(const SpriteSheet &obj);  // copy constructor

        void Render( const Camera& camera, const Vector3& worldPos, const Vector3& scale = Vector3(1),
                float angle = 0, const Vector2& center = Vector2(0.5,0.5)) const {

            Rect clip(mColStep * mCurrentCol, mRowStep * mCurrentRow, mColStep, mRowStep);

            mTexture.Render(camera, worldPos, scale, angle, center, clip);
        }

        int GetTotalFrames(){ return mSheetWidth * mSheetHeight; }
        int GetSheetWidth(){ return mSheetWidth; }
        int GetSheetHeight(){ return mSheetHeight; }

        int GetWidth(){ return mRowStep; }
        int GetHeight(){ return mColStep; }

        void SetFrame(int frame);
        void SetFrame(int row, int col);

        int GetFrame(){ return mCurrentRow * mSheetWidth + mCurrentCol; }
        int GetRow(){ return mCurrentRow; }
        int GetCol(){ return mCurrentCol; }

    protected:
    private:
        Texture mTexture;

        int mSheetWidth;
        int mSheetHeight;

        int mRowStep;
        int mColStep;

        int mCurrentCol = 0;
        int mCurrentRow = 0;
};

} // namespace safe

#endif // SSPRITESHEET_H
