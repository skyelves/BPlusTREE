//
// Created by 王柯 on 2021-01-27.
//

#include "lockManager.h"

void lockManager::shared_lock(bptNode *node) {
    node->lock.lock_shared();
}

void lockManager::shared_unlock(bptNode *node) {
    node->lock.unlock_shared();
}

void lockManager::exclusive_lock(bptNode *node) {
    node->lock.lock();
}

void lockManager::exclusive_unlock(bptNode *node) {
    node->lock.unlock();
}