#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct RopeNode {
    float x;
    float y;
    struct RopeNode *nextNode;
};

struct RopeNode *get_last_rope_node(struct RopeNode *node) {
    struct RopeNode *nodeBuffer = node;

    while (nodeBuffer->nextNode != NULL) {
        nodeBuffer = nodeBuffer->nextNode;
    }

    return nodeBuffer;
}

void print_list(struct RopeNode *firstNode) {
    system("clear");
    struct RopeNode *nodeBuffer = firstNode;

    while (nodeBuffer != NULL) {
        printf("X: %f\n", nodeBuffer->x);
        printf("Y: %f\n", nodeBuffer->y);

        nodeBuffer = nodeBuffer->nextNode;
    }
}

float distance_between_adjacent_nodes(struct RopeNode *node) {
    float dx = node->nextNode->x - node->x;
    float dy = node->nextNode->y - node->y;

    return sqrtf((dx*dx) + (dy*dy));
}

float angle_between_adjacent_nodes(struct RopeNode *node) {
    return atan2f(node->nextNode->y - node->y, node->nextNode->x - node->x)*(180.0f/M_PI);
}

void draw_rectangle(Vector2 v1, Vector2 v2, float height, Color color) {
    Vector2 pivot = v1;
    Vector2 pivot2 = v2;

    Vector2 translation = { -pivot.x, -pivot.y };
    Vector2 translation2 = { -pivot2.x, -pivot2.y };

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float distance = sqrtf((dx*dx) + (dy*dy));
    float angleBetweenPoints = atan2f(v2.y - v1.y, v2.x - v1.x)*(180.0f/M_PI);

    Vector2 t1v1 = { (v1.x + distance) + 5, v1.y + height/2 };
    Vector2 t1v2 = { v1.x - 5, v1.y - height/2 };
    Vector2 t1v3 = { v1.x - 5, v1.y + height/2 };

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
    Vector2 t2v3 = { v2.x + 5, v2.y - height/2 };

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

void render_rope_nodes(struct RopeNode *node) {
    while (node != NULL) {
        if (node->nextNode != NULL) {
            float distance = distance_between_adjacent_nodes(node);
            float angleRot = angle_between_adjacent_nodes(node);

            draw_rectangle(
                (Vector2){ node->x, node->y },
                (Vector2){ node->nextNode->x, node->nextNode->y },
                30,
                WHITE
            );
        }

        DrawCircle(node->x, node->y, 20, RED);

        node = node->nextNode;
    }
}

int main() {
    InitWindow(1200, 800, "Rope simulation");

    struct RopeNode *firstNode = NULL;

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePosition = GetMousePosition();

            if (firstNode == NULL) {
                firstNode = malloc(sizeof(struct RopeNode));

                firstNode->x = mousePosition.x;
                firstNode->y = mousePosition.y;
                firstNode->nextNode = NULL;

                /* system("clear"); */
                /* print_list(firstNode); */
            } else {
                struct RopeNode *lastNode = get_last_rope_node(firstNode);
                struct RopeNode *newNode = malloc(sizeof(struct RopeNode));

                newNode->x = mousePosition.x;
                newNode->y = mousePosition.y;
                newNode->nextNode = NULL;

                lastNode->nextNode = newNode;

                /* system("clear"); */
                /* print_list(firstNode); */
            }
        }

        render_rope_nodes(firstNode);

        EndDrawing();
    }

    return 0;
}
