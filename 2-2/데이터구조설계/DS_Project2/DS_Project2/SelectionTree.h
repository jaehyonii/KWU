#pragma once

#include "SelectionTreeNode.h"

#include <fstream>
#include <iostream>
#include <queue>
class SelectionTree
{
private:
    SelectionTreeNode* root;
    SelectionTreeNode* leafNode[8];
    ofstream* fout;

public:
    SelectionTree(ofstream* fout) {
        this->fout = fout;
        
        root = new SelectionTreeNode();

        queue<SelectionTreeNode*> Queue;
        Queue.push(root);
        
        // there's 7 nodes except leaf nodes
        for (int i = 0; i < 7; i++) {
            SelectionTreeNode* cur = Queue.front();
            Queue.pop();
            
            // create left node
            SelectionTreeNode* left = new SelectionTreeNode();
            cur->setLeftChild(left);
            left->setParent(cur);
            Queue.push(left);

            // create right node
            SelectionTreeNode* right = new SelectionTreeNode();
            right = new SelectionTreeNode();
            cur->setRightChild(right);
            right->setParent(cur);
            Queue.push(right);
        }
        
        // create LoanBookHeap at leaf nodes
        int cnt = 0;
        while (!Queue.empty()) {
            LoanBookHeap* lbh = new LoanBookHeap();
            leafNode[cnt] = Queue.front();
            leafNode[cnt++]->setHeap(lbh);
            Queue.pop();
        }
    }
    ~SelectionTree() {
        queue<SelectionTreeNode*> Queue;
        Queue.push(root);

        // delete all selectionTreeNode
        while (!Queue.empty()) {
            SelectionTreeNode* cur = Queue.front();
            Queue.pop();

            if (cur->getLeftChild())
                Queue.push(cur->getLeftChild());
            else if(cur->getRightChild())
                Queue.push(cur->getRightChild());
            
            delete cur;
        }
    }

    void setRoot(SelectionTreeNode* pN) { this->root = pN; }
    SelectionTreeNode* getRoot() { return root; }

    bool Insert(LoanBookData* newData);
    bool Delete();
    bool printBookData(int bookCode);
};