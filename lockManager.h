//
// Created by 王柯 on 2021-01-27.
//

#ifndef TREE_LOCKMANAGER_H
#define TREE_LOCKMANAGER_H

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <pthread.h>
#include "struct.h"

using namespace std;

class lockManager {
public:
    void shared_lock(bptNode *node);//shared lock

    void exclusive_lock(bptNode *node);//exclusive lock

    void shared_exclusive_lock(bptNode *node);//shared exclusive lock

    void shared_unlock(bptNode *node);//shared unlock

    void exclusive_unlock(bptNode *node);//exclusive unlock

    void shared_exclusive_unlock(bptNode *node);//shared exclusive unlock
};


#endif //TREE_LOCKMANAGER_H
