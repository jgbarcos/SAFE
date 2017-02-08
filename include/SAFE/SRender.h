#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <memory>

#include <SDL2_gfxPrimitives.h>

#include "SAFE/CSprite.h"
#include "SAFE/CTransform.h"
#include "SAFE/Camera.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"
#include "SAFE/TextureManager.h"

namespace safe {


class SRender : public System
{
    public:
        SRender(TextureManager* textureMgr, Camera* camera) :
            mpTextureManager(textureMgr),
            mpCamera(camera)
        {}

        void Update(float delta, std::vector<std::unique_ptr<Entity>>& entities) override;
        
        float GetDepth(CTransform* pTransform, CSprite* pSprite);
        
        bool dRenderPhysics = false;

    private:
        TextureManager* mpTextureManager;
        Camera* mpCamera;
};

} // namespace safe

#endif // RENDER_SYSTEM_H
