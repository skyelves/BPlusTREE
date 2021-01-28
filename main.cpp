#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <sys/time.h>
#include "BPlusTree.h"

using namespace std;

#define TESTNUM 1000000

string nodeNvmFile = "/aepmount/test0.txt";
string kvNvmFile = "/aepmount/test1.txt";

BPlusTree mytree(nodeNvmFile, kvNvmFile);
map<Key, Value> mm;

bool testPut() {
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
        if (y != mytree.get(x)) {
            flag = false;
            break;
        }
    }
    return flag;
}

bool testUpdate() {
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
        Value res = mytree.get(x);
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
        if (y != mytree.get(x)) {
            cout << x << " " << y << endl;
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
    double timeCost = (ends.tv_sec - start.tv_sec) * 1000000 + ends.tv_usec - start.tv_usec;
    double throughPut = (double) TESTNUM / timeCost;
    cout << "Put " << TESTNUM << " kv pais in " << timeCost / 1000000 << " s" << endl;
    cout << "Put ThroughPut: " << throughPut << " Mops" << endl;
}


int main() {
//    cout << testPut() << endl;
//    cout << testUpdate() << endl;
//    cout << testDel() << endl;
    speedTest();
    return 0;
}