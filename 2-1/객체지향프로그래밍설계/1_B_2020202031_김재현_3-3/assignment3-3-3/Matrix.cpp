#include "Matrix.h"

/* 객체 생성 시 매개변수가 없을 경우 0으로 초기화*/
Matrix::Matrix() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			matrix[i][j] = 0;
	}
}
/* 객체 생성 시 매개변수로 이차원 배열이 입력되면 이차원 배열에 저장된 수로 초기화*/
Matrix::Matrix(double(*mat)[3]) {
	setMatrix(mat);
}
/* 복사 생성자*/
Matrix::Matrix(Matrix& mat) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			matrix[i][j] = mat.matrix[i][j];
	}
}
Matrix::~Matrix(){}

/* 객체의 matrix값을 초기화 및 변경.*/
void Matrix::setMatrix(double(*mat)[3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			matrix[i][j] = mat[i][j];
	}
}

/* mat의 matrix값과 +=을 실행하는 객체의 matrix값을 더해서 실행주체의 matrix에 저장함*/
Matrix& Matrix::operator+=(const Matrix& mat) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			matrix[i][j] += mat.matrix[i][j];
		}
	}

	return *this;
}
/* mat의 matrix값과 -=을 실행하는 객체의 matrix값을 빼서 실행주체의 matrix에 저장함*/
Matrix& Matrix::operator-=(const Matrix& mat) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			matrix[i][j] -= mat.matrix[i][j];
		}
	}

	return *this;
}
/* mat의 matrix와 *을 실행하는 객체의 matrix를 곱해서 새로운 객체를 만들고 이를 반환함*/
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

/* 행렬 출력*/
void Matrix::print() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout.width(5); // 너비 5
			cout << matrix[i][j] << " "; // 공백으로 구분
		}
		cout << endl; // 개행으로 구분
	}
}