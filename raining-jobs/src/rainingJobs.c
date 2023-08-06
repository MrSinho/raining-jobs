#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "raining-jobs/rainingJobs.h"

#include <stdlib.h>
#include <memory.h>
#include <inttypes.h>



uint8_t rainingGetSupportedMemoryFormats(
	RainHost* p_host
) {
	rainingError(p_host == NULL, "rainingGetSupportedMemoryFormats: invalid raining host memory", return 0);

	int8_t i_negative_one_magnitude_complement = (int8_t)0x81; // = 0b10000001 last bit is sign bit
	int8_t i_negative_one_one_complement       = (int8_t)0xFE; // = 0b11111110 = invert all bits from positive
	int8_t i_negative_one_two_complement       = (int8_t)0xFF; // = 0b11111111 = one complement + 1 unit

	if (i_negative_one_magnitude_complement == (int8_t)(-1)) {
		p_host->supported_memory_formats |= RAIN_INT_MEMORY_FORMAT_MAGNITUDE_COMPLEMENT;
	}
	else if (i_negative_one_one_complement == (int8_t)(-1)) {
		p_host->supported_memory_formats |= RAIN_INT_MEMORY_FORMAT_ONE_COMPLEMENT;
	}
	else if (i_negative_one_two_complement == (int8_t)(-1)) {
		p_host->supported_memory_formats |= RAIN_INT_MEMORY_FORMAT_TWO_COMPLEMENT;
	}
	else {
		p_host->supported_memory_formats |= RAIN_INT_UNSUPPORTED_MEMORY_FORMAT;
	}

	uint32_t bin_negative_one_IEE_754 = 0xBF800000; // = 0b10111111100000000000000000000000;
	float* p_f_negative_one_IEE_754   = (float*)&bin_negative_one_IEE_754;
	float      f_negative_one_IEE_754 = (*p_f_negative_one_IEE_754);

	if (f_negative_one_IEE_754 == -1.0f) {
		p_host->supported_memory_formats |= RAIN_FLOAT_MEMORY_FORMAT_IEEE_754;
	}
	else {
		p_host->supported_memory_formats |= RAIN_FLOAT_UNSUPPORTED_MEMORY_FORMAT;
	}

	return 1;
}

