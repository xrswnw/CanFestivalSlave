#ifndef _ANYID_CANFESTIVAL_SYSCFG_H
#define _ANYID_CANFESTIVAL_SYSCFG_H


#include "AnyID_CanFestival_RTC.h"
#include "AnyID_CanFestival_WDG_HL.h"
#include "AnyID_CanFestival_Tim_HL.h"
#include "AnyID_CanFestival_Can_HL.h"
#include "AnyID_CanFestival_RS485.h"
#include "AnyID_CanFestival_Periph_HL.h"
#include "AnyID_CanFestival_Device.h"

extern u32 g_nSysState;
#define SYS_NULL_TICK                   0

#define SYS_STAT_IDLE                   0x00000001	        //
#define SYS_STAT_RUNLED                 0x00000002
#define SYS_STAT_ALARMLED               0x00000004
#define SYS_STAT_ALARMDLY				0x00000008
#define SYS_STAT_ALARMOFF				0x00000010
#define SYS_STAT_CAN_ERR				0x00000020

#define SYS_STAT_COM_RS485              0x00000040
#define SYS_STAT_COM_CAN                0x00000080
#define SYS_STAT_RS485_TX               0x00000100
#define SYS_STAT_COM_CHK                0x00000200
//≤‚ ‘
#define SYS_STAT_FIRE_TEST               0x80000000
#define SYS_STAT_SOUND_TEST              0x40000000
#define SYS_STAT_LTE_TEST                0x20000000
#define SYS_STAT_GATE_TEST               0x10000000


#define SYS_LED_DELAY                   20
#define SYS_CHECK_CFGMODE_TIM           200

#define Sys_ComTo485()              do{a_SetStateBit(g_nSysState, SYS_STAT_COM_RS485 | SYS_STAT_COM_CHK);a_ClearStateBit(g_nSysState, SYS_STAT_COM_CAN | SYS_STAT_COM_CHK);}while(0)
#define Sys_ComToCan()              do{a_SetStateBit(g_nSysState, SYS_STAT_COM_CAN);a_ClearStateBit(g_nSysState, SYS_STAT_COM_RS485 | SYS_STAT_COM_CHK);}while(0)
void Sys_Delayms(u32 n);
void Sys_CfgClock(void);
void Sys_CfgPeriphClk(FunctionalState state);
void Sys_CfgNVIC(void);

void Sys_CtrlIOInit(void);
void Sys_Rs485Task(void);
void Sys_Init(void);
void Sys_LedTask(void);
void Sys_UartTask(void);
void Sys_DeviceTask();
void Sys_CanTsak();
#endif
