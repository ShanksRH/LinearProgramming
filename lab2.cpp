#include <iostream>
#include <time.h>
#include "Fraction.h"
#include "Matrix.h"

template <class T>
T calcZFunc(const std::vector<T>& z, const std::vector<T>& solution) {
	T res(z[0]);

	for (int i = 0; i < solution.size(); i++) {
		res += z[i + 1] * solution[i];
	}

	return res;
}

template <class T>
void printVector(std::ostream& out, const std::vector<T>& v) {
	out << "(";
	for (int i = 0; i < v.size() - 1; i++) {
		out << v[i] << ' ';
	}
	out << v.back() << ")";
}

template <class T>
void printMatrix(std::ostream& out, const lp::Matrix<T>& matrix) {
	for (int i = 0; i < matrix.m(); i++) {
		for (int j = 0; j < matrix.n(); j++) {
			out << matrix[i][j] << ' ';
		}

		out << "| " << matrix.constants[i] << std::endl;
	}
}

int main() {
	int m, n;

	std::cout << "Sizes:" << std::endl;
	std::cin >> m >> n;

	lp::Matrix< lp::Fraction<int64_t> > matrix(m, n);
	std::vector< lp::Fraction<int64_t> > z;
	z.resize(matrix.n() + 1);

	std::cout << std::endl;
	std::cout << "Matrix + constants:" << std::endl;
	for (int i = 0; i < matrix.m(); i++) {
		for (int j = 0; j < matrix.n(); j++) {
			std::cin >> matrix[i][j];
		}
		std::cin >> matrix.constants[i];
	}

	std::cout << std::endl;
	std::cout << "Z-func:" << std::endl;
	for (int i = 0; i < z.size(); i++) {
		std::cin >> z[i];
	}
	std::cout << std::endl;

	return 0;
}
