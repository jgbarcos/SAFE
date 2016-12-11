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
            mPixelsPerMeter(scale),
            mWorld(b2Vec2(0.0f,0.0f))
        {}
        
        void Update(float delta, std::vector<std::unique_ptr<Entity>>& entities) override {
            // Pre-Physics
            for(auto&& e : entities){
                // Preconditions
                auto pCollider = e->Get<CCollider>();
                if(!pCollider) continue;
                
                auto pTransform = e->Get<CTransform>();
                if(!pTransform) continue;
                
                // Update Logic                
                if(!pCollider->mpBody) AddCollider(pCollider, pTransform);
                
                b2Vec2 vel;
                vel.x = pTransform->mVelocity.x / mPixelsPerMeter;
                vel.y = pTransform->mVelocity.y / mPixelsPerMeter;
                pCollider->mpBody->SetLinearVelocity(vel);
            }
            
            // Update Physics System
            UpdateBox2D(delta);
            
            // Post-Physics
            for(auto&& e : entities){
                // Preconditions
                auto pTransform = e->Get<CTransform>();
                if(!pTransform) continue;
                
                auto pCollider = e->Get<CCollider>();
                // Update Logic
                if(!pCollider)
                    pTransform->mPosition += pTransform->mVelocity * delta;
                else{
                    pTransform->mPosition.x = pCollider->mpBody->GetPosition().x * mPixelsPerMeter;
                    pTransform->mPosition.y = pCollider->mpBody->GetPosition().y * mPixelsPerMeter;
                }
            }
        }
        
        void AddCollider(CCollider* pCollider, CTransform* pTransform){
            // Configure body
            b2BodyDef bodyDef;
            if(pCollider->mType == CCollider::Type::DYNAMIC) bodyDef.type = b2_dynamicBody;
            else if(pCollider->mType == CCollider::Type::STATIC) bodyDef.type = b2_staticBody;
            bodyDef.position.Set(pTransform->mPosition.x / mPixelsPerMeter, pTransform->mPosition.y / mPixelsPerMeter);
            pCollider->mpBody = mWorld.CreateBody(&bodyDef);
            
            // Configure shape
            Vector2 pos = pCollider->mCenter / mPixelsPerMeter;
            Vector2 size = pCollider->mSize / mPixelsPerMeter;
            float angle = pCollider->mAngle;
            
            std::unique_ptr<b2Shape> pShape;
            
            switch( pCollider->mShape ){
            case( CCollider::Shape::CIRCLE ):
                b2CircleShape* pCircle;
                pCircle = new b2CircleShape;
                pCircle->m_p.Set(pos.x, pos.y);
                pCircle->m_radius = size.x;
                pShape.reset(pCircle);
                break;
                
            case( CCollider::Shape::RECTANGLE ):
                b2PolygonShape* pRect;
                pRect = new b2PolygonShape;
                pRect->SetAsBox(size.x, size.y, b2Vec2(pos.x, pos.y),angle);
                pShape.reset(pRect);
                break;
            }
            
            // Configure fixture
            b2FixtureDef pFixture;
            pFixture.shape = pShape.get();
            pCollider->mpBody->CreateFixture(&pFixture);
        }
        
        void UpdateBox2D(float delta){
            mWorld.Step(delta, 6, 2);
        }
        
        float mPixelsPerMeter; // Pixels per 1 meter
        b2World mWorld;

};

} // namespace safe

#endif // PHYSICS_SYSTEM_H
