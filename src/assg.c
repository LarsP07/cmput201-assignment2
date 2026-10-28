#include <stdio.h>
#include <stdbool.h>
#include "assg.h"

#define HEAP_CAPACITY 100

int compare_nodes(MinHeap *heap, int index1, int index2) {
    // Returns 1 if node A has priority, 0 if node B has priority.
    Node *a = heap -> data[index1], *b = heap -> data[index2];

    if ((a -> g) + (a -> h) != (b -> g) + (b -> h)) return (a -> g) + (a -> h) < (b -> g) + (b -> h);
    else if (a -> h != b -> h) return a -> h < b -> h;
    else if (a -> x != b -> x) return a -> x < b -> x;
    else return a -> y < b -> y;
}

void swap_nodes(MinHeap *heap, int index1, int index2) {
    Node *temp = heap -> data[index1];
    heap -> data[index1] = heap -> data[index2];
    heap -> data[index2] = temp;
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

void heapify_up(MinHeap *heap, int index) {
    int i = index;
    while (i > 0 && heap -> data[(i - 1) / 2] > heap -> data[i]) {
        int pi = (i - 1) / 2;
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
    heapify_up(heap, (heap -> size)++);
}

Node *heap_extract_min(MinHeap *heap) {
    Node *ret = heap -> data[0];
    swap_nodes(heap, 0, (heap -> size)--);
    heapify_down(heap, 0);
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
    free(heap);
}

int main() {

}
