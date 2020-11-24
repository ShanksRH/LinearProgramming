#pragma once

#include <sstream>
#include <vector>
#include "Vector.h"

namespace lp {

template<class T>
class Matrix {
private:
	Vector<T>* matrix = nullptr;
	int _size = 0;

public:
	T* constants = nullptr;

	Matrix(int m, int n) : _size(m) {
		this->matrix = new Vector<T>[this->_size];
		this->constants = new T[this->_size];

		for (int i = 0; i < this->_size; i++) {
			this->matrix[i].resize(n);
		}
	}

	Matrix(const Matrix& matrix) : _size(matrix._size) {
		this->matrix = new Vector<T>[this->_size];
		this->constants = new T[this->_size];

		for (int i = 0; i < this->_size; i++) {
			this->matrix[i] = matrix.matrix[i];
			this->constants[i] = matrix.constants[i];
		}
	}

	Matrix(Matrix&& matrix) : _size(matrix._size) {
		this->matrix = matrix.matrix;
		this->constants = matrix.constants;
		matrix.matrix = nullptr;
		matrix.constants = nullptr;
		matrix._size = 0;
	}

	Matrix& operator=(Matrix&& matrix) {
		this->_size = matrix._size;
		this->matrix = matrix.matrix;
		this->constants = matrix.constants;
		matrix.matrix = nullptr;
		matrix.constants = nullptr;
		matrix._size = 0;

		return *this;
	}

	Vector<T>& operator[](int index) const {
		return this->matrix[index];
	}

	void truncate(int newSize) {
		this->_size = newSize;
	}

	void gaussStep(int i, int j) {
		T controlElement = matrix[i][j];

		if (controlElement == 0) throw "Divide on zero";

		matrix[i] /= controlElement;
		constants[i] /= controlElement;

		for (int ti = 0; ti < i; ti++) {
			controlElement = matrix[ti][j];
			matrix[ti] -= matrix[i] * controlElement;
			constants[ti] -= constants[i] * controlElement;
		}

		for (int ti = i + 1; ti < _size; ti++) {
			controlElement = matrix[ti][j];
			matrix[ti] -= matrix[i] * controlElement;
			constants[ti] -= constants[i] * controlElement;
		}
	}

	std::vector<int> eliminate() {
		int ti, j = 0, i = 0;
		std::vector<int> basis = {};

		while (i < _size && j < matrix[0].size()) {
			ti = i;

			while (ti < _size && matrix[ti][j] == 0) ti++;

			if (ti >= _size) {
				j++;
				continue;
			}

			if (ti != i) swapLines(ti, i);

			this->gaussStep(i++, j);
			basis.push_back(j++);
		}

		this->truncate(i);
		return basis;
	}

	Matrix eliminated() const {
		Matrix res(*this);
		res.eliminate();

		return res;
	}

	void setBasis(const std::vector<int>& basis) {
		if (basis.size() > _size) throw "Basis size bigger than number of strings";

		for (int i = 0; i < basis.size(); i++) {
			int j = basis[i];
			int ti = i;

			while (ti < _size && matrix[ti][j] == 0) ti++;

			if (ti >= _size) {
				std::stringstream ss;
				ss << "Can't make {" << j << "} as basis";
				throw ss.str();
			}

			if (ti != i) swapLines(ti, i);

			this->gaussStep(i, j);
		}
	}

	Matrix toBasis(const std::vector<int>& basis) const {
		Matrix res(*this);
		res.setBasis(basis);

		return res;
	}

	void swapLines(int index1, int index2) {
		this->matrix[index1].swap(this->matrix[index2]);
		T t = constants[index1];
		constants[index1] = constants[index2];
		constants[index2] = t;
	}

	int m() const {
		return this->_size;
	}

	int n() const {
		return this->matrix[0].size();
	}

	std::vector<T> solution(const std::vector<int>& basis) const {
		std::vector<T> res = {};
		res.resize(this->n());

		for (int i = 0; i < basis.size(); i++) {
			res[basis[i]] = constants[i];
		}

		return res;
	}

	~Matrix() {
		delete[] this->matrix;
		delete[] this->constants;
		this->matrix = nullptr;
		this->constants = nullptr;
		this->_size = 0;
	}
};

}
