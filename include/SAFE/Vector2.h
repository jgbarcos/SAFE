#ifndef SVECTOR2_H
#define SVECTOR2_H

#include <ostream>
#include <stdexcept>
#include <math.h>

#include <sol.hpp>

namespace safe {

class Vector2
{
    public:
        Vector2(double xv, double yv)
            : x(xv), y(yv) {};
        Vector2(double val = 0.0f)
            : x(val), y(val) {};
        Vector2(const Vector2& other) {
            x = other.x;
            y = other.y;
        };
        
        Vector2(sol::table luaT){
            x = luaT.get_or("x", 0.0);
            y = luaT.get_or("y", 0.0);
        }

        Vector2 operator + (const Vector2& t) const {
            return Vector2(x+t.x, y+t.y);
        }
        Vector2 operator - (const Vector2& t) const {
            return Vector2(x-t.x, y-t.y);
        }
        
        Vector2 operator * (const float v) const {
            return Vector2(x*v, y*v);
        }
        Vector2 operator / (const float v) const {
            return Vector2(x/v, y/v);
        }
        
        void normalize(){
            float l = length();
            x = x/l;
            y = y/l;
        }
        // Use it if length was previously calculated
        void normalize(float length){
            x = x/length;
            y = y/length;
        }
        
        float length(){
            return sqrt(x*x+y*y);
        }

        double x;
        double y;

        double getX() const { return x; }
        double getY() const { return y; }
        double get(int i) const {
            if(i == 0) return x;
            else if (i == 1) return y;
            else throw std::out_of_range ("SVector2 only accepts indices: 0 (x) and 1 (y)");
        }
};

} // namespace safe

inline std::ostream& operator<<(std::ostream& strm, const safe::Vector2& a) {
    strm << "(" << a.x << "," << a.y << ")";
    return strm;
}

#endif // SVECTOR2_H
