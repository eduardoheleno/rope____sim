#include "custom_rendering.h"
#include <math.h>

void draw_rectangle(Vector2 v1, Vector2 v2, float height, Color color) {
    const int DISTANCE_FROM_ORIGIN = 5;

    Vector2 pivot = v1;
    Vector2 pivot2 = v2;

    Vector2 translation = { -pivot.x, -pivot.y };
    Vector2 translation2 = { -pivot2.x, -pivot2.y };

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float distance = sqrtf((dx*dx) + (dy*dy));
    float angleBetweenPoints = atan2f(v2.y - v1.y, v2.x - v1.x)*(180.0f/M_PI);

    Vector2 t1v1 = { (v1.x + distance) + DISTANCE_FROM_ORIGIN, v1.y + height/2 };
    Vector2 t1v2 = { v1.x - DISTANCE_FROM_ORIGIN, v1.y - height/2 };
    Vector2 t1v3 = { v1.x - DISTANCE_FROM_ORIGIN, v1.y + height/2 };

    Vector2 vertices[] = {
        t1v1,
        t1v2,
        t1v3
    };

    for (int i = 0; i < 3; ++i) {
        float x = vertices[i].x + translation.x;
        float y = vertices[i].y + translation.y;

        vertices[i].x = x*cosf(angleBetweenPoints*DEG2RAD) - y*(sinf(angleBetweenPoints*DEG2RAD));
        vertices[i].y = x*sinf(angleBetweenPoints*DEG2RAD) + y*(cosf(angleBetweenPoints*DEG2RAD));
    }

    translation.x = pivot.x;
    translation.y = pivot.y;

    for (int i = 0; i < 3; ++i) {
        vertices[i].x += translation.x;
        vertices[i].y += translation.y;
    }

    Vector2 t2v1 = vertices[1];
    Vector2 t2v2 = vertices[0];
    Vector2 t2v3 = { v2.x + DISTANCE_FROM_ORIGIN, v2.y - height/2 };

    float tx = t2v3.x + translation2.x;
    float ty = t2v3.y + translation2.y;

    t2v3.x = tx*cosf(angleBetweenPoints*DEG2RAD) - ty*sinf((angleBetweenPoints*DEG2RAD));
    t2v3.y = tx*sinf(angleBetweenPoints*DEG2RAD) + ty*(cosf(angleBetweenPoints*DEG2RAD));

    translation2.x = pivot2.x;
    translation2.y = pivot2.y;

    t2v3.x += translation2.x;
    t2v3.y += translation2.y;

    DrawTriangle(vertices[0], vertices[1], vertices[2], color);
    DrawTriangle(t2v1, t2v2, t2v3, color);
}
