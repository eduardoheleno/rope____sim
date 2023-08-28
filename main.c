#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "custom_rendering.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Rope simulation"

#define TARGET_DISTANCE 150.0f
#define FORCE_MAGNITUDE 0.15f
#define FRICTION 0.999f

struct RopeNode {
    float x;
    float y;
    Vector2 velocity;
    struct RopeNode *nextNode;
};

void clear_nodes(struct RopeNode *node) {
    struct RopeNode *nodeBuffer = node;
    struct RopeNode *nextNodeBuffer = NULL;

    while (nodeBuffer != NULL) {
        nextNodeBuffer = nodeBuffer->nextNode;
        free(nodeBuffer);

        nodeBuffer = nextNodeBuffer;
    }
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

            draw_rectangle(
                (Vector2){ node->x, node->y },
                (Vector2){ node->nextNode->x, node->nextNode->y },
                20,
                WHITE
            );
            DrawCircle(node->x, node->y, 20, RED);
        } else {
            DrawCircle(node->x, node->y, 20, GREEN);
        }

        node = node->nextNode;
    }
}

void alloc_rope_nodes(struct RopeNode **node, int nodes_qtd) {
    struct RopeNode *head = malloc(sizeof(struct RopeNode));
    struct RopeNode *nextNodeBuffer = malloc(sizeof(struct RopeNode));
    nextNodeBuffer = head;

    Vector2 defaultVelocity = { 0.0f, 0.0f };

    head->x = 300;
    head->y = 300;
    head->velocity = defaultVelocity;
    head->nextNode = NULL;

    for (int i = 0; i < nodes_qtd; i++) {
        struct RopeNode *tail = malloc(sizeof(struct RopeNode));
        tail->x = 300;
        tail->y = 300;
        tail->velocity = defaultVelocity;
        tail->nextNode = nextNodeBuffer;

        nextNodeBuffer = tail;
    }

    *node = nextNodeBuffer;
}

void rope_frame(struct RopeNode **ropeNode) {
    struct RopeNode *firstNodeBuffer = (*ropeNode);

    while ((*ropeNode) != NULL) {
        if ((*ropeNode)->nextNode == NULL) {
            Vector2 mousePosition = GetMousePosition();

            (*ropeNode)->x = mousePosition.x;
            (*ropeNode)->y = mousePosition.y;
        } else {

            float dx = (*ropeNode)->nextNode->x - (*ropeNode)->x;
            float dy = (*ropeNode)->nextNode->y - (*ropeNode)->y;
            float distance = sqrtf((dx*dx) + (dy*dy));

            if (distance > TARGET_DISTANCE) {
                float forceDirectionX = dx/distance;
                float forceDirectionY = dy/distance;

                float forceX = forceDirectionX*FORCE_MAGNITUDE;
                float forceY = forceDirectionY*FORCE_MAGNITUDE;

                (*ropeNode)->velocity.x = forceX;
                (*ropeNode)->velocity.y = forceY;
            }

            (*ropeNode)->velocity.x *= FRICTION;
            (*ropeNode)->velocity.y *= FRICTION;

            (*ropeNode)->x += (*ropeNode)->velocity.x;
            (*ropeNode)->y += (*ropeNode)->velocity.y;
        }

        (*ropeNode) = (*ropeNode)->nextNode;
    }

    (*ropeNode) = firstNodeBuffer;
}

// TODO: add acceleration between rope nodes

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    struct RopeNode *firstNode = NULL;
    alloc_rope_nodes(&firstNode, 3);

    while(!WindowShouldClose()) {
        rope_frame(&firstNode);

        BeginDrawing();

        ClearBackground(BLACK);
        render_rope_nodes(firstNode);

        EndDrawing();
    }

    clear_nodes(firstNode);
    CloseWindow();

    return 0;
}
