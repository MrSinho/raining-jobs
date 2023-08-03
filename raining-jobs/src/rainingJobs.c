#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "raining-jobs/rainingJobs.h"

#include <stdlib.h>
#include <memory.h>


uint8_t rainingHostInit(
	uint32_t  item_size_bytes,
	uint32_t  src_length,
	uint8_t   is_type_signed,
	RainHost* p_host
) {
	rainingError(p_host == NULL, "rainingHostInit: invalid raining host memory", return 0);

	RainHost host = {
	    item_size_bytes,
		src_length,
		is_type_signed
	};

	(*p_host) = host;

	//max number of iterations = src_length * bit_resolution
	//total memory usage       = item_size_bytes * src_length * 3
	
	return 1;
}

uint64_t rainingWorkGroup(
	RainingWorkGroupInfo* p_info
) {

	rainingError(
		p_info == NULL,
		"rainingWorkGroup: invalid info memory",
		return 0
	);

	uint32_t left_count  = 0;
	uint32_t right_count = 0;
	uint32_t item_size   = p_info->src_size / p_info->src_length;



	left_count = rainingUnit(
		p_info->memory_type_size_bytes, //memory_type_size_bytes
		p_info->src_length,             //src_length
		p_info->src_size,               //src_size
		p_info->src_offset,             //src_offset
		p_info->p_src,                  //p_src
		p_info->next_src_left_offset,   //next_src_left_offset
		p_info->next_src_right_offset,  //next_src_right_offset
		p_info->start_bit               //bit_idx
	);

	right_count = p_info->src_length - left_count;
	


	if (p_info->start_bit == 0) {//copy duplicates to destination memory

		memcpy(
			&((char*)p_info->p_dst)[p_info->src_offset % p_info->dst_size],
			&((char*)p_info->p_src)[p_info->src_offset],
			p_info->src_size
		);

	}//after that, do the last checks if anything changed

	if (left_count > 1 && p_info->start_bit > 0) {//need to discriminate between at least two numbers, continue, invert dst and src

		uint32_t current_bit = p_info->start_bit - 1;

		RainingWorkGroupInfo next_group_info = {//restrict total size to a local one, set new offset
			p_info->memory_type_size_bytes,                          //memory_type_size
			current_bit,                                             //start_bit
			left_count,                                              //src_length
			left_count * item_size,                                  //src_size
			p_info->src_offset,                                      //next_src_left_offset   //overwrite
			p_info->src_offset + left_count * item_size - item_size, //next_src_right_offset  //overwrite
			p_info->next_src_left_offset,                            //src_offset
			p_info->p_src,                                           //p_src
			p_info->dst_size,                                        //dst_size
			p_info->p_dst                                            //p_dst
		};
		
		rainingWorkGroup(
			&next_group_info //p_info
		);
		
	}
	else if (left_count == 1) {//successfully isolated one number, and found the index of the number

		memcpy(
			&((char*)p_info->p_dst)[p_info->next_src_left_offset % p_info->dst_size],
			&((char*)p_info->p_src)[p_info->next_src_left_offset],
			item_size
		);

	}



	if (right_count > 1 && p_info->start_bit > 0) {//need to discriminate between at least two numbers, continue, invert dst and src

		uint32_t current_bit = p_info->start_bit - 1;

		RainingWorkGroupInfo next_group_info = {
			p_info->memory_type_size_bytes,                                //memory_type_size
			current_bit,                                                   //start_bit
			right_count,                                                   //src_length
			right_count * item_size,                                       //src_size
			p_info->src_offset + left_count * item_size,                   //next_src_left_offset //overwrite
			p_info->src_offset + p_info->src_size - item_size,             //next_src_right_offset //overwrite
			p_info->next_src_right_offset - item_size * (right_count - 1), //src_offset
			p_info->p_src,                                                 //p_src
			p_info->dst_size,                                              //dst_size
			p_info->p_dst,                                                 //p_dst
		};

		rainingWorkGroup(
			&next_group_info //p_info
		);

	}
	else if (right_count == 1) {//successfully isolated one number, and found the index of the number

		memcpy(
			&((char*)p_info->p_dst)[p_info->next_src_right_offset % p_info->dst_size],
			&((char*)p_info->p_src)[p_info->next_src_right_offset],
			item_size
		);

	}

	return 1;//reached max number of bits or all numbers have been isolated from the work group
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

#ifdef _MSC_VER
		if (item & (1i64 << bit_idx)) {//64 bit shift, MSVC complains otherwise
#else
		if (item & (1 << bit_idx)) {
#endif//_MSC_VER
			
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

uint8_t rainingSegregate(
	uint32_t sign_bit,
	uint32_t item_size,
	void*    p_src,
	uint32_t dst_size,
	void*    p_dst
) {
	uint32_t negative_count = 0;

	for (uint32_t item_offset = 0; item_offset < dst_size; item_offset += item_size) {
		
		uint64_t* p_item = (uint64_t*)&((char*)p_dst)[item_offset];
		uint64_t    item = (*p_item);

		//save negative numbers at unused p_src memory
#ifdef _MSC_VER
		if (item & (1i64 << sign_bit)) {
#else
		if (item & (1 << sign_bit)) {
#endif//_MSC_VER

			memcpy(
				&((char*)p_src)[item_offset],
				p_item,
				item_size
			);

			negative_count++;
		}

	}

	if (negative_count == 0) {
		return 1;
	}

	uint32_t shift_offset = 0;

	for (uint32_t item_offset = (negative_count * item_size); item_offset < dst_size; item_offset += item_size) {

		uint64_t* p_item = (uint64_t*)&((char*)p_dst)[item_offset];
		uint64_t    item = (*p_item);

		//shift positive numbers to the origin of dst
		memcpy(
			&((char*)p_dst)[shift_offset],
			&((char*)p_dst)[item_offset],
			item_size
		);
		
		shift_offset += item_size;

	}

	uint32_t negative_src_offset = negative_count * item_size - item_size;

	for (; shift_offset < dst_size; shift_offset += item_size) {

		uint64_t* p_item = (uint64_t*)&((char*)p_dst)[shift_offset];
		uint64_t    item = (*p_item);

		//shift negative numbers to the end
		memcpy(
			&((char*)p_dst)[shift_offset],
			&((char*)p_src)[negative_src_offset],
			item_size
		);

		negative_src_offset -= item_size;
	}

	return 1;
}

uint8_t rainingHostSubmit(
	RainHost* p_host,
	void*     p_src,
	void*     p_dst
) {
	rainingError(p_host == NULL, "rainingHostSubmit: invalid raining host memory", return 0);

	RainingWorkGroupInfo start_group_info = {
		p_host->item_size_bytes,                                                   //memory_type_size_bytes
		p_host->item_size_bytes * 8 - 1,                                           //start_bit
		p_host->src_length,                                                        //src_length
		p_host->src_length* p_host->item_size_bytes,                               //src_size
		p_host->src_length* p_host->item_size_bytes,                               //next_src_left_offset
		p_host->src_length* p_host->item_size_bytes * 2 - p_host->item_size_bytes, //next_src_right_offset
		0,                                                                         //src_offset
		p_src,                                                                     //p_src
		p_host->src_length* p_host->item_size_bytes,                               //dst_size
		p_dst                                                                      //p_dst
	};

	rainingWorkGroup(
		&start_group_info //p_info
	);

	if (!p_host->is_type_signed) {
		return 1;
	}

	rainingSegregate(
		p_host->item_size_bytes * 8 - 1,              //sign_bit
		p_host->item_size_bytes,                      //item_size
		p_src,                                        //p_src
		p_host->src_length * p_host->item_size_bytes, //dst_size
		p_dst                                         //p_dst
	);

	return 1;
}

uint8_t rainingHostRelease(
	RainHost* p_host
) {
	rainingError(p_host == NULL, "rainingHostRelease: invalid raining host memory", return 0);

	memset(p_host, 0, sizeof(RainHost));

	return 1;
}




#ifdef __cplusplus
}
#endif//__cplusplus