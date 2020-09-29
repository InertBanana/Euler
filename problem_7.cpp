#include "problem_7.h"

int problem_7() {
	std::vector<int> primes;

	long long int current = 1;
	long long int max = 600851475143;

	// fill the sieve, start with cap of 10,000 (pure fucking shot in the dark on how big it's gonna be)
	while (current < 125000) {
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

	std::cout << primes.size() << "\n";

	if (primes.size() >= 10001)
		return primes[10000];
	else
		return primes[primes.size() - 1];
}