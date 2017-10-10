#ifndef SVECTOR2_H
#define SVECTOR2_H

#include <ostream>
#include <stdexcept>
#include <math.h>

#include <sol.hpp>

#include "SAFE/Vector3.h"

namespace safe {

class Vector2 {
public:

    Vector2(double xv, double yv)
    : x(xv), y(yv) { };

    Vector2(double val = 0.0f)
    : x(val), y(val) { };

    Vector2(const Vector2& other) {
        x = other.x;
        y = other.y;
    };

    Vector2(sol::table luaT) {
        x = luaT.get_or("x", 0.0);
        y = luaT.get_or("y", 0.0);
    }

    Vector2 operator+(const Vector2& rh) const {
        return Vector2(x + rh.x, y + rh.y);
    }

    Vector2 operator-(const Vector2& rh) const {
        return Vector2(x - rh.x, y - rh.y);
    }

    Vector2 operator*(const Vector2& rh) const {
        return Vector2(x * rh.x, y * rh.y);
    }

    Vector2 operator/(const Vector2& rh) const {
        return Vector2(x / rh.x, y / rh.y);
    }

    Vector2 operator*(const float v) const {
        return Vector2(x*v, y * v);
    }

    Vector2 operator/(const float v) const {
        return Vector2(x / v, y / v);
    }

    void normalize() {
        float l = length();
        x = x / l;
        y = y / l;
    }
    // Use it if length was previously calculated

    void normalize(float length) {
        x = x / length;
        y = y / length;
    }

    float length() {
        return sqrt(x * x + y * y);
    }

    double x;
    double y;

    double getX() const { return x; }
    double getY() const { return y; }

    double get(int i) const {
        if (i == 0) return x;
        else if (i == 1) return y;
        else throw std::out_of_range("SVector2 only accepts indices: 0 (x) and 1 (y)");
    }

    // TODO: consider moving these methods to a "utils" class

    static Vector2 Reduce(const Vector3& v, int dim = 2) {
        if (dim == 0) return Vector2(v.get(1), v.get(2));
        else if (dim == 1) return Vector2(v.get(0), v.get(2));
        else if (dim == 2) return Vector2(v.get(0), v.get(1));
        else throw std::out_of_range("Vector2 can only be reduced from Vector3 with dims 0, 1 or 2");
    }

    static Vector3 Extend(const Vector2& v, double dimval = 0.0, int dim = 2) {
        if (dim == 0) return Vector3(dimval, v.get(0), v.get(1));
        else if (dim == 1) return Vector3(v.get(0), dimval, v.get(1));
        else if (dim == 2) return Vector3(v.get(0), v.get(1), dimval);
        else throw std::out_of_range("Vector2 can only be extended to Vector3 with dims 0, 1 or 2");
    }
};

} // namespace safe

inline std::ostream& operator<<(std::ostream& strm, const safe::Vector2& a) {
    strm << "(" << a.x << "," << a.y << ")";
    return strm;
}

#endif // SVECTOR2_H
