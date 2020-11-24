#pragma once

#include "iostream"
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
		while (j < z.size() && z[j] >= 0) j++;
		if (j >= z.size()) return false;

		j--;

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

		T t = z[j + 1];
		z[0] -= t * matrix.constants[imin];
		for (i = 1; i < z.size(); i++) {
			z[i] -= t * matrix[imin][i - 1];
		}

		return true;
	}

	T result() {
		return z[0];
	}

	void print(std::ostream& os) {
		for (int i = 0; i < matrix.m(); i++) {
			os << matrix.constants[i] << " | ";
			for (int j = 0; j < matrix.n(); j++) {
				os << matrix[i][j] << ' ';
			}
			os << std::endl;
		}
		os << "-  ";
		for (auto& val : z) {
			os << val << ' ';
		}
	}
};

}
