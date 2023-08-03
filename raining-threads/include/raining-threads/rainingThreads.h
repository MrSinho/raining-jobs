#ifndef RAINING_THREADS_H
#define RAINING_THREADS_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shthreads/shthreads.h>
#include <shvulkan/shVulkan.h>

#include <stdint.h>
#include <stdio.h>



#define rainingError(condition, error_msg, failure_expression)\
	if ((int)(condition)) {\
		printf("raining error: %s\n", error_msg);\
		failure_expression;\
	}

#define RAINING_TRUE  1
#define RAINING_FALSE 0


typedef struct RainingHost {
	uint32_t     used_threads;
	ShThreadPool thread_pool;
} RainingHost;


typedef struct RainingVkCore {
	VkDevice                   device;
	VkPhysicalDevice           physical_device;
	VkPhysicalDeviceProperties physical_device_properties;
} RainingVkCore;

typedef struct RainingCompute {
	RainingVkCore core;
} RainingCompute;



extern uint8_t rainingHostInit(
	uint32_t     max_sets_length, 
	RainingHost* p_host
);

extern uint8_t rainingComputeInit(
	uint32_t        max_sets_length, 
	RainingVkCore*  p_core,
	RainingCompute* p_compute
);


extern uint32_t rainingUnit(
	uint32_t memory_type_size_bytes,
	uint32_t src_length,
	uint32_t src_size,
	uint32_t src_offset,
	uint8_t* p_src,
	uint32_t next_src_left_offset,
	uint32_t next_src_right_offset,
	uint32_t bit_idx
);



extern uint8_t rainingHostSubmit(
	RainingHost* p_host,
	uint32_t     set_lenght,
	void*        p_src
);

extern uint8_t rainingComputeSubmit(
	RainingCompute* p_compute,
	uint32_t        set_lenght,
	void*           p_src 
);



extern uint8_t rainingWaitForHost(
	RainingHost* p_host,
	uint32_t*    p_dst_size,
	void*        p_dst
);

extern uint8_t rainingWaitForQueue(
	RainingCompute* p_compute,
	uint32_t*       p_dst_size,
	void*           p_dst
);



extern uint8_t rainingHostRelease(
	RainingHost* p_host
);

extern uint8_t rainingComputeRelease(
	RainingCompute* p_compute
);



#ifdef __cplusplus
}
#endif//_cplusplus

#endif//RAINING_THREADS_H