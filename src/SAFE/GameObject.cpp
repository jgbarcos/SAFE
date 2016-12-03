#include "SAFE/GameObject.h"

namespace safe {

GameObject::GameObject()
{
    //ctor
}

GameObject::~GameObject()
{
    //dtor
}

float GameObject::CalculateZ() const
{
    return mTransform.mPosition.z;
}

} // namespace safe
