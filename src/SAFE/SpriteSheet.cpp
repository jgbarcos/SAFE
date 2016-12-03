#include "SAFE/SpriteSheet.h"

#include <iostream>

namespace safe {
SpriteSheet::SpriteSheet(Texture texture, int width, int height)
:  mTexture(texture), mSheetWidth(width), mSheetHeight(height), mCurrentCol(0), mCurrentRow(0)
{
    mSheetWidth = width;
    mSheetHeight = height;

    mRowStep = mTexture.GetHeight() / mSheetHeight;
    mColStep = mTexture.GetWidth() / mSheetWidth;
}


void SpriteSheet::SetFrame(int frame)
{
    frame = frame % GetTotalFrames();
    int row = frame / mSheetHeight;
    int col = frame % mSheetWidth;
    SetFrame(row,col);
}

void SpriteSheet::SetFrame(int row, int col)
{
    row = row % mSheetWidth;
    col = col % mSheetHeight;
    mCurrentRow = row;
    mCurrentCol = col;
}

} // namespace safe
