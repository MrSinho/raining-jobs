#ifdef __cplusplus
extern "C" {
#endif//_cplusplus

#include <raining-jobs/rainingJobs.h>

#include <stdlib.h>
#include <time.h>


#define SRC_LENGTH  ((int)1E4)
#define DEBUG_PRINT 1


int main(void) {

	float* src = (float*)calloc(SRC_LENGTH * 2, sizeof(float));
	float* dst = (float*)calloc(SRC_LENGTH * 2, sizeof(float));

	rainingError(src == NULL, "main: invalid src memory", return -1);
	rainingError(dst == NULL, "main: invalid dst memory", return -1);

	for (uint32_t i = 0; i < SRC_LENGTH; i++) {
		src[i] = (float)rand() / (float)rand() * (float)(i % 2 == 0 ? -1 : 1);
		//src[i] = rand() % rand() * (i % 2 == 0 ? -1 : 1);
	}

#if DEBUG_PRINT == 1
	printf("\n\n\n\tSRC:\n\n");
	for (uint32_t i = 0; i < SRC_LENGTH; i++) {
		printf("%i: %f\n", i, src[i]);
	}
#endif//DEBUG_PRINT

	
	RainHost host = { 0 };
	uint8_t  r    =   0;

	r = rainingGetSupportedMemoryFormats(&host);

	r = rainingHostInit(
		sizeof(float),          //item_size_bytes 
		SRC_LENGTH,             //src_length 
		RAINING_TRUE,           //is_type_signed
		RAIN_MEMORY_TYPE_FLOAT, //queried_memory_type
		&host                   //p_host    
	);

	rainingError(r == 0, "failed initializing host", return 0);

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


#if DEBUG_PRINT == 1
	printf("\n\n\n\tDST:\n\n");
	for (uint32_t i = 0; i < SRC_LENGTH; i++) {
		printf("%i: %f\n", i, dst[i]);
	}
#endif//DEBUG_PRINT

	float elapsed_time_s = (float)(end_ticks - start_ticks) / (float)CLOCKS_PER_SEC;
	printf("Time consumed to sort %i values: %.3f milliseconds\n", SRC_LENGTH, elapsed_time_s * 1000.0);

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus