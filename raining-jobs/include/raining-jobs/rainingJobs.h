#ifndef RAINING_THREADS_H
#define RAINING_THREADS_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <stdint.h>
#include <stdio.h>



#define RAINING_TRUE  1
#define RAINING_FALSE 0
#define RAINING_ENABLE_MULTITHREADING 0

#if RAINING_ENABLE_MULTITHREADING
#include <shthreads/shThreads.h>
#endif//RAINING_ENABLE_MULTITHREADING



#define rainingError(condition, error_msg, failure_expression)\
	if ((int)(condition)) {\
		printf("raining error: %s\n", error_msg);\
		failure_expression;\
	}



typedef struct RainHost {
	uint32_t      item_size_bytes;
	uint32_t      src_length;
	uint8_t       is_type_signed;
#if RAINING_ENABLE_MULTITHREADING == 1
	ShThreadPool  thread_pool; //safe, a copy is submitted
	uint32_t      thread_count;//unsafe, shared
	ShMutex       mutex;       //safe, it's a mutex
	uint64_t*     p_exit_codes;
#endif//RAINING_ENABLE_MULTITHREADING
} RainHost;



typedef struct RainingWorkGroupInfo {
	uint32_t      memory_type_size_bytes;
	uint32_t      start_bit;
	uint32_t      src_length;
	uint32_t      src_size;
	uint32_t      next_src_left_offset;
	uint32_t      next_src_right_offset;
	uint32_t      src_offset;
	void*         p_src;
	uint32_t      dst_size;
	void*         p_dst;
#if RAINING_ENABLE_MULTITHREADING == 1
	uint32_t*     p_thread_count; //shared, stored at RainHost
	ShThreadPool* p_thread_pool;  //shared, stored at RainHost
	ShMutex       mutex;
#endif//RAINING_ENABLE_MULTITHREADING
} RainingWorkGroupInfo;



extern uint8_t rainingHostInit(
	uint32_t  item_size_bytes,
	uint32_t  src_length,
	uint8_t   is_type_signed,
	RainHost* p_host
);

extern uint64_t rainingWorkGroup(
	RainingWorkGroupInfo* p_info
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

extern uint8_t rainingSegregate(
	uint32_t sign_bit,
	uint32_t item_size,
	void*    p_src,
	uint32_t dst_size,
	void*    p_dst
);

extern uint8_t rainingHostSubmit(
	RainHost* p_host,
	void*     p_src,
	void*     p_dst
);

extern uint8_t rainingHostWaitForAll(
	RainHost* p_host,
	uint64_t  timeout_ms
);

extern uint8_t rainingHostRelease(
	RainHost* p_host
);



#ifdef __cplusplus
}
#endif//_cplusplus

#endif//RAINING_THREADS_H