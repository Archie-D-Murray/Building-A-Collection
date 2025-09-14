#pragma once
#include "raylib.h"
#include <vector>

template<typename T> void remove_swap_back(std::vector<T>* vector, size_t idx);

Rectangle RectangleV(const Vector2& position, const Vector2& size);

Rectangle RectangleVCentred(const Vector2& position, const Vector2& size);

Rectangle CameraView(const Camera2D& camera, const Vector2& screenSize);

Vector2 TopLeft(const Rectangle& rect);

Vector2 CentrePos(const Rectangle& rect);

Vector2 RectangleSize(const Rectangle& rect);

float DeltaAngle(float current, float target);

float NormalizeAngle(float angle);

float ClockwiseAngle(float angle);

float AntiClockwiseAngle(float angle);

float MoveTowardsAngle(float current, float target, float maxDelta);

float MoveTowards(float current, float target, float maxDelta);

bool PointInRectangle(const Vector2& point, const Rectangle& rectangle);
