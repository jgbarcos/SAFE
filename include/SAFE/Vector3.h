#ifndef SVECTOR3_H
#define SVECTOR3_H


#include <ostream>
#include <stdexcept>

#include "SAFE/Vector3.h"

namespace safe {

class Vector3
{
    public:
        Vector3(double xv, double yv, double zv)
            : x(xv), y(yv), z(zv){};
        Vector3(double val = 0.0f)
            : x(val), y(val), z(val){};
        Vector3(const Vector3& other) {
            x = other.x;
            y = other.y;
            z = other.z;
        };

        Vector3 operator + (const Vector3& rh) const {
            return Vector3(x+rh.x, y+rh.y, z+rh.z);
        }
        Vector3& operator += (const Vector3 rh){
            x += rh.x;
            y += rh.y;
            z += rh.z;
            return *this;
        }
        
        Vector3 operator * (const float v) const {
            return Vector3(x*v, y*v, z*v);
        }

        double get(int index) const {
            if(index == 0) return x;
            else if(index == 1) return y;
            else if(index == 2) return z;
            else throw std::out_of_range ("SVector3 only accepts indices: 0 (x), 1 (y) and 2 (z)");
        }
        double getX() const { return x; }
        double getY() const { return y; }
        double getZ() const { return z; }

        double x;
        double y;
        double z;
};

} // namespace safe

inline std::ostream& operator<<(std::ostream &strm, const safe::Vector3& a) {
  return strm << "(" << a.getX()<< "," << a.getY() << "," << a.getZ() <<")";
}

#endif // SVECTOR3_H
