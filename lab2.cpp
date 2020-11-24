#include <iostream>
#include "Fraction.h"
#include "Matrix.h"
#include "SimplexTableau.h"
#include "utils.h"

int main() {
	typedef lp::Fraction<int64_t> basetype;

	auto [m, n] = inputMatrixSizes(std::cin);
	auto matrix = inputMatrixWithConstants<basetype>(std::cin, m, n);
	auto z = inputZ<basetype>(std::cin, n + 1);

	lp::SimplexTableau<basetype> tableau(matrix, z);

	try {
		bool can_do_step = tableau.step();

		while (can_do_step) {
			can_do_step = tableau.step();
		}

		std::cout << "Result: " << tableau.result() << std::endl;
	} catch (...) {
		std::cout << "Unexpected error";
	}

	return 0;
}
