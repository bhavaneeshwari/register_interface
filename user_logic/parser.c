#include <string.h>
#include <stdlib.h>
#include "xil_printf.h"
#include "../axi_regs.h"
#include "parser.h"
#include "func_desc.h"
#include "mapper.h"

void parse_and_store(char *input)
{
    if (READ_CMD() != 0) {
        xil_printf("[PARSER] HW busy\r\n");
        return;
    }

    /* clear operand registers */
    for (int i = 0; i < 32; i++) HW_OPERAND_BASE[i] = 0x00;

    /* STEP 1 — extract cmd name */
    char *cmd = strtok(input, "(");
    if (!cmd) return;

    /* STEP 2 — trim trailing whitespace/newline from cmd */
    char *end = cmd + strlen(cmd) - 1;
    while (end > cmd && (*end == ' '  ||
                         *end == '\r' ||
                         *end == '\n' ||
                         *end == '\t')) {
        *end = '\0';
        end--;
    }

    /* STEP 3 — map cmd string to opcode */
    opcode_t opcode = OPCODE_INVALID;
    for (size_t i = 0; i < FUNC_TABLE_LEN; i++) {
        if (strcmp(FUNC_TABLE[i].cmd_name, cmd) == 0) {
            opcode = FUNC_TABLE[i].opcode;
            break;
        }
    }

    if (opcode == OPCODE_INVALID) {
        xil_printf("[PARSER] unknown cmd: '%s'\r\n", cmd);
        WRITE_OPCODE(OPCODE_INVALID);
        WRITE_STATUS(TI_AFE_RET_EXEC_FAIL);
        WRITE_CMD(0);
        return;
    }

    /* STEP 4 — lookup descriptor by opcode */
    const func_desc_t *desc = lookup_by_opcode(opcode);
    if (!desc) {
        xil_printf("[PARSER] no descriptor for opcode: 0x%02X\r\n", opcode);
        WRITE_OPCODE(OPCODE_INVALID);
        WRITE_STATUS(TI_AFE_RET_EXEC_FAIL);
        WRITE_CMD(0);
        return;
    }

    /* STEP 5 — write opcode to HW */
    WRITE_OPCODE(desc->opcode);
    xil_printf("[PARSER] cmd=%s opcode=0x%02X\r\n", desc->cmd_name, desc->opcode);

    /* STEP 6 — extract arg string */
    char *arg_string = strtok(NULL, ")");
    if (!arg_string) {
        xil_printf("[PARSER] no args found\r\n");
        WRITE_STATUS(TI_AFE_RET_EXEC_FAIL);
        WRITE_CMD(0);
        return;
    }

    /* STEP 7 — parse comma separated args into flat array */
    uint32_t args[MAX_ARG_TYPES] = {0};
    int argc = 0;

    char *token = strtok(arg_string, ",");
    while (token != NULL && argc < MAX_ARG_TYPES) {
        /* trim leading whitespace */
        while (*token == ' ' || *token == '\t') token++;
        args[argc] = (uint32_t)strtoul(token, NULL, 0);
        argc++;
        token = strtok(NULL, ",");
    }

    /* STEP 8 — validate min arg count against descriptor */
    if (argc < desc->min_args) {
        xil_printf("[PARSER] too few args: got %d expected %d\r\n",
                   argc, desc->min_args);
        WRITE_STATUS(TI_AFE_RET_EXEC_FAIL);
        WRITE_CMD(0);
        return;
    }

    /* STEP 9 — log all parsed args */
    for (int i = 0; i < argc; i++) {
        xil_printf("[PARSER] arg[%d] = 0x%08X\r\n", i, args[i]);
    }

    /* STEP 10 — dispatch mapper handler */
    desc->handler(args, argc);

    /* STEP 11 — signal HW */
    WRITE_CMD(1);
    xil_printf("[PARSER] done\r\n");
}
