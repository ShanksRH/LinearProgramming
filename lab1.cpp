#include <iostream>
#include "Fraction.h"
#include "Matrix.h"
#include "utils.h"

template <class T>
T calcZFunc(const std::vector<T>& z, const std::vector<T>& solution) {
	T res = z[0];

	for (int i = 0; i < solution.size(); i++) {
		res += z[i + 1] * solution[i];
	}

	return res;
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
	typedef lp::Fraction<int64_t> basetype;

	auto [m, n] = inputMatrixSizes(std::cin);
	auto matrix = inputMatrixWithConstants<basetype>(std::cin, m, n);
	auto z = inputZ<basetype>(std::cin, n + 1);

	auto pureMatrix = matrix.eliminated();
	auto basises = combinations(pureMatrix.n(), pureMatrix.m());
	auto maxZVal = basetype(0);

	for (auto& basis : basises) {
		std::cout << "Basis: ";
		printVector(std::cout, basis);
		std::cout << std::endl;

		try {
			auto currentMatrix = pureMatrix.toBasis(basis);
			auto solution = currentMatrix.solution(basis);

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
		} catch (...) {
			std::cout << "Impossible basis" << std::endl;
		}
	}

	std::cout << "Best Z-value: " << maxZVal << std::endl;

	return 0;
}