uint8_t rainingHostInit(
	uint32_t       item_size_bytes,
	uint32_t       src_length,
	uint8_t        is_type_signed,
	RainMemoryType queried_memory_type,
	RainHost* p_host
) {
	rainingError(p_host == NULL, "rainingHostInit: invalid raining host memory", return 0);

	p_host->item_size_bytes = item_size_bytes;
	p_host->src_length = src_length;
	p_host->is_type_signed = is_type_signed;
	p_host->queried_memory_type = queried_memory_type;

	//max number of iterations = src_length * bit_resolution
	//total memory usage       = item_size_bytes * src_length * 3

	if (p_host->queried_memory_type == RAIN_MEMORY_TYPE_INT) {

		RainMemoryFormat result = p_host->supported_memory_formats & RAIN_INT_UNSUPPORTED_MEMORY_FORMAT;

		rainingError(
			result == RAIN_INT_UNSUPPORTED_MEMORY_FORMAT,
			"rainingHostInit: unsupported int memory format, may lead to unexpected output",
			return 0
		);
	}
	else if (p_host->queried_memory_type == RAIN_MEMORY_TYPE_FLOAT) {
		RainMemoryFormat result = p_host->supported_memory_formats & RAIN_FLOAT_UNSUPPORTED_MEMORY_FORMAT;

		rainingError(
			result == RAIN_FLOAT_UNSUPPORTED_MEMORY_FORMAT,
			"rainingHostInit: unsupported float memory format, may lead to unexpected output",
			return 0
		);
	}

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

	//puts("\n");
	//printf("start_bit:              %i\n", p_info->start_bit);
	//printf("src_length:             %i\n", p_info->src_length);
	//printf("src_size:               %i\n", p_info->src_size);
	//printf("next_src_left_offset:   %i\n", p_info->next_src_left_offset);
	//printf("next_src_right_offset:  %i\n", p_info->next_src_right_offset);
	//printf("src_offset:             %i\n", p_info->src_offset);
	//printf("dst_size:               %i\n", p_info->dst_size);


	uint32_t left_count  = 0;
	uint32_t right_count = 0;
	uint32_t item_size   = p_info->src_size / p_info->src_length;



	left_count = rainingUnit(
		p_info->src_length,            //src_length
		p_info->src_size,              //src_size
		p_info->src_offset,            //src_offset
		p_info->p_src,                 //p_src
		p_info->next_src_left_offset,  //next_src_left_offset
		p_info->next_src_right_offset, //next_src_right_offset
		p_info->start_bit              //bit_idx
	);

	right_count = p_info->src_length - left_count;
	
	//printf("left_count:  %i\n", left_count);
	//printf("right_count: %i\n", right_count);
	//puts("\n");

	if (p_info->start_bit == 0) {//copy duplicates to destination memory

		uint8_t* _p_dst = (uint8_t*)p_info->p_dst;
		uint8_t* _p_src = (uint8_t*)p_info->p_src;

		memcpy(
			&_p_dst[p_info->src_offset % p_info->dst_size],
			&_p_src[p_info->src_offset],
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
			p_info->p_dst,                                           //p_dst
		};
		
		rainingWorkGroup(
			&next_group_info //p_info
		);
		
	}
	else if (left_count == 1) {//successfully isolated one number, and found the index of the number

		uint8_t* _p_dst = (uint8_t*)p_info->p_dst;
		uint8_t* _p_src = (uint8_t*)p_info->p_src;
		
		memcpy(
			&_p_dst[p_info->next_src_left_offset % p_info->dst_size],
			&_p_src[p_info->next_src_left_offset],
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

		uint8_t* _p_dst = (uint8_t*)p_info->p_dst;
		uint8_t* _p_src = (uint8_t*)p_info->p_src;

		memcpy(
			&_p_dst[p_info->next_src_right_offset % p_info->dst_size],
			&_p_src[p_info->next_src_right_offset],
			item_size
		);

	}

	return 1;//reached max number of bits or all numbers have been isolated from the work group
}

uint32_t rainingUnit(
	uint32_t src_length,
	uint32_t src_size,
	uint32_t src_offset,
	void*    p_src,
	uint32_t next_src_left_offset,
	uint32_t next_src_right_offset,
	uint32_t bit_idx
) {
	uint32_t item_size              = src_size / src_length;//in bytes

	uint32_t _next_src_left_offset  = next_src_left_offset;
	uint32_t _next_src_right_offset = next_src_right_offset;//_0 = src_size - item_size

	uint32_t left_count             = 0;//bigger

	for (uint32_t local_item_offset = 0; local_item_offset < src_size; local_item_offset += item_size) {
		
		//find memory address
		uint8_t* _p_src_as_bytes = (uint8_t*)p_src;

		uint64_t* p_item = (uint64_t*)(&_p_src_as_bytes[src_offset + local_item_offset]);
		uint64_t  item   = (uint64_t)(*p_item);//not actual value, but value with potentially junk bits coming from next value, which are discarded

#ifdef _MSC_VER
		if ((uint64_t)(item & (1i64 << bit_idx)) == (uint64_t)(1i64 << bit_idx)) {//64 bit shift, MSVC complains otherwise, no sense btw, I lost my most beautiful years for that
#else
		if ((uint64_t)(item & (1ULL << bit_idx)) == (uint64_t)(1ULL << bit_idx)) {//very important ULL for 64 bit results, no sense btw, I lost my most beautiful years for that
#endif//_MSC_VER
			
			memcpy(
				&_p_src_as_bytes[_next_src_left_offset],
				p_item,
				item_size
			);
		
			_next_src_left_offset += item_size;
			left_count++;
		}
		else {
			memcpy(
				&_p_src_as_bytes[_next_src_right_offset],
				p_item,
				item_size
			);
		
			_next_src_right_offset -= item_size;
		}
		
	}

	return left_count;
}

uint8_t rainingSegregate(
	uint32_t                 sign_bit,
	uint32_t                 item_size,
	void*                    p_src,
	uint32_t                 dst_size,
	void*                    p_dst,
	RainSegregateInstruction instruction
) {
	uint32_t negative_count = 0;

	uint8_t* p_dst_as_bytes = (uint8_t*)p_dst;
	uint8_t* p_src_as_bytes = (uint8_t*)p_src;

	for (uint32_t item_offset = 0; item_offset < dst_size; item_offset += item_size) {

		uint8_t* p_dst_at_offset = &p_dst_as_bytes[item_offset];

		uint64_t* p_item = (uint64_t*)p_dst_at_offset;
		uint64_t    item = (*p_item);


		//save negative numbers at unused p_src memory, the most negative numbers are at the beginning of src
#ifdef _MSC_VER
		if ((uint64_t)(item & (1i64 << sign_bit)) == (uint64_t)(1i64 << sign_bit)) {
#else
		if ((uint64_t)(item & (1ULL << sign_bit)) == (uint64_t)(1ULL << sign_bit)) {
#endif//_MSC_VER

			memcpy(
				&p_src_as_bytes[item_offset],
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

		//uint64_t* p_item = (uint64_t*)&((uint8_t*)p_dst)[item_offset];
		//uint64_t    item = (*p_item);

		//shift positive numbers to the origin of dst
		memcpy(
			&p_dst_as_bytes[shift_offset],
			&p_dst_as_bytes[item_offset],
			item_size
		);
		
		shift_offset += item_size;

	}

	//the most negative numbers are at the beginning of the src, move them to the end of dst by reversing the source order
	if (instruction == RAIN_SEGREGATE_INSTRUCTION_INVERSE_COPY_NEGATIVE) {
		uint32_t negative_src_offset = negative_count * item_size - item_size;

		for (; shift_offset < dst_size; shift_offset += item_size) {

			//uint64_t* p_item = (uint64_t*)&((uint8_t*)p_dst)[shift_offset];
			//uint64_t    item = (*p_item);

			//shift negative numbers to the end
			memcpy(
				&p_dst_as_bytes[shift_offset],
				&p_src_as_bytes[negative_src_offset],
				item_size
			);

			negative_src_offset -= item_size;
		}
	}
	else if (instruction == RAIN_SEGREGATE_INSTRUCTION_COPY_NEGATIVE) {
		uint32_t negative_src_offset = 0;

		for (; shift_offset < dst_size; shift_offset += item_size) {

			//uint64_t* p_item = (uint64_t*)&((uint8_t*)p_dst)[shift_offset];
			//uint64_t    item = (*p_item);

			//copy negative numbers without inverting the order
			memcpy(
				&p_dst_as_bytes[shift_offset],
				&p_src_as_bytes[negative_src_offset],
				item_size
			);

			negative_src_offset += item_size;
		}
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
		p_host->item_size_bytes,                                                    //memory_type_size_bytes
		p_host->item_size_bytes * 8 - 1,                                            //start_bit
		p_host->src_length,                                                         //src_length
		p_host->src_length * p_host->item_size_bytes,                               //src_size
		p_host->src_length * p_host->item_size_bytes,                               //next_src_left_offset
		p_host->src_length * p_host->item_size_bytes * 2 - p_host->item_size_bytes, //next_src_right_offset
		0,                                                                          //src_offset
		p_src,                                                                      //p_src
		p_host->src_length * p_host->item_size_bytes,                               //dst_size
		p_dst,                                                                      //p_dst
	};

	rainingWorkGroup(
		&start_group_info //p_info
	);


	if (!p_host->is_type_signed) {
		return 1;
	}


	RainSegregateInstruction instruction = RAIN_SEGREGATE_INSTRUCTION_MAX_ENUM;

	if (p_host->queried_memory_type == RAIN_MEMORY_TYPE_FLOAT) {
		instruction = RAIN_SEGREGATE_INSTRUCTION_INVERSE_COPY_NEGATIVE;
	}
	else if (p_host->queried_memory_type == RAIN_MEMORY_TYPE_INT) {
		instruction = RAIN_SEGREGATE_INSTRUCTION_COPY_NEGATIVE;
	}
	else {
		rainingError(1, "invalid queried memory type", return 0);
	}

	rainingSegregate(
		p_host->item_size_bytes * 8 - 1,              //sign_bit
		p_host->item_size_bytes,                      //item_size
		p_src,                                        //p_src
		p_host->src_length * p_host->item_size_bytes, //dst_size
		p_dst,                                        //p_dst
		instruction                                   //instruction
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