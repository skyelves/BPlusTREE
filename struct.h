//
// Created by 王柯 on 2020-12-08.
//

#ifndef TREE_STRUCT_H
#define TREE_STRUCT_H

#include <cstdint>

typedef int64_t Key; // key 8 bytes
typedef int64_t Value; // value 8 bytes

class Node {
public:
    bool isRoot = false;
    bool isLeaf = false;

};


class KeyValue {
public:
    Key k;
    Value v;

    KeyValue(Key _k, Value _v) : k(_k), v(_v) {}
};

#endif //TREE_STRUCT_H
