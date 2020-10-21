#include "problem_3.h"
/*
The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143?
*/
int problem_3() {
	// biggest prime factor just might be 1 
	int biggest = 1;

	std::vector<int> primes;

	int current = 1;
	long long int max = 600851475143;

	// fill the sieve, start with cap of 10,000 (pure fucking shot in the dark on how big it's gonna be)
	while (current < 10000) {
		primes.push_back(current);
		current++;
	}

	for (int i = 0; i < primes.size(); i++) {
		// start looking for multiples of i after ith elem
		for (int j = i + 1; j < primes.size(); j++) {
			// kill multiples of primes(i)
			if (primes[j] % primes[i] == 0)
				// fucking vector deletion syntax i swear to god
				primes.erase(primes.begin() + j);
			
		}
	}

	// now let's get the biggest factor that fits
	for (int i = 0; i < primes.size(); i++) {
		// sieve in ascending order, any factor found is the biggest so far
		if (max % primes[i] == 0) {
			biggest = primes[i];
		}
	}
	

	return biggest;
}