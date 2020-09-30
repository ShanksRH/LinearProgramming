#include <iostream>
#include <time.h>
#include "Fraction.h"
#include "Matrix.h"

template<class T>
T gcd(T a, T b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

lp::Fraction<int> calcZFunc(
	const std::vector< lp::Fraction<int> >& z,
	const std::vector< lp::Fraction<int> >& solution
) {

}

int main() {
	int m, n;

	std::cout << "Sizes:" << std::endl;
	std::cin >> m >> n;

    lp::Matrix< lp::Fraction<int> > matrix(m, n);
	std::vector< lp::Fraction<int> > z;
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

	matrix.eliminate();
	bool flag = true;

	do {
		auto solution = matrix.solution();
		auto basis = matrix.basis();

		std::cout << "Result:" << std::endl;
		for (int i = 0; i < matrix.m(); i++) {
			std::cout << "  ";
			for (int j = 0; j < matrix.n(); j++) {
				std::cout << matrix[i][j] << "  ";
			}
			std::cout << "| " << matrix.constants[i] << std::endl;
		}

		std::cout << "  Basis: (";
		for (int i = 0; i < basis.size() - 1; i++) {
			std::cout << basis[i] << ", ";
		}
		std::cout << basis.back() << ")" << std::endl;

		bool negative = false;
		std::cout << "  Solution: (";
		for (int i = 0; i < solution.size() - 1; i++) {
			std::cout << solution[i] << ", ";
			negative = negative || (solution[i] < 0);
		}
		std::cout << solution.back() << ")" << std::endl;

		if (!negative) {
			auto resZ = calcZFunc(z, solution);
			std::cout << "  Result z: " << resZ << std::endl;
		} else {
			std::cout << "  Negative" << std::endl;
		}

		std::cout << std::endl;

		try {
			matrix.basisExchange(1, 2);
		} catch (const char* err) {
			std::cout << err << std::endl;
			flag = false;
		} catch (...) {
			std::cout << "err" << std::endl;
			flag = false;
		}
	} while(flag);

    return 0;
}
