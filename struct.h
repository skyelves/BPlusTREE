//
// Created by 王柯 on 2020-12-08.
//

#ifndef TREE_STRUCT_H
#define TREE_STRUCT_H

#include <cstdint>
#include <shared_mutex>

using namespace std;

#define MAP_SYNC 0x080000 /* perform synchronous page faults for the mapping */
#define MAP_SHARED_VALIDATE 0x03    /* share + validate extension flags */

typedef int64_t Key; // key 8 bytes
typedef int64_t Value; // value 8 bytes

#define ORDER 32

class Node {
public:
    bool isRoot = false;
    bool isLeaf = false;

    Node() {}

    Node(bool _isRoot, bool _isLeaf) : isRoot(_isRoot), isLeaf(_isLeaf) {}
};


class KeyValue {
public:
    Key k;
    Value v;

    KeyValue() {}

    KeyValue(Key _k, Value _v) : k(_k), v(_v) {}
};


class bptNode : public Node {
public:
    int nKeys = 0;
    bptNode *prev = nullptr;
    bptNode *next = nullptr;
    Key keys[ORDER] = {0};
    bptNode *child[ORDER + 1];
    KeyValue *kv[ORDER + 1];
    bool *valid = nullptr;
    shared_mutex lock;
};

#endif //TREE_STRUCT_H
