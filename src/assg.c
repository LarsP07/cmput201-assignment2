#include <stdio.h>
#include "assg.h"

Node* create_node(int x, int y, int g, int h, Node *parent) {
    Node* ret = (Node*)malloc(sizeof(Node));
    ret -> x = x;
    ret -> y = y;
    ret -> g = g;
    ret -> h = h;
    ret -> parent = parent;
    return ret;
}

int main() {

}
