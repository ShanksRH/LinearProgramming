#include <iostream>
#include <vector>

std::pair<int, int> getSizes() {
	return {3, 4};
}

int main() {
	auto [a, b] = getSizes();

	std::cout << a << ' ' << b << std::endl;

	return 0;
}
