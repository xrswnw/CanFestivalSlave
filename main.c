#include "AnyID_CanFestival_SysCfg.h"

int main(void)
{
    Sys_Init();
    while(1)
    {
        Sys_LedTask();
		Sys_DeviceTask();
		Sys_Rs485Task();
		Sys_CanTsak();
    }
}


