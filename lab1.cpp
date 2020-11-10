#include <iostream>
#include <time.h>
#include "Fraction.h"
#include "Matrix.h"

template <class T>
T calcZFunc(const std::vector<T>& z, const std::vector<T>& solution) {
	T res(0);

	for (int i = 0; i < z.size(); i++) {
		res += z[i] * solution[i];
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

std::vector< std::vector<int> > combinations(int n, int k) {
	if (n < k || k < 1) return {};

	std::vector< std::vector<int> > combinations{};
	std::vector<int> combination(k);

	for (int i = 0; i < k; i++) {
		combination[i] = i;
	}
	combinations.push_back(combination);

	int current = k - 1;
	while (current >= 0) {
		if (combination[current] + k < n + current) {
			combination[current++]++;

			while (current < k) {
				combination[current++] = combination[current - 1] + 1;
			}

			combinations.push_back(combination);
		}

		current--;
	}

	return combinations;
}

int main() {
	int m, n;

	std::cout << "Sizes:" << std::endl;
	std::cin >> m >> n;

	lp::Matrix< lp::Fraction<int64_t> > matrix(m, n);
	std::vector< lp::Fraction<int64_t> > z;
	z.resize(matrix.n());

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

	auto pureMatrix = matrix.eliminated();
	auto basises = combinations(pureMatrix.n(), pureMatrix.m());
	auto maxZVal = lp::Fraction<int64_t>(0);

	for (auto& basis : basises) {
		std::cout << "Basis: ";
		printVector(std::cout, basis);
		std::cout << std::endl;

		try {
			auto currentMatrix = pureMatrix.toBasis(basis);
			auto solution = currentMatrix.solution();

			std::cout << "Matrix:" << std::endl;
			printMatrix(std::cout, currentMatrix);

			std::cout << "Solution: ";
			printVector(std::cout, solution);
			std::cout << std::endl;

			bool onlyPositive = true;
			for (auto& val : solution) {
				onlyPositive = onlyPositive && (val >= 0);
			}
			if (onlyPositive) {
				std::cout << "No negative elements" << std::endl;
				auto zVal = calcZFunc(z, solution);
				std::cout << "Z-value: " << zVal << std::endl;
				if (zVal > maxZVal) {
					maxZVal = zVal;
				}
			}

			std::cout << std::endl;
			std::cout << std::endl;
		} catch (...) {
			std::cout << "Impossible basis" << std::endl;
		}
	}

	std::cout << "Best Z-value: " << maxZVal << std::endl;

	return 0;
}
