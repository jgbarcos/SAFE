#ifndef ENTITY_H
#define ENTITY_H

#include <typeindex>
#include <typeinfo>
#include <memory>
#include <unordered_map>

#include "SAFE/Component.h"

namespace safe {

class Entity
{
    public:
        int mFlags;

        template <typename T>
        T* Get(){
            auto it = mComponents.find(std::type_index(typeid(T)));
            if(it != mComponents.end()){
                return dynamic_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        template <typename T>
        void Remove(){
            mComponents.erase(std::type_index(typeid(T)));
        }

        template <typename T>
        Entity& Add(std::unique_ptr<Component> pComponent){
            return AddComponent(std::type_index(typeid(T)), std::move(pComponent));
        }

        Entity& AddComponent(std::type_index type, std::unique_ptr<Component> pComponent){
            mComponents[type] = std::move(pComponent);
            return *this;
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<Component> > mComponents;

};

} // namespace safe

#endif // ENTITY_H
