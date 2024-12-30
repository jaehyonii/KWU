#include "Matrix.h"

/* ��ü ���� �� �Ű������� ���� ��� 0���� �ʱ�ȭ*/
Matrix::Matrix() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			matrix[i][j] = 0;
	}
}
/* ��ü ���� �� �Ű������� ������ �迭�� �ԷµǸ� ������ �迭�� ����� ���� �ʱ�ȭ*/
Matrix::Matrix(double(*mat)[3]) {
	setMatrix(mat);
}
/* ���� ������*/
Matrix::Matrix(Matrix& mat) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			matrix[i][j] = mat.matrix[i][j];
	}
}
Matrix::~Matrix(){}

/* ��ü�� matrix���� �ʱ�ȭ �� ����.*/
void Matrix::setMatrix(double(*mat)[3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			matrix[i][j] = mat[i][j];
	}
}

/* mat�� matrix���� +=�� �����ϴ� ��ü�� matrix���� ���ؼ� ������ü�� matrix�� ������*/
Matrix& Matrix::operator+=(const Matrix& mat) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			matrix[i][j] += mat.matrix[i][j];
		}
	}

	return *this;
}
/* mat�� matrix���� -=�� �����ϴ� ��ü�� matrix���� ���� ������ü�� matrix�� ������*/
Matrix& Matrix::operator-=(const Matrix& mat) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			matrix[i][j] -= mat.matrix[i][j];
		}
	}

	return *this;
}
/* mat�� matrix�� *�� �����ϴ� ��ü�� matrix�� ���ؼ� ���ο� ��ü�� ����� �̸� ��ȯ��*/
Matrix Matrix::operator*(const Matrix& mat) {
	double temp[3][3] = { 0 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				temp[i][j] += matrix[i][k] * mat.matrix[k][j];
			}
		}
	}
	
	Matrix tmp(temp);

	return tmp;
}

/* ��� ���*/
void Matrix::print() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout.width(5); // �ʺ� 5
			cout << matrix[i][j] << " "; // �������� ����
		}
		cout << endl; // �������� ����
	}
}