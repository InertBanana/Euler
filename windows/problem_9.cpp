#include "problem_9.h"
#define TRIPLET_SUM 1000

// only have 4 cores :-(
#define MAX_THREAD 4

// highest value for a in pythagorean triplet
#define MAX_A 200
const int THREAD_LIMS[] = { 50, 100, 150, 200 };
int prod;
/*
A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,

	a^2 + b^2 = c^2

For example, 32 + 42 = 9 + 16 = 25 = 52.

There exists exactly one Pythagorean triplet for which a + b + c = 1000.
Find the product abc.

*/

// explanation in problem_4.cpp
typedef struct thread_params {
	int t_id;
} THREAD_PARAMS, * T_DATA;


int problem_9() {

	T_DATA thread_data_array[MAX_THREAD];
	DWORD dw_thread_id_array[MAX_THREAD];
	HANDLE h_thread_array[MAX_THREAD];

	for (int t = 0; t < MAX_THREAD; t++) {
		// alloc mem 
		thread_data_array[t] = (T_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(THREAD_PARAMS));

		// array allocation failure - out of memory
		if (thread_data_array[t] == NULL)
		{
			ExitProcess(2);
		}

		thread_data_array[t]->t_id = t;

		h_thread_array[t] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			search_pythagorean_triplet,				// thread function name
			(void*)thread_data_array[t],	// argument to thread function 
			0,                      // use default creation flags 
			&dw_thread_id_array[t]);   // returns the thread identifier 

		//cout << "Thread made!";
	}

	// wait...
	WaitForMultipleObjects(MAX_THREAD, h_thread_array, TRUE, INFINITE);


	//close
	for (int t = 0; t < MAX_THREAD; t++)
	{
		CloseHandle(h_thread_array[t]);
		
		if (thread_data_array[t] != NULL)
		{
			// dobby is a free parameter
			HeapFree(GetProcessHeap(), 0, thread_data_array[t]);
			thread_data_array[t] = NULL;    // Ensure address is not reused.
		}
	}
	//std::cout << std::endl;
	return prod;
}




DWORD WINAPI search_pythagorean_triplet(LPVOID param) {
	T_DATA thread_data_array = (T_DATA)param;
	int me = thread_data_array->t_id;
	int my_limit = THREAD_LIMS[me];

	for (int a = (my_limit - 50) + 1; a <= my_limit; a++) {
		for (int b = 0; b <= 500; b++) {
			for (int c = 0; c <= 500; c++) {
				// looking for 1000
				int sum = a + b + c;
				// zero iff a^2 + b^2 = c^2
				int a2 = a * a;
				int b2 = b * b;
				int c2 = c * c;
				int is_not_py_trip = a2 + b2 - c2;

				if (sum == 1000 && is_not_py_trip == 0) {
					prod = a * b * c;
					std::string str_out ="sum: " + std::to_string(sum) + "\nprod: " + std::to_string(prod) + "\na^2, b^2, c^2: " + std::to_string(a*a) + ", " + std::to_string(b*b) + ", " + std::to_string(c*c) + "\n\n";
					std::cout << str_out;
					return 0;
				}
			}
		}
	}

	return 0;
}