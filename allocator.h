//
// Created by 王柯 on 2021-01-23.
//

#ifndef TREE_ALLOCATOR_H
#define TREE_ALLOCATOR_H

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <new>
#include <shared_mutex>
#include <pthread.h>
#include "struct.h"

using namespace std;

class ALLOCATOR {
private:
    int nodeNvmCnt = 0;
    int nodeDramCnt = 0;
    int kvNvmCnt = 0;
    int kvDramCnt = 0;
    bptNode *nodeNvmSpace = nullptr;
    KeyValue *kvNvmSpace = nullptr;
    bptNode *nodeDramSpace = nullptr;
    KeyValue *kvDramSpace = nullptr;
    shared_mutex nodeNvmLock;
    shared_mutex kvNvmLock;
    shared_mutex nodeDramLock;
    shared_mutex kvDramLock;

public:
    ALLOCATOR();

    ~ALLOCATOR();

    void initNodeNvmSpace(string _fileName = "./nodeNvm");

    void initKvNvmSpace(string _fileName = "./kvNvm");

    bptNode *allocateNode(bool _onNvm = false);

    KeyValue *allocateKv(Key k, Value v, bool _onNvm = false);

    void deleteNode();

    void deleteKv();
};


#endif //TREE_ALLOCATOR_H
