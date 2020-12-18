#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <sys/time.h>
#include "BPlusTree.h"

using namespace std;

#define TESTNUM 10000000

BPlusTree mytree(32);
map<Key, Value> mm;

void interactiveTest() {
    cout << "Input tree order: (recommand 32)  ";
    int order = 32;
    cin >> order;
    mytree.initialize(order);
    cout << "Put:p Get:g Del:d Quit:q" << endl << "Example:(when order == 3)" << endl
         << "p 1" << endl
         << "p 5" << endl
         << "p 3" << endl
         << "p 14" << endl
         << "p 16" << endl;
    mytree.put(1, 1);
    mytree.put(5, 1);
    mytree.put(3, 1);
    mytree.put(14, 1);
    mytree.put(16, 1);
    cout << "Tree structure:" << endl;
    mytree.printTree();
    Value v;
    mytree.del(1, &v);
    mytree.del(5, &v);
    mytree.del(3, &v);
    mytree.del(14, &v);
    mytree.del(16, &v);
    char op;
    int num;
    cout << "Input operation: ";
    while (cin >> op) {
        switch (op) {
            case 'p':
                cin >> num;
                mytree.put(num, num);
                mytree.printTree();
                break;
            case 'g':
                cin >> num;
                cout << mytree.get(num, &v) << endl;
                break;
            case 'd':
                cin >> num;
                mytree.del(num, &v);
                mytree.printTree();
                break;
            case 'q':
                return;
            default:
                cout << "invalid operation,try again" << endl;
                mytree.printTree();
        }
        cout << "Input operation: ";
    }
}

bool testPut() {
    Value *v = nullptr;
    for (int i = 0; i < TESTNUM; ++i) {
        int x = rand() % TESTNUM;
        int y = rand() % TESTNUM;
        if (mm.find(x) == mm.end()) {
            mm[x] = y;
            mytree.put(x, y);
        }
    }
    bool flag = true;
    while (!mm.empty()) {
        Key x = mm.begin()->first;
        Value y = mm.begin()->second;
        mm.erase(mm.begin());
        if (y != mytree.get(x, v)) {
            flag = false;
            break;
        }
    }
    return flag;
}

bool testUpdate() {
    Value *v = nullptr;
    for (int i = 0; i < TESTNUM; ++i) {
        int x = rand() % TESTNUM;
        int y = rand() % TESTNUM;
        if (mm.find(x) == mm.end()) {
            mytree.put(x, y);
        } else {
            mytree.update(x, y);
        }
        mm[x] = y;
    }
    bool flag = true;
    while (!mm.empty()) {
        Key x = mm.begin()->first;
        Value y = mm.begin()->second;
        mm.erase(mm.begin());
        Value res = mytree.get(x, v);
        if (y != res) {
            flag = false;
            break;
        }
    }
    return flag;
}

bool testDel() {
//    Value v;
//    for (int i = 6; i > 0; --i) {
//        mytree.put(i, i);
//    }
//    mytree.printTree();
//    for (int i = 6; i > 0; --  i) {
//        mytree.del(i, &v);
//        mytree.printTree();
//    }
    Value v;
    for (int i = 0; i < TESTNUM; ++i) {
        int x = rand() % TESTNUM;
        int y = rand() % TESTNUM;
        if (mm.find(x) == mm.end()) {
            mytree.put(x, y);
            mm[x] = y;
        } else {
            mytree.del(x, &v);
            if (v != mm[x]) {
//                cout << x << " " << v << " " << mm[x] << endl;
                return false;
            }
            mm.erase(x);
        }
//        cout<<i<<endl<<endl;
//        mytree.printTree();
    }
    bool flag = true;
    while (!mm.empty()) {
        Key x = mm.begin()->first;
        Value y = mm.begin()->second;
        if (y != mytree.get(x, &v)) {
            cout << x << " " << v << " " << y << endl;
            flag = false;
            break;
        }
        mm.erase(mm.begin());

    }
    return flag;
}

void speedTest() {
    timeval start, ends;
    gettimeofday(&start, NULL);
    for (int i = 0; i < TESTNUM; ++i) {
        int x = rand();
        mytree.put(x, i);
    }
    gettimeofday(&ends, NULL);
    int timeCost = (ends.tv_sec - start.tv_sec) * 1000000 + ends.tv_usec - start.tv_usec;
    double throughPut = (double) TESTNUM / timeCost;
    cout << "Put ThroughPut: " << throughPut << " Mops"<< endl;
}

int main() {
//    cout << testPut() << endl;
//    cout << testUpdate() << endl;
//    cout << testDel() << endl;
//    interactiveTest();
    speedTest();
    return 0;
}