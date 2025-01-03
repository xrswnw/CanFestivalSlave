#ifndef _ANYID_CANFESTIVAL_SYSTICK_HL_H
#define _ANYID_CANFESTIVAL_SYSTICK_HL_H


#include "AnyID_CanFestival_Config.h"
#include "slaveNormal.h"


//extern vu32 g_nSysTick;
extern vs32 g_nSysDelay;

#define STICK_1MS                           120000                    //72M的系统时钟



void STick_InitSysTick(void);

#define STick_StartSysTick()                SysTick->CTRL  |= SysTick_CTRL_ENABLE_Msk
#define STick_StopSysTick()                 SysTick->VAL = 0;  SysTick->CTRL  &= (~SysTick_CTRL_ENABLE_Msk)

#define STick_SetDelayTime(delay)           (g_nSysDelay = delay) //以一个Tick为单位
#define STick_IsDelayTimeOver()             (g_nSysDelay <= 0)



#define STICK_TIME_MS                       5

#define STICK_1S                            (1000 / STICK_TIME_MS)


#endif

