/*
 * func_desc.h
 *
 *  Created on: Mar 13, 2026
 *      Author: vlsiuser
 */



#ifndef FUNC_DESCRIPTOR_H
#define FUNC_DESCRIPTOR_H

#include <stdint.h>
#include "xil_types.h"
#include "parser.h"
#include "../core_logic/api.h"


typedef void (*mapper_fn)(uint32_t *args, int argc);

#define MAX_ARG_TYPES  8

typedef struct {
    const char   *cmd_name;
    opcode_t  opcode;
    int           min_args;
    mapper_fn     handler;
} func_desc_t;
extern func_desc_t FUNC_TABLE[];
extern const size_t FUNC_TABLE_LEN;
const func_desc_t *lookup_by_opcode(opcode_t  opcode);

#endif /* SRC_USER_LOGIC_FUNC_DESC_H_ */
