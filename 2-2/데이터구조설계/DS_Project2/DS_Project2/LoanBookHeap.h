#pragma once

#include "LoanBookData.h"
#include "LoanBookHeapNode.h"

class LoanBookHeap
{
private:
    LoanBookHeapNode* root;

    void deleteRecursive(LoanBookHeapNode* cur) {
        if (cur == NULL)
            return;

        deleteRecursive(cur->getLeftChild());
        deleteRecursive(cur->getRightChild());
        delete cur;
    }

public:
    LoanBookHeap() {
        this->root = NULL;
    }
    ~LoanBookHeap() {
        deleteRecursive(root);
    }

    void setRoot(LoanBookHeapNode* pN) { this->root = pN; }
    LoanBookHeapNode* getRoot() { return root; }

    void heapifyUp(LoanBookHeapNode* pN);
    void heapifyDown(LoanBookHeapNode* pN);

    bool Insert(LoanBookData* data);

    
};