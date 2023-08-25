#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "custom_rendering.h"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const char WINDOW_TITLE[] = "Rope simulation";

const float TARGET_DISTANCE = 150.0f;
const float FORCE_MAGNITUDE = 0.15f;
const float FRICTION = 0.999f;

struct RopeNode {
    float x;
    float y;
    Vector2 velocity;
    struct RopeNode *nextNode;
};

struct RopeNode *get_last_rope_node(struct RopeNode *node) {
    struct RopeNode *nodeBuffer = node;

    while (nodeBuffer->nextNode != NULL) {
        nodeBuffer = nodeBuffer->nextNode;
    }

    return nodeBuffer;
}

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

void test_func(struct RopeNode **node) {
    struct RopeNode *head = malloc(sizeof(struct RopeNode));
    struct RopeNode *tail1 = malloc(sizeof(struct RopeNode));
    struct RopeNode *tail2 = malloc(sizeof(struct RopeNode));
    struct RopeNode *tail3 = malloc(sizeof(struct RopeNode));

    Vector2 defaultVelocity = { 0.0f, 0.0f };

    head->x = 300;
    head->y = 400;
    head->velocity = defaultVelocity;
    head->nextNode = NULL;

    tail1->x = 300;
    tail1->y = 300;
    tail1->velocity = defaultVelocity;
    tail1->nextNode = head;

    tail2->x = 500;
    tail2->x = 500;
    tail2->velocity = defaultVelocity;
    tail2->nextNode = tail1;

    tail3->x = 500;
    tail3->x = 500;
    tail3->velocity = defaultVelocity;
    tail3->nextNode = tail2;

    *node = tail3;
}

void head_update(struct RopeNode **tail) {
    struct RopeNode *firstNodeBuffer = (*tail);

    while ((*tail) != NULL) {
        if ((*tail)->nextNode == NULL) {
            Vector2 mousePosition = GetMousePosition();

            (*tail)->x = mousePosition.x;
            (*tail)->y = mousePosition.y;
        } else {

            float dx = (*tail)->nextNode->x - (*tail)->x;
            float dy = (*tail)->nextNode->y - (*tail)->y;
            float distance = sqrtf((dx*dx) + (dy*dy));

            if (distance > TARGET_DISTANCE) {
                float forceDirectionX = dx/distance;
                float forceDirectionY = dy/distance;

                float forceX = forceDirectionX*FORCE_MAGNITUDE;
                float forceY = forceDirectionY*FORCE_MAGNITUDE;

                (*tail)->velocity.x = forceX;
                (*tail)->velocity.y = forceY;
            }

            (*tail)->velocity.x *= FRICTION;
            (*tail)->velocity.y *= FRICTION;

            (*tail)->x += (*tail)->velocity.x;
            (*tail)->y += (*tail)->velocity.y;
        }

        (*tail) = (*tail)->nextNode;
    }

    (*tail) = firstNodeBuffer;
}

// TODO: refac names
// TODO: define constants using #define
// TODO: add acceleration between rope nodes

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    struct RopeNode *firstNode = NULL;
    test_func(&firstNode);

    while(!WindowShouldClose()) {
        head_update(&firstNode);

        BeginDrawing();

        ClearBackground(BLACK);
        render_rope_nodes(firstNode);

        EndDrawing();
    }

    clear_nodes(firstNode);

    CloseWindow();

    return 0;
}
