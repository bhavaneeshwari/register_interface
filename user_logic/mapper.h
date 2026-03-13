/*
 * mapper.h
 *
 *  Created on: Mar 13, 2026
 *      Author: vlsiuser
 */
#ifndef SRC_USER_LOGIC_MAPPER_H_
#define SRC_USER_LOGIC_MAPPER_H_

#include <stdint.h>
#include "xil_types.h"
#include "../axi_regs.h"
#include "func_desc.h"

#ifndef NUM_SPI
#define NUM_SPI      4
#endif

void map_spiRawWrite       (uint32_t *args, int argc);
void map_spiRawRead        (uint32_t *args, int argc);
void map_spiBurstWrite     (uint32_t *args, int argc);
void map_spiBurstRead      (uint32_t *args, int argc);
void map_spiRawWriteMulti  (uint32_t *args, int argc);
void map_spiRawReadMulti   (uint32_t *args, int argc);
void map_spiBurstWriteMulti(uint32_t *args, int argc);

#endif /* SRC_USER_LOGIC_MAPPER_H_ */
