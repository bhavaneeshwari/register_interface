/*
 * mapper.c
 *
 *  Created on: Mar 13, 2026
 *      Author: vlsiuser
 */

#include <string.h>
#include "mapper.h"
#include "func_desc.h"
#include "../axi_regs.h"

#define OPERAND_PTR  ((u8 *)(uintptr_t)HW_OPERAND_BASE)

void map_spiRawWrite(uint32_t *args, int argc)
{
    if (argc < 3) return;

    u8  afeInst = (u8) args[0];
    u16 addr    = (u16)args[1];
    u8  data    = (u8) args[2];

    u32 offset = 0;
    memcpy(OPERAND_PTR + offset, &afeInst, sizeof(afeInst)); offset += sizeof(afeInst);
    memcpy(OPERAND_PTR + offset, &addr,    sizeof(addr));    offset += sizeof(addr);
    memcpy(OPERAND_PTR + offset, &data,    sizeof(data));    offset += sizeof(data);
}


void map_spiRawRead(uint32_t *args, int argc)
{
    if (argc < 2) return;

    u8  afeInst = (u8) args[0];
    u16 addr    = (u16)args[1];

    u32 offset = 0;
    memcpy(OPERAND_PTR + offset, &afeInst, sizeof(afeInst)); offset += sizeof(afeInst);
    memcpy(OPERAND_PTR + offset, &addr,    sizeof(addr));    offset += sizeof(addr);
}


void map_spiBurstWrite(uint32_t *args, int argc)
{
    if (argc < 3) return;

    u8  afeInst = (u8) args[0];
    u16 addr    = (u16)args[1];
    u16 size    = (u16)args[2];

    u32 offset = 0;
    memcpy(OPERAND_PTR + offset, &afeInst, sizeof(afeInst)); offset += sizeof(afeInst);
    memcpy(OPERAND_PTR + offset, &addr,    sizeof(addr));    offset += sizeof(addr);
    memcpy(OPERAND_PTR + offset, &size,    sizeof(size));    offset += sizeof(size);

    for (int i = 0; i < size && (3 + i) < argc; i++) {
        u8 byte = (u8)args[3 + i];
        memcpy(OPERAND_PTR + offset, &byte, sizeof(byte));
        offset += sizeof(byte);
    }
}


void map_spiBurstRead(uint32_t *args, int argc)
{
    if (argc < 3) return;

    u8  afeInst = (u8) args[0];
    u16 addr    = (u16)args[1];
    u16 size    = (u16)args[2];

    u32 offset = 0;
    memcpy(OPERAND_PTR + offset, &afeInst, sizeof(afeInst)); offset += sizeof(afeInst);
    memcpy(OPERAND_PTR + offset, &addr,    sizeof(addr));    offset += sizeof(addr);
    memcpy(OPERAND_PTR + offset, &size,    sizeof(size));    offset += sizeof(size);
}


void map_spiRawWriteMulti(uint32_t *args, int argc)
{
    if (argc < 3) return;

    u8  sel  = (u8) args[0];
    u16 addr = (u16)args[1];
    u8  data = (u8) args[2];

    u32 offset = 0;
    memcpy(OPERAND_PTR + offset, &sel,  sizeof(sel));  offset += sizeof(sel);
    memcpy(OPERAND_PTR + offset, &addr, sizeof(addr)); offset += sizeof(addr);
    memcpy(OPERAND_PTR + offset, &data, sizeof(data)); offset += sizeof(data);
}

void map_spiRawReadMulti(uint32_t *args, int argc)
{
    if (argc < 2) return;

    u8  sel  = (u8) args[0];
    u16 addr = (u16)args[1];

    u32 offset = 0;
    memcpy(OPERAND_PTR + offset, &sel,  sizeof(sel));  offset += sizeof(sel);
    memcpy(OPERAND_PTR + offset, &addr, sizeof(addr)); offset += sizeof(addr);
}


void map_spiBurstWriteMulti(uint32_t *args, int argc)
{
    if (argc < 3) return;

    u8  sel  = (u8) args[0];
    u16 addr = (u16)args[1];
    u16 size = (u16)args[2];

    u32 offset = 0;
    memcpy(OPERAND_PTR + offset, &sel,  sizeof(sel));  offset += sizeof(sel);
    memcpy(OPERAND_PTR + offset, &addr, sizeof(addr)); offset += sizeof(addr);
    memcpy(OPERAND_PTR + offset, &size, sizeof(size)); offset += sizeof(size);

    for (int i = 0; i < size && (3 + i) < argc; i++) {
        u8 byte = (u8)args[3 + i];
        memcpy(OPERAND_PTR + offset, &byte, sizeof(byte));
        offset += sizeof(byte);
    }
}
