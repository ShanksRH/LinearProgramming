#pragma once

#include <iostream>
#include "Matrix.h"

std::pair<int, int> inputMatrixSizes(std::istream& in) {
	int m, n;

	std::cout << "Sizes:" << std::endl;
	in >> m >> n;

	return {m, n};
}

template <class T>
lp::Matrix<T> inputMatrixWithConstants(std::istream& in, int m, int n) {
	lp::Matrix<T> matrix(m, n);

	std::cout << "Matrix + constants:" << std::endl;
	for (int i = 0; i < matrix.m(); i++) {
		for (int j = 0; j < matrix.n(); j++) {
			in >> matrix[i][j];
		}
		in >> matrix.constants[i];
	}

	return matrix;
}

template <class T>
std::vector<T> inputZ(std::istream& in, int size) {
	std::vector<T> z;
	z.resize(size);

	std::cout << "Z-func:" << std::endl;
	for (int i = 0; i < z.size(); i++) {
		in >> z[i];
	}
	std::cout << std::endl;

	return z;
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
