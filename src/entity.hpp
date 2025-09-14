#pragma once
#include "raylib.h"
#include <vector>

enum Layer {
    LayerNone             = 0,
    LayerPlayer           = 1 >> 0,
    LayerPlayerProjectile = 1 >> 1,
    LayerEnemy            = 1 >> 2,
    LayerEnemyProjectile  = 1 >> 3
};

class Component;
class Game;
class RenderData;

class Entity {
public:
    Vector2 position;
    Vector2 size;
    Layer layer;
    bool remove;
    std::vector<Component*> components;

    Entity(const Vector2& position, const Vector2& size);

    void Update(Game* game, float dt);
    void Render(RenderData* data);
    void OnCollision(Game* game, Entity* other);
    template<typename T> void AddComponent(T component);
    template<typename T> Entity* ChainComponent(T component);
    template<typename T> T* GetComponent();
    template<typename T> bool HasComponent();
    template<typename T> bool TryGetComponent(T* out);
};

class Component {
    Entity* entity = nullptr;
public:
    bool render = false;

public:
    Component(Entity* entity) : entity(entity) { }
    virtual void Update(Game* game, float dt) { }
    virtual void Render(RenderData* data) { }
    virtual void OnCollision(Game* game, Entity* other) { }
};
