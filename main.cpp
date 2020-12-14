#include <iostream>
#include <queue>
#include "BPlusTree.h"

using namespace std;

BPlusTree mytree(3);
queue<int> myqueue;

bool testFunction(){
    Value *v = nullptr;
    for (int i = 0; i < 10000; ++i) {
        int x = rand();
        myqueue.push(x);
        mytree.put(x, x);
    }
    bool flag = true;
    while (!myqueue.empty()) {
        int x = myqueue.front();
        myqueue.pop();
        if (x != mytree.get(x, v)) {
            flag = false;
            break;
        }
    }
    return flag;
}

int main() {
    cout<<testFunction()<<endl;
//    mytree.printTree();

    return 0;
}