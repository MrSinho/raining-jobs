# Raining jobs

[![](.shci/linux/exit_code.svg)](.shci/linux/log.md) [![](.shci/windows/exit_code.svg)](.shci/windows/log.md)

"Raining jobs" is a simple and stable algorithm used to quickly sort numbers (with size ranging from one byte to 8 bytes, floats and integers, signed and unsigned) without exceeding with too much extra memory and iterations: in the worst case scenario, with a 64 bit resolution, the maximum number of checks is `n_samples * 64`, while the memory usage is always stable and equal to three times the original set size, destination buffer included. The library is written in C.

---

## Benchmarks and comparison with other known sorting algorithms

---

## Implementation

See the `raining-example.c` [here](./examples/src/raining-example.c):

```c
#ifdef __cplusplus
extern "C" {
#endif//_cplusplus

#include <raining-jobs/rainingJobs.h>

#include <stdlib.h>
#include <time.h>


#define SRC_LENGTH  ((int)1000000)

int main(void) {

	float* src = (float*)calloc(SRC_LENGTH * 2, sizeof(float));
	float* dst = (float*)calloc(SRC_LENGTH * 2, sizeof(float));

	rainingError(src == NULL, "main: invalid src memory", return -1);
	rainingError(dst == NULL, "main: invalid dst memory", return -1);

	for (uint32_t i = 0; i < SRC_LENGTH; i++) {
		src[i] = (float)rand() / (float)rand() * (float)(i % 2 == 0 ? -1 : 1);
	}

	RainHost host = { 0 };
	
	rainingHostInit(
		sizeof(float),   //item_size_bytes 
		SRC_LENGTH,      //src_length 
		RAINING_TRUE,    //is_type_signed
		&host            //p_host    
	);


	clock_t start_ticks = clock();

	rainingHostSubmit(
		&host, //p_host
		src,   //p_src
		dst    //p_dst
	);

	clock_t end_ticks = clock();

	rainingHostRelease(
		&host //p_host
	);


	double elapsed_time_s = (double)(end_ticks - start_ticks) / (double)CLOCKS_PER_SEC;
	printf("Time consumed to sort %i values: %.3f milliseconds\n", SRC_LENGTH, elapsed_time_s * 1000.0);

	return 0;
}

#ifdef __cplusplus
}
#endif//_cplusplus
```

---

## Known issues

There is certainly room for improvements. The algorithm was first meant to be used within a multi-threading library, since it was designed to not create conflicts with read-write operations between multiple threads. Currently the library runs on the main application thread only.

---