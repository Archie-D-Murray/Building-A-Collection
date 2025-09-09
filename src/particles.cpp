#include "particles.hpp"
#include "raylib.h"
#include "raymath.h"
#include "utilities.hpp"
#include "game.hpp"
#include <cmath>
#include <cstdlib>

void Particle::Render(float time) {
    DrawRectanglePro(RectangleV(position, Vector2Ones * size), Vector2Ones * (size * 0.5f),
    360 * (0.5f + 0.5f * sinf((time + position.x) * 0.25f * PI)),
    ColorAlpha(base, 0.75f + 0.25 * sinf((time + position.x) * PI)));
}

AbyssParticles::AbyssParticles(float angleIncrement, float radius, Vector2 centre) {
    this->angleIncrement = angleIncrement;
    this->radius = radius;
    this->centre = centre;
    intersections = 0;
    capacity = (size_t) ceilf(360 / angleIncrement);
}

void AbyssParticles::Render(float time) {
    if (intersections != 2) { return; }
    float angleDelta = DeltaAngle(angles[0], angles[1]);
    float increment = copysignf(angleIncrement, angleDelta);
    float iterations = angleDelta / increment;
    for (float i = 0; i < iterations; i++) { 
        float angle = angles[0] + angleDelta * i;
        angle = ClockwiseAngle(angle) * RAD2DEG;
        angle = floorf((angle / 360.0f) * (float) capacity);
        RenderParticle(time, angle);
    }
}

void AbyssParticles::Debug(Game* game, float time, float angle) {
    float angleDelta = DeltaAngle(angles[0], angles[1]) * RAD2DEG;
    float increment = copysignf(angleIncrement, angleDelta) * RAD2DEG;
    float debugRadius = game->screenRect.height * 0.05f;
    Vector2 debug = TopLeft(game->screenRect) + Vector2Ones * (game->screenRect.height * 0.1f);
    debug.y += game->screenRect.height * 0.1f;
    DrawCircleV(debug, debugRadius, LIGHTGRAY);
    Vector2 diff = CentrePos(game->screenRect) - game->screenSize * 0.5f;
    Vector2 cameraSize = RectangleSize(game->screenRect) / game->worldRadius * debugRadius;
    DrawRectanglePro(RectangleV(debug + Vector2Normalize(diff) * Remap(Vector2Length(diff), 0, game->worldRadius, 0, debugRadius), cameraSize), cameraSize * 0.5f, 0.0f, WHITE);
    if (intersections == 2) {
        DrawLineEx(debug, debug + Vector2Rotate({0, -debugRadius}, angles[0]), 0.5f, GRAY);
        DrawLineEx(debug, debug + Vector2Rotate({0, -debugRadius}, angles[1]), 0.5f, DARKGRAY);
    }
    const char* message = TextFormat("%.1f -> %.1f: Angle Delta: %.1f, increment: %.2f, expected iterations: %.0f, intersections: %d", angles[0] * RAD2DEG, angles[1] * RAD2DEG, angleDelta, increment, angleDelta / increment, intersections);
    DrawText(message, game->GetScreenRect().x + 10, game->GetScreenRect().y + 10, 4, GREEN);
}

void AbyssParticles::RenderParticle(float time, float angle) {
    const float size = 8.0f;
    Vector2 position = Vector2Rotate({ 0, -radius }, angle) + centre;
    float rotation = 360.0f * (0.5f + 0.5f * sinf((time + position.x) * PI));
    Color colour = ColorAlpha(BLACK, 0.75f + 0.25 * sinf((time + position.x) * PI));
    DrawRectanglePro(RectangleV(position, Vector2Ones * size), Vector2Ones * (size * 0.5f), rotation, colour);
}

