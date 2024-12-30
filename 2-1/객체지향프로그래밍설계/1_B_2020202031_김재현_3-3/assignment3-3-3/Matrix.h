#pragma once
#include<iostream>

using namespace std;

class Matrix
{
private:
	double matrix[3][3];

public:
	Matrix();
	Matrix(double(*mat)[3]);
	Matrix(Matrix& mat);
	~Matrix();

	void setMatrix(double(*mat)[3]);
	Matrix& operator+=(const Matrix& mat);
	Matrix& operator-=(const Matrix& mat);
	Matrix operator*(const Matrix& mat);
	void print();
};