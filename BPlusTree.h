//
// Created by 王柯 on 2020-12-08.
//

#ifndef TREE_BPLUSTREE_H
#define TREE_BPLUSTREE_H

#include <iostream>
#include <queue>
#include <stack>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <new>
#include "struct.h"
#include "allocator.h"
#include "lockManager.h"

using namespace std;

// n pointers with (n-1) keys
// https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html
// http://mysql.taobao.org/monthly/2018/09/01/ algorithm 4 and algorithm 5
class BPlusTree {
private:
     int minOrder = 15;
    bptNode *root = nullptr;
    ALLOCATOR allocator;
    lockManager mylock;

public:
    BPlusTree();

    BPlusTree(string _nodeNvmFile, string _kvNvmFile);

    ~BPlusTree();

    void initialize();

    void initNodeNvm(string _fileName);

    void initKvNvm(string _fileName);

    void clear();

    bptNode *makeNode(bool _isLeaf = false, bool _onNvm = false);

    inline bool isFull(bptNode *node);

    bptNode *findLeaf(Key k);

    bptNode *findParent(bptNode *node); // we don't maintain parent pointers, instead we traverse to find the parent

    int findPlace(Key *keys, int nKeys, Key k);

    void spliteInnerNode(stack<bptNode *> parentNode, Key k, bptNode *leftChild, bptNode *rightChild);

    bool IndexA2B(Key A, Key B);

    bool borrowFromLeft(bptNode *node, bptNode *left);

    bool borrowFromRigjt(bptNode *node, bptNode *right);

    bool mergeInnerNode(bptNode *node);

    bool put(Key k, Value v);

    Value get(Key k);

    bool del(Key k, Value *v);

    bool update(Key k, Value v);

    void printTree();
};


#endif //TREE_BPLUSTREE_H
