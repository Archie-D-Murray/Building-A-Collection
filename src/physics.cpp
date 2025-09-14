#include "physics.hpp"
#include "entity.hpp"
#include "raylib.h"
#include "raymath.h"
#include "utilities.hpp"
#include "game.hpp"

PhysicsSystem::PhysicsSystem(const Rectangle& boundary, Vector2 poolDimensions) {
    dimensions[0] = static_cast<size_t>(ceilf(boundary.width / poolDimensions.x));
    dimensions[1] = static_cast<size_t>(ceilf(boundary.height / poolDimensions.y));

    this->boundary = boundary;
    poolsCount = dimensions[0] * dimensions[1];
    pools = new std::vector<Entity*>[poolsCount];
    poolSize = poolDimensions;

    for (size_t i = 0; i < poolsCount; i++) {
        pools[i].reserve((200 + 199) / poolsCount); // Should only really have ~200 entities as both spawners have max of 100
    }
}

PhysicsSystem::~PhysicsSystem() {
    if (pools) {
        delete[] pools;
    }
}

void PhysicsSystem::Update(Game* game, float dt) {
    static std::vector<std::vector<size_t>> remove;
    for (size_t i = 0; i < poolsCount; i++) {
        for (size_t idx = 0; idx < pools[i].size(); idx++) {
            Entity* en = pools[i][idx];
            if (en->remove) { remove[i].push_back(idx); continue; }
            AdjacentEdge edge = CloseToEdge(idx, en->position, en->size.x);
            static size_t poolIndexes[4];
            size_t count = GetPoolIndexes(i, edge, poolIndexes);
            for (size_t otherPool = 0; otherPool < count; i++) {
                for (size_t otherIdx = 0; otherIdx < pools[otherPool].size(); otherIdx++) {
                    Entity* other = pools[otherPool][otherIdx];
                    if (other == en) { continue; }
                    if (!(other->layer & en->layer)) { continue; } 
                    if (CheckCollisionCircles(en->position, en->size.x, other->position, other->size.x)) {
                        en->OnCollision(game, other);
                        other->OnCollision(game, en);
                    }
                }
            }
        }
    }
}

size_t PhysicsSystem::GetPoolID(Vector2 position) {
    return static_cast<size_t>(floorf(position.y / poolSize.y)) * dimensions[0] + static_cast<size_t>(floorf(position.x / poolSize.x));
}

void PhysicsSystem::GetXYFromIndex(size_t index, size_t* x, size_t* y) {
    *x = index % dimensions[0];
    *y = index / dimensions[0];
}

size_t PhysicsSystem::GetPoolIndexes(size_t poolID, AdjacentEdge adjacent, size_t indexes[4]) {
    size_t x, y;
    GetXYFromIndex(poolID, &x, &y);
    size_t count = 1;
    switch (adjacent) {
    case AdjacentEdge::EdgeNone:
        indexes[0] = poolID;
        break;
    case AdjacentEdge::EdgeTop:
        indexes[0] = poolID;
        if (y > 0) { indexes[count++] = y - 1 * dimensions[0] + x; }
        break;
    case AdjacentEdge::EdgeBottom:
        indexes[0] = poolID;
        if (y - 1 < dimensions[count++]) { indexes[1] = y + 1 * dimensions[0] + x; }
        break;
    case AdjacentEdge::EdgeLeft:
        indexes[0] = poolID;
        if (x > 0) { indexes[count++] = y * dimensions[0] + x + 1; }
        break;
    case AdjacentEdge::EdgeRight:
        indexes[0] = poolID;
        if (x - 1 < dimensions[count++]) { indexes[1] = y * dimensions[0] + x - 1; }
        break;
    case AdjacentEdge::EdgeTopLeft:
        indexes[0] = poolID;
        if (y > 0) { indexes[count++] = y - 1 * dimensions[0] + x; }
        if (x > 0) { indexes[count++] = y * dimensions[0] + x + 1; }
        break;
    case AdjacentEdge::EdgeBottomLeft:
        indexes[0] = poolID;
        if (y - 1 < dimensions[count++]) { indexes[1] = y + 1 * dimensions[0] + x; }
        if (x > 0) { indexes[count++] = y * dimensions[0] + x + 1; }
        break;
    case AdjacentEdge::EdgeTopRight:
        indexes[0] = poolID;
        if (y > 0) { indexes[count++] = y - 1 * dimensions[0] + x; }
        if (x - 1 < dimensions[count++]) { indexes[1] = y * dimensions[0] + x - 1; }
        break;
    case AdjacentEdge::EdgeBottomRight:
        indexes[0] = poolID;
        if (y - 1 < dimensions[count++]) { indexes[1] = y + 1 * dimensions[0] + x; }
        if (x - 1 < dimensions[count++]) { indexes[1] = y * dimensions[0] + x - 1; }
        break;
    };
    return count;
}

