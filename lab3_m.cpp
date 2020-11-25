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

template<class T>
lp::SimplexTableau<T> toM(const lp::Matrix<T>& matrix, const std::vector<T>& z) {
	std::vector<int> basis(matrix.m());

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

	int artVarsNumber = 0;
	for (auto& val : basis) if (val == -1) val = matrix.n() + artVarsNumber++;

	lp::Matrix<T> resultMatrix(matrix.m(), matrix.n() + artVarsNumber);
	std::vector<T> resultZ(resultMatrix.n() + 1);
	T max = 0;

	for (int i = 0; i < matrix.m(); i++) {
		for (int j = 0; j < matrix.n(); j++) {
			resultMatrix[i][j] = matrix[i][j];

			if (matrix[i][j] > max) max = matrix[i][j];
		}
		resultMatrix.constants[i] = matrix.constants[i];
	}

	for (int i = 0; i < z.size(); i++) {
		resultZ[i] = z[i];
	}

	max *= 20;

	for (int i = z.size(); i < resultZ.size(); i++) {
		resultZ[i] = -max;
	}

	printVector(std::cout, resultZ);
	std::cout << std::endl;
	printMatrix(std::cout, resultMatrix);
	std::cout << std::endl;

	for (int i = 0; i < basis.size(); i++) {
		T controlElement = resultMatrix[i][basis[i]];

		if (basis[i] >= matrix.n()) {
			resultMatrix[i][basis[i]] = 1;
		} else if (controlElement != 1) {
			resultMatrix[i] /= controlElement;
			resultMatrix.constants[i] /= controlElement;
		}

		controlElement = resultZ[basis[i] + 1];
		resultZ[0] -= resultMatrix.constants[i] * controlElement;
		for (int j = 1; j < resultZ.size(); j++) {
			resultZ[j] -= resultMatrix[i][j - 1] * controlElement;
		}
	}

	return lp::SimplexTableau<T>(resultMatrix, resultZ);
}

int main() {
	typedef lp::Fraction<int64_t> basetype;
	std::ifstream in;
	in.open("test.txt");

	auto [m, n] = inputMatrixSizes(in);
	auto matrix = inputMatrixWithConstants<basetype>(in, m, n);
	auto limitations = inputLimitationsTypes(in, m);
	auto z = inputZ<basetype>(in, n + 1);

	auto tableau = toM(toCanon(matrix, limitations), z);

	try {
		tableau.print(std::cout);
		std::cout << std::endl;
		std::cout << std::endl;

		bool can_do_step = tableau.step();

		std::cout << "step" << std::endl;
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
