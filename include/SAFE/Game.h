#ifndef SGAME_H
#define SGAME_H

#include <SDL.h>
#include <sol.hpp>

#include "SAFE/Vector2.h"
#include "SAFE/Vector3.h"

namespace safe {

class Game
{
    public:
        Game(int screenWidth, int screenHeight, SDL_Window* pWindow, SDL_Renderer* pRenderer);

        void Start();
    protected:
    private:
        Vector2 mScreenSize;
        SDL_Window* mpWindow = nullptr;
        SDL_Renderer* mpRenderer = nullptr;
        sol::state mLua;
        
};

} // namespace safe

#endif // SGAME_H
