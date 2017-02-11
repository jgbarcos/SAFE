#ifndef SINPUT_H
#define SINPUT_H

#include <SDL.h>

#include <string>

namespace safe {

/**
 * Updates and gives access to the input current state. Updated at Game loop.
 */
class Input
{
    public:
        static bool IsDown(Uint8 key);
        static bool WasDown(Uint8 key);
        static bool IsReleased(Uint8 key);
        static bool IsPressed(Uint8 key);
        
        static Uint8 KeyFromName(const std::string& name);
        
        static int GetMouseX();
        static int GetMouseY();
        static bool IsMouseDown(int button);
        static bool WasMouseDown(int button);
        static bool IsMouseReleased(int button);
        static bool IsMousePressed(int button);

        static void StartInput();
        static void UpdateInput();

    private:
        static int sNumKeys;
        static const Uint8* spKeyboard;
        static Uint8* spPrevKeyboard;
        
        static Uint32 sMouseState;
        static Uint32 sPrevMouseState;
        
        static int sMouseX;
        static int sMouseY;
};

} // namespace safe

#endif // SINPUT_H
