#include <fstream>
#include <iostream>
#include "Fraction.h"
#include "Matrix.h"
#include "SimplexTableau.h"
#include "utils.h"

int main() {
	typedef lp::Fraction<int64_t> basetype;
	std::ifstream in;
	in.open("lab2.txt");

	auto [m, n] = inputMatrixSizes(in);
	auto matrix = inputMatrixWithConstants<basetype>(in, m, n);
	auto z = inputZ<basetype>(in, n + 1);

	lp::SimplexTableau<basetype> tableau(matrix, z);

	try {
		bool can_do_step = tableau.step();
		tableau.print(std::cout);

		while (can_do_step) {
			can_do_step = tableau.step();
			std::cout << std::endl << "------" << std::endl;
			tableau.print(std::cout);
		}

		std::cout << "Result: " << tableau.result() << std::endl;
	} catch (...) {
		std::cout << "Unexpected error";
	}

	in.close();
	return 0;
}
