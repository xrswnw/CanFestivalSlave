#include "AnyID_CanFestival_SysCfg.h"

u32 g_nSysState = SYS_STAT_IDLE;
u32 g_nLedDelayTime = 0;

u32 g_nChkComTime = 0;

#if SYS_ENABLE_WDT
    #define SYS_ENABLE_TEST         0
#else
    #define SYS_ENABLE_TEST         1
#endif

void Sys_Delayms(u32 n)
{
    //72MHZ
    n *= 0x6000;
    n++;
    while(n--);
}


void Sys_CfgClock(void)
{
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit();
    //Enable HSE
    RCC_HSEConfig(RCC_HSE_ON);

    //Wait till HSE is ready
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if(HSEStartUpStatus == SUCCESS)
    {

        //HCLK = SYSCLK = 72.0M
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        //PCLK2 = HCLK = 72.0M
        RCC_PCLK2Config(RCC_HCLK_Div1);

        //PCLK1 = HCLK/2 = 33.9M
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //ADCCLK = PCLK2/2
        RCC_ADCCLKConfig(RCC_PCLK2_Div8);

        // Select USBCLK source 72 / 1.5 = 48M
        RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

        //Flash 2 wait state
        FLASH_SetLatency(FLASH_Latency_2);

        //Enable Prefetch Buffer
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_10);    //PLL在最后设置

        //Enable PLL
        RCC_PLLCmd(ENABLE);

        //Wait till PLL is ready
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        //Select PLL as system clock source
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        //Wait till PLL is used as system clock source
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
}

void Sys_CfgPeriphClk(FunctionalState state)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_AFIO |
                           RCC_APB2Periph_USART1 |
                           RCC_APB2Periph_SPI1 |
                           RCC_APB2Periph_TIM1 |
                           RCC_APB2Periph_GPIOD, state);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 |
                           RCC_APB1Periph_PWR  |
                           RCC_APB1Periph_BKP  |
                           RCC_APB1Periph_WWDG , state);
}

void Sys_CfgNVIC(void)
{
    //NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM
    //Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  //VECT_TAB_FLASH
    //Set the Vector Table base location at 0x08000000
#ifdef _ANYID_BOOT_GD32_
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);
#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif
#endif

    //Configure the Priority Group to 2 bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}


void Sys_CtrlIOInit(void)
{
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

}

void Sys_Init(void)
{
    Sys_CfgClock();
    Sys_CfgNVIC();
    Sys_CfgPeriphClk(ENABLE);
		
	#if SYS_ENABLE_WDT
    WDG_InitIWDG();
	#endif
    Sys_CtrlIOInit();
	
    RCC_ClocksTypeDef sysclockFrep; RCC_GetClocksFreq(&sysclockFrep);    //查看时钟频率
    
	FRam_InitInterface();
	Device_ReadDeviceParamenter();
    Periph_InitInterface();
    while(Can_InitInterface(CAN_PSC_BUD_500K))
	{			
		Sys_Delayms(10);
		Sys_Delayms(10);
		#if SYS_ENABLE_WDT
    	WDG_FeedIWDog();
		#endif
	}
    

   //  setState(&slaveNormal_Data, Pre_operational);

    Uart_InitInterface(UART_BAUDRARE);
    Uart_ConfigInt();
    Uart_EnableInt(ENABLE, DISABLE);
    Sys_ComTo485();
    g_nChkComTime = g_nSysTick ;
    a_SetStateBit(g_nSysState, SYS_STAT_COM_CHK);
    
	Device_Init();
	
	#if SYS_ENABLE_WDT
	WDG_FeedIWDog();
	#endif
			
	STick_InitSysTick();
	Tim_InitInterface();
    Sys_EnableInt();
}

void Sys_LedTask(void)
{		
	if(a_CheckStateBit(g_nSysState, SYS_STAT_RUNLED))
	{	
		a_ClearStateBit(g_nSysState, SYS_STAT_RUNLED);

        #if SYS_ENABLE_WDT
        WDG_FeedIWDog();
        #endif
	}
}

