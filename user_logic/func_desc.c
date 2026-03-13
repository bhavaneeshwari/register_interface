#include <string.h>
#include "func_desc.h"
#include "mapper.h"

func_desc_t FUNC_TABLE[] = {
    { "spiRawWrite",         OPCODE_RAW_WRITE,         3, map_spiRawWrite        },
    { "spiRawRead",          OPCODE_RAW_READ,          2, map_spiRawRead         },
    { "spiBurstWrite",       OPCODE_BURST_WRITE,       3, map_spiBurstWrite      },
    { "spiBurstRead",        OPCODE_BURST_READ,        3, map_spiBurstRead       },
    { "spiRawWriteMulti",    OPCODE_RAW_WRITE_MULTI,   3, map_spiRawWriteMulti   },
    { "spiRawReadMulti",     OPCODE_RAW_READ_MULTI,    2, map_spiRawReadMulti    },
    { "spiBurstWriteMulti",  OPCODE_BURST_WRITE_MULTI, 3, map_spiBurstWriteMulti },
};

const size_t FUNC_TABLE_LEN = sizeof(FUNC_TABLE) / sizeof(FUNC_TABLE[0]);

const func_desc_t *lookup_by_opcode(opcode_t opcode)
{
    for (size_t i = 0; i < FUNC_TABLE_LEN; i++) {
        if (FUNC_TABLE[i].opcode == opcode)
            return &FUNC_TABLE[i];
    }
    return NULL;
}
