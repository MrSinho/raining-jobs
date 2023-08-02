#ifdef __cplusplus
extern "C" {
#endif//_cplusplus

#include <raining-threads/rainingThreads.h>

#include <math.h>
#include <stdlib.h>


#define SRC_LENGTH ((int)5)
#define ITEM_SIZE  sizeof(uint32_t)


//I PREFER CALLING THIS RAIN INSTEAD OF RECURSION
//BECAUSE IT SOUNDS MORE CREATIVE
//
void rainingWorkGroup(
	uint32_t          memory_type_size_bytes,
	uint8_t           is_signed,
	uint32_t          start_bit, 
	uint32_t          src_length, 
	uint32_t          src_size, 
	uint32_t          next_src_left_offset,
	uint32_t          next_src_right_offset,
	uint32_t          src_offset, 
	void*             p_src,
	void*             p_dst,
	uint32_t          src_original_size
) {
	uint8_t* p_a0 = &((char*)p_src)[0 * src_size / src_length];
	uint8_t* p_a1 = &((char*)p_src)[1 * src_size / src_length];
	uint8_t* p_a2 = &((char*)p_src)[2 * src_size / src_length];
	uint8_t* p_a3 = &((char*)p_src)[3 * src_size / src_length];
	uint8_t* p_a4 = &((char*)p_src)[4 * src_size / src_length];
	uint8_t* p_a5 = &((char*)p_src)[5 * src_size / src_length];
	uint8_t* p_a6 = &((char*)p_src)[6 * src_size / src_length];
	uint8_t* p_a7 = &((char*)p_src)[7 * src_size / src_length];
	uint8_t* p_a8 = &((char*)p_src)[8 * src_size / src_length];
	uint8_t* p_a9 = &((char*)p_src)[9 * src_size / src_length];
	
	uint8_t* p_d0 = &((char*)p_dst)[0 * src_size / src_length];
	uint8_t* p_d1 = &((char*)p_dst)[1 * src_size / src_length];
	uint8_t* p_d2 = &((char*)p_dst)[2 * src_size / src_length];
	uint8_t* p_d3 = &((char*)p_dst)[3 * src_size / src_length];
	uint8_t* p_d4 = &((char*)p_dst)[4 * src_size / src_length];



	uint32_t left_count  = 0;
	uint32_t right_count = 0;
	uint32_t item_size   = src_size / src_length;



	left_count = rainingUnit(
		memory_type_size_bytes, //memory_type_size_bytes
		is_signed,              //is_signed
		src_length,             //src_length
		src_size,               //src_size
		src_offset,             //src_offset
		p_src,                  //p_src
		next_src_left_offset,   //next_src_left_offset
		next_src_right_offset,  //next_src_right_offset
		start_bit               //bit_idx
	);

	right_count = src_length - left_count;
	


	if (start_bit == 0) {//copy duplicates to destination memory

		memcpy(
			&((char*)p_dst)[src_offset % src_original_size],
			&((char*)p_src)[src_offset],
			src_size
		);

	}//after that, do the last checks if anything changed



	if (left_count > 1 && start_bit > 0) {//need to discriminate between at least two numbers, continue, invert dst and src

		uint32_t current_bit = start_bit - 1;

		rainingWorkGroup(//restrict total size to a local one, set new offset
			memory_type_size_bytes,                          //memory_type_size
			is_signed,                                       //is_signed
			current_bit,                                     //start_bit
			left_count,                                      //src_length
			left_count * item_size,                          //src_size
			src_offset,                                      //next_src_left_offset   //overwrite
			src_offset + left_count * item_size - item_size, //next_src_right_offset  //overwrite
			next_src_left_offset,                            //src_offset
			p_src,                                           //p_src
			p_dst,                                           //p_dst
			src_original_size                                //src_original_size
		);

	}
	else if (left_count == 1) {//successfully isolated one number, and found the index of the number

		((char*)p_dst)[next_src_left_offset % src_original_size] = ((char*)p_src)[next_src_left_offset];

	}



	if (right_count > 1 && start_bit > 0) {//need to discriminate between at least two numbers, continue, invert dst and src

		uint32_t current_bit = start_bit - 1;

		rainingWorkGroup(
			memory_type_size_bytes,                                //memory_type_size
			is_signed,                                             //is_signed
			current_bit,                                           //start_bit
			right_count,                                           //src_length
			right_count * item_size,                               //src_size
			src_offset + left_count * item_size,                   //next_src_left_offset //overwrite
			src_offset + src_size - item_size,                     //next_src_right_offset //overwrite
			next_src_right_offset - item_size * (right_count - 1), //src_offset
			p_src,                                                 //p_src
			p_dst,                                                 //p_dst
			src_original_size                                      //src_original_size
		);

	}
	else if (right_count == 1) {//successfully isolated one number, and found the index of the number

		((char*)p_dst)[next_src_right_offset % src_original_size] = ((char*)p_src)[next_src_right_offset];

	}

	return;//reached max number of bits or all numbers have been isolated from the work group
}


int main(void) {

	uint32_t src[SRC_LENGTH * 2] = {//double size
		//153, 224, 189, 23, 7  //OK
		//1, 8, 2, 184, 81  //OK
		//255, 165, 84, 132, 11  //OK
		//7, 49, 113, 165, 98 //OK
		//34, 32, 89, 156, 34 //OK
		//1, 1, 32, 123, 79//OK
		1, 32, 98, 123, 123//OK
	};

	uint32_t dst[SRC_LENGTH] = { 0 };
	
	rainingWorkGroup(
		4,//memory_type_size_bytes
		0,//is_signed
		31,//start_bit
		SRC_LENGTH, 
		SRC_LENGTH * ITEM_SIZE, 
		SRC_LENGTH * ITEM_SIZE,                 //next_src_left_offset
		SRC_LENGTH * ITEM_SIZE * 2 - ITEM_SIZE, //next_src_right_offset
		0, 
		src,
		dst,
		SRC_LENGTH * ITEM_SIZE
	);

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus