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
            x = a.x;
            y = a.y;
            width = b.x - a.x;
            height = b.y - a.y;
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
        bool Contains(Vector2 vector){
            double u = getU();
            double v = getV();
            
            return ( (x < vector.x && vector.x < u) || (u < vector.x && vector.x < x) )
                && ( (y < vector.y && vector.y < v) || (v < vector.y && vector.y < y) );           
        }

        double getX() const { return x; }
        double getY() const { return y; }
        double getU() const { return x + width; }
        double getV() const { return y + height; }
        double getWidth() const { return width; }
        double getHeight() const { return height; }
        Vector2 getInit() const { return Vector2(x,y); }
        Vector2 getEnd() const { return Vector2(getU(), getV()); }

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
