#include "TermsBST.h"

TermsBST::TermsBST() : root(nullptr)
{

}
TermsBST::~TermsBST()
{
	postOrderDelete(root);
}


TermsBSTNode* TermsBST::getRoot()
{
	return root;
}

void TermsBST::setRoot(TermsBSTNode* node) {
	root = node;
}

void TermsBST::postOrderDelete(TermsBSTNode* cur) {
	if (!cur)
		return;
	postOrderDelete(cur->getLeft());
	postOrderDelete(cur->getRight());
	delete cur;
}

// insert
void TermsBST::insert(TermsBSTNode* cur) {
	if (root == NULL) {
		root = cur;
		return;
	}

	TermsBSTNode* p = root, * pp = nullptr;
	/* find proper location of cur */
	while (p) {
		pp = p;
		if (cur->getEndInfoCollection() < p->getEndInfoCollection())
			p = p->getLeft();
		else
			p = p->getRight();
	}
	/* insert cur to BST */
	if (cur->getEndInfoCollection() < pp->getEndInfoCollection())
		pp->setLeft(cur);
	else
		pp->setRight(cur);
}

// search
TermsBSTNode* TermsBST::search(string date) {
	TermsBSTNode* p = root;

	while (p) {
		if (date < p->getEndInfoCollection()) { p = p->getLeft(); }
		else if (date > p->getEndInfoCollection()) { p = p->getRight(); }
		else break;
	}

	return p;
}

// print
void TermsBST::printAll(ofstream& flog) {
	inorderPrint(flog, root);
}
void TermsBST::inorderPrint(ofstream& flog, TermsBSTNode* cur) {
	if (cur == nullptr)
		return;
	inorderPrint(flog, cur->getLeft());
	cur->printInfo(flog);
	inorderPrint(flog, cur->getRight());
}

// delete
bool TermsBST::deleteNode(NameBSTNode* cur) {
	/* find node that should be deleted */
	TermsBSTNode* delNode = root, *parent = nullptr;
	
	while (delNode && delNode->getName() != cur->getName()) {
		parent = delNode;
		if (delNode->getEndInfoCollection() > cur->getEndInfoCollection())
			delNode = delNode->getLeft();
		else
			delNode = delNode->getRight();
	}
	/* if the node that has same name not exists */
	if (delNode == nullptr)
		return false;

	// delNode is a leaf node
	if (!delNode->getLeft() && !delNode->getRight()) {
		if (!parent)
			root = nullptr;
		else if (parent->getLeft() == delNode)
			parent->setLeft(nullptr);
		else
			parent->setRight(nullptr);

		delete delNode;
	}
	// delNode has only right child
	else if (!delNode->getLeft()) {
		if (!parent)
			root = delNode->getRight();
		else if (parent->getLeft() == delNode)
			parent->setLeft(delNode->getRight());
		else
			parent->setRight(delNode->getRight());

		delete delNode;
	}
	// delNode has only left child
	else if (!delNode->getRight()) {
		if (!parent)
			root = delNode->getLeft();
		else if (parent->getLeft() == delNode)
			parent->setLeft(delNode->getLeft());
		else
			parent->setRight(delNode->getLeft());

		delete delNode;
	}
	// delNode has left and right child
	else {
		TermsBSTNode* pprev = delNode;
		TermsBSTNode* prev = delNode->getRight();
		TermsBSTNode* curr = delNode->getRight()->getLeft();

		while (curr) {
			pprev = prev;
			prev = curr;
			curr = curr->getLeft();
		}
		delNode->updateInfo(prev);
		if (pprev == delNode)
			pprev->setRight(prev->getRight());
		else
			pprev->setLeft(prev->getRight());
		delete prev;
	}

	return true;
}