AdjacentEdge PhysicsSystem::CloseToEdge(size_t poolID, Vector2 position, float distance) {
    Rectangle poolRect = GetPoolRect(poolID);
    if (!CheckCollisionRecs(RectangleVCentred(position, Vector2Ones * distance), poolRect)) {
        return AdjacentEdge::EdgeNone;
    }

    if (position.y - poolRect.y < distance) {
        if (position.x - poolRect.x < distance) {
            return AdjacentEdge::EdgeTopLeft;
        } else if (poolRect.x + poolRect.width - position.x <= distance) {
            return AdjacentEdge::EdgeTopRight;
        } else {
            return AdjacentEdge::EdgeTop;
        }
    } else {
        if (position.x - poolRect.x < distance) {
            return AdjacentEdge::EdgeBottomLeft;
        } else if (poolRect.x + poolRect.width - position.x <= distance) {
            return AdjacentEdge::EdgeBottomRight;
        } else {
            return AdjacentEdge::EdgeBottom;
        }
    }
}

bool PhysicsSystem::InsertEntity(Entity* add) {
    if (!add) { TraceLog(LOG_WARNING, "Cannot add null entity"); return false; }
    size_t poolID = GetPoolID(add->position);
    if (poolID != null) {
        pools[GetPoolID(add->position)].push_back(add);
    } else {
        TraceLog(LOG_WARNING, "Could not add entity at [ %.1f, %.1f] to pool", add->position.x, add->position.y);
    }
    return poolID != null;
}

bool PhysicsSystem::RemoveEntity(Entity* remove) {
    if (!remove) { TraceLog(LOG_WARNING, "Cannot remove null entity"); return false; }
    size_t poolID = GetPoolID(remove->position);
    if (poolID == null) { TraceLog(LOG_WARNING, "Cannot remove unknown entity"); return false; }
    size_t idx = lookups[poolID][remove];
    Entity* swap = pools[poolID].back();
    lookups[poolID][swap] = idx;
    lookups->erase(pools[poolID][idx]);
    pools[poolID][idx] = swap;
    pools[poolID].pop_back();
    return true;
}

std::vector<Entity*> PhysicsSystem::GetEntities(const Vector2& position, float radius, Layer layer) {
    std::vector<Entity*> entities;
    size_t poolID = GetPoolID(position);
    for (Entity* entity : pools[poolID]) {
        if (Vector2LengthSqr(position - entity->position) <= radius * radius) {
            entities.push_back(entity);
        }
    }
    return entities;
}

Rectangle PhysicsSystem::GetPoolRect(size_t poolID) {
    size_t x, y;
    GetXYFromIndex(poolID, &x, &y);
    return RectangleV(TopLeft(boundary) + Vector2 { static_cast<float>(x), static_cast<float>(y) } * poolSize, poolSize);
}

Entity* PhysicsSystem::GetEntity(const Vector2& position, float radius, Layer layer) {
    size_t poolID = GetPoolID(position);
    Entity* closest = nullptr;
    float closestDistance = radius * radius + 1;
    static size_t poolIndexes[4];
    size_t count = GetPoolIndexes(poolID, CloseToEdge(poolID, position, radius), poolIndexes);
    for (size_t i = 0; i < count; i++) {
        for (Entity* entity : pools[poolIndexes[i]]) {
            float distance = Vector2LengthSqr(position - entity->position);
            if (distance >= radius * radius) {
                continue;
            } else if (distance < closestDistance) {
                closestDistance = distance;
                closest = entity;
            }
        }
    }
    return closest;
}
