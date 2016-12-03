#ifndef SRECT_H
#define SRECT_H

#include "SAFE/Vector2.h"

namespace safe {

class Rect
{
    public:
        Rect()
            : mX(0), mY(0), mWidth(0), mHeight(0) {};
        Rect(double x, double y, double w, double h)
            : mX(x), mY(y), mWidth(w), mHeight(h) {}
        Rect(const Vector2& a, const Vector2& b){
            mX = std::min(a.get(0), b.get(0));
            mY = std::min(a.get(1), b.get(1));
            mWidth = std::max(a.get(0), b.get(0)) - mX;
            mHeight = std::max(a.get(1), b.get(1)) - mY;
        }

        double getX() const { return mX; }
        double getY() const { return mY; }
        double getWidth() const { return mWidth; }
        double getHeight() const { return mHeight; }

        double mX; // min x
        double mY; // min y
        double mWidth;
        double mHeight;
};

} // namespace safe

inline std::ostream& operator<<(std::ostream &strm, const safe::Rect& a) {
  return strm << "Rect(" << a.getX()<< "," << a.getY() << "," << a.getWidth() << "," << a.getHeight() << ")";
}

#endif // SRECT_H
