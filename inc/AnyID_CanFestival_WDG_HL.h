#ifndef _ANYID_CANFESTIVAL_WDG_HL_
#define _ANYID_CANFESTIVAL_WDG_HL_

#include "AnyID_CanFestival_Config.h"


void WDG_InitIWDG(void);


#if SYS_ENABLE_WDT
    #define WDG_FeedIWDog()	IWDG_ReloadCounter()
#else
    #define WDG_FeedIWDog()	
#endif


#endif

