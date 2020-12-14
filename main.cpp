#include <iostream>
#include "BPlusTree.h"

using namespace std;

BPlusTree mytree(3);

int main() {
    Value *v = nullptr;
    for (int i = 0; i < 4; ++i) {
        mytree.put(i, i);
    }
    for (int i = 0; i < 4; ++i) {
        mytree.update(i, i * i);
    }
    for (int i = 0; i < 4; ++i) {
        cout << mytree.get(i, v) << endl;
    }

    return 0;
}