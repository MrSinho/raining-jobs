#ifdef __cplusplus
extern "C" {
#endif//_cplusplus

#include <raining-threads/rainingThreads.h>

#include <math.h>
#include <stdlib.h>


#define SRC_LENGTH ((int)5)
#define ITEM_SIZE  sizeof(uint8_t)



void unit(
	uint32_t  start_bit, 
	uint32_t  src_length, 
	uint32_t  src_size, 
	uint32_t  next_src_left_offset,
	uint32_t  next_src_right_offset,
	uint32_t  src_offset, 
	uint8_t*  src,
	uint8_t*  dst,
	uint32_t  src_original_size
) {
	//already checked with left_count and right_count
	//if (src_length <= 1) {
	//	return;
	//}

	uint8_t* p_a0 = &src[0];
	uint8_t* p_a1 = &src[1];
	uint8_t* p_a2 = &src[2];
	uint8_t* p_a3 = &src[3];
	uint8_t* p_a4 = &src[4];
	uint8_t* p_a5 = &src[5];
	uint8_t* p_a6 = &src[6];
	uint8_t* p_a7 = &src[7];
	uint8_t* p_a8 = &src[8];
	uint8_t* p_a9 = &src[9];

	uint32_t left_count  = 0;
	uint32_t right_count = 0;
	uint32_t item_size   = src_size / src_length;

	uint8_t* p_d0 = &dst[0];
	uint8_t* p_d1 = &dst[1];
	uint8_t* p_d2 = &dst[2];
	uint8_t* p_d3 = &dst[3];
	uint8_t* p_d4 = &dst[4];

	//R0
	//
	//
	left_count = rainingRaining(//main
		src_length,                                    //src_length OK
		src_size,                                      //src_size   OK
		src_offset,                                    //src_offset OK
		src,                                           //p_src   OK
		next_src_left_offset,                          //next_src_left_offset  OK
		next_src_right_offset,                         //next_src_right_offset OK
		start_bit                                      //bit_idx OK
	);

	if (start_bit == 0) {//fine
		return;
	}

	right_count = src_length - left_count;


	uint32_t current_bit      = start_bit   - 1;
	//uint32_t next_bit         = current_bit - 1;

	//uint32_t left_left_count  = 0;
	//uint32_t right_left_count = 0;

	if (left_count > 1) {//need to discriminate between at least two numbers, continue, invert dst and src
		//R1
		//
		//
		//left_left_count = rainingRaining(//left, so painful to debug
		//	left_count,                         //src_length OK
		//	left_count * item_size,             //src_size   OK
		//	src_size,                           //src_offset OK
		//	src,                                //p_src   OK
		//	0,                                  //left_dst_item_offset OK
		//	left_count * item_size - item_size, //right_dst_item_offset OK
		//	current_bit                         //bit_idx OK
		//);
		//
		//unit(
		//	next_bit, 
		//	left_left_count, 
		//	left_left_count * item_size, 
		//	0,
		//	src
		//);

		unit(//restrict total size to a local one, set new offset
			current_bit,            //start_bit
			left_count,             //src_length
			left_count * item_size, //src_size
			src_offset,             //next_src_left_offset //overwrite
			left_count * item_size - item_size, //next_src_right_offset //overwrite
			next_src_left_offset,   //src_offset
			src,                    //src
			dst,                     //dst
			src_original_size
		);
	}
	else if (left_count == 1) {//successfully isolated one number, and found the index of the number
		((char*)dst)[next_src_left_offset % src_original_size] = ((char*)src)[next_src_left_offset];
	}

	if (right_count > 1) {//need to discriminate between at least two numbers, continue, invert dst and src
		//R2
		//
		//
		//right_left_count = rainingRaining(//right, so painful to debug
		//	right_count,                            //src_length OK
		//	right_count * item_size,                //src_size   OK
		//	src_size * 2 - right_count * item_size, //src_offset
		//	src,                                    //p_src   OK
		//	src_size - right_count * item_size,     //left_dst_item_offset OK
		//	src_size - item_size,                   //right_dst_item_offset OK
		//	current_bit                             //bit_idx OK
		//);
		//
		//unit(
		//	next_bit,
		//	right_left_count,
		//	right_left_count * item_size,
		//	src_size - right_count * item_size,
		//	src
		//);

		unit(
			current_bit,//start_bit
			right_count,//src_length
			right_count * item_size,//src_size
			src_offset + left_count * item_size,//next_src_left_offset //overwrite
			src_offset + src_size - item_size,//next_src_right_offset //overwrite
			next_src_right_offset - item_size * (right_count - 1),//src_offset
			src,//src
			dst,//dst
			src_original_size
		);
	}
	else if (right_count == 1) {//successfully isolated one number, and found the index of the number
		((char*)dst)[next_src_right_offset % src_original_size] = ((char*)src)[next_src_right_offset];
	}

	//check if in order, otherwise repeat loop with next bit

	return;
}


int main(void) {
	
	                       //n bits
	

	//uint32_t* p_src = calloc(SRC_LENGTH, 4);
	//uint32_t* p_dst = calloc(SRC_LENGTH, 4);
	//
	//for (uint32_t i = 0; i < SRC_LENGTH; i++) {
	//	float v = (float)sin(i) + 
	//	         1 / (float)cos(i) +
	//		     (float)(i % 16) / 
	//	         (float)pow(10, (float)(i % 24));
	//	p_src[i] = (uint32_t)(v * 1000.0f);
	//}
	//
	//for (uint32_t i = 0; i < SRC_LENGTH; i++) {
	//	printf("%i: %i\n", i, p_src[i]);
	//}

	uint8_t src[SRC_LENGTH * 2] = {//double size
		//153, 224, 189, 23, 7  //OK
		//1, 8, 2, 184, 81  //OK
		//255, 165, 84, 132, 11  //OK
		 7, 49, 113, 165, 98 //NOT OK
	};

	uint8_t dst[SRC_LENGTH] = { 0 };
	
	//src becomes dst
	//dst becomes src
	//
	unit(
		7, 
		SRC_LENGTH, 
		SRC_LENGTH * ITEM_SIZE, 
		SRC_LENGTH * ITEM_SIZE,                 //next_src_left_offset
		SRC_LENGTH * ITEM_SIZE * 2 - ITEM_SIZE, //next_src_right_offset
		0, 
		src,
		dst,
		SRC_LENGTH * ITEM_SIZE
	);

	//WITH ENDLESS MEMORY
	//
	//
#if 0
	uint32_t start_bit = 7;

	uint32_t left_count  = 0;
	uint32_t right_count = 0;

	left_count = rainingRaining(//main
		SRC_LENGTH,                                               //src_length OK
		SRC_LENGTH * ITEM_SIZE,                                   //src_size   OK
		SRC_LENGTH * ITEM_SIZE * (8 - start_bit - 1),             //src_offset OK
		src,                                                      //p_src   OK
		SRC_LENGTH * ITEM_SIZE * (8 - start_bit),                 //next_src_left_offset  OK
		SRC_LENGTH * ITEM_SIZE * (8 - start_bit + 1) - ITEM_SIZE, //next_src_right_offset OK
		start_bit//1 << 7                                         //bit_idx OK
	);

	if (start_bit == 0) {//fine
		return;
	}

	right_count = SRC_LENGTH - left_count;

	uint32_t current_bit = start_bit - 1;

	if (left_count > 1) {
		rainingRaining(//left, so painful to debug
			left_count,                                                                      //src_length OK
			left_count * ITEM_SIZE,                                                          //src_size   OK
			SRC_LENGTH * ITEM_SIZE * (8 - current_bit - 1),                                  //src_offset OK
			src,                                                                             //p_src   OK
			SRC_LENGTH * ITEM_SIZE * (8 - current_bit),                                      //left_dst_item_offset OK
			SRC_LENGTH * ITEM_SIZE * (8 - current_bit) + left_count * ITEM_SIZE - ITEM_SIZE, //right_dst_item_offset OK
			current_bit                                                                      //bit_idx OK
		);
	}

	if (right_count > 1) {
		rainingRaining(//right, so painful to debug
			right_count,                                                              //src_length OK
			right_count * ITEM_SIZE,                                                  //src_size   OK
			SRC_LENGTH  * ITEM_SIZE * (8 - current_bit) - right_count * ITEM_SIZE,    //src_offset
			src,                                                                      //p_src   OK
			SRC_LENGTH  * ITEM_SIZE * (8 - current_bit) + left_count * ITEM_SIZE,     //left_dst_item_offset OK
			SRC_LENGTH  * ITEM_SIZE * (8 - current_bit  + 1) - ITEM_SIZE,             //right_dst_item_offset OK
			current_bit                                                               //bit_idx OK
		);
	}
#endif

#if 0
	uint32_t start_bit = 7;

	uint32_t left_count  = 0;
	uint32_t right_count = 0;

	left_count = rainingRaining(//main
		SRC_LENGTH,                         //src_length OK
		SRC_LENGTH * ITEM_SIZE,             //src_size   OK
		0,                                  //src_offset OK
		src,                                //p_src   OK
		SRC_LENGTH * ITEM_SIZE,             //next_src_left_offset  OK
		SRC_LENGTH * ITEM_SIZE * 2 - ITEM_SIZE, //next_src_right_offset OK
		start_bit//1 << 7                   //bit_idx OK
	);

	if (start_bit == 0) {//fine
		return;
	}

	right_count = SRC_LENGTH - left_count;

	uint32_t current_bit = start_bit - 1;

	if (left_count > 1) {//invert dst and src
		rainingRaining(//left, so painful to debug
			left_count,                         //src_length OK
			left_count * ITEM_SIZE,             //src_size   OK
			SRC_LENGTH * ITEM_SIZE,             //src_offset OK
			src,                                //p_src   OK
			0,                                  //left_dst_item_offset OK
			left_count * ITEM_SIZE - ITEM_SIZE, //right_dst_item_offset OK
			current_bit                         //bit_idx OK
		);
	}

	if (right_count > 1) {
		rainingRaining(//right, so painful to debug
			right_count,                                           //src_length OK
			right_count * ITEM_SIZE,                               //src_size   OK
			SRC_LENGTH  * ITEM_SIZE * 2 - right_count * ITEM_SIZE, //src_offset
			src,                                                   //p_src   OK
			SRC_LENGTH  * ITEM_SIZE - right_count * ITEM_SIZE,     //left_dst_item_offset OK
			SRC_LENGTH  * ITEM_SIZE - ITEM_SIZE,                   //right_dst_item_offset OK
			current_bit                                            //bit_idx OK
		);
	}

	//check if in order, otherwise repeat loop with next bit
#endif//0

	//COME TRASFORMARLO IN UN LOOP?


	//RainingHost host = { 0 };
	//
	//rainingHostInit(SET_LENGTH, &host);
	//
	//rainingHostSubmit(&host, SET_LENGTH, src);
	//
	//rainingWaitForHost(&host, NULL, src);

	return 0;
}



#ifdef __cplusplus
}
#endif//_cplusplus