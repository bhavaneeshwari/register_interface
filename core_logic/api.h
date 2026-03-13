#ifndef API_H
#define API_H

#include "xil_types.h"

typedef u16 (*api_func_ptr)(volatile u8 *operands);

typedef enum {
    OPCODE_RAW_WRITE         = 0x00,
    OPCODE_RAW_READ          = 0x01,
    OPCODE_BURST_WRITE       = 0x02,
    OPCODE_BURST_READ        = 0x03,
    OPCODE_RAW_WRITE_MULTI   = 0x04,
    OPCODE_RAW_READ_MULTI    = 0x05,
    OPCODE_BURST_WRITE_MULTI = 0x06,
    API_TABLE_SIZE           = 0x07,
    OPCODE_INVALID           = 0xFF,

} opcode_t;
extern api_func_ptr api_table[API_TABLE_SIZE];

#endif