void AbyssParticles::Update(const Rectangle& cameraView, const Vector2& centre, float radius) {
    static Vector2 worldIntersections[2];
    intersections = CircleRectangleTwoIntersections(centre, radius, cameraView, worldIntersections);
    if (intersections == 2) {
        angles[0] = Vector2Angle({ 0, -1 }, Vector2Normalize(worldIntersections[0] - centre));
        angles[1] = Vector2Angle({ 0, -1 }, Vector2Normalize(worldIntersections[1] - centre));
    }
}

int AbyssParticles::CircleRectangleTwoIntersections(Vector2 circle, float r, Rectangle rect, Vector2 intersections[2]) {
    int count = 0;
    static Vector2 points[4];
    // Left
    float dx = rect.x - circle.x;
    float disc = r * r - dx * dx;
    // r*r - (px - cx) * (px - cx)
    if (disc >= 0) {
        float dy = sqrtf(disc);
        float y1 = circle.y - dy;
        float y2 = circle.y + dy;
        Vector2 point = { rect.x, y1 };
        if (PointInRectangle(point, rect)) {
            points[count] = point;
            count++;
        }
        point.y = y2;
        if (PointInRectangle(point, rect)) {
            points[count] = point;
            count++;
        }
    }

    // Right
    dx = rect.x + rect.width - circle.x;
    disc = r * r - dx * dx;
    if (disc >= 0) {
        float dy = sqrtf(disc);
        float y1 = circle.y - dy;
        float y2 = circle.y + dy;
        Vector2 point = { rect.x + rect.width, y1 };
        if (PointInRectangle(point, rect)) {
            points[count] = point;
            count++;
        }
        point.y = y2;
        if (PointInRectangle(point, rect)) {
            points[count] = point;
            count++;
        }
    }

    // Top
    float dy = rect.y - circle.y;
    disc = r * r - dy * dy;
    if (disc >= 0) {
        float dx2 = sqrtf(disc);
        float x1 = circle.x - dx2;
        float x2 = circle.x + dx2;
        Vector2 point = { x1, rect.y };
        if (PointInRectangle(point, rect)) {
            points[count] = point;
            count++;
        }
        point.x = x2;
        if (PointInRectangle(point, rect)) {
            points[count] = point;
            count++;
        }
    }

    // Bottom
    dy = rect.y + rect.height - circle.y;
    disc = r * r - dy * dy;
    if (disc >= 0) {
        float dx2 = sqrtf(disc);
        float x1 = circle.x - dx2;
        float x2 = circle.x + dx2;
        Vector2 point = { x1, rect.y + rect.height };
        if (PointInRectangle(point, rect)) {
            points[count] = point;
            count++;
        }
        point.x = x2;
        if (PointInRectangle(point, rect)) {
            points[count] = point;
            count++;
        }
    }

    if (count <= 2) { // Viewport is not very close to the centre x or y axis
        for (size_t i = 0; i < count; i++) {
            intersections[i] = points[i];
        }
        return count;
    }

    float angles[4];
    for (size_t i = 0; i < count; i++) {
        angles[i] = Vector2Angle(points[i] - circle, CentrePos(rect) - circle);
    }

    size_t minIdx = 0;
    size_t maxIdx = 0;
    float min = 1.0f;
    float max = -1.0f;

    for (size_t i = 0; i < count; i++) {
        if (angles[i] < min) {
            min = angles[i];
            minIdx = i;
        }
        if (angles[i] > max) {
            max = angles[i];
            maxIdx = i;
        }
    }

    intersections[0] = points[minIdx];
    intersections[1] = points[maxIdx];

    return 2;
}

static void UpdateIntersections(const Rectangle& cameraView, const Vector2& centre, float radius, int* intersection, float angles[2]) {
    static Vector2 intersections[2];
    *intersection = AbyssParticles::CircleRectangleTwoIntersections(centre, radius, cameraView, intersections);
    if (*intersection == 2) {
        angles[0] = Vector2Angle({ 0, -1 }, Vector2Normalize(intersections[0] - centre));
        angles[1] = Vector2Angle({ 0, -1 }, Vector2Normalize(intersections[1] - centre));
    }
}

AbyssParticles::~AbyssParticles() {}
