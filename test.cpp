#include "src/utilities.hpp"
#include "raymath.h"

#include <stdio.h>
#include <math.h>

#ifndef RAD2DEG
#define RAD2DEG (180.0f / (float)M_PI)
#endif

#ifndef DEG2RAD
#define DEG2RAD ((float)M_PI / 180.0f)
#endif

void TestClamp() {
    printf("Testing Clamp...\n");
    printf("Clamp(5, 0, 10) = %.2f (expected 5.00)\n", Clamp(5, 0, 10));
    printf("Clamp(-2, 0, 10) = %.2f (expected 0.00)\n", Clamp(-2, 0, 10));
    printf("Clamp(15, 0, 10) = %.2f (expected 10.00)\n", Clamp(15, 0, 10));
}

void TestDeltaAngle() {
    printf("\nTesting DeltaAngle (results in degrees)...\n");

    float a = DeltaAngle(0.0f, 90.0f * DEG2RAD);
    printf("DeltaAngle(0°, 90°) = %.2f° (expected +90.00°)\n", a * RAD2DEG);

    float b = DeltaAngle(175.0f * DEG2RAD, -175.0f * DEG2RAD);
    printf("DeltaAngle(175°, -175°) = %.2f° (expected +10.0°)\n", b * RAD2DEG);

    float c = DeltaAngle(-175.0f * DEG2RAD, 175.0f * DEG2RAD);
    printf("DeltaAngle(-175°, 175°) = %.2f° (expected -10.0°)\n", c * RAD2DEG);

    float d = DeltaAngle(343.0f * DEG2RAD, 11.0f * DEG2RAD);
    printf("DeltaAngle(343°, 11°) = %.2f° (expected +28.00°)\n", d * RAD2DEG);

    float e = DeltaAngle(11.0f * DEG2RAD, 343.0f * DEG2RAD);
    printf("DeltaAngle(11°, 343°) = %.2f° (expected -28.00°)\n", e * RAD2DEG);
}

void TestMoveTowardsAngle() {

    float current = 155.0f * DEG2RAD;
    float target = -155.0f * DEG2RAD;
    float step = 5.0f * DEG2RAD;
    printf("\nTesting MoveTowardsAngle (values in degrees)...%.0f° -> %.0f°\n", current * RAD2DEG, target * RAD2DEG);

    for (int i = 0; !FloatEquals(current, target); i++) {
        current = MoveTowardsAngle(current, target, step);
        printf("Step %d: %.2f°\n", i+1, current * RAD2DEG);
        if (FloatEquals(current, target)) {
            printf("Reached target at step %d\n", i);
        }
    }
}

int main(void) {
    TestClamp();
    TestDeltaAngle();
    TestMoveTowardsAngle();
    return 0;
}
