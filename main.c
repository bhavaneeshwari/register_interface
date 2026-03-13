#include <stdio.h>
#include <string.h>
#include "xil_printf.h"
#include "axi_regs.h"
#include "user_logic/uart.h"
#include "user_logic/parser.h"
#include "user_logic/mapper.h"
#include "core_logic/executor.h"
#include "core_logic/api.h"
#include "core_logic/afe_drivers.h"

#define CMD_BUFFER_SIZE 128

/* Little Endian HW Extraction Macro */
#define GET_HW_U16(base, offset) (uint16_t)( \
    (base[offset] & 0xFF) |                  \
    ((base[offset + 1] & 0xFF) << 8)         \
)

void print_help()
{
    xil_printf("\r\n--- Command Input Formats (Hex) ---\r\n");
    xil_printf("1. Raw Write:   spiRawWrite(Inst, Addr, Data)\r\n");
    xil_printf("2. Raw Read:    spiRawRead(Inst, Addr)\r\n");
    xil_printf("3. Burst Write: spiBurstWrite(Inst, Addr, Size, D0, D1...)\r\n");
    xil_printf("4. Burst Read:  spiBurstRead(Inst, Addr, Size)\r\n");
    xil_printf("5. Multi Write: spiRawWriteMulti(Sel, Addr, Data)\r\n");
    xil_printf("6. Multi Read:  spiRawReadMulti(Sel, Addr)\r\n");
    xil_printf("7. Multi Burst: spiBurstWriteMulti(Sel, Addr, Size, D0, D1...)\r\n");
    xil_printf("-----------------------------------\r\n");
}

int main()
{
    uart_init();
    print_help();

    char cmd_buffer[CMD_BUFFER_SIZE];

    while (1)
    {
        xil_printf("\r\nCMD> ");
        uart_getline(cmd_buffer, CMD_BUFFER_SIZE);

        if (strlen(cmd_buffer) == 0) continue;

        /* STEP 1 — parse input, map args to operand regs, dispatch mapper */
        parse_and_store(cmd_buffer);

        /* STEP 2 — executor reads opcode + operands, calls driver, writes result */
        executor_poll();

        /* STEP 3 — read back status and opcode */
        u16 status = READ_STATUS();
        u8  opcode = READ_OPCODE();

        /* STEP 4 — handle result */
        if (status == TI_AFE_RET_EXEC_PASS)
        {
            xil_printf("[MAIN] SUCCESS: Opcode 0x%02X\r\n", opcode);

            if (opcode == OPCODE_RAW_READ)
            {
                xil_printf("   -> Result: 0x%02X\r\n", HW_RESULT_BASE[0]);
            }
            else if (opcode == OPCODE_RAW_READ_MULTI)
            {
                for (int i = 0; i < NUM_SPI; i++)
                    xil_printf("      SPI[%d]: 0x%02X\r\n", i, HW_RESULT_BASE[i]);
            }
            else if (opcode == OPCODE_BURST_READ)
            {
                uint16_t size = GET_HW_U16(HW_OPERAND_BASE, 3);
                xil_printf("   -> Burst Data:\r\n");
                for (uint16_t i = 0; i < size; i++)
                    xil_printf("      [%d]: 0x%02X\r\n", i, HW_RESULT_BASE[i]);
            }
        }
        else if (opcode != OPCODE_INVALID)
        {
            xil_printf("[MAIN] ERROR: Failed (0x%04X)\r\n", status);
        }
        else
        {
            xil_printf("[MAIN] ERROR: Unknown Command Syntax.\r\n");
            print_help();
        }
    }

    return 0;
}
