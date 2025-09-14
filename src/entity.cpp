#include "entity.hpp"
#include "game.hpp"
#include <type_traits>

Entity::Entity(const Vector2& position, const Vector2& size) : 
    position(position), 
    size(size),
    remove(false), 
    components() 
{}

void Entity::Update(Game* game, float dt) {
    for (Component* component : components) {
        component->Update(game, dt);
    }
}

void Entity::Render(RenderData* data) {
    for (Component* component : components) {
        if (!component->render) { continue; }
        component->Render(data);
    }
}

void Entity::OnCollision(Game* game, Entity* other) {
    for (Component* component : components) {
        component->OnCollision(game, other);
    }
}

template<typename T> Entity* Entity::ChainComponent(T component) {
    static_assert(std::is_convertible<Component*, T*>(), "Type: %s does not inherit Component");
    components.push_back(dynamic_cast<Component*>(component));
    return this;
}

template<typename T> void Entity::AddComponent(T component) {
    static_assert(std::is_convertible<Component*, T*>(), "Type: %s does not inherit Component");
    components.push_back(dynamic_cast<Component*>(component));
}

template<typename T> T* Entity::GetComponent() {
    static_assert(std::is_convertible<Component*, T*>(), "Type: %s does not inherit Component");
    for (Component* candidate : components) {
        T* component = dynamic_cast<T*>(candidate);
        if (component) {
            return component;
        }
    }
    return nullptr;
}

template<typename T> bool Entity::HasComponent() {
    static_assert(std::is_convertible<Component*, T*>(), "Type: %s does not inherit Component");
    for (Component* candidate : components) {
        T* component = dynamic_cast<T*>(candidate);
        if (component) {
            return true;
        }
    }
    return false;
}

template<typename T> bool Entity::TryGetComponent(T* out) {
    static_assert(std::is_convertible<Component*, T*>(), "Type: %s does not inherit Component");
    for (Component* candidate : components) {
        out = dynamic_cast<T*>(candidate);
        if (out) {
            return true;
        }
    }
    out = nullptr;
    return false;
}