void Sys_Rs485Task(void)
{
    if(!a_CheckStateBit(g_nSysState, SYS_STAT_COM_RS485))
    {
        return;
    }

    if(USART_GetFlagStatus(UART_PORT, USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
    {
        USART_ClearFlag(UART_PORT, USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
        Uart_InitInterface(UART_BAUDRARE);
        Uart_ConfigInt();
        Uart_EnableInt(ENABLE, DISABLE);
    }
    if(Uart_IsRcvFrame(g_sRS485RcvFrame))
    {
        memcpy(&g_sUartRcvTempFrame, &g_sRS485RcvFrame, sizeof(g_sRS485RcvFrame));
        Uart_ResetFrame(&g_sRS485RcvFrame);
        
        if(g_sUartRcvTempFrame.length >= UART_FRAME_MIN_LEN)
        {
            u16 crc1 = 0, crc2 = 0;
            
            crc1 = Uart_GetFrameCrc(g_sUartRcvTempFrame.buffer, g_sUartRcvTempFrame.length);
            crc2 = a_GetCrc(g_sUartRcvTempFrame.buffer + UART_FRAME_POS_LEN, g_sUartRcvTempFrame.length - 4);

            if(crc1 == crc2)
            {
                u16 txLen = 0;
                
                txLen = Reader_ProcessUartFrame(g_sUartRcvTempFrame.buffer, g_sUartRcvTempFrame.length);
                if(txLen > 0)
                {
                    a_SetStateBit(g_nSysState, SYS_STAT_RS485_TX);
                }
            }
        }
    }
    
    if(a_CheckStateBit(g_nSysState, SYS_STAT_RS485_TX))
    {
        a_ClearStateBit(g_nSysState, SYS_STAT_RS485_TX);
        Uart_WriteBuffer(g_sReaderRspFrame.buffer, g_sReaderRspFrame.len);

    }

}

void Sys_CanTsak()
{

	/*
	CAN_FLAG_EWG : 错误警告标志位        CAN_FLAG_EPV : 错误被动标志位       CAN_FLAG_BOF : 离线标志位
	*/
	if(CAN_GetFlagStatus(CAN_HARDPORT, CAN_FLAG_BOF))
	{
		//监控总线错误处理、、、、、
/*		if(g_sCanFrame.repat <= CAN_HARD_REAST_MAX_TICK)										//允许范围内尝试次数，超出则退出网络
		{
			Sys_Delayms(5);						
			Can_InitInterface(5);		//can硬件层复位				
			g_sCanFrame.repat ++;
		}
		else
		{
			if(!a_CheckStateBit(g_sCanFrame.state, CAN_STATE_ERR))
			{
				a_SetStateBit(g_sCanFrame.state, CAN_STATE_ERR);								//重试次数？？？？外接指示
			}
		}
		CAN_ClearFlag(CAN_HARDPORT, CAN_FLAG_BOF);
*/
	}
	
	if(a_CheckStateBit(g_sCanFrame.state, CAN_STATE_RX_OVER))									
	{	
		Message canRcvFrame = {0};
		
		memcpy(&canRcvFrame, &g_sCanFrame.rxFrame, sizeof(Message));
        a_ClearStateBit(g_sCanFrame.state, CAN_STATE_RX_OVER);
        //canDispatch(&slaveNormal_Data, &canRcvFrame);
	}
}

void Sys_DeviceTask(void)
{
	CO_Data *pOd = NULL;
	
	pOd = &slaveNormal_Data;
	if(pOd->nodeState == Initialisation)
	{

	}
	else if(pOd->nodeState == Disconnected)	
	{//连接断开
	
	}
	else if(pOd->nodeState == Connecting)	
	{//连接中
	
	}
	else if(pOd->nodeState == Preparing)	
	{//准备中
	
	}
	else if(pOd->nodeState == Stopped)	
	{//停止

	}
	else if(pOd->nodeState == Operational)	
	{//动作
		sendPDOevent(pOd);		//监控事件变化
        
        Device_LedTask();
	}
	else if(pOd->nodeState == Pre_operational)	
	{//预操作
	
	}
	else if(pOd->nodeState == Unknown_state)	
	{//未知态
        setNodeId(& slaveNormal_Data, g_sDeviceParams.addr);
        setState(&slaveNormal_Data, Initialisation);					//设备上电后初始状态
	}
}