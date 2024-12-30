#include "NameBST.h"

NameBST::NameBST() : root(nullptr)
{

}
NameBST::~NameBST()
{
	// delete all nodes using Postorder
	postOrderDel(root);
}

void NameBST::postOrderDel(NameBSTNode* cur) {
	if (!cur)
		return;
	postOrderDel(cur->getLeft());
	postOrderDel(cur->getRight());
	delete cur;
}
NameBSTNode* NameBST::getRoot()
{
	return root;
}

// insert
void NameBST::insert(NameBSTNode* cur) {
	if (root == NULL) {
		root = cur;
		return;
	}

	NameBSTNode* p = root, * pp = nullptr;
	/* find proper location of cur
	# there is no one who has same name*/
	
	while (p) {
		pp = p;
		if (cur->getName() < p->getName())
			p = p->getLeft();
		else 
			p = p->getRight();
	}

	/* insert cur to BST */
	if (cur->getName() < pp->getName())
		pp->setLeft(cur);
	else
		pp->setRight(cur);
}

// search
NameBSTNode* NameBST::search(string name) {
	NameBSTNode* p = root;

	while (p) {
		if (name < p->getName()) { p = p->getLeft(); }
		else if (name > p->getName()) { p = p->getRight(); }
		else break;
	}

	return p;
}

// print
void NameBST::printAll(ofstream& flog) {
	inorderPrint(flog, root);
}
void NameBST::inorderPrint(ofstream& flog, NameBSTNode* cur) {
	if (cur == nullptr)
		return;
	inorderPrint(flog, cur->getLeft());
	cur->printInfo(flog);
	inorderPrint(flog, cur->getRight());
}

// delete
NameBSTNode* NameBST::deleteNode(string name) {
	/* find node that should be deleted */
	NameBSTNode* delNode = root, * parent = nullptr;
	while (delNode && delNode->getName() != name) {
		parent = delNode;
		if (delNode->getName() > name)
			delNode = delNode->getLeft();
		else
			delNode = delNode->getRight();
	}

	/* if the node that has same name not exists */
	if (delNode == nullptr)
		return nullptr;

	// delNode is a leaf node
	if (!delNode->getLeft() && !delNode->getRight()) {
		if (!parent) 
			root = nullptr;
		else if (parent->getLeft() == delNode) 
			parent->setLeft(nullptr);
		else 
			parent->setRight(nullptr);

		return delNode;
	}
	// delNode has only right child
	else if (!delNode->getLeft()) {
		if (!parent)
			root = delNode->getRight();
		else if (parent->getLeft() == delNode)
			parent->setLeft(delNode->getRight());
		else
			parent->setRight(delNode->getRight());

		return delNode;
	}
	// delNode has only left child
	else if (!delNode->getRight()) {
		if (!parent)
			root = delNode->getLeft();
		else if (parent->getLeft() == delNode)
			parent->setLeft(delNode->getLeft());
		else
			parent->setRight(delNode->getLeft());

		return delNode;
	}
	// delNode has left and right child
	else {
		NameBSTNode* pprev = delNode;
		NameBSTNode* prev = delNode->getRight();
		NameBSTNode* curr = delNode->getRight()->getLeft();
		
		while (curr) {
			pprev = prev;
			prev = curr;
			curr = curr->getLeft();
		}
		NameBSTNode tmp(*delNode);
		delNode->updateInfo(prev);
		if (pprev == delNode)
			pprev->setRight(prev->getRight());
		else
			pprev->setLeft(prev->getRight());

		prev->updateInfo(&tmp);
		return prev;
	}
}