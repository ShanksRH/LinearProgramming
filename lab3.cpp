#include <iostream>
#include <map>
#include <string>
#include "Fraction.h"
#include "Matrix.h"
#include "SimplexTableau.h"
#include "SimplexTableauArtificial.h"
#include "utils.h"

std::vector<int> inputLimitationsTypes(std::istream& in, int count) {
	std::vector<int> limitations(count);
	std::map<std::string, int> limitationsTypes {
		{"=", 0}, {"<", 1}, {">", -1}, {"<=", 1}, {">=", -1}
	};

	std::cout << "Input limitations types" <<std::endl;

	for (int i = 0; i < count; i++) {
		std::string signs;
		in >> signs;
		limitations[i] = limitationsTypes[signs];
	}

	return limitations;
}

template<class T>
lp::Matrix<T> toCanon(const lp::Matrix<T>& matrix, const std::vector<int>& limitations) {
	std::vector<int> tlimits(limitations);
	std::vector<int> additionalVars = {};
	for (int i = 0; i < limitations.size(); i++) {
		if (limitations[i] != 0) additionalVars.push_back(i);
	}

	lp::Matrix<T> canonMatrix(matrix.m(), matrix.n() + additionalVars.size());
	for (int i = 0; i < matrix.m(); i++) {
		for (int j = 0; j < matrix.n(); j++) {
			canonMatrix[i][j] = matrix[i][j];
		}
		if (matrix.constants[i] < 0) {
			canonMatrix[i] *= -1;
			canonMatrix.constants[i] = -matrix.constants[i];
			tlimits[i] *= -1;
		} else {
			canonMatrix.constants[i] = matrix.constants[i];
		}
	}

	for (int i = 0; i < additionalVars.size(); i++) {
		canonMatrix[additionalVars[i]][matrix.n() + i] = limitations[additionalVars[i]];
	}

	return canonMatrix;
}

int main() {
	typedef lp::Fraction<int64_t> basetype;

	auto [m, n] = inputMatrixSizes(std::cin);
	auto matrix = inputMatrixWithConstants<basetype>(std::cin, m, n);
	auto limitations = inputLimitationsTypes(std::cin, m);
	auto z = inputZ<basetype>(std::cin, n + 1);

	lp::SimplexTableauArtific<basetype> artTableau(toCanon(matrix, limitations));

	printVector(std::cout, limitations);

	try {
		bool can_do_step = artTableau.step();

		while (can_do_step) {
			can_do_step = artTableau.step();
		}

		if (artTableau.controlF() != 0) {
			std::cout << "max F != 0" << std::endl;

			return 0;
		}

		can_do_step = artTableau.forceStep();

		while(can_do_step) {
			artTableau.forceStep();
		}

		auto tableau = artTableau.toCommonSimplex(z);

		can_do_step = artTableau.step();

		while (can_do_step) {
			can_do_step = artTableau.step();
		}

		std::cout << "Result: " << tableau.result() << std::endl;
	} catch (...) {
		std::cout << "Unexpected error";
	}

	return 0;
}
