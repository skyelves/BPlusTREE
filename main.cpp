#include <iostream>
#include "BPlusTree.h"

using namespace std;

BPlusTree mytree(3);

int main() {
    Value *v = nullptr;
    for (int i = 0; i < 111; ++i) {
        mytree.put(i, i);
    }
    for (int i = 0; i < 10; ++i) {
        cout << mytree.get(i, v) << endl;
    }
    for (int i = 0; i < 10; ++i) {
        mytree.update(i, i * i);
    }
    for (int i = 0; i < 10; ++i) {
        cout << mytree.get(i, v) << endl;
    }
    mytree.printTree();

    return 0;
}