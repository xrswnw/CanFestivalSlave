#include "AnyID_Boot_SysCfg.h"
#include "AnyID_Boot_R485.h"

int main(void)
{
    Sys_Init();
    while(1)
    {
        Sys_LedTask();
        Sys_R485Task();
        Sys_BootTask();
        //Sys_CanTask();
        
    }
}


#ifdef  DEBUG
void assert_failed(u8* file, u32 line)
{
    while(1)
    {
    }
}
#endif
