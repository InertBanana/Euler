#include "problem_5.h"
#define NUM_CAP 20
/*
2520 is the smallest number that can be divided by each of the numbers from 1 to NUM_CAP without any remainder.

What is the smallest positive number that is evenly divisible by all of the numbers from 1 to NUM_CAP?
*/

int problem_5() {
	int smallest_multiple = 1;
	std::vector<int> sieve;
	std::vector<int> factors;
	
	for (int i = 2; i <= NUM_CAP; i++) {
		sieve.push_back(i);
	}
	factors = sieve;

	for (int i = 0; i < sieve.size() - 1; i++) {
		for (int j = i + 1; j < sieve.size(); j++) {
			if (sieve[j] % sieve[i] == 0) {
				sieve.erase(sieve.begin() + j);
			}
		}
	}

	// product of primes - the absolute minimum
	for (int i = 0; i < sieve.size(); i++) {
		smallest_multiple *= sieve[i];
	}

	for (int i = 1; i <= NUM_CAP; i++) {
		// if we find in integer in the range which does not evenly divide the current smallest multiple...
		if (smallest_multiple % i != 0) {
			for (int j = 0; j < factors.size(); j++) {
				/* 
				find the smallest prime factor of that integer i and multiply the smallest_multiple by it
				note that this may only work with the range 1..20!!!!!!!
				designed with the 1..10 range in mind, where the first ints that didn't divide the 
				product of the primes under 10 happened to be squares of primes (2^2 for 4, 3^3 for 9).
				*/
				if (i % factors[j] == 0) { smallest_multiple *= factors[j]; break; }
			}
		}
	}

	return smallest_multiple;
}