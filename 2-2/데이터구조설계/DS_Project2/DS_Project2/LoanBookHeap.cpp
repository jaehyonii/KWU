#include "LoanBookHeap.h"
#include <queue>
#include <iostream>

void LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) {
    // swap LoanBookData
    while(pN != root && pN->getBookData()->getName() < pN->getParent()->getBookData()->getName()){
        LoanBookData* lbd = pN->getBookData();
        pN->setBookData(pN->getParent()->getBookData());
        pN->getParent()->setBookData(lbd);
        
        pN = pN->getParent();
    }
}

void LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) {
    if (pN == NULL)
        return;

    queue<LoanBookHeapNode*> Queue;
    
    // find last node
    Queue.push(root);
    LoanBookHeapNode* lastNode = root;
    while (!Queue.empty()) {
        lastNode = Queue.front();
        Queue.pop();
        if(lastNode->getLeftChild())
            Queue.push(lastNode->getLeftChild());
        if (lastNode->getRightChild())
            Queue.push(lastNode->getRightChild());
    }

    // there's only root node in Heap
    if (root == lastNode) {
        delete root;
        root = NULL;
        return;
    }
    
    // move lastNode's information to pN and delete lastNode
    delete pN->getBookData();
    pN->setBookData(lastNode->getBookData());
    lastNode->setBookData(NULL);
    if (lastNode->getParent()->getRightChild())
        lastNode->getParent()->setRightChild(NULL);
    else
        lastNode->getParent()->setLeftChild(NULL);
    delete lastNode;
    

    // bubbleDown LoanBookData
    while (pN->getLeftChild() != NULL) {
        LoanBookData* lbd = pN->getBookData();
        LoanBookHeapNode* child;
        if (pN->getRightChild() != NULL) {
            if (pN->getLeftChild()->getBookData()->getName() < pN->getRightChild()->getBookData()->getName())
                child = pN->getLeftChild();
            else
                child = pN->getRightChild();
        }
        else
            child = pN->getLeftChild();

        if (pN->getBookData()->getName() > child->getBookData()->getName()) {
            pN->setBookData(child->getBookData());
            child->setBookData(lbd);
            pN = child;
        }
        else
            break;
    }
}

bool LoanBookHeap::Insert(LoanBookData* data) {
    queue<LoanBookHeapNode*> Queue;
    LoanBookHeapNode* lbhn = new LoanBookHeapNode();
    lbhn->setBookData(data);

    if (root == NULL) {
        root = lbhn;
        return true;
    }

    Queue.push(root);
    LoanBookHeapNode* cur = root;
    while (cur->getLeftChild() && cur->getRightChild()) {
        Queue.push(cur->getLeftChild());
        Queue.push(cur->getRightChild());
        Queue.pop();

        cur = Queue.front();
    }
    
    if (cur->getLeftChild() == NULL)
        cur->setLeftChild(lbhn);
    else
        cur->setRightChild(lbhn);
    lbhn->setParent(cur);
    
    heapifyUp(lbhn);

    return true;
}