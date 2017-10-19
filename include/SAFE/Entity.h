#ifndef ENTITY_H
#define ENTITY_H

#include <typeindex>
#include <typeinfo>
#include <memory>
#include <unordered_map>
#include <iostream>

#include "SAFE/Component.h"

namespace safe {

/**
 * Entity in the ECS system. Its the owner of its components.
 */
class Entity {
public:

    Entity(std::string name) : mIsActive(true), mEntityName(name) { }

    /**
     * Gets a component by its class name.
     * @tparam T Component class.
     * @return Pointer to the component T (non-ownership) or nullptr.
     */
    template <typename T>
    T* Get() {
        auto it = mComponents.find(std::type_index(typeid (T)));
        if (it != mComponents.end()) {
            return dynamic_cast<T*> (it->second.get());
        }
        return nullptr;
    }

    /**
     * Removes a component by its class name.
     * @tparam T derived class of Component.
     */
    template <typename T>
    void Remove() {
        mComponents.erase(std::type_index(typeid (T)));
    }

    /**
     * Adds a new Component, derived component must be specified.
     * @tparam T derived class of Component.
     * @param pComponent pointer to component (ownership).
     */
    template <typename T>
    Entity& Add(std::unique_ptr<Component> pComponent) {
        return AddComponent(std::type_index(typeid (T)), std::move(pComponent));
    }

    /**
     * Adds a new Component, derived component must be specified.
     * @param type type_index of the derived class of Component.
     * @param pComponent pointer to component (ownership).
     */
    Entity& AddComponent(std::type_index type, std::unique_ptr<Component> pComponent) {
        mComponents[type] = std::move(pComponent);
        return *this;
    }
    
    
    Entity& AddExtraComponent(std::string name, sol::table component) {
        mExtraComponents[name] = component;
        return *this;
    }
    
    sol::table GetComponent(std::string name){
        return mExtraComponents[name];
    }

    /**
     * Returns the name of the entity
     * @return A string representing the ID of the entity
     */
    std::string GetName() {
        return mEntityName;
    }

    bool mIsActive;
private:
    std::unordered_map<std::type_index, std::unique_ptr<Component> > mComponents;
    std::unordered_map<std::string, sol::table > mExtraComponents;

    std::string mEntityName;

};

} // namespace safe

#endif // ENTITY_H
