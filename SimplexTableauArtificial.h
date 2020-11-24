#pragma once

#include "iostream"
#include <vector>
#include "Matrix.h"
#include "SimplexTableau.h"

namespace lp {

template <class T>
class SimplexTableauArtific {
	Matrix<T> matrix;
	std::vector<T> f;
	std::vector<int> basis;
	int artVarsNumber;

public:
	SimplexTableauArtific(const Matrix<T>& matrix) : matrix(0, 0) {
		basis.resize(matrix.m());

		for (auto& val : basis) val = -1;

		for (int i = 0; i < matrix.n(); i++) {
			int j = 0;

			while (j < matrix.m() && matrix[j][i] == 0) j++;

			if (j < matrix.m() && matrix[j][i] > 0) {
				int jj = j + 1;

				while (jj < matrix.m() && matrix[jj][i] == 0) jj++;

				if (jj >= matrix.m()) basis[j] = i;
			}
		}

		artVarsNumber = 0;
		for (auto& val : basis) if (val == -1) val = matrix.n() + artVarsNumber++;

		this->matrix = Matrix<T>(matrix.m(), matrix.n() + artVarsNumber);
		for (int i = 0; i < matrix.m(); i++) {
			for (int j = 0; j < matrix.n(); j++) {
				this->matrix[i][j] = matrix[i][j];
			}
			this->matrix.constants[i] = matrix.constants[i];
		}

		for (int i = 0; i < basis.size(); i++) {
			T controlElement = this->matrix[i][basis[i]];
			if (controlElement == 0) {
				this->matrix[i][basis[i]] = 1;
			} else if (controlElement != 1) {
				this->matrix[i] /= controlElement;
				this->matrix.constants[i] /= controlElement;
			}
		}

		this->f.resize(this->matrix.n() + 1);
		for (int i = f.size() - artVarsNumber; i < f.size(); i++) f[i] = -1;
	}

	bool step() {
		int j = 1;
		while (j < f.size() && f[j] >= 0) j++;
		if (j >= f.size()) return false;

		int i = 0;
		while (i < matrix.m() && matrix[i][j] <= 0) i++;
		if (i >= matrix.m()) throw "Z-func unlimited";

		T min = matrix.constants[i] / matrix[i][j];
		int imin = i;

		while (++i < matrix.m()) {
			if (matrix[i][j] > 0) {
				T t = matrix.constants[i] / matrix[i][j];

				if (t < min) {
					imin = i;
					min = t;
				}
			}
		}

		matrix.gaussStep(imin, j);
		basis[imin] = j;

		T t = f[j];
		f[0] -= t * matrix.constants[imin];
		for (i = 1; i < f.size(); i++) {
			f[i] -= t * matrix[imin][i];
		}

		return true;
	}

	T controlF() const {
		return f[0];
	}

	bool forceStep() {
		int i = 0;
		while (i < basis.size() && basis[i] < matrix.n()) i++;
		if (i >= basis.size()) return false;

		int j = 0;
		while (j < matrix.n() && matrix[i][j] == 0) j++;
		if (j >= matrix.n()) return false;

		matrix.gaussStep(i, j);
		basis[i] = j;

		return true;
	}

	SimplexTableau<T> toCommonSimplex(const std::vector<T>& z) {
		Matrix<T> matrix(this->matrix.m(), this->matrix.n() - artVarsNumber);
		std::vector<T> resultZ(z);

		for (int i = 0; i < matrix.m(); i++) {
			for (int j = 0; j < matrix.n(); j++) {
				matrix[i][j] = this->matrix[i][j];
			}
		}

		for (int i = 0; i < basis.size(); i++) {
			T controlElement = z[basis[i] + 1];
			if (controlElement != 0) {
				resultZ[0] -= controlElement * matrix.constants[i];
				for (int j = 1; j < z.size(); j++) {
					resultZ[j] -= controlElement * matrix[i][j - 1];
				}
			}
		}

		return SimplexTableau(matrix, resultZ);
	}

	void print(std::ostream& os) {
		for (int i = 0; i < matrix.m(); i++) {
			os << matrix.constants[i] << " | ";
			for (int j = 0; j < matrix.n(); j++) {
				os << matrix[i][j] << ' ';
			}
			os << std::endl;
		}
		os << "---  ";
		for (auto& val : f) {
			os << val << ' ';
		}
	}
};

}
