#include "Queue.h"

int main() {
	Queue queue;
	Node* node;

	/* Stack Size 입력*/
	int size;
	cout << "Set Size: "; cin >> size;
	queue.SetSize(size);
	cout << "Queue Size " << size << endl << endl;

	/* queue가 비어있을 때 실험*/
	cout << "===== 0 Nodes =====" << endl;
	cout << "IsEmpty: "; // IsEmpty 여부 출력
	if (queue.IsEmpty())
		cout << "True" << endl;
	else
		cout << "False" << endl;

	cout << "IsFull: "; // IsFull 여부 출력
	if (queue.IsFull())
		cout << "True" << endl;
	else
		cout << "False" << endl;
	
	cout << "Print Queue: "; // Node 전체 출력
	queue.PrintQueue();
	cout << endl << endl;
	
	/* queue에 노드를 하나씩 채우고 실험*/
	for (int i = 1; ; i++) {
		node = new Node();
		node->SetData(i * 4 - 3); 
		if (!queue.Push(node)) {
			delete node;
			break;
		}

		cout << "===== " << i << " Nodes =====" << endl;
		cout << "IsEmpty: "; // IsEmpty 여부 출력
		if (queue.IsEmpty())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "IsFull: "; // IsFull 여부 출력
		if (queue.IsFull())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "Print Queue: "; // Node 전체 출력
		queue.PrintQueue();
		cout << endl << endl;
	}

	/* 노드를 하나씩 삭제하면서 실험*/
	for(int i = 0; ; i++){
		/* 더 이상 pop할 노드가 존재하지 않으면 반복문 탈출*/
		if (!(node = queue.Pop())) {
			break;
		}
	
		cout << "===== Pop =====" << endl;
		cout << "Poped Data: " << node->GetData() << endl;
		delete node;

		cout << "IsEmpty: "; // IsEmpty 여부 출력
		if (queue.IsEmpty())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "IsFull: "; // IsFull 여부 출력
		if (queue.IsFull())
			cout << "True" << endl;
		else
			cout << "False" << endl;

		cout << "Print Queue: "; // Node 전체 출력
		queue.PrintQueue();
		cout << endl << endl;
	}
	
	return 0;
}