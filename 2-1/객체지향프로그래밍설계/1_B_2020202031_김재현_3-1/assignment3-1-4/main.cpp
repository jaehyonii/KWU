#include "Stack.h"

int main() {
	Stack stack;
	Node* node;

	/* Stack Size 입력*/
	int size;
	cout << "Set Size: "; cin >> size;
	stack.SetSize(size);
	cout << "stack Size " << size << endl << endl;

	/* stack이 비어있을 때 실험*/
	cout << "===== 0 Nodes =====" << endl;
	cout << "IsEmpty: "; // IsEmpty 여부 출력
	if (stack.IsEmpty())
		cout << "True" << endl;
	else
		cout << "False" << endl;

	cout << "IsFull: "; // IsFull 여부 출력
	if (stack.IsFull())
		cout << "True" << endl;
	else
		cout << "False" << endl;

	cout << "Print stack: "; // Node 전체 출력
	stack.PrintStack();
	cout << endl << endl;

	/* stack에 노드를 하나씩 채우고 실험*/
	for (int i = 1; ; i++) {
		node = new Node();
		node->SetData(i * 4 - 3);
		if (!stack.Push(node)) {
			delete node;
			break;
		}

		cout << "===== " << i << " Nodes =====" << endl;
		cout << "IsEmpty: "; // IsEmpty 여부 출력
		if (stack.IsEmpty())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "IsFull: "; // IsFull 여부 출력
		if (stack.IsFull())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "Print stack: "; // Node 전체 출력
		stack.PrintStack();
		cout << endl << endl;
	}

	/* 노드를 하나씩 삭제하면서 실험*/
	for (int i = 0; ; i++) {
		/* 더 이상 pop할 노드가 존재하지 않으면 반복문 탈출*/
		if (!(node = stack.Pop())) {
			break;
		}

		cout << "===== Pop =====" << endl;
		cout << "Poped Data: " << node->GetData() << endl;
		delete node;

		cout << "IsEmpty: "; // IsEmpty 여부 출력
		if (stack.IsEmpty())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "IsFull: "; // IsFull 여부 출력
		if (stack.IsFull())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "Print stack: "; // Node 전체 출력
		stack.PrintStack();
		cout << endl << endl;
	}

	return 0;
}