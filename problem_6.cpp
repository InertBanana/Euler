#include "problem_6.h"
#define MAX_THREAD 2
/*
The sum of the squares of the first ten natural numbers is
	1e2 + 2e2 + 3e2 + .. + 10e2 = 385

The square of the sum of the first ten natural numbers is
	(1 + 2 + .. + 10) e2 = 3025

Hence the difference between the sum of the squares of the first 
ten natural and the square of the sum is 
	3025 - 385 = 2640

Find the difference between the sum of the squares of the first 
one hundred natural numbers and the square of the sum.
*/

int problem_6() {
	
	int pr1 = 0;
	int pr2 = 0;
	DWORD   dwThreadIdArray[MAX_THREAD];
	HANDLE hThreadArray[MAX_THREAD];

	hThreadArray[0] = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		square_sum,       // thread function name
		&pr1,          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadIdArray[0]);   // returns the thread identifier 

	hThreadArray[1] = CreateThread(
		NULL,                   // default security attributes
		0,                      // use default stack size  
		sum_square,       // thread function name
		&pr2,          // argument to thread function 
		0,                      // use default creation flags 
		&dwThreadIdArray[1]);   // returns the thread identifier 	

	WaitForMultipleObjects(MAX_THREAD, hThreadArray, TRUE, INFINITE);

	for (int i = 0; i < MAX_THREAD; i++) { CloseHandle(hThreadArray[i]); }

	return pr1 - pr2;
}

DWORD WINAPI square_sum(LPVOID lpParam) {
	int* square_sum = (int*)lpParam;

	for (int i = 1; i <= 100; i++) { *square_sum += i; }
	(*square_sum) *= (*square_sum);

	return 0;
}
DWORD WINAPI sum_square(LPVOID lpParam){
	int* sum_squares = (int*) lpParam;

	for (int i = 1; i <= 100; i++) { *sum_squares += i * i; }

	return 0;
}