#include "euler.h"

int main() {
	// some results need 64 bits, may exceed 2^32 (UINT maximum)
	// int64_t result_64 = problem_11();
	int result = problem_11();

	std::cout << "Result: " << result;

	return 0;
}