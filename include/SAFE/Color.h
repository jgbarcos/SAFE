#ifndef SCOLOR_H
#define SCOLOR_H

namespace safe {

class Color
{
    public:
        Color(int red, int green, int blue, int alpha = 255)
            : r(red), g(green), b(blue), a(alpha) {}

        uint8_t r,g,b,a;



        static Color from32BE(uint32_t value){
            uint8_t red = value >> 24;
            uint8_t green = value >> 16;
            uint8_t blue = value >> 8;
            uint8_t alpha = value;
            return Color(red,green,blue,alpha);
        }

        uint32_t to32BE() const {
            return ((int)r << 24) | ((int)g << 16) | ((int)b << 8) | ((int)a);
        }


    protected:
    private:
};

} // namespace safe

inline std::ostream& operator<<(std::ostream &strm, const safe::Color& a) {
  return strm << "c("
        << (unsigned int)a.r << ","
        << (unsigned int)a.g << ","
        << (unsigned int)a.b << ","
        << (unsigned int)a.a << ")";
}

#endif // SCOLOR_H
