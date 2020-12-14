#include <iostream>
#include "BPlusTree.h"

using namespace std;

BPlusTree mytree(3);

int main() {
    Value *v = nullptr;
    for (int i = 0; i < 4; ++i) {
        mytree.put(i, i);
    }
    mytree.printTree();
    cout << mytree.get(0, v)<<endl;
    return 0;
}