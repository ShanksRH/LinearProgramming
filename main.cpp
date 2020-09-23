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
    lp::Matrix< lp::Fraction<int> > matrix(2, 2);

    for (int i = 0; i < matrix.m(); i++) {
        for (int j = 0; j < matrix.n(); j++) {
            std::cout << matrix[i][j] << " | ";
        }
        std::cout << std::endl;
    }

    return 0;
}
