#include "euler.h"

int main() {
	// some results need 64 bits, may exceed 2^32 (UINT maximum)
	int64_t result_64 = problem_10();
	//int result = problem_9();

	std::cout << "Result: " << result_64;

	return 0;
}