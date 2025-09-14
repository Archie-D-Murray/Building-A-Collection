#pragma once

#include "entity.hpp"
#include "raylib.h"
#include <cstddef>
#include <unordered_map>

static constexpr size_t null = size_t(-1);

class Game;

enum class AdjacentEdge {
    EdgeNone,
    EdgeTop,
    EdgeBottom,
    EdgeLeft,
    EdgeRight,
    EdgeTopLeft,
    EdgeBottomLeft,
    EdgeTopRight,
    EdgeBottomRight
};

class PhysicsSystem {
    std::vector<Entity*>* pools;
    std::unordered_map<Entity*, size_t>* lookups;
    Rectangle boundary;
    Vector2 poolSize;
    size_t poolsCount;
    size_t dimensions[2];

    PhysicsSystem(const Rectangle& boundary, Vector2 pools);
    ~PhysicsSystem();

    void Update(Game* game, float dt);
    bool InsertEntity(Entity* add);
    bool RemoveEntity(Entity*);
    void GetXYFromIndex(size_t index, size_t* x, size_t* y);

    Rectangle GetPoolRect(size_t poolID);
    size_t GetPoolIndexes(size_t poolID, AdjacentEdge adjacent, size_t indexes[4]);
    size_t GetPoolID(Vector2 point);
    AdjacentEdge CloseToEdge(size_t poolID, Vector2 position, float distance);
    std::vector<Entity*> GetEntities(const Vector2& position, float radius, Layer layer);
    Entity* GetEntity(const Vector2& position, float radius, Layer layer);
};
