#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <sys/time.h>
#include <pthread.h>
#include "BPlusTree.h"

using namespace std;

#define TESTNUM 1000000

int numThread = 1;

string nodeNvmFile = "/aepmount/test0.txt";
string kvNvmFile = "/aepmount/test1.txt";
//adaptive_radix_tree *mytree= new_adaptive_radix_tree();
BPlusTree mytree(nodeNvmFile, kvNvmFile);

map<Key, Value> mm;

void *putFunc(void *) {
    for (int i = 0; i < TESTNUM; ++i) {
        int x = rand();
//        adaptive_radix_tree_put(mytree, (const void *) &x, 8);
        mytree.put(x, i);
    }
}

void speedTest() {
    timeval start, ends;
    gettimeofday(&start, NULL);
//    put();
    pthread_t *tids = new pthread_t[numThread];
    for (int i = 0; i < numThread; ++i) {
        int ret = pthread_create(&tids[i], NULL, &putFunc, NULL);
        if (ret != 0) {
            cout << "pthread_create error: error_code=" << ret << endl;
        }
    }
    for (int j = 0; j < numThread; ++j) {
        pthread_join(tids[j], NULL);
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