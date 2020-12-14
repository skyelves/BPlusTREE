//
// Created by 王柯 on 2020-12-08.
//

#ifndef TREE_BPLUSTREE_H
#define TREE_BPLUSTREE_H

#include "struct.h"

using namespace std;

class bptNode : public Node {
public:
    int nKeys = 0;
    bptNode *prev = nullptr;
    bptNode *next = nullptr;
    Key *keys = nullptr;
    bptNode **child = nullptr;
    KeyValue **kv = nullptr;
};

// n pointers with (n-1) keys
// https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html
class BPlusTree {
private:
    int order = 3;
    bptNode *root = nullptr;

public:
    BPlusTree();

    BPlusTree(int _order);

    ~BPlusTree();

    bptNode *makeNode(bool _isLeaf = false);

    inline bool isFull(bptNode *node);

    bptNode *findParent(bptNode *node); // we don't maintain parent pointers, instead we traverse to find the parent

    int findPlace(Key *keys, int nKeys, Key k);

    void spliteInnerNode(bptNode *node, Key k, bptNode *leftChild, bptNode *rightChild);

    void recursiveInsert(bptNode *tmproot, Key k, Value v);

    void put(Key k, Value v);

    void get(Key k, Value &v);

    void del(Key k, Value &v);

    void update(Key k, Value v);

    void printTree();
};


#endif //TREE_BPLUSTREE_H
