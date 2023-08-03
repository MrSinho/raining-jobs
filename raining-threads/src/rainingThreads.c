#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "raining-threads/rainingThreads.h"



uint8_t rainingHostInit(
	uint32_t     max_sets_length, 
	RainingHost* p_host
) {
	rainingError(p_host == NULL, "rainingHostInit: invalid raining host memory", return 0);

	//The algorithm predicts that the maximum number of threads is the 
	//half of the number of items
	//
	shAllocateThreads(max_sets_length / 2 + 1, &p_host->thread_pool);

	return 1;
}

uint8_t rainingComputeInit(
	uint32_t        max_sets_length,
	RainingVkCore*  p_core,
	RainingCompute* p_compute
) {
	rainingError(p_compute == NULL, "rainingComputeInit: invalid raining compute memory", return 0);

	return 1;
}



uint32_t rainingUnit(
	uint32_t memory_type_size_bytes,
	uint32_t src_length,
	uint32_t src_size,
	uint32_t src_offset,
	uint8_t* p_src,
	uint32_t next_src_left_offset,
	uint32_t next_src_right_offset,
	uint32_t bit_idx
) {
	uint32_t item_size              = src_size / src_length;//in bytes

	uint32_t _next_src_left_offset  = next_src_left_offset;
	uint32_t _next_src_right_offset = next_src_right_offset;//_0 = src_size - item_size

	uint32_t left_count             = 0;//bigger

	for (uint32_t local_item_offset = 0; local_item_offset < src_size; local_item_offset += item_size) {
		
		//debug
		//uint8_t* p_debug = (uint8_t*)&((char*)p_src)[src_offset + local_item_offset];

		//find memory address
		uint64_t* p_item = (uint64_t*)&((char*)p_src)[src_offset + local_item_offset];
		uint64_t  item   = (uint64_t)(*p_item);//not actual value, but value with potentially junk bits coming from next value, which are discarded

		if (item & (1i64 << bit_idx)) {//64 bit shift, MSVC complains otherwise
			
			memcpy(
				&((char*)p_src)[_next_src_left_offset],
				p_item,
				item_size
			);
		
			_next_src_left_offset += item_size;
			left_count++;
		}
		else {
			memcpy(
				&((char*)p_src)[_next_src_right_offset],
				p_item,
				item_size
			);
		
			_next_src_right_offset -= item_size;
		}
		
	}

	return left_count;
}



uint8_t rainingHostSubmit(
	RainingHost* p_host,
	uint32_t     set_lenght,
	void*        p_src
) {
	rainingError(p_host == NULL, "rainingHostSubmit: invalid raining host memory", return 0);

	return 1;
}

uint8_t rainingComputeSubmit(
	RainingCompute* p_compute,
	uint32_t        set_lenght,
	void*           p_src 
) {
	rainingError(p_compute == NULL, "rainingComputeSubmit: invalid raining compute memory", return 0);

	return 1;
}



uint8_t rainingWaitForHost(
	RainingHost* p_host,
	uint32_t*    p_dst_size,
	void*        p_dst
) {
	rainingError(p_host == NULL, "rainingWaitForHost: invalid raining host memory", return 0);

	return 1;
}

uint8_t rainingWaitForQueue(
	RainingCompute* p_compute,
	uint32_t*       p_dst_size,
	void*           p_dst
) {
	rainingError(p_compute == NULL, "rainingWaitForQueue: invalid raining compute memory", return 0);

	return 1;
}



uint8_t rainingHostRelease(
	RainingHost* p_host
) {
	rainingError(p_host == NULL, "rainingHostRelease: invalid raining host memory", return 0);

	return 1;
}

uint8_t rainingComputeRelease(
	RainingCompute* p_compute
) {
	rainingError(p_compute == NULL, "rainingComputeRelease: invalid raining compute memory", return 0);

	return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus