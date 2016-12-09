#include "SAFE/Input.h"

#include <SDL.h>

#include <iostream>

namespace safe {

// Static data
int Input::sNumKeys;
const Uint8* Input::spKeyboard;
Uint8* Input::spPrevKeyboard;
        
Uint32 Input::sMouseState;
Uint32 Input::sPrevMouseState;

int Input::sMouseX;
int Input::sMouseY;

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

int Input::GetMouseX(){
    return sMouseX;
}
int Input::GetMouseY() {
    return sMouseY;
}
bool Input::IsMouseDown(int button){
    return sMouseState & SDL_BUTTON(button);
}
bool Input::WasMouseDown(int button){
    return sPrevMouseState & SDL_BUTTON(button);
}
bool Input::IsMouseReleased(int button){
    return WasMouseDown(button) && !IsMouseDown(button);
}
bool Input::IsMousePressed(int button){
    return !WasMouseDown(button) && IsMouseDown(button);
}

void Input::StartInput()
{
    spKeyboard = SDL_GetKeyboardState(&sNumKeys);
    spPrevKeyboard = (unsigned char*)malloc(sizeof(unsigned char) * sNumKeys);
    
    sMouseState = SDL_GetMouseState (&sMouseX, &sMouseY);
    sPrevMouseState = 0;
}

void Input::UpdateInput()
{
    memcpy(spPrevKeyboard, spKeyboard, sNumKeys);
    SDL_PumpEvents();
    
    sPrevMouseState = sMouseState;
    sMouseState = SDL_GetMouseState (&sMouseX, &sMouseY);
}

} // namespace safe
