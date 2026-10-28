#include <stdio.h>
#include "assg.h"

#define HEAP_CAPACITY 100

Node* create_node(int x, int y, int g, int h, Node *parent) {
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

int main() {

}
