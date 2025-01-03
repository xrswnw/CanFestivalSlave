#include "AnyID_Boot_SysTick_HL.h"

vu32 g_nSysTick = 0;
vs32 g_nSysDelay = 0;

void STick_InitSysTick(void)
{
    //Configure HCLK clock as SysTick clock source  48M
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    //SysTick interrupt each 1000 Hz with HCLK equal
    SysTick_Config(STICK_1MS * STICK_TIME_MS);

    g_nSysTick = 0;
    g_nSysDelay = 0;

    NVIC_SetPriority(SysTick_IRQn, INT_PRIORITY_STICK);     //优先级最低
}

