#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "assg.h"

#define HEAP_CAPACITY 100

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

void initialInserts(FILE *f, MinHeap *openList) {
    char line[100];
    fgets(line, sizeof(line), f);
    int x, y, g, h;
    while (1) {
        fgets(line, sizeof(line), f);
        if (!sscanf(line, "%d, %d, %d, %d", &x, &y, &g, &h)) break;
        heap_insert(openList, create_node(x, y, g, h, openList -> data[openList -> size / 2]));
    }
}

void executeCommands(FILE *input, FILE *output, MinHeap *openList, ClosedList *closedList) {
    char line[100];
    fgets(line, sizeof(line), input);
    fgets(line, sizeof(line), input);
    char dest, command[50];
    int arg1, arg2, arg3;
    while (fgets(line, sizeof(line), input)) {
        sscanf(line, "[%c] %s %d %d %d", &dest, command, &arg1, &arg2, &arg3);

        if (strcmp(command, "Extract")) {
            Node *node = heap_extract_min(openList);
            print_heap(openList, 0, 0);
            printf("\n");
            fprintf(output, "Extracted node (%d, %d), f = %d, g = %d, h = %d\n", node -> x, node -> y, node -> g + node -> h, node -> g, node -> h);
        }
    }
}

int main(int argc, char **argv) {
    MinHeap *openList = (MinHeap*)malloc(sizeof(MinHeap));
    ClosedList *closedList = (ClosedList*)malloc(sizeof(ClosedList));

    heap_init(openList);
    closed_list_init(closedList);

    FILE *input;
    char path[100];
    sprintf(path, "Testcases/Milestone Test Cases/%s", argv[1]);
    input = fopen(path, "r");

    FILE *output;
    output = fopen("output.txt", "w");

    initialInserts(input, openList);
    executeCommands(input, output, openList, closedList);

    heap_free(openList);
    closed_list_free(closedList);

    fclose(input);
    fclose(output);
}
