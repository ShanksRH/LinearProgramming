#ifndef LPMATRIX_H
#define LPMATRIX_H

#include <algorithm>
#include <sstream>
#include <vector>
#include "Vector.h"

namespace lp {

template<class T>
class Matrix {
private:
    Vector<T>* matrix = nullptr;
    int _size = 0;
    std::vector<int> _basis = {};

public:
    T* constants = nullptr;

    Matrix(int m, int n) : _size(m) {
        this->matrix = new Vector<T>[m];
        this->constants = new T[m];

        for (int i = 0; i < m; i++) {
            this->matrix[i].resize(n);
        }
    }

    Vector<T>& operator[](int index) {
        return this->matrix[index];
    }

	void truncate(int newSize) {
		this->_size = newSize;
	}

    void eliminate() {
        int ti, j = 0, i = 0;

        _basis.clear();

        while (i < _size && j < matrix[0].size()) {
            ti = i;

            while (ti < _size && matrix[ti][j] == 0) ti++;

            if (ti >= _size) {
                j++;
                continue;
            }

            if (ti != i) swapStrings(ti, i);

			T controlElement = matrix[i][j];
            matrix[i] /= controlElement;
            constants[i] /= controlElement;

            for (ti = i + 1; ti < _size; ti++) {
                matrix[ti] -= matrix[i] * matrix[ti][j];
                constants[ti] -= constants[i] * matrix[ti][j];
            }

            i++;
            _basis.push_back(j++);
        }

        this->truncate(i--);

        while (i > 0) {
            for (ti = 0; ti < i; ti++) {
                constants[ti] -= constants[i] * matrix[ti][_basis[i]];
                matrix[ti] -= matrix[i] * matrix[ti][_basis[i]];
            }

            i--;
        }
    }

    void toBasis(const std::vector<int>& newBasis) {
        if (newBasis.size() > _size) throw "Basis size bigger than number of string";

        std::vector<int> basis(newBasis);
        std::sort(basis.begin(), basis.end());

        for (int i = 0; i < basis.size(); i++) {
            int j = basis[i];
            int ti = i;

            while (ti < _size && matrix[ti][j] == 0) ti++;

            if (ti >= _size) {
                std::stringstream ss;
                ss << "Can't make {" << j << "} as basis";
                throw ss.str();
            }

            if (ti != i) swapStrings(ti, i);

            T controlElement = matrix[i][j];
            matrix[i] /= controlElement;
            constants[i] /= controlElement;

            for (ti = i + 1; ti < _size; ti++) {
                constants[ti] -= constants[i] * matrix[ti][j];
                matrix[ti] -= matrix[i] * matrix[ti][j];
            }
        }

        for (int i = basis.size() - 1; i > 0; i--) {
            for (int ti = 0; ti < i; ti++) {
                constants[ti] -= constants[i] * matrix[ti][_basis[i]];
                matrix[ti] -= matrix[i] * matrix[ti][_basis[i]];
            }
        }

		_basis = basis;
    }

    void basisExchange(int var1, int var2) {
		if (var1 == var2) return;

		int ib = 0;
		while (ib < _basis.size() && _basis[ib] != var1) {
			ib++;
		}
		if (ib >= basis.size()) throw "Var1 not in basis";

        T controlElement = matrix[ib][var2];
		if (controlElement == 0) throw "Impossible basis exchange";

		matrix[ib] /= controlElement;
        constants[ib] /= controlElement;
		for (int i = 0; i < ib; i++) {
            constants[i] -= constants[ib] * matrix[i][var2];
			matrix[i] -= matrix[ib] * matrix[i][var2];
		}
		for (int i = ib + 1; i < _size; i++) {
            constants[i] -= constants[ib] * matrix[i][var2];
			matrix[i] -= matrix[ib] * matrix[i][var2];
		}

		if (ib < var2) {
			int i = ib + 1;
			while (i < _basis.size() && _basis[i] < var2) {
				_basis[i - 1] = _basis[i];
				swapStrings(i - 1, i);
				i++;
			}
			_basis[i - 1] = var2;
		}
    }

    void swapStrings(int index1, int index2) {
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

    ~Matrix() {
        delete[] this->matrix;
        delete[] this->constants;
        this->matrix = nullptr;
        this->constants = nullptr;
        this->_size = 0;
    }
};

}

#endif
