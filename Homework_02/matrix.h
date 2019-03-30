#ifndef MATRIX_H
#define MATRIX_H

#include <iostream> 

using namespace std;

struct HelpingArray
{
private:
	int* array;
	size_t size;
public:
	HelpingArray() {}

	HelpingArray(int *row, size_t x) 
	{
		array = row;
		size = x;
	}

	~HelpingArray() {}

	int get(size_t i)
	{
		return array[i];
	}

	int &operator[](size_t i)
	{
		if (i >= size)
			throw out_of_range("");
		return array[i];
	}

	const int &operator[](size_t i) const
	{
		if (i >= size)
			throw out_of_range("");
		return array[i];
	}
};

struct Matrix
{
private:
	size_t rows;
	size_t cols;
	int **matrix;
public:
	Matrix(size_t r, size_t c)
	{
		rows = r;
		cols = c;
		matrix = new int*[rows];
		for (int i = 0; i < rows; i++)
			matrix[i] = new int[cols];
	}

	~Matrix()
	{
		for (int i = 0; i < rows; i++)
			delete[] matrix[i];
		delete[] matrix;
	}

	HelpingArray operator[](size_t i)
	{
		if (i >= rows)
			throw out_of_range("");
		return HelpingArray(matrix[i], cols);
	}

	const HelpingArray operator[](size_t i) const
	{
		if (i >= rows)
			throw out_of_range("");
		return HelpingArray(matrix[i], cols);
	}

	/*
	Matrix operator*=(Matrix A)
	{
	Matrix B(rows, A.cols);
	for (int i = 0; i < rows; i++)
	for (int j = 0; j < A.cols; j++)
	for (int k = 0; k < cols; k++)
	B.matrix[i][j] = matrix[i][k] * A.matrix[k][j];
	return B;
	}
	*/

	Matrix &operator*=(int x)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				matrix[i][j] *= x; 
        return *this;   
	}

	bool operator==(const Matrix &A) const
	{
		if (this->rows != A.rows)
			return false;
		if (this->cols != A.cols)
			return false;
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				if ((*this)[i][j] != A[i][j])
					return false;
		return true;
	}

	bool operator!=(const Matrix &A) const
	{
		return !(this->operator==(A));   // не сразу понял, что так можно было!
	}

	size_t getRows()
	{
		return rows;
	}

	size_t getColumns()
	{
		return cols;
	}
};

#endif