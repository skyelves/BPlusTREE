//
// Created by 王柯 on 2020-12-08.
//

#include <iostream>
#include <queue>
#include "BPlusTree.h"

using namespace std;

bptNode *BPlusTree::makeNode(bool _isLeaf) {
    bptNode *newNode = new bptNode;
    newNode->keys = new Key[order];
    for (int i = 0; i < order; ++i) {
        newNode->keys[i] = -1;
    }
    newNode->isLeaf = _isLeaf;
    if (_isLeaf)
        newNode->kv = new KeyValue *[order + 1];
    else
        newNode->child = new bptNode *[order + 1];
    return newNode;
}

BPlusTree::BPlusTree() {
    order = 3;
    root = makeNode(true);
    root->isRoot = 1;
}

BPlusTree::BPlusTree(int _order) {
    order = _order;
    root = makeNode(true);
    root->isRoot = 1;
}

BPlusTree::~BPlusTree() {

}

inline bool BPlusTree::isFull(bptNode *node) {
    return node->nKeys >= order;
}

bptNode *BPlusTree::findParent(bptNode *node) {
    if (node->isRoot)
        return nullptr;
    Key k = node->keys[0];
    bptNode *parent = root, *tmp = root;
    while (tmp != node) {
        bool flag = true;
        for (int i = 0; i < tmp->nKeys; ++i) {
            if (k < tmp->keys[i]) {
                parent = tmp;
                tmp = tmp->child[i];
                flag = false;
                break;
            }
        }
        if (flag) {
            parent = tmp;
            tmp = tmp->child[tmp->nKeys];
        }
    }
    return parent;
}

int BPlusTree::findPlace(Key *keys, int nKeys, Key k) {
    int place = nKeys;
    for (int i = 0; i < nKeys; ++i) {
        if (k < keys[i]) {
            place = i;
            break;
        }
    }
    return place;
}

void BPlusTree::spliteInnerNode(bptNode *node, Key k, bptNode *leftChild, bptNode *rightChild) {
    if (node == nullptr) {
        //splite root
        node = makeNode();
        node->isRoot = 1;
        node->nKeys = 1;
        node->keys[0] = k;
        node->child[0] = leftChild;
        node->child[1] = rightChild;
        root = node;
    } else {
        // insert first, which can be optimize
        int place = findPlace(node->keys, node->nKeys, k);
        for (int i = node->nKeys - 1; i >= place; --i) {
            node->keys[i + 1] = node->keys[i];
            node->child[i + 2] = node->child[i + 1];
        }
        node->nKeys++;
        node->keys[place] = k;
        node->child[place] = leftChild;
        node->child[place + 1] = rightChild;
        //judge if full, if full,then splite
        if (isFull(node)) {
            bptNode *leftNode = makeNode();
            bptNode *rightNode = makeNode();
            int mid = order / 2;
            for (int i = 0; i < mid; ++i) {
                leftNode->keys[i] = node->keys[i];
                leftNode->child[i] = node->child[i];
                leftNode->nKeys++;
            }
            leftNode->child[mid] = node->child[mid];
            int j = 0;
            for (int i = mid + 1; i < node->nKeys; ++i) {
                rightNode->keys[j] = node->keys[i];
                rightNode->child[j] = node->child[i];
                rightNode->nKeys++;
                ++j;
            }
            rightNode->child[j] = node->child[node->nKeys];
            spliteInnerNode(findParent(node), node->keys[mid], leftNode, rightNode);
            delete node;
        }
    }
}

void BPlusTree::recursiveInsert(bptNode *tmproot, Key k, Value v) {
    if (tmproot->isLeaf) {
        //insert first
        KeyValue *tmpkv = new KeyValue(k, v);
        int place = findPlace(tmproot->keys, tmproot->nKeys, k);
        for (int i = tmproot->nKeys - 1; i >= place; --i) {
            tmproot->keys[i + 1] = tmproot->keys[i];
            tmproot->kv[i + 1] = tmproot->kv[i];
        }
        tmproot->keys[place] = k;
        tmproot->kv[place] = tmpkv;
        tmproot->nKeys++;
        //judge if full, if full,then splite
        if (isFull(tmproot)) {
            bptNode *leftNode = makeNode(true);
            bptNode *rightNode = makeNode(true);
            int mid = order / 2;
            for (int i = 0; i < mid; ++i) {
                leftNode->keys[i] = tmproot->keys[i];
                leftNode->kv[i] = tmproot->kv[i];
                leftNode->nKeys++;
            }
            int j = 0;
            for (int i = mid; i < tmproot->nKeys; ++i) {
                rightNode->keys[j] = tmproot->keys[i];
                rightNode->kv[j] = tmproot->kv[i];
                rightNode->nKeys++;
                ++j;
            }
            //rightNode->nKeys = order + 1 - leftNode->nKeys;
            leftNode->prev = tmproot->prev;
            leftNode->next = rightNode;
            rightNode->prev = leftNode;
            rightNode->next = tmproot->next;
            spliteInnerNode(findParent(tmproot), rightNode->keys[0], leftNode, rightNode);
            delete tmproot;
        }
    } else {
        for (int i = 0; i < tmproot->nKeys; ++i) {
            if (k < tmproot->keys[i]) {
                recursiveInsert(tmproot->child[i], k, v);
                return;
            }
        }
        recursiveInsert(tmproot->child[tmproot->nKeys], k, v);
    }
}

void BPlusTree::put(Key k, Value v) {
    if (root == nullptr) {
        root = makeNode();
        root->isLeaf = 1;
        root->isRoot = 1;
    }
    recursiveInsert(root, k, v);
}

Value BPlusTree::get(Key k, Value *v) {
    bptNode *tmp = root;
    while (!tmp->isLeaf) {
        bool flag = true;
        for (int i = 0; i < tmp->nKeys; ++i) {
            if (flag && k < tmp->keys[i]) {
                tmp = tmp->child[i];
                flag = false;
                break;
            }
        }
        if (flag)
            tmp = tmp->child[tmp->nKeys];
    }
    for (int i = 0; i < tmp->nKeys; ++i) {
        if (k == tmp->keys[i]) {
            v = &(tmp->kv[i]->v);
            return *v;
        }
    }
    return -1;
}

void BPlusTree::del(Key k, Value *v) {

}

void BPlusTree::update(Key k, Value v) {

}

void BPlusTree::printTree() {
    queue<bptNode *> q[2];
    q[0].push(root);
    bool flag = false;
    while (!q[flag].empty()) {
        while (!q[flag].empty()) {
            bptNode *node = q[flag].front();
            q[flag].pop();
            if (!node->isLeaf) {
                for (int i = 0; i < node->nKeys; ++i) {
                    cout << node->keys[i] << " ";
                    q[!flag].push(node->child[i]);
                }
                q[!flag].push(node->child[node->nKeys]);
            } else {
                for (int i = 0; i < node->nKeys; ++i) {
                    cout << node->keys[i] << " ";
                }
            }
            cout << "     ";
        }
        cout << endl;
        flag = !flag;
    }
}