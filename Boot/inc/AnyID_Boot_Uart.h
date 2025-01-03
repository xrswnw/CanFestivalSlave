#ifndef _ANYID_BOOT_UART_
#define _ANYID_BOOT_UART_

#include "AnyID_Boot_Config.h"
#include "AnyID_Uart_Receive.h"

typedef struct uartTxFrame{
    u16 index;
    u16 len;
    u8 frame[UART_BUFFER_MAX_LEN];
}UART_TXFRAME;

extern UART_TXFRAME g_sUartTxFrame;
extern UART_RCVFRAME g_sUartRcvFrame;


#define UART_FRAME_FLAG_OK              0x00
#define UART_FRAME_FLAG_FAIL            0x01
#define UART_FRAME_RSP_NOERR            0x00
#define UART_FRAME_RSP_TAGERR           0x01
#define UART_FRAME_RSP_CRCERR           0x02
#define UART_FRAME_RSP_NORSP            0x03

/*
#define UART_FRAME_CMD_BOOT           0x801
#define UART_FRAME_CMD_DL             0x802
#define UART_FRAME_CMD_JMP            0x803
#define UART_FRAME_CMD_RESET          0x804
#define UART_FRAME_CMD_ERASE          0x805
#define UART_FRAME_CMD_UPDATA_CTR	  0x806
#define UART_FRAME_CMD_RCVINFO  	  0x807
#define UART_FRAME_CMD_TVER			  0x8E2
#define UART_FRAME_CMD_SVER			  0x8E3
#define UART_FRAME_CMD_HVER			  0x8E4
#define UART_FRAME_CMD_VER            0x8F7
*/

#define UART_FRAME_CMD_BOOT           0x01
#define UART_FRAME_CMD_DL             0x02
#define UART_FRAME_CMD_JMP            0x03
#define UART_FRAME_CMD_RESET          0x04
#define UART_FRAME_CMD_ERASE          0x05
#define UART_FRAME_CMD_UPDATA_CTR	  0x06
#define UART_FRAME_CMD_RCVINFO  	  0x07
#define UART_FRAME_CMD_TVER			  0xE2
#define UART_FRAME_CMD_SVER			  0xE3
#define UART_FRAME_CMD_HVER			  0xE4
#define UART_FRAME_CMD_VER            0xF7

#define UART_FRAME_RESPONSE_FLAG      0x1F

#define UART_FRAME_OP_OK              0
#define UART_FRAME_OP_FAIL            1

u16 Uart_CalCrc(u8 *pFrame, u16 len);
u32 Uart_RspBoot(void);
u32 Uart_RspJmp(void);
u32 Uart_RspReset(void);
u32 Uart_RspDownload(BOOL bCheck, u32 addr, u32 size);
BOOL BL_WriteImagePage(u32 addr, u8 *pBuffer, u32 size);
BOOL BL_ReadImagePage(u32 addr, u8 *pBuffer, u32 size);

BOOL Uart_EraseFlash(u32 addr);
u32 Uart_RspErase(BOOL b);
u32 Uart_RspFrame(u8 *pBuffer, u8 cmd, u8 *pParams, u8 paramsLen, u8 flag, u8 err);
#endif
