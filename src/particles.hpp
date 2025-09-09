#pragma once
#include "raylib.h"
#include <cstdlib>

class Game;

struct Particle {
    Vector2 position = { 0, 0 };
    Color base = BLACK;
    float size = 4;

    void Render(float time);
};

class AbyssParticles {
    float angles[2] = { 0, 0 };
    int intersections = -1;
    size_t capacity = 0;
    float angleIncrement = 0;
    float radius = 0;
    Vector2 centre = Vector2 { 0, 0 };

public:
    AbyssParticles(float angleIncrement, float radius, Vector2 centre);

    void Render(float time);
    void Update(const Rectangle& cameraView, const Vector2& centre, float radius);
    void Debug(Game* game, float time, float angle);
    void RenderParticle(float time, float angle);
    static int CircleRectangleTwoIntersections(Vector2 circle, float r, Rectangle rect, Vector2 intersections[2]);

    ~AbyssParticles();
};
