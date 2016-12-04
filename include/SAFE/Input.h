#ifndef SINPUT_H
#define SINPUT_H

#include <SDL.h>

namespace safe {

class Input
{
    public:
        static bool IsDown(Uint8 key);
        static bool WasDown(Uint8 key);
        static bool IsReleased(Uint8 key);
        static bool IsPressed(Uint8 key);

        static void StartInput();
        static void UpdateInput();

    private:
        static int sNumKeys;
        static const Uint8* spKeyboard;
        static Uint8* spPrevKeyboard;
};

} // namespace safe

#endif // SINPUT_H
