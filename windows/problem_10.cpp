#include "problem_10.h"
#define MAX_THREAD 4
#define PRIME_LIMIT 2000000
//#pragma warning( disable: 6386 )
//#pragma warning( disable: 6011 )

/*
The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.
*/

// explanation in problem_4.cpp
typedef struct thread_params {
	int* primes_ptr;
	int arr_size;
} THREAD_PARAMS, * T_DATA;


HANDLE sum_mutex;
int64_t prime_sum = 0;
int64_t chksum = 0;

int64_t problem_10() {

	// make our lovely mutex
	sum_mutex = CreateMutex(
		NULL,	// default security atts
		FALSE,	// initially not owned
		NULL	// unnamed mutex
	);

	if (sum_mutex == NULL) {
		printf("Mutex is kill: %d\n", GetLastError());
		return 1;
	}
	// because things take quite a long time, I became interested in HOW long sieve, thread and sequential take
	time_t sieve_start_time = time(NULL);
	// before we thread, need to chop this lad up so each thread for neatness's sake
	std::vector<int> primes = sieve();

	time_t sieve_end_time = time(NULL);
	time_t sieve_time_elapsed = sieve_end_time - sieve_start_time;
	time_t thread_start_time = time(NULL);

	int num_primes = primes.size();

	int quarter_length = (num_primes / 4);
	int quarter_size = quarter_length * sizeof(primes[0]);

	// not guaranteed the last vector will be divisible by 4, 
	// so make length the remainder of vector length - 3 "quarters"
	int last_array_length = primes.size() - (3 * quarter_length);
	int last_array_size = last_array_length * sizeof(primes[0]);

	int* t0_primes = (int*) malloc(quarter_size);
	int* t1_primes = (int*) malloc(quarter_size);
	int* t2_primes = (int*) malloc(quarter_size);
	int* t3_primes = (int*) malloc(last_array_size);

	int* prime_ptrs[] = { t0_primes, t1_primes, t2_primes, t3_primes };
	int arr_sizes[] = { quarter_length, quarter_length, quarter_length, last_array_length };

	for (int i = 0; i < quarter_length; i++) {
		t0_primes[i] = primes[i];
		t1_primes[i] = primes[quarter_length + i];
		t2_primes[i] = primes[(2 * quarter_length) + i];
	}

	// know what element the last array starts at but do not know what it'll end on
	for (int i = 0; i < last_array_length; i++) {
		t3_primes[i] = primes[(3 * quarter_length) + i];
	}
	

	// the exciting thread boilerplate is back!
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

		thread_data_array[t]->primes_ptr = prime_ptrs[t];
		thread_data_array[t]->arr_size = arr_sizes[t];

		h_thread_array[t] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			sum_primes,				// thread function name
			(void*)thread_data_array[t],	// argument to thread function 
			0,                      // use default creation flags 
			&dw_thread_id_array[t]);   // returns the thread identifier 

		//cout << "Thread made!";
	}

	// wait...
	WaitForMultipleObjects(MAX_THREAD, h_thread_array, TRUE, INFINITE);


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
	free(t0_primes);
	free(t1_primes);
	free(t2_primes);
	free(t3_primes);

	time_t thread_end_time = time(NULL);
	time_t thread_time_elapsed = thread_end_time - thread_start_time;

	for (int i = 0; i < primes.size(); i++) { chksum += primes[i]; }

	time_t sequential_time_elapsed = time(NULL) - thread_end_time;

	std::cout << "Time taken to sieve primes to 2 million: " << sieve_time_elapsed << std::endl;

	std::cout << "Time taken sum primes under 2 million with 4 threads (setup included): " << thread_time_elapsed << std::endl;

	std::cout << "Time taken sum primes under 2 million with 4 threads (setup excluded): " << sequential_time_elapsed << std::endl;

	return prime_sum;
}


std::vector<int> sieve() {

	std::vector<int> primes;

	int current = 2;

	// fill the sieve, start with cap of 10,000 (pure fucking shot in the dark on how big it's gonna be)
	while (current < PRIME_LIMIT) {
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

	return primes;
}

DWORD WINAPI sum_primes(LPVOID param) {
	T_DATA thread_data_array = (T_DATA)param;
	DWORD mutex_wait_result;

	int* prime_ptr = (int*)thread_data_array->primes_ptr;
	int size = (int)thread_data_array->arr_size;

	int64_t thread_sum = 0;

	for (int i = 0; i < size; i++) {
		thread_sum += *(prime_ptr + i);
	}

	mutex_wait_result = WaitForSingleObject(
		sum_mutex,
		INFINITE
	);

	// when done calculating own sum, await the mutex and add to the global total 
	switch (mutex_wait_result) {
	case WAIT_OBJECT_0:
		prime_sum += thread_sum;

		if (!ReleaseMutex(sum_mutex))
		{
			// This is where I would keep my error handler... IF I HAD ONE :(
		}
		break;

	case WAIT_ABANDONED:
		return false; //oops
	}
	
	return 0;
}