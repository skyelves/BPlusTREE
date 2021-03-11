//
// Created by 王柯 on 2021-01-23.
//

#include "allocator.h"

#define TESTNUM 1000000


ALLOCATOR::ALLOCATOR() {
    nodeNvmCnt = 0;
    nodeDramCnt = 0;
    kvNvmCnt = 0;
    kvDramCnt = 0;
    nodeDramSpace = new bptNode[TESTNUM];
    kvDramSpace = new KeyValue[TESTNUM];
}

ALLOCATOR::~ALLOCATOR() {
    delete[]nodeDramSpace;
    delete[]kvDramSpace;
    munmap(nodeNvmSpace, 1 << 30);
    munmap(kvNvmSpace, 1 << 30);
}

void ALLOCATOR::initNodeNvmSpace(string _fileName) {
    int nvmFd = open(_fileName.c_str(), O_CREAT | O_RDWR, 0644);
    nodeNvmSpace = (bptNode *) mmap(NULL, 1 << 30, PROT_READ | PROT_WRITE, MAP_SYNC | MAP_SHARED_VALIDATE, nvmFd, 0);
    close(nvmFd);
}


void ALLOCATOR::initKvNvmSpace(string _fileName) {
    int nvmFd = open(_fileName.c_str(), O_CREAT | O_RDWR, 0644);
    kvNvmSpace = (KeyValue *) mmap(NULL, 1 << 30, PROT_READ | PROT_WRITE, MAP_SYNC | MAP_SHARED_VALIDATE, nvmFd, 0);
    close(nvmFd);
}

bptNode *ALLOCATOR::allocateNode(bool _onNvm) {
    bptNode *res = nullptr;
    if (_onNvm && nodeNvmSpace != nullptr) {
//        nodeNvmLock.lock();
        res = new(nodeNvmSpace + nodeNvmCnt) bptNode;
        nodeNvmCnt++;
//        nodeNvmLock.unlock();
    } else {
//        nodeDramLock.lock();
        res = new(nodeDramSpace + nodeDramCnt) bptNode;
        nodeDramCnt++;
//        nodeDramLock.unlock();
    }
    return res;
}


KeyValue *ALLOCATOR::allocateKv(Key k, Value v, bool _onNvm) {
    KeyValue *res = nullptr;
    if (_onNvm && kvNvmSpace != nullptr) {
//        kvNvmLock.lock();
        res = new(kvNvmSpace + kvNvmCnt) KeyValue;
        kvNvmCnt++;
//        kvNvmLock.unlock();
    } else {
//        kvDramLock.lock();
        res = new(kvDramSpace + kvDramCnt) KeyValue;
        kvDramCnt++;
//        kvDramLock.unlock();
    }
    res->k = k;
    res->v = v;
    return res;
}


void ALLOCATOR::deleteNode() {

}


void ALLOCATOR::deleteKv() {

}