#ifndef SIMPLEX_TABLEAU_H
#define SIMPLEX_TABLEAU_H

#include <vector>
#include "Matrix.h"

namespace lp {

template <class T>
class SimplexTableau {
	Matrix<T> matrix;
	std::vector<T> z;
public:
	SimplexTableau(const Matrix<T>& matrix, const std::vector<T>& z) :
	matrix(matrix), z(z) {
		for (auto& val : this->z) val *= -1;
	}

	bool step() {
		int j = 1;
		while (j < z.size() && z[j] > 0) j++;
		if (j >= z.size()) return false;

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

		T controlElement = matrix[imin][j];
		matrix.constants[imin] /= controlElement;
		matrix[imin] /= controlElement;

		for (i = 0; i < imin; i++) {
			matrix.constants[i] -= matrix.constants[imin] * matrix[i][j];
			matrix[i] -= matrix[imin] * matrix[i][j];
		}

		for (i = imin + 1; i < matrix.m(); i++) {
			matrix.constants[i] -= matrix.constants[imin] * matrix[i][j];
			matrix[i] -= matrix[imin] * matrix[i][j];
		}

		return true;
	}
};

}

#endif
