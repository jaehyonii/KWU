#include "Queue.h"

int main() {
	Queue queue;
	Node* node;

	/* Stack Size �Է�*/
	int size;
	cout << "Set Size: "; cin >> size;
	queue.SetSize(size);
	cout << "Queue Size " << size << endl << endl;

	/* queue�� ������� �� ����*/
	cout << "===== 0 Nodes =====" << endl;
	cout << "IsEmpty: "; // IsEmpty ���� ���
	if (queue.IsEmpty())
		cout << "True" << endl;
	else
		cout << "False" << endl;

	cout << "IsFull: "; // IsFull ���� ���
	if (queue.IsFull())
		cout << "True" << endl;
	else
		cout << "False" << endl;
	
	cout << "Print Queue: "; // Node ��ü ���
	queue.PrintQueue();
	cout << endl << endl;
	
	/* queue�� ��带 �ϳ��� ä��� ����*/
	for (int i = 1; ; i++) {
		node = new Node();
		node->SetData(i * 4 - 3); 
		if (!queue.Push(node)) {
			delete node;
			break;
		}

		cout << "===== " << i << " Nodes =====" << endl;
		cout << "IsEmpty: "; // IsEmpty ���� ���
		if (queue.IsEmpty())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "IsFull: "; // IsFull ���� ���
		if (queue.IsFull())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "Print Queue: "; // Node ��ü ���
		queue.PrintQueue();
		cout << endl << endl;
	}

	/* ��带 �ϳ��� �����ϸ鼭 ����*/
	for(int i = 0; ; i++){
		/* �� �̻� pop�� ��尡 �������� ������ �ݺ��� Ż��*/
		if (!(node = queue.Pop())) {
			break;
		}
	
		cout << "===== Pop =====" << endl;
		cout << "Poped Data: " << node->GetData() << endl;
		delete node;

		cout << "IsEmpty: "; // IsEmpty ���� ���
		if (queue.IsEmpty())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "IsFull: "; // IsFull ���� ���
		if (queue.IsFull())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "Print Queue: "; // Node ��ü ���
		queue.PrintQueue();
		cout << endl << endl;
	}
	
	return 0;
}