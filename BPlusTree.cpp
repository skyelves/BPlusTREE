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
    minOrder = 1;
    root = makeNode(true);
    root->isRoot = 1;
}

BPlusTree::BPlusTree(int _order) {
    order = _order;
    minOrder = (order + 1) / 2 - 1;
    root = makeNode(true);
    root->isRoot = 1;
}

BPlusTree::~BPlusTree() {
    
}

void BPlusTree::initialize(int _order) {
    order = _order;
    minOrder = (order + 1) / 2 - 1;
    root = makeNode(true);
    root->isRoot = 1;
}

inline bool BPlusTree::isFull(bptNode *node) {
    return node->nKeys >= order;
}

bptNode *BPlusTree::findLeaf(Key k) {
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
    return tmp;
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
    // if exist, return -1
    int place = nKeys;
    for (int i = 0; i < nKeys; ++i) {
        if (k == keys[i]) {
            place = -1;
            break;
        }
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

bool BPlusTree::put(Key k, Value v) {
    if (root == nullptr) {
        root = makeNode(true);
        root->isRoot = 1;
    }
    //find the leaf node
    bptNode *tmp = findLeaf(k);
    //insert to the leaf node
    KeyValue *tmpkv = new KeyValue(k, v);
    int place = findPlace(tmp->keys, tmp->nKeys, k);
    if (place == -1) // k exists in B+Tree
        return false;
    for (int i = tmp->nKeys - 1; i >= place; --i) {
        tmp->keys[i + 1] = tmp->keys[i];
        tmp->kv[i + 1] = tmp->kv[i];
    }
    tmp->keys[place] = k;
    tmp->kv[place] = tmpkv;
    tmp->nKeys++;
    //judge if full, if full,then splite
    if (isFull(tmp)) {
        bptNode *leftNode = makeNode(true);
        bptNode *rightNode = makeNode(true);
        int mid = order / 2;
        for (int i = 0; i < mid; ++i) {
            leftNode->keys[i] = tmp->keys[i];
            leftNode->kv[i] = tmp->kv[i];
            leftNode->nKeys++;
        }
        int j = 0;
        for (int i = mid; i < tmp->nKeys; ++i) {
            rightNode->keys[j] = tmp->keys[i];
            rightNode->kv[j] = tmp->kv[i];
            rightNode->nKeys++;
            ++j;
        }
        //rightNode->nKeys = order + 1 - leftNode->nKeys;
        leftNode->prev = tmp->prev;
        leftNode->next = rightNode;
        rightNode->prev = leftNode;
        rightNode->next = tmp->next;
        spliteInnerNode(findParent(tmp), rightNode->keys[0], leftNode, rightNode);
        delete tmp;
    }
    return true;
}

Value BPlusTree::get(Key k, Value *v) {
    bptNode *tmp = findLeaf(k);
    for (int i = 0; i < tmp->nKeys; ++i) {
        if (k == tmp->keys[i]) {
            v = &(tmp->kv[i]->v);
            return *v;
        }
    }
    return -1;
}

bool BPlusTree::IndexA2B(Key A, Key B) {
    if (A == B)
        return true;
    bptNode *tmp = root;
    bool flag = true;
    while (flag && !tmp->isLeaf) {
        bool flag1 = true;
        for (int i = 0; i < tmp->nKeys; ++i) {
            if (tmp->keys[i] == A) {
                tmp->keys[i] = B;
                flag = false;
                break;
            } else if (tmp->keys[i] > A) {
                tmp = tmp->child[i];
                flag1 = false;
                break;
            }
        }
        if (flag1)
            tmp = tmp->child[tmp->nKeys];
    }
}

//for leaf nodes
bool BPlusTree::borrowFromLeft(bptNode *node, bptNode *left) {
    //node's keys shift right and fill the key[0] with largest key in the left sibling
    for (int i = node->nKeys; i > 0; --i) {
        node->keys[i] = node->keys[i - 1];
        node->kv[i] = node->kv[i - 1];
    }
    node->keys[0] = left->keys[left->nKeys - 1];
    node->kv[0] = left->kv[left->nKeys - 1];
    node->nKeys++;
    left->nKeys--;
}

//for leaf nodes
bool BPlusTree::borrowFromRigjt(bptNode *node, bptNode *right) {
    node->keys[node->nKeys] = right->keys[0];
    node->kv[node->nKeys] = right->kv[0];
    for (int i = 1; i < right->nKeys; ++i) {
        right->keys[i - 1] = right->keys[i];
        right->kv[i - 1] = right->kv[i];
    }
    node->nKeys++;
    right->nKeys--;
}

bool BPlusTree::mergeInnerNode(bptNode *node) {
    if (node->isRoot) {
        if (node->nKeys == 0) {
            root = node->child[0];
            node->child[0]->isRoot = 1;
            return true;
        }
        return true;
    }
    if (node->nKeys < minOrder) {
        //find left and right siblings
        bptNode *parent = findParent(node);
        bptNode *left = nullptr, *right = nullptr;
        int index = 0;
        for (index = 0; index < parent->nKeys; ++index) {
            if (parent->child[index] == node) {
                break;
            }
        }
        if (index == 0) {
            right = parent->child[index + 1];
        } else if (index == parent->nKeys) {
            left = parent->child[index - 1];
        } else {
            left = parent->child[index - 1];
            right = parent->child[index + 1];
        }
        //borrow or merge
        if (left != nullptr && left->nKeys > minOrder) {
            node->child[node->nKeys + 1] = node->child[node->nKeys];
            for (int i = node->nKeys; i > 0; --i) {
                node->keys[i] = node->keys[i - 1];
                node->child[i] = node->child[i - 1];
            }
            node->keys[0] = parent->keys[index - 1];
            parent->keys[index - 1] = left->keys[left->nKeys - 1];
            node->child[0] = left->child[left->nKeys];
            node->nKeys++;
            left->nKeys--;
        } else if (right != nullptr && right->nKeys > minOrder) {
            node->keys[node->nKeys] = parent->keys[index];
            node->child[node->nKeys + 1] = right->child[0];
            parent->keys[index] = right->keys[0];
            for (int i = 0; i < right->nKeys - 1; ++i) {
                right->keys[i] = right->keys[i + 1];
                right->child[i] = right->child[i + 1];
            }
            right->child[right->nKeys - 1] = right->child[right->nKeys];
            node->nKeys++;
            right->nKeys--;
        } else if (left != nullptr) {
            bptNode *newnode = makeNode();
            newnode->nKeys = left->nKeys + node->nKeys + 1;
            for (int i = 0; i < left->nKeys; ++i) {
                newnode->keys[i] = left->keys[i];
                newnode->child[i] = left->child[i];
            }
            newnode->keys[left->nKeys] = parent->keys[index - 1];
            newnode->child[left->nKeys] = left->child[left->nKeys];
            for (int i = 0, j = left->nKeys + 1; i < node->nKeys; ++i, ++j) {
                newnode->keys[j] = node->keys[i];
                newnode->child[j] = node->child[i];
            }
            newnode->child[left->nKeys + node->nKeys + 1] = node->child[node->nKeys];
            parent->child[index - 1] = newnode;
            for (int i = index; i < parent->nKeys; ++i) {
                parent->keys[i - 1] = parent->keys[i];
                parent->child[i] = parent->child[i + 1];
            }
            parent->nKeys--;
            mergeInnerNode(parent);
            delete node;
            delete left;
        } else {
            bptNode *newnode = makeNode();
            newnode->nKeys = right->nKeys + node->nKeys + 1;
            for (int i = 0; i < node->nKeys; ++i) {
                newnode->keys[i] = node->keys[i];
                newnode->child[i] = node->child[i];
            }
            newnode->keys[node->nKeys] = parent->keys[index];
            newnode->child[node->nKeys] = node->child[node->nKeys];
            for (int i = 0, j = node->nKeys + 1; i < right->nKeys; ++i, ++j) {
                newnode->keys[j] = right->keys[i];
                newnode->child[j] = right->child[i];
            }
            newnode->child[node->nKeys + right->nKeys + 1] = right->child[right->nKeys];
            parent->child[index] = newnode;
            for (int i = index + 1; i < parent->nKeys; ++i) {
                parent->keys[i - 1] = parent->keys[i];
                parent->child[i] = parent->child[i + 1];
            }
            parent->nKeys--;
            mergeInnerNode(parent);
            delete node;
            delete right;
        }
    }
}

bool BPlusTree::del(Key k, Value *v) {
    bptNode *tmp = findLeaf(k);
    int place = -1;
    for (int i = 0; i < tmp->nKeys; ++i) {
        if (k == tmp->keys[i]) {
            place = i;
            break;
        }
    }
    if (place == -1)
        return false;
    *v = tmp->kv[place]->v;
    delete tmp->kv[place];
    //delete k first
    for (int i = place + 1; i < tmp->nKeys; ++i) {
        tmp->keys[i - 1] = tmp->keys[i];
        tmp->kv[i - 1] = tmp->kv[i];
    }
    tmp->nKeys--;
    if (tmp->isRoot)
        return true;
    //set index
    if (place == 0)
        IndexA2B(k, tmp->keys[0]);
    //borrow or merge
    if (tmp->nKeys < minOrder) {
        //find left and right siblings
        bptNode *parent = findParent(tmp);
        bptNode *left = nullptr, *right = nullptr;
        int index = 0;
        for (index = 0; index < parent->nKeys; ++index) {
            if (parent->child[index] == tmp) {
                break;
            }
        }
        if (index == 0) {
            right = parent->child[index + 1];
        } else if (index == parent->nKeys) {
            left = parent->child[index - 1];
        } else {
            left = parent->child[index - 1];
            right = parent->child[index + 1];
        }

        //borrow or merge
        if (left != nullptr && left->nKeys > minOrder) {
            Key oldTmpIndex = tmp->keys[0];
            Key oldLeftIndex = left->keys[0];
            borrowFromLeft(tmp, left);
            IndexA2B(oldTmpIndex, tmp->keys[0]);
            IndexA2B(oldLeftIndex, left->keys[0]);
        } else if (right != nullptr && right->nKeys > minOrder) {
            Key oldTmpIndex = tmp->keys[0];
            Key oldRightIndex = right->keys[0];
            borrowFromRigjt(tmp, right);
            IndexA2B(oldRightIndex, right->keys[0]);
            IndexA2B(oldTmpIndex, tmp->keys[0]);
        } else if (left != nullptr) {
            //merge left leaf node
            bptNode *newnode = makeNode(true);
            newnode->nKeys = left->nKeys + tmp->nKeys;
            for (int i = 0; i < left->nKeys; ++i) {
                newnode->keys[i] = left->keys[i];
                newnode->kv[i] = left->kv[i];
            }
            for (int i = 0, j = left->nKeys; i < tmp->nKeys; ++i, ++j) {
                newnode->keys[j] = tmp->keys[i];
                newnode->kv[j] = tmp->kv[i];
            }
            //set parent node
            parent->child[index - 1] = newnode;
            for (int i = index; i < parent->nKeys; ++i) {
                parent->keys[i - 1] = parent->keys[i];
                parent->child[i] = parent->child[i + 1];
            }
            parent->nKeys--;
            //always equal, but keep the behavior same as merge right
            IndexA2B(left->keys[0], newnode->keys[0]);
            mergeInnerNode(parent);
            delete left;
            delete tmp;
        } else {
            //merge right leaf node
            bptNode *newnode = makeNode(true);
            newnode->nKeys = right->nKeys + tmp->nKeys;
            for (int i = 0; i < tmp->nKeys; ++i) {
                newnode->keys[i] = tmp->keys[i];
                newnode->kv[i] = tmp->kv[i];
            }
            for (int i = 0, j = tmp->nKeys; i < right->nKeys; ++i, ++j) {
                newnode->keys[j] = right->keys[i];
                newnode->kv[j] = right->kv[i];
            }
            //set parent node
            parent->child[index] = newnode;
            for (int i = index; i < parent->nKeys - 1; ++i) {
                parent->keys[i] = parent->keys[i + 1];
                parent->child[i + 1] = parent->child[i + 2];
            }
            parent->nKeys--;
            //necessary, when tmp->nKeys==0
            IndexA2B(tmp->keys[0], newnode->keys[0]);
            mergeInnerNode(parent);
            delete right;
            delete tmp;
        }
    }
    return true;
}

bool BPlusTree::update(Key k, Value v) {
    bptNode *tmp = findLeaf(k);
    for (int i = 0; i < tmp->nKeys; ++i) {
        if (k == tmp->keys[i]) {
            tmp->kv[i]->v = v;
            return true;
        }
    }
    return false;
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