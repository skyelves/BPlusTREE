# BPlusTREE
An implementation of BPlusTree in C++

All inner nodes in B+Tree have n pointers with (n-1) keys except the root node.

The nodes named bptnode are derived from Node in struct.h, where I illustrate some base structures.

Everyone can see the visualization of B+Tree on <https://www.cs.usfca.edu/~galles/visualization/BPlusTree.html>

This B+Tree support four operations:
    
    bool put(Key k, Value v);

    Value get(Key k, Value *v = nullptr);

    bool del(Key k, Value *v);

    bool update(Key k, Value v);
    
The average Put throughput ( When tree order set to 32) is about 1.63174 Mops/s