#ifndef RAINING_THREADS_H
#define RAINING_THREADS_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <stdint.h>
#include <stdio.h>



#define RAINING_TRUE  1
#define RAINING_FALSE 0



#define rainingError(condition, error_msg, failure_expression)\
	if ((int)(condition)) {\
		printf("raining error: %s\n", error_msg);\
		failure_expression;\
	}



typedef enum RainMemoryFormat {
	RAIN_INT_MEMORY_FORMAT_MAGNITUDE_COMPLEMENT = 1 << 0,//supported
	RAIN_INT_MEMORY_FORMAT_ONE_COMPLEMENT       = 1 << 1,//supported, different segregation
	RAIN_INT_MEMORY_FORMAT_TWO_COMPLEMENT       = 1 << 2,//supported, different segregation
	RAIN_INT_UNSUPPORTED_MEMORY_FORMAT          = 1 << 3,
	RAIN_FLOAT_MEMORY_FORMAT_IEEE_754           = 1 << 15,//supported
	RAIN_FLOAT_UNSUPPORTED_MEMORY_FORMAT        = 1 << 16
} RainMemoryFormat;

typedef enum RainMemoryType {
	RAIN_MEMORY_TYPE_INT   = 0,//integer of any size (up to 64 bits)
	RAIN_MEMORY_TYPE_FLOAT = 1 //float of single and double precision
} RainMemoryType;

typedef struct RainHost {
	uint32_t         item_size_bytes;
	uint32_t         src_length;
	uint8_t          is_type_signed;
	RainMemoryFormat supported_memory_formats;
	RainMemoryType   queried_memory_type;
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
} RainingWorkGroupInfo;



extern uint8_t rainingGetSupportedMemoryFormats(
	RainHost* p_host
);

extern uint8_t rainingHostInit(
	uint32_t       item_size_bytes,
	uint32_t       src_length,
	uint8_t        is_type_signed,
	RainMemoryType queried_memory_type,
	RainHost*      p_host
);

extern uint64_t rainingWorkGroup(
	RainingWorkGroupInfo* p_info
);

#define RAINING_GET_NEGATIVE_STORAGE_SYSTEM 

extern uint32_t rainingUnit(
	uint32_t src_length,
	uint32_t src_size,
	uint32_t src_offset,
	void*    p_src,
	uint32_t next_src_left_offset,
	uint32_t next_src_right_offset,
	uint32_t bit_idx
);



typedef enum RainSegregateInstruction {
	RAIN_SEGREGATE_INSTRUCTION_COPY_NEGATIVE         = 0,
	RAIN_SEGREGATE_INSTRUCTION_INVERSE_COPY_NEGATIVE = 1,
	RAIN_SEGREGATE_INSTRUCTION_MAX_ENUM
} RainSegregateInstruction;



extern uint8_t rainingSegregate(
	uint32_t                 sign_bit,
	uint32_t                 item_size,
	void*                    p_src,
	uint32_t                 dst_size,
	void*                    p_dst,
	RainSegregateInstruction instruction
);

extern uint8_t rainingHostSubmit(
	RainHost* p_host,
	void*     p_src,
	void*     p_dst
);

extern uint8_t rainingHostRelease(
	RainHost* p_host
);



#ifdef __cplusplus
}
#endif//_cplusplus

#endif//RAINING_THREADS_H