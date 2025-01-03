#ifndef _ANYID_BOOT_SYSCFG_
#define _ANYID_BOOT_SYSCFG_


#include "AnyID_Boot_Config.h"
#include "AnyID_Boot_Uart.h"
#include "AnyID_Boot_Can_HL.h"
#include "AnyID_Boot_R485.h"
#include "AnyID_Boot_UART.h"
#include "AnyID_Boot_Fram.h"
#include "AnyID_Boot_SysTick_HL.h"
#include "AnyID_Boot_FRam.h"


extern u32 g_nSysState;
#define SYS_STAT_IDLE           0x00000001	    //系统初始化后处于空闲状态
#define SYS_STAT_RUNLED         0x00000002
#define SYS_STAT_DOWNLOAD       0x00000040
#define SYS_STAT_JMP            0x00000080
#define SYS_STAT_RESET          0x00000100
#define SYS_STAT_ALARMLED       0x00000200

void Sys_Jump(u32 address);
typedef  void (*pSysFunction)(void);
//灯
extern const PORT_INF SYS_RUNLED_COM;
#define Sys_RunLedOff()             SYS_RUNLED_COM.Port->BSRR = SYS_RUNLED_COM.Pin
#define Sys_RunLedOn()              SYS_RUNLED_COM.Port->BRR = SYS_RUNLED_COM.Pin

//extern const PORT_INF SYS_ALARMBUZZER_COM;
//#define Sys_AlarmBuzzerOff()        SYS_ALARMBUZZER_COM.Port->BRR = SYS_ALARMBUZZER_COM.Pin
//#define Sys_AlarmBuzzerOn()         SYS_ALARMBUZZER_COM.Port->BSRR = SYS_ALARMBUZZER_COM.Pin

extern const PORT_INF SYS_ALARMLED_COM;
#define Sys_AlarmLedOn()        SYS_ALARMLED_COM.Port->BRR = SYS_ALARMLED_COM.Pin
#define Sys_AlarmLedOff()       SYS_ALARMLED_COM.Port->BSRR = SYS_ALARMLED_COM.Pin



#define SYS_FRAME_CMD_WRITE_PARAMS			0xF5


#define SYS_BOOT_DOWNLOAD_FRAME_MAX_LEN		1024
#define SYS_BOOT_DOWNLOAD_FRAME_LEN			8	
#define SYS_BOOT_DOWNLOAD_START_CTR			0x00000000
#define SYS_BOOT_DOWNLOAD_END_CTR			0xFFFFFFFF


#define SYS_BOOT_DOWNLOAD_FLAG_STRAT		0x01
#define SYS_BOOT_DOWNLOAD_FLAG_OVER		    0x04
typedef struct bootInfo{
	u8 flag;
	u8 frame[SYS_BOOT_DOWNLOAD_FRAME_MAX_LEN];
	u16 index;
	u16 crc;
	u32 addr;
}BOOT_INFO;
extern BOOT_INFO g_sBootInfo;

void Sys_Delay5ms(u32 n);

void Sys_CfgClock(void);
void Sys_CfgPeriphClk(FunctionalState state);
void Sys_CfgNVIC(void);

#define Sys_SoftReset()     (*((u32 *)0xE000ED0C) = 0x05fa0004)

void Sys_CtrlIOInit(void);
void Sys_Init(void);

void Sys_UartTask(void);
void Sys_ProcessUartFrame(u8 *pBuffer, u16 len);
void Sys_LedTask(void);
//void Sys_UartTask(void);
void Sys_R485Task(void);
void Sys_BootTask(void);
void Sys_CanTask();
u8 Sys_ProcessBootFrame(UART_RCVFRAME *pRcvFrame, u8 com);
u16 Sys_GetBudPsc(u8 bud);
#define SYS_VER_HEAD                            "CanFestival "


#define SYS_VER_HEAD_SIZE                       7

#define SYS_VER_TYPE_SIZE                       4
#define SYS_VER_SOFT_SIZE                       8
#define SYS_VER_HARD_SIZE                       8

#define SYS_VERSION_SIZE                        50  
#define SYS_BOOT_VER_ADDR                       0x08005000          //版本固定在当前系统
#define SYS_APP_START_ADDR                      0x08004000

#define SYS_FRAME_BROADCAST_ADDR                0xFFFF
BOOL Sys_CheckAppEmpty(void);
BOOL Sys_CheckVersion(void);
void Sys_CanProcessFrame(CAN_FRAME *pFrame);
#define SYS_COM_R485        0
#define SYS_COM_UART        1
#define SYS_COM_USB         2
#endif
