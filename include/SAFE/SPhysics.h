#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <memory>

#include <Box2D/Box2D.h>

#include "SAFE/CCollider.h"
#include "SAFE/CTransform.h"
#include "SAFE/Entity.h"
#include "SAFE/System.h"

namespace safe {

/**
 * Updates Box2D Engine and propagates its changes through CTransform components.
 * @param scale Box2D to pixel size ratio.
 */
class SPhysics : public System {
public:

    SPhysics(float scale) :
    mPixelsPerMeter(scale),
    mWorld(b2Vec2(0.0f, 0.0f)) { }

    void Update(float delta, std::vector<Entity*>& entities) override;

    void AddCollider(CCollider* pCollider, CTransform* pTransform);

    float mPixelsPerMeter; // Pixels per 1 meter
    b2World mWorld;

};

} // namespace safe

#endif // PHYSICS_SYSTEM_H
