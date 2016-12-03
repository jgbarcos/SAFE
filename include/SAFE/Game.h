#ifndef SGAME_H
#define SGAME_H

#include "SAFE/Vector3.h"
#include "SAFE/World.h"

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

        World mWorld;
};

} // namespace safe

#endif // SGAME_H
