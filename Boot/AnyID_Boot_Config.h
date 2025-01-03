#ifndef _ANYID_BOOT_CONFIG_
#define _ANYID_BOOT_CONFIG_
//******************************************************************************
//包含头文件
//******************************************************************************
#include "AnyID_Type.h"
#include "AnyID_Stdlib.h"
#include <stdio.h>

//******************************************************************************
//宏定义
//******************************************************************************
#define SYS_CLOCK_FREQ                  72000000                                    //设定工作时钟
#define STM32_CPUID_LEN                 12
#define SYS_ENABLE_WDT                  0                                           //看门狗宏定义

#if (defined(_ANYID_STM32_) || defined(_ANYID_GD32_))                                                    //STM32平台相关
    #include "stm32f10x.h"

    //端口定义
    typedef struct Port_Inf
    {
        GPIO_TypeDef* Port;
        u16 Pin;
    } PORT_INF;
#endif

#define STM32_CPUID_LEN                 12
#define STM32_CPUID_ADDR                (0x1FFFF7E8)

#define INT_PRIORITY_GROUP              2                                       //优先级分组是2:2
#define INT_PRIORITY_STICK              15                                      //STICK的优先级最低
#define INT_PRIORITY_RF433              0
#define INT_PRIORITY_UART               4                                      //串口接收
#define INT_PRIORITY_R485               0                                      //485接收
#define INT_PRIORITY_UART_DMA           8                                       //串口发送DMA
#define INT_PRIORITY_DOUTDMA            4                                       //ADDMA
#define INT_PRIORITY_EM4094_DIN         0                                       //15693的发送定时器优先级最高
#define INT_PRIORITY_SAM_IO             0                                       //SAM的IO数据，与INT_PRIORITY_EM4094_DIN不在同一个时刻工作
#define INT_PRIORITY_AUDIO_IO           0

#define Sys_EnableInt()                 __enable_irq()                          //开启系统中断
#define Sys_DisableInt()                __disable_irq()

#endif