#include "A.h"

/*������*/
A::A(double n) :num(n) {
	cout << "Class A is created with " << num << endl;
}
/*�Ҹ���*/
A::~A() {
	cout << "Class A is destroyed with " << num << endl;
}