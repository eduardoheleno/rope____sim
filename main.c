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

void render_rope_nodes(struct RopeNode *node) {
    while (node != NULL) {
        if (node->nextNode != NULL) {
            float distance = distance_between_adjacent_nodes(node);
            float angleRot = angle_between_adjacent_nodes(node);
            Rectangle rec = { node->x, node->y, distance, 20 };

            DrawRectanglePro(
                rec,
                (Vector2){ 0, 0 },
                angleRot,
                WHITE
            );
        }

        DrawCircle(node->x, node->y, 20, RED);

        node = node->nextNode;
    }
}

void draw_rectangle(Vector2 v1, Vector2 v2, float height, Color color) {
    Vector2 t1v1 = { v2.x + 5, v1.y + height/2 };
    Vector2 t1v2 = { v1.x - 5, v1.y - height/2 };
    Vector2 t1v3 = { v1.x - 5, v1.y + height/2 };

    Vector2 t2v1 = { v1.x - 5, v2.y - height/2 };
    Vector2 t2v2 = { v2.x + 5, v2.y + height/2 };
    Vector2 t2v3 = { v2.x + 5, v2.y - height/2 };

    DrawTriangle(t1v1, t1v2, t1v3, color);
    DrawTriangle(t2v1, t2v2, t2v3, color);
}

int main() {
    InitWindow(1200, 800, "Rope simulation");

    struct RopeNode *firstNode = NULL;

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        draw_rectangle(
            (Vector2){ 400, 400 },
            (Vector2){ 800, 400 },
            30,
            WHITE
        );

        /* if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { */
        /*     Vector2 mousePosition = GetMousePosition(); */

        /*     if (firstNode == NULL) { */
        /*         firstNode = malloc(sizeof(struct RopeNode)); */

        /*         firstNode->x = mousePosition.x; */
        /*         firstNode->y = mousePosition.y; */
        /*         firstNode->nextNode = NULL; */

        /*         /\* system("clear"); *\/ */
        /*         /\* print_list(firstNode); *\/ */
        /*     } else { */
        /*         struct RopeNode *lastNode = get_last_rope_node(firstNode); */
        /*         struct RopeNode *newNode = malloc(sizeof(struct RopeNode)); */

        /*         newNode->x = mousePosition.x; */
        /*         newNode->y = mousePosition.y; */
        /*         newNode->nextNode = NULL; */

        /*         lastNode->nextNode = newNode; */

        /*         /\* system("clear"); *\/ */
        /*         /\* print_list(firstNode); *\/ */
        /*     } */
        /* } */

        /* render_rope_nodes(firstNode); */

        EndDrawing();
    }

    return 0;
}
