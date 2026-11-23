#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "assg.h"

#define HEAP_CAPACITY 100
#define MAX_WALLS 10000

typedef struct {
    int **grid;
    int x, y, w, h;
} Grid;

int compare_nodes(MinHeap *heap, int index1, int index2) {
    // Returns 1 if node A has priority, 0 if node B has priority.
    Node *a = heap -> data[index1], *b = heap -> data[index2];

    int af = (a -> g) + (a -> h), bf = (b -> g) + (b -> h);

    if (af != bf) return af < bf;
    else if (a -> h != b -> h) return a -> h < b -> h;
    else if (a -> x != b -> x) return a -> x < b -> x;
    else return a -> y < b -> y;
}

void swap_nodes(MinHeap *heap, int index1, int index2) {
    Node *temp = heap -> data[index1];
    heap -> data[index1] = heap -> data[index2];
    heap -> data[index2] = temp;

    Node *n1 = heap -> data[index1], *n2 = heap -> data[index2];
    temp = n1 -> parent;
    n1 -> parent = n2 -> parent;
    n2 -> parent = temp;

    if (2*index1 + 1 < heap -> size) heap -> data[2*index1 + 1] -> parent = n1;
    if (2*index1 + 2 < heap -> size) heap -> data[2*index1 + 2] -> parent = n1;
    if (2*index2 + 1 < heap -> size) heap -> data[2*index2 + 1] -> parent = n2;
    if (2*index2 + 2 < heap -> size) heap -> data[2*index2 + 2] -> parent = n2;
}

Node *create_node(int x, int y, int g, int h, Node *parent) {
    Node* ret = (Node*)malloc(sizeof(Node));
    ret -> x = x;
    ret -> y = y;
    ret -> g = g;
    ret -> h = h;
    ret -> parent = parent;
    return ret;
}

void heap_init(MinHeap *heap) {
    heap -> data = (Node**)malloc(HEAP_CAPACITY * sizeof(Node*));
    heap -> size = 0;
    heap -> capacity = HEAP_CAPACITY;
}

void heapify_up(MinHeap *heap, int i) {
    int pi;
    while (i > 0 && compare_nodes(heap, i, pi = (i - 1) / 2)) {
        swap_nodes(heap, i, pi);
        i = pi;
    }
}

void heapify_down(MinHeap *heap, int index) {
    int c1i = 2*index + 1, c2i = 2*index + 2;
    
    int smallestI = index;
    if (c1i < heap -> size && compare_nodes(heap, c1i, smallestI)) smallestI = c1i;
    if (c2i < heap -> size && compare_nodes(heap, c2i, smallestI)) smallestI = c2i;

    if (smallestI != index) {
        swap_nodes(heap, smallestI, index);
        heapify_down(heap, smallestI);
    }
}

void heap_insert(MinHeap *heap, Node *node) {
    if (heap -> size == heap -> capacity) return;
    heap -> data[heap -> size] = node;
    node -> parent = heap -> data[(heap -> size) / 2];
    heapify_up(heap, heap -> size++);
}

Node *heap_extract_min(MinHeap *heap) {
    if (heap -> size == 0) return NULL;
    Node *ret = heap -> data[0];
    if (heap -> size > 1) {
        swap_nodes(heap, 0, --heap -> size);
        heapify_down(heap, 0);
    } else heap -> size = 0;
    return ret;
}

int heap_find(const MinHeap *heap, int x, int y) {
    for (int i = 0; i < heap -> size; i++) {
        if (heap -> data[i] -> x == x && heap -> data[i] -> y == y) return i;
    }
    return -1;
}

bool heap_is_empty(const MinHeap *heap) {
    return heap -> size == 0;
}

void heap_decrease_key(MinHeap *heap, int index, int new_g, int new_h) {
    Node *node = heap -> data[index];
    node -> g = new_g;
    node -> h = new_h;
    heapify_up(heap, index);
}

void heap_free(MinHeap *heap) {
    for (int i = 0; i < heap -> size; i++) free(heap -> data[i]);
    free(heap -> data);
    free(heap);
}

int is_valid_heap(MinHeap *heap) {
    for (int i = 1; i < heap -> size; i++) {
        if (compare_nodes(heap, i, (i - 1) / 2)) return 0;
    }
    return 1;
}

void print_heap(MinHeap *heap, int index, unsigned int depth) {
    if (index >= heap -> size) return;
    Node *node = heap -> data[index];
    char indent[100];
    for (int i = 0; i < depth; i++) {
        indent[2*i] = ' ';
        indent[2*i + 1] = ' ';
    }
    indent[2*depth] = '\0';
    printf("%sNode %d: %d, %d, %d, %d\n", indent, index, node -> g + node -> h, node -> h, node -> x, node -> y);
    printf("%s- Left:\n", indent);
    print_heap(heap, 2 * index + 1, depth + 1);
    printf("%s- Right:\n", indent);
    print_heap(heap, 2 * index + 2, depth + 1);
}

void closed_list_init(ClosedList *list) {
    list -> data = (Node**)malloc(HEAP_CAPACITY * sizeof(Node*));
    list -> capacity = HEAP_CAPACITY;
    list -> size = 0;
}

void closed_list_add(ClosedList *list, Node *node) {
    if (list -> size == list -> capacity) return;
    list -> data[list -> size++] = node;
}

bool closed_list_contains(const ClosedList *list, int x, int y) {
    Node *node;
    for (int i = 0; i < list -> size; i++) {
        node = list -> data[i];
        if (node -> x = x && node -> y == y) return true;
    }
    return false;
}

void closed_list_free(ClosedList *list) {
    for (int i = 0; i < list -> size; i++) free(list -> data[i]);
    free(list -> data);
    free(list);
}

Grid generateGrid() {
    int sx, sy, ex, ey;

    printf("Start:\n");
    scanf("(%d, %d)", &sx, &sy);
    printf("End: \n");
    scanf(" (%d, %d)", &ex, &ey);

    int minX = sx < ex ? sx : ex;
    int maxX = sx > ex ? sx : ex;
    int minY = sy < ey ? sy : ey;
    int maxY = sy > ey ? sy : ey;

    int wallsCount;
    int wallsX[MAX_WALLS], wallsY[MAX_WALLS], wallsW[MAX_WALLS], wallsH[MAX_WALLS];
    printf("Walls:\n");
    int x, y, w, h;
    for (wallsCount = 0; scanf(" (%d, %d), %d, %d", &x, &y, &w, &h); wallsCount++) {
        wallsX[wallsCount] = x;
        wallsY[wallsCount] = y;
        wallsW[wallsCount] = w;
        wallsH[wallsCount] = h;

        minX = x < minX ? x : minX;
        maxX = x + w > maxX ? x + w : maxX;
        minY = y < minY ? y : minY;
        maxY = y + h > maxY ? y + h : maxY;
    }

    int gridWidth = maxX - minX, gridHeight = maxY - minY;
    int **grid = (int**)malloc(gridWidth * sizeof(int*));
    for (int i = 0; i < gridWidth; i++) grid[i] = (int*)calloc(gridHeight, sizeof(int));

    for (int c = 0; c < wallsCount; c++) {
        for (int i = wallsX[c]; i < wallsX[c] + wallsW[c]; i++) {
            for (int j = wallsY[c]; j < wallsY[c] + wallsH[c]; j++) {
                grid[i - minX][j - minY] = 1;
            }
        }
    }

    return (Grid){.grid = grid, .x = minX, .y = minY, .w = gridWidth, .h = gridHeight};
}

int main(int argc, char **argv) {
    Grid grid = generateGrid();
}
