#ifndef _ANYID_CANFESTIVAL_TIM_HL_H
#define _ANYID_CANFESTIVAL_TIM_HL_H

#include "AnyID_CanFestival_Config.h"
#include "AnyID_CanFestival_SysTick_HL.h"




#define TIM_PORT					TIM1
#define TIM_PSE_CNT			        0xFFFF - 1				//Ö÷Æµ72Mhz
#define TIM_PSE_SYS_FREQ			120 - 1//(SYS_CLOCK_FREQ / 1000000) - 1				//Ö÷Æµ72Mhz

	
void setTimer(TIMEVAL value);
void Tim_InitInterface();
TIMEVAL getElapsedTime(void);


#endif 

