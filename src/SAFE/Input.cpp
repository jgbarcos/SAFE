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

int Input::sMouseWheelY;

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

Uint8 Input::KeyFromName(const std::string& name){
    return SDL_GetScancodeFromName( name.c_str() );
}

int Input::GetMouseX(){
    return sMouseX;
}
int Input::GetMouseY() {
    return sMouseY;
}
Vector2 Input::GetMousePos(){
    return Vector2(sMouseX, sMouseY);
}
int Input::GetMouseWheel() {
    return sMouseWheelY;
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
    
    sMouseWheelY = 0;
}


void Input::ProcessEvent(SDL_Event* event){
    if( event->type == SDL_MOUSEWHEEL){
        sMouseWheelY = event->wheel.y;
    }
}

} // namespace safe
