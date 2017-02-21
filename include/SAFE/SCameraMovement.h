#ifndef CAMERAMOVEMENT_SYSTEM_H
#define CAMERAMOVEMENT_SYSTEM_H

#include <vector>

#include "SAFE/Entity.h"
#include "SAFE/System.h"

#include "SAFE/Camera.h"
#include "SAFE/Input.h"

namespace safe {

class SCameraMovement : public System {
public:    
    SCameraMovement(Camera* camera) : mpCamera(camera) {}
    
    void Update(float delta, std::vector<Entity*>& entities) override {
        if(Input::IsMousePressed(SDL_BUTTON_MIDDLE)){
            origMouse = mpCamera->Screen2Camera( Input::GetMousePos() );
            origPos = mpCamera->mTransform.mPosition;
        }
        if(Input::IsMouseDown(SDL_BUTTON_MIDDLE)){          
            Vector3 currentMouse = mpCamera->Screen2Camera( Input::GetMousePos() );
            mpCamera->mTransform.mPosition = origPos + (origMouse - currentMouse);
        }
        
        int wheel = Input::GetMouseWheel();
        if(wheel < 0){
            mpCamera->mTransform.mScale = mpCamera->mTransform.mScale * (1.0/1.1);
        }
        else if(wheel > 0){
            mpCamera->mTransform.mScale = mpCamera->mTransform.mScale * 1.1;
        }
    } 
    
private:
    Camera* mpCamera;
    
    Vector3 origMouse;
    Vector3 origPos;
};

} // namespace safe

#endif /* CAMERAMOVEMENT_SYSTEM_H */

