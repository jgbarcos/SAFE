#include "SAFE/Input.h"

#include <SDL.h>

namespace safe {

// Static data
int Input::sNumKeys;
const Uint8* Input::spKeyboard;
Uint8* Input::spPrevKeyboard;

Input::Input()
{
    //ctor
}

Input::~Input()
{
    //dtor
}

bool Input::IsDown(Uint8 key)
{
    return spKeyboard[key];
}

bool Input::WasDown(Uint8 key)
{
    return spPrevKeyboard[key];
}

bool Input::IsReleased(Uint8 key)
{
    return WasDown(key) && !IsDown(key);
}

bool Input::IsPressed(Uint8 key)
{
    return !WasDown(key) && IsDown(key);
}

void Input::StartInput()
{
    spKeyboard = SDL_GetKeyboardState(&sNumKeys);
    spPrevKeyboard = (unsigned char*)malloc(sizeof(unsigned char) * sNumKeys);
}

void Input::UpdateInput()
{
    memcpy(spPrevKeyboard, spKeyboard, sNumKeys);
    SDL_PumpEvents();
}

} // namespace safe
