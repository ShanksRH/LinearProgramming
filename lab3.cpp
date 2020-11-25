#include <iostream>
#include <fstream>
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
	std::ifstream in;
	in.open("test.txt");

	auto [m, n] = inputMatrixSizes(in);
	auto matrix = inputMatrixWithConstants<basetype>(in, m, n);
	auto limitations = inputLimitationsTypes(in, m);
	auto z = inputZ<basetype>(in, n + 1);

	lp::SimplexTableauArtific<basetype> artTableau(toCanon(matrix, limitations));

	try {
		artTableau.print(std::cout);
		std::cout << std::endl;
		std::cout << std::endl;

		bool can_do_step = artTableau.step();

		std::cout << "art step 1" << std::endl;
		artTableau.print(std::cout);
		std::cout << std::endl;
		std::cout << std::endl;

		while (can_do_step) {
			can_do_step = artTableau.step();
			artTableau.print(std::cout);
			std::cout << std::endl;
			std::cout << std::endl;
		}

		std::cout << "art steps" << std::endl;

		if (artTableau.controlF() != 0) {
			std::cout << "max F != 0" << std::endl;

			return 0;
		}

		std::cout << "control" << std::endl;

		can_do_step = artTableau.forceStep();

		std::cout << "art force 1" << std::endl;
		artTableau.print(std::cout);
		std::cout << std::endl;
		std::cout << std::endl;

		while(can_do_step) {
			artTableau.forceStep();
			artTableau.print(std::cout);
			std::cout << std::endl;
			std::cout << std::endl;
		}

		std::cout << "art forces" << std::endl;

		auto tableau = artTableau.toCommonSimplex(z);
		tableau.print(std::cout);
		std::cout << std::endl;
		std::cout << std::endl;

		can_do_step = tableau.step();

		std::cout << "common step 1" << std::endl;
		tableau.print(std::cout);
		std::cout << std::endl;
		std::cout << std::endl;

		while (can_do_step) {
			can_do_step = tableau.step();
			tableau.print(std::cout);
			std::cout << std::endl;
			std::cout << std::endl;
		}

		std::cout << "Result: " << tableau.result() << std::endl;
	} catch (...) {
		std::cout << "Unexpected error";
	}

	return 0;
}
