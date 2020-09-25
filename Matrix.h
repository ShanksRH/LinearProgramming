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
    Matrix(int m, int n) : _size(m) {
        this->matrix = new Vector<T>[m];

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

            for (ti = i + 1; ti < _size; ti++) {
                matrix[ti] -= matrix[i] * matrix[ti][j];;
            }

            i++;
            _basis.push_back(j++);
        }

        this->truncate(i--);

        while (i > 0) {
            for (ti = 0; ti < i; ti++) {
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

            matrix[i] /= matrix[i][j];

            for (ti = i + 1; ti < _size; ti++) {
                matrix[ti] -= matrix[i] * matrix[ti][j];
            }
        }

        for (int i = basis.size() - 1; i > 0; i--) {
            for (int ti = 0; ti < i; ti++) {
                matrix[ti] -= matrix[i] * matrix[ti][_basis[i]];
            }
        }
    }

    void basisExchange(int var1, int var2) {
        matrix[var1][var2];
    }

    void swapStrings(int index1, int index2) {
        this->matrix[index1].swap(this->matrix[index2]);
    }

    int m() const {
        return this->_size;
    }

    int n() const {
        return this->matrix[0].size();
    }

    ~Matrix() {
        delete[] this->matrix;
        this->matrix = nullptr;
        this->_size = 0;
    }
};

}

#endif
