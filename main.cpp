#include <iostream>
#include "BPlusTree.h"

using namespace std;

BPlusTree mytree(3);

int main() {
    for (int i =0; i < 4; ++i) {
        mytree.put(i, 1);
    }
    mytree.printTree();
    return 0;
}