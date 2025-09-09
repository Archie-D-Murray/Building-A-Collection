#include "utilities.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

Rectangle RectangleV(const Vector2& position, const Vector2& size) {
    return Rectangle { position.x, position.y, size.x, size.y };
};

Rectangle CameraView(const Camera2D& camera, const Vector2& screenSize) {
    Vector2 topLeft = GetScreenToWorld2D(Vector2Zeros, camera);
    return RectangleV(topLeft, GetScreenToWorld2D(screenSize, camera) - topLeft);
}

Vector2 TopLeft(const Rectangle &rect) {
    return Vector2 { rect.x, rect.y };
}

Vector2 CentrePos(const Rectangle& rect) {
    return Vector2 { rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f };
};

Vector2 RectangleSize(const Rectangle &rect) {
    return Vector2 { rect.width, rect.height };
}

float DeltaAngle(float current, float target) {
    float diff = fmodf(target - current, 2.0f * PI);
    if (diff > PI) diff -= 2.0f * PI;
    if (diff < -PI) diff += 2.0f * PI;
    return diff;
    return diff;
}

float NormalizeAngle(float angle) {
    angle = fmod(angle, 2.0f * PI);
    if (angle < -PI) {
        angle += 2.0f * PI;
    }
    if (angle > +PI) {
        angle -= 2.0f * PI;
    }
    return angle;
};

float ClockwiseAngle(float angle) {
    angle = NormalizeAngle(angle);
    if (angle >= 0) {
        return angle;
    } else {
       return 2 * PI + angle;
    }
};

float AntiClockwiseAngle(float angle) {
    angle = NormalizeAngle(angle);
    if (angle <= 0) {
        return angle;
    } else {
        return -2 * PI - angle;
    }
}

float MoveTowardsAngle(float current, float target, float maxDelta) {
    float diff = DeltaAngle(current, target);
    if (fabsf(diff) <= maxDelta) {
        return target;
    }
    return NormalizeAngle(current + Clamp(diff, -maxDelta, maxDelta));
};

float MoveTowards(float current, float target, float maxDelta) {
    if (fabsf(target - current) <= maxDelta) {
        return target;
    }
    return current + copysignf(maxDelta, target - current);
};

bool PointInRectangle(const Vector2 &point, const Rectangle &rectangle) {
    return (point.x >= rectangle.x) && (point.x <= (rectangle.x + rectangle.width)) && (point.y >= rectangle.y) && (point.y <= (rectangle.y + rectangle.height));
}
