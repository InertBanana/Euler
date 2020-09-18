/*
If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9. The sum of these multiples is 23.

Find the sum of all the multiples of 3 or 5 below 1000.
*/

int sum_of_multiples() {
	int result = 0;

	int sum3 = 0;
	int sum5 = 0;

	for (int i = 1; i * 3 < 1000; i++) {
		if (i % 5 != 0) // exclude multiples of 3 and 5 once, don't double-count
			sum3 += i * 3;
	}

	for (int i = 1; i * 5 < 1000; i++) {
		sum5 += i * 5;
	}

	return sum3 + sum5;
}


