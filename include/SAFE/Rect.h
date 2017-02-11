#ifndef SRECT_H
#define SRECT_H

#include <sol.hpp>

#include "SAFE/Vector2.h"

namespace safe {
/**
 * Represents and axis aligned rectangle.
 */
class Rect
{
    public:
        Rect()
            : x(0), y(0), width(0), height(0) {};
        Rect(double x, double y, double w, double h)
            : x(x), y(y), width(w), height(h) {}
        Rect(const Vector2& a, const Vector2& b){
            x = std::min(a.get(0), b.get(0));
            y = std::min(a.get(1), b.get(1));
            width = std::max(a.get(0), b.get(0)) - x;
            height = std::max(a.get(1), b.get(1)) - y;
        }
        
        Rect(sol::table luaT){
            x = luaT.get_or("x", 0.0);
            y = luaT.get_or("y", 0.0);
            width = luaT.get_or("w", 0.0);
            height = luaT.get_or("h", 0.0);
        }
        
        Rect operator + (const Vector2& t) const {
            return Rect(x+t.x, y+t.y, width, height);
        }
        Rect operator - (const Vector2& t) const {
            return Rect(x-t.x, y-t.y, width, height);
        }
        bool IsInside(Vector2& v){
            return v.x > x && v.x < getU()
                    && v.y > y && v.y < getV();
        }

        double getX() const { return x; }
        double getY() const { return y; }
        double getU() const { return x + width; }
        double getV() const { return y + height; }
        double getWidth() const { return width; }
        double getHeight() const { return height; }

        double x; // min x
        double y; // min y
        double width;
        double height;
};

} // namespace safe

inline std::ostream& operator<<(std::ostream &strm, const safe::Rect& a) {
  return strm << "Rect(" << a.getX()<< "," << a.getY() << "," << a.getWidth() << "," << a.getHeight() << ")";
}

#endif // SRECT_H
