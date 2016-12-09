#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <memory>

#include <Box2D/Box2D.h>

#include "SAFE/CTransform.h"
#include "SAFE/CCollider.h"

namespace safe {

class SPhysics : public System
{
    public:
        SPhysics(float scale) :
            mScale(scale),
            mWorld(b2Vec2(0.0f,0.0f))
        {}
        
        void Update(float delta, std::vector<std::unique_ptr<Entity>>& entities) override {
            for(auto&& e : entities){
                auto pCollider = e->Get<CCollider>();
                if(!pCollider) continue;
                
                auto pTransform = e->Get<CTransform>();
                if(!pTransform) continue;
                
                if(!pCollider->mpBody) AddCollider(pCollider, pTransform);
                
                b2Vec2 vel;
                vel.x = pTransform->mVelocity.x / mScale;
                vel.y = pTransform->mVelocity.y / mScale;
                pCollider->mpBody->SetLinearVelocity(vel);
            }
            
            UpdateBox2D(delta);
            
            for(auto&& e : entities){
                // Preconditions
                auto pTransform = e->Get<CTransform>();
                if(!pTransform) continue;
                
                auto pCollider = e->Get<CCollider>();
                // Update Logic
                if(!pCollider)
                    pTransform->mPosition += pTransform->mVelocity * delta;
                else{
                    pTransform->mPosition.x = pCollider->mpBody->GetPosition().x * mScale;
                    pTransform->mPosition.y = pCollider->mpBody->GetPosition().y * mScale;
                }
            }
        }
        
        void AddCollider(CCollider* pCollider, CTransform* pTransform){
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(pTransform->mPosition.x / mScale, pTransform->mPosition.y / mScale);
            pCollider->mpBody = mWorld.CreateBody(&bodyDef);

            b2CircleShape pShape;
            pShape.m_p.Set(pCollider->mCenter.x/mScale, -pCollider->mCenter.y/mScale);
            pShape.m_radius = pCollider->mSize/mScale;
            
            b2FixtureDef pFixture;
            pFixture.shape = &pShape; //this is a pointer to the shape above
            pCollider->mpBody->CreateFixture(&pFixture); //add a fixture to the body

        }
        
        void UpdateBox2D(float delta){
            mWorld.Step(delta, 6, 2);
        }
        
        float mScale; // Pixels per 1 meter
        b2World mWorld;

};

} // namespace safe

#endif // PHYSICS_SYSTEM_H
