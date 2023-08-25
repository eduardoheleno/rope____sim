#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "raylib.h"
#include "custom_rendering.h"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const char WINDOW_TITLE[] = "Rope simulation";

const float TARGET_DISTANCE = 150.0f;
const float FORCE_MAGNITUDE = 0.2f;
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
    struct RopeNode *tail = malloc(sizeof(struct RopeNode));

    Vector2 defaultVelocity = { 0.0f, 0.0f };

    head->x = 300;
    head->y = 400;
    head->velocity = defaultVelocity;
    head->nextNode = NULL;

    tail->x = 300;
    tail->y = 300;
    tail->velocity = defaultVelocity;
    tail->nextNode = head;

    *node = tail;
}

void head_update(struct RopeNode **tail) {
    Vector2 mousePosition = GetMousePosition();

    (*tail)->nextNode->x = mousePosition.x;
    (*tail)->nextNode->y = mousePosition.y;

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

    printf("vX: %f\n", (*tail)->velocity.x);
    printf("vY: %f\n", (*tail)->velocity.y);

    (*tail)->velocity.x *= FRICTION;
    (*tail)->velocity.y *= FRICTION;

    (*tail)->x += (*tail)->velocity.x;
    (*tail)->y += (*tail)->velocity.y;

    /* float distance = distance_between_adjacent_nodes((*tail)); */
    /* float angleDegrees = angle_between_adjacent_nodes((*tail)); */
    /* float angleRadians = angleDegrees*DEG2RAD; */

    /* float forceX = FORCE_MAGNITUDE*cosf(angleRadians); */
    /* float forceY = FORCE_MAGNITUDE*sinf(angleRadians); */

    /* if (distance > TARGET_DISTANCE) { */
    /*     (*tail)->velocity.x += forceX; */
    /*     (*tail)->velocity.y += forceY; */
    /* } */

    /* (*tail)->x += (*tail)->velocity.x; */
    /* (*tail)->y += (*tail)->velocity.y; */

    /* printf("velocityX: %f\n", (*tail)->velocity.x); */
    /* printf("velocityY: %f\n", (*tail)->velocity.y); */
}

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    struct RopeNode *firstNode = NULL;
    test_func(&firstNode);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        head_update(&firstNode);

        /* if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) { */
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

        /* if (IsKeyPressed(KEY_C)) { */
        /*     clear_nodes(firstNode); */
        /*     firstNode = NULL; */
        /* } */

        render_rope_nodes(firstNode);

        EndDrawing();
    }

    free(firstNode);
    CloseWindow();

    return 0;
}
