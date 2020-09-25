#include <iostream>
#include "Matrix.h"
#include "Fraction.h"
#include <time.h>

template<class T>
T gcd(T a, T b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
	int m, n;
	std::cin >> m >> n;
    lp::Matrix< lp::Fraction<int> > matrix(m, n);

    for (int i = 0; i < matrix.m(); i++) {
        for (int j = 0; j < matrix.n(); j++) {
            std::cin >> matrix[i][j];
        }
    }

    std::cout << std::endl;
    std::cout << std::endl;

	matrix.eliminate();

    for (int i = 0; i < matrix.m(); i++) {
        for (int j = 0; j < matrix.n(); j++) {
            std::cout << matrix[i][j] << " | ";
        }
        std::cout << std::endl;
    }

    return 0;
}
