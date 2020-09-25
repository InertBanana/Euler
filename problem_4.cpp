#include "problem_4.h"
#define MAX_THREADS 4

const int T_LIMS[] = { 100, 250, 500, 750, 1000 };
HANDLE product_mutex;
int biggest = -1;

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
typedef struct thread_params {
	int t_id;
} THREAD_PARAMS, * T_DATA;


/*
A palindromic number reads the same both ways. The largest palindrome made from the product of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.

Note: I did this with threads for the sake of practicing threads. 
Followed tutorial at https://docs.microsoft.com/en-us/windows/win32/procthread/ (creating-threads and using-mutex-objects)

*/
int problem_4() {

	T_DATA thread_data_array[MAX_THREADS];
	DWORD dw_thread_id_array[MAX_THREADS];
	HANDLE h_thread_array[MAX_THREADS];
	
	product_mutex = CreateMutex(
		NULL,	// default security atts
		FALSE,	// initially not owned
		NULL	// unnamed mutex
	);

	if (product_mutex == NULL) {
		printf("Mutex is kill: %d\n", GetLastError());
		return 1;
	}


	// following windows thread tutorial

	// create worker threads 
	

	for (int t = 0; t < MAX_THREADS; t++) {
		// alloc mem 
		thread_data_array[t] = (T_DATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(THREAD_PARAMS));

		// array allocation failure - out of memory
		if (thread_data_array[t] == NULL)
		{
			ExitProcess(2);
		}

		thread_data_array[t] -> t_id = t;
		thread_data_array[t]->t_id = t;

		h_thread_array[t] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			thread_fun,				// thread function name
			(void*)thread_data_array[t],	// argument to thread function 
			0,                      // use default creation flags 
			&dw_thread_id_array[t]);   // returns the thread identifier 

		//cout << "Thread made!";
	}
	
	// wait...
	WaitForMultipleObjects(MAX_THREADS, h_thread_array, TRUE, INFINITE);


	//close
	for (int t = 0; t < MAX_THREADS; t++)
	{

		CloseHandle(h_thread_array[t]);
		// free parameters, but never were malloced in this case
		if (thread_data_array[t] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, thread_data_array[t]);
			thread_data_array[t] = NULL;    // Ensure address is not reused.
		}
	}

	return biggest;
}

DWORD WINAPI thread_fun(LPVOID param) {

	DWORD mutex_wait_result;

	T_DATA thread_data_array = (T_DATA)param;
	int low_lim = T_LIMS[ thread_data_array->t_id ];
	int upper_lim = T_LIMS[ thread_data_array->t_id + 1 ];

	for (int i = low_lim; i < upper_lim; i++) {
		for (int j = low_lim; j < upper_lim; j++) {
			int prod = i * j;
			std::string p_str = std::to_string(prod);

			if (check_palindrome(p_str)) {
				mutex_wait_result = WaitForSingleObject(
					product_mutex,
					INFINITE
				);

				switch (mutex_wait_result) {
				case WAIT_OBJECT_0:
					if (prod > biggest)
						biggest = prod;
					if (!ReleaseMutex(product_mutex))
					{
						// Handle error.
					}
					break;

				case WAIT_ABANDONED:
					return false; //oops
				}
			}
		}
	}
	//return true;
}

bool check_palindrome(std::string str) {
	// assume true, find one false occurance
	bool is_pal = true;
	int len = str.length();

	for (int i = 0; i < (len / 2); i++) {
		// abort at any instance of non-matching chars
		// "burn at both ends"
		if (str[i] != str[len - 1 - i]) {
			return false;
		}
	}
}