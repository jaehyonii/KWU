#include "Stack.h"

int main() {
	Stack stack;
	Node* node;

	/* Stack Size �Է�*/
	int size;
	cout << "Set Size: "; cin >> size;
	stack.SetSize(size);
	cout << "stack Size " << size << endl << endl;

	/* stack�� ������� �� ����*/
	cout << "===== 0 Nodes =====" << endl;
	cout << "IsEmpty: "; // IsEmpty ���� ���
	if (stack.IsEmpty())
		cout << "True" << endl;
	else
		cout << "False" << endl;

	cout << "IsFull: "; // IsFull ���� ���
	if (stack.IsFull())
		cout << "True" << endl;
	else
		cout << "False" << endl;

	cout << "Print stack: "; // Node ��ü ���
	stack.PrintStack();
	cout << endl << endl;

	/* stack�� ��带 �ϳ��� ä��� ����*/
	for (int i = 1; ; i++) {
		node = new Node();
		node->SetData(i * 4 - 3);
		if (!stack.Push(node)) {
			delete node;
			break;
		}

		cout << "===== " << i << " Nodes =====" << endl;
		cout << "IsEmpty: "; // IsEmpty ���� ���
		if (stack.IsEmpty())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "IsFull: "; // IsFull ���� ���
		if (stack.IsFull())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "Print stack: "; // Node ��ü ���
		stack.PrintStack();
		cout << endl << endl;
	}

	/* ��带 �ϳ��� �����ϸ鼭 ����*/
	for (int i = 0; ; i++) {
		/* �� �̻� pop�� ��尡 �������� ������ �ݺ��� Ż��*/
		if (!(node = stack.Pop())) {
			break;
		}

		cout << "===== Pop =====" << endl;
		cout << "Poped Data: " << node->GetData() << endl;
		delete node;

		cout << "IsEmpty: "; // IsEmpty ���� ���
		if (stack.IsEmpty())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "IsFull: "; // IsFull ���� ���
		if (stack.IsFull())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "Print stack: "; // Node ��ü ���
		stack.PrintStack();
		cout << endl << endl;
	}

	return 0;
}