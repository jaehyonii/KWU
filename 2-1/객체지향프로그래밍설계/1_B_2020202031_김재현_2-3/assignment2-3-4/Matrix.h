#pragma once
#include<iostream>

using namespace std;

namespace ooplinalg {

	class Matrix {
	private:
		float** data;
		int rows;
		int cols;

	public:
		Matrix();
		Matrix(const Matrix& mat);
		Matrix(int rows, int cols);
		~Matrix();

		float getElement(int row, int col) const;
		float** getData() const;
		void setElement(const int row, const int col, float value);
		void setData(const int rows, const int cols, float** data);
		void setTable(const int rows, const int cols);
		int getRows() const;
		int getCols() const;
		void setRows(const int rows);
		void setCols(const int cols);

		float determinant();
		void print();
	};

	float deter(float** data, int size);
	
	Matrix& add(Matrix& r, Matrix& a, Matrix& b);
	Matrix& sub(Matrix& r, Matrix& a, Matrix& b);
	Matrix& mul(Matrix& r, Matrix& a, Matrix& b);

	Matrix& elementAdd(Matrix& r, Matrix& a, float v);
	Matrix& elementSub(Matrix& r, Matrix& a, float v);
	Matrix& elementMul(Matrix& r, Matrix& a, float v);
	Matrix& elementDiv(Matrix& r, Matrix& a, float v);

	Matrix& transpose(Matrix& r, Matrix& m);
	Matrix& adjoint(Matrix& r, Matrix& m);
	Matrix& inverse(Matrix& r, Matrix& m);

	float** memory_alloc2D(int rows, int cols);
	int memory_free2D(float** data);
}