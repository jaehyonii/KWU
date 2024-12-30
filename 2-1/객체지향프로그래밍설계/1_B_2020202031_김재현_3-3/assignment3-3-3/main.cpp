#include "Matrix.h"

int main() {
	double mat[3][3];

	/* matrix1 입력*/
	cout << "==========Input matrix1==========\n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cin >> mat[i][j];
	}
	Matrix matrix1(mat);

	/* matrix2 입력*/
	cout << "\n==========Input matrix2==========\n";
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			cin >> mat[i][j];
	}
	Matrix matrix2(mat);
	
	/* matrix1+=matrix2 연산 실행 및 출력*/
	cout << "\n==========matrix1+=matrix2==========\n";
	(matrix1 += matrix2).print();

	/* matrix1-=matrix2 연산 실행 및 출력*/
	cout << "\n==========matrix1-=matrix2==========\n";
	(matrix1 -= matrix2).print();
	
	/* matrix1*matrix2 연산 실행 및 출력*/
	cout << "\n==========matrix1*matrix2==========\n";
	(matrix1 * matrix2).print();
}