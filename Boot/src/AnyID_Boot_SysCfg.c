#include "AnyID_Boot_SysCfg.h"

u32 g_nSysState = 0;
u32 g_nDeviceNxtEraseAddr = 0;
u32 g_nDeviceNxtDownloadAddr = 0;


#define SYS_BOOT_VER_SIZE               50
const u8 SYS_BOOT_VERSION[SYS_BOOT_VER_SIZE] = "Boot V3.0_20030601 GD32F1xx";

void Sys_Delayms(u32 n)             //系统延时n毫秒
{
    n *= 0x3800;
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
        //HCLK = SYSCLK = 67.8M
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        //PCLK2 = HCLK = 67.8M
        RCC_PCLK2Config(RCC_HCLK_Div1);

        //PCLK1 = HCLK/2 = 33.9M
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //ADCCLK = PCLK2/2
        RCC_ADCCLKConfig(RCC_PCLK2_Div2);
        
        // Select USBCLK source 72 / 1.5 = 48M
        RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

        //Flash 2 wait state
        FLASH_SetLatency(FLASH_Latency_2);

        //Enable Prefetch Buffer
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

        //PLLCLK = 12.00MHz * 6 = 72.0 MHz
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);    //PLL在最后设置

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
    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_USART1 |
                           RCC_APB2Periph_AFIO, state);
    //
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 |
						   RCC_APB1Periph_CAN1, state);
}

void Sys_CfgNVIC(void)
{
    //NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM
    //Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  //VECT_TAB_FLASH
    //Set the Vector Table base location at 0x08000000
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif
    //Configure the Priority Group to 2 bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

const PORT_INF SYS_RUNLED_COM		= {GPIOB, GPIO_Pin_0};
const PORT_INF SYS_ALARMLED_COM		= {GPIOB, GPIO_Pin_1};
void Sys_CtrlIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    //RUNLED
    GPIO_InitStructure.GPIO_Pin = SYS_RUNLED_COM.Pin;
    GPIO_Init(SYS_RUNLED_COM.Port, &GPIO_InitStructure);

    //ALARMLED
    GPIO_InitStructure.GPIO_Pin = SYS_ALARMLED_COM.Pin;
    GPIO_Init(SYS_ALARMLED_COM.Port, &GPIO_InitStructure);
}

void Sys_Init(void)
{
#if SYS_ENABLE_WDT
    WDG_InitIWDG();
#endif
    //
    Sys_CfgClock();

    Sys_CfgNVIC();
    Sys_CfgPeriphClk(ENABLE);

    //禁止中断
    Sys_DisableInt();

    Sys_CtrlIOInit();
    Sys_RunLedOn();
    Sys_AlarmLedOn();

    Sys_Delayms(10);                        //FRAM上电必须延时10ms

    FRam_InitInterface();
    Fram_ReadBootParamenter();
    //如果appState状态正常，但是版本信息校验错误，恢复默认状态
    if(g_sFramBootParamenter.appState == FRAM_BOOT_APP_OK)
    {
        if(Sys_CheckVersion() == FALSE)
        {
			g_sFramBootParamenter.appState = FRAM_BOOT_APP_FAIL;
			g_sFramBootParamenter.addr = 0x0001;
            g_sFramBootParamenter.br = FRAM_CAN_BUD_500K; 
        }
    }
	else if(g_sFramBootParamenter.appState == FRAM_BOOT_APP_NONE)
    {
        g_sFramBootParamenter.appState = FRAM_BOOT_APP_FAIL;
        g_sFramBootParamenter.addr = 0x0001;
        g_sFramBootParamenter.br = FRAM_CAN_BUD_500K;
        if(Sys_CheckVersion() == TRUE && Sys_CheckAppEmpty() == FALSE)          //状态错误，但是版本信息校验正确
        {
            g_sFramBootParamenter.appState = FRAM_BOOT_APP_OK;
        }
    }
    
	//Can_InitInterface(Sys_GetBudPsc(g_sFramBootParamenter.br));
    //SysTick 初始化 5ms
    
    R485_InitInterface(R485_BAUDRARE);
    R485_ConfigInt();
    R485_EnableInt(ENABLE, DISABLE);
    
    STick_InitSysTick();
    Sys_RunLedOff();
    Sys_AlarmLedOff();
    //系统空闲状态
    if(g_sFramBootParamenter.appState == FRAM_BOOT_APP_OK)
    {
        a_SetState(g_nSysState, SYS_STAT_IDLE);
    }
    else
    {
        a_SetState(g_nSysState, SYS_STAT_DOWNLOAD);

    }
    //使能中断
    Sys_EnableInt();
}

void Sys_LedTask(void)
{
    if(a_CheckStateBit(g_nSysState, SYS_STAT_RUNLED))
    {
        static u32 ledTimes = 0;

        a_ClearStateBit(g_nSysState, SYS_STAT_RUNLED);

        ledTimes++;

        if(ledTimes & 0x01)
        {
            Sys_RunLedOn();
			Sys_AlarmLedOn();
        }
        else
        {
            Sys_RunLedOff();
			Sys_AlarmLedOff();
        }

    #if SYS_ENABLE_WDT
        WDG_FeedIWDog();
    #endif
    }
}

typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
void Sys_Jump(u32 address)
{
    u32 stackAddr = 0;
    Sys_DisableInt();
    stackAddr = *((vu32 *)address);
    //查看栈地址是否在RAM区或者CCRAM区
    if((stackAddr & 0x2FFE0000) == 0x20000000)
    {
        JumpAddress = *(vu32 *)(address + 4);
        Jump_To_Application = (pFunction) JumpAddress;

        __set_MSP(*(vu32 *)address);
        Jump_To_Application();
    }
    //else
    {
        a_SetState(g_nSysState, SYS_STAT_IDLE);
        g_nSysTick = 0;
    }
    //while(1)
    {
    #if SYS_ENABLE_WDT
        WDG_FeedIWDog();
    #endif
    }
    Sys_EnableInt();
}

u16 Sys_GetBudPsc(u8 bud)
{
	u16 value = CAN_BUD_500K_PSCVALUE;
	
	if(bud == FRAM_CAN_BUD_1000K)
	{
		value = CAN_BUD_1000K_PSCVALUE;
	}
	else if(bud == FRAM_CAN_BUD_800K)
	{
		value = CAN_BUD_800K_PSCVALUE;
	}
	else if(bud == FRAM_CAN_BUD_250K)
	{
		value = CAN_BUD_250K_PSCVALUE;
	}	
	else if(bud == FRAM_CAN_BUD_125K)
	{
		value = CAN_BUD_125K_PSCVALUE;
	}
	else if(bud == FRAM_CAN_BUD_50K)
	{
		value = CAN_BUD_50K_PSCVALUE;
	}
	else if(bud == FRAM_CAN_BUD_20K)
	{
		value = CAN_BUD_20K_PSCVALUE;
	}
	else if(bud == FRAM_CAN_BUD_10K)
	{
		value = CAN_BUD_10K_PSCVALUE;
	}
	else
	{
		value = CAN_BUD_500K_PSCVALUE;
	}
	
	return value;
}

void Sys_BootTask(void)
{
    if(!a_CheckStateBit(g_nSysState, SYS_STAT_DOWNLOAD))
    {
        if(g_nSysTick > 40 && g_sFramBootParamenter.appState == FRAM_BOOT_APP_OK)                //TICK = STICK_TIME_MS = 5MS, 延时200ms等待boot选择
        {
            a_SetStateBit(g_nSysState, SYS_STAT_JMP);
        }
    }

    if(a_CheckStateBit(g_nSysState, SYS_STAT_JMP))
    {
        Sys_RunLedOff();
        Sys_Jump(SYS_APP_START_ADDR);
    }
}


void Sys_R485Task(void)
{
    //串口错误处理:重新初始化
    if(USART_GetFlagStatus(R485_PORT, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
    {
        USART_ClearFlag(R485_PORT, USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
        R485_InitInterface(Fram_GetUartBaudrate());
        R485_ConfigInt();
        R485_EnableInt(ENABLE, DISABLE);
    }

    //串口数据帧解析
    if(Uart_IsRcvFrame(g_sR485RcvFrame))
    {
        Sys_ProcessBootFrame(&g_sR485RcvFrame, SYS_COM_R485);
        Uart_ResetFrame(g_sR485RcvFrame);
    }
}


void Sys_CanTask()
{
	if(CAN_GetFlagStatus(CAN_HARDPORT, CAN_FLAG_BOF))
	{
		//监控总线错误处理、、、、、
		if(g_sCanFrame.repat <= CAN_HARD_REAST_MAX_TICK)										//允许范围内尝试次数，超出则退出网络
		{
			Sys_Delayms(5);						
			Can_InitInterface(Sys_GetBudPsc(g_sFramBootParamenter.br));		
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
	}

	if(a_CheckStateBit(g_sCanFrame.state, CAN_STATE_RX_OVER))
	{	
		Can_ClrInfo();
		if(1)//g_sCanFrame.rxMasg.ExtId || ((g_sCanFrame.rxMasg.StdId & 0xFF) == UART_FRAME_CMD_JMP))//跳转支持标准帧
		{
			Sys_CanProcessFrame(&g_sCanFrame);	
			a_SetStateBit(g_sCanFrame.state, CAN_STATE_TX);
		}
	}
	
	if(a_CheckStateBit(g_sCanFrame.state, CAN_STATE_TX))
	{	
		CanSend(&g_sCanFrame.txMasg);
		a_ClearStateBit(g_sCanFrame.state, CAN_STATE_TX);
		
		if((g_sCanFrame.rxMasg.ExtId & 0xFF) == UART_FRAME_CMD_RESET)
		{
			Sys_Delayms(5);
			Sys_SoftReset();
		}
	}
}

#define BOOT_UPDATA_INFO_1K_LENTH			1024
u8 g_nRcvUpDateInfo[BOOT_UPDATA_INFO_1K_LENTH] = {0};


BOOT_INFO g_sBootInfo = {0};
void Sys_CanProcessFrame(CAN_FRAME *pFrame)
{
	u8 *pPras = NULL;
	u16 cmd = 0;
	u32 errInfo = CAN_OD_VALUE_RANGE_EXCEEDED;
	
	cmd = pFrame->rxMasg.ExtId;
	pPras = pFrame->rxMasg.Data;
	switch(cmd)
	{
		case UART_FRAME_CMD_RESET:
			g_nSysTick = 0;
			errInfo = CAN_OD_VALUE_SUCC;
			Can_FormatFrame(CAN_RSP_INFO_LEN, (u8 *)&errInfo);
			break;
		case UART_FRAME_CMD_ERASE:
			if(a_CheckStateBit(g_nSysState, SYS_STAT_DOWNLOAD))
			{
				u32 addr = 0;
				u8 sector = 0;
				
				sector = *(u32 *)pPras;
				addr = SYS_APP_START_ADDR + (sector << 10);         //每个扇区1K
				if(addr >= SYS_APP_START_ADDR)
				{
					if(g_nDeviceNxtEraseAddr == addr)               //擦除地址必须是连续的，否则会有区域未擦除
					{
						g_nDeviceNxtEraseAddr = addr + (1 << 10);   //每个扇区1K
						
						if(Uart_EraseFlash(addr))
						{
							errInfo = CAN_OD_VALUE_SUCC;
						}
						else
						{
							errInfo = CAN_OD_VALUE_RANGE_EXCEEDED;
						}
					}
				}
			}
			Can_FormatFrame(CAN_RSP_INFO_LEN, (u8 *)&errInfo);
		break;
		/*case UART_FRAME_CMD_DL:
			if(a_CheckStateBit(g_nSysState, SYS_STAT_DOWNLOAD))
			{
				u32 addr = 0, size = 0;
				
				addr = *(u32 *)pPras;
				size = 4;						//每次操作四个字节？？？
				if(addr >= SYS_APP_START_ADDR)
				{
					//第一次不需要判定地址连续问题，因为boot程序是从后向前写数据，第一次不知道地址是什么
					if(addr + size == g_nDeviceNxtDownloadAddr || g_nDeviceNxtDownloadAddr == 0)
					{
						g_nDeviceNxtDownloadAddr = addr;
						Sys_AlarmLedOn();
						Sys_RunLedOn();
						if(BL_WriteImagePage(addr, pPras + 4, size))					//index复位
						{
							errInfo = CAN_OD_VALUE_SUCC;
						}
						else
						{
							errInfo = CAN_OD_VALUE_RANGE_EXCEEDED;
						}
						Sys_AlarmLedOff();
						Sys_RunLedOff();
					}
					else 
					{
						errInfo = CAN_OD_VALUE_RANGE_EXCEEDED;
					}
					Can_FormatFrame(CAN_RSP_INFO_LEN, (u8 *)&errInfo);
				}
			}
		break;*/
		case UART_FRAME_CMD_BOOT:
			a_ClearStateBit(g_nSysState, SYS_STAT_IDLE);
			a_SetStateBit(g_nSysState, SYS_STAT_DOWNLOAD);
			g_sFramBootParamenter.appState = FRAM_BOOT_APP_FAIL;
			Fram_WriteBootParamenter();
			Fram_WriteBackupBootParamenter();
			FLASH_Unlock();
			Uart_EraseFlash(SYS_BOOT_VER_ADDR);                 //版本信息区域擦除
			g_nDeviceNxtEraseAddr = SYS_APP_START_ADDR;
			g_nDeviceNxtDownloadAddr = 0;                       //boot是由后向前写入数据
			errInfo = CAN_OD_VALUE_SUCC;				
			Can_FormatFrame(CAN_RSP_INFO_LEN, (u8 *)&errInfo);
		break;
		case UART_FRAME_CMD_JMP:
			if(Sys_CheckVersion() == TRUE)
			{
				a_SetStateBit(g_nSysState, SYS_STAT_JMP);
				g_sFramBootParamenter.appState = FRAM_BOOT_APP_OK;
				Fram_WriteBootParamenter();
				Fram_WriteBackupBootParamenter();
				FLASH_Lock();
				errInfo = CAN_OD_VALUE_SUCC;				
			}
			else
			{
				errInfo = CAN_OD_VALUE_RANGE_EXCEEDED;				
			}
			Can_FormatFrame(CAN_RSP_INFO_LEN, (u8 *)&errInfo);
			break;
		case UART_FRAME_CMD_UPDATA_CTR:
			if(a_CheckStateBit(g_nSysState, SYS_STAT_DOWNLOAD))
			{
                static BOOL bWrite = FALSE;
				u16 crc1 = 0;
				u32 ctr = 0, value = 0;
				
				ctr = *(u32 *)pPras;
				value = *(u32 *)(pPras + 4);
				if(ctr == SYS_BOOT_DOWNLOAD_START_CTR)					//开始接收，1k字节为基准
				{
					if(!bWrite && g_nDeviceNxtDownloadAddr + SYS_BOOT_DOWNLOAD_FRAME_MAX_LEN == value || g_nDeviceNxtDownloadAddr == 0)
					{
						memset(&g_sBootInfo, 0, sizeof(BOOT_INFO));
						g_nDeviceNxtDownloadAddr = value;
						errInfo = CAN_OD_VALUE_SUCC;
						g_sBootInfo.addr = value;
						a_SetStateBit(g_sBootInfo.flag, SYS_BOOT_DOWNLOAD_FLAG_STRAT);
					}
				}
				else if(ctr == SYS_BOOT_DOWNLOAD_END_CTR)			//停止接受。同时写入数据
				{
					if(a_CheckStateBit(g_sBootInfo.flag, SYS_BOOT_DOWNLOAD_FLAG_OVER))
					{
						crc1 = a_GetCrc(g_sBootInfo.frame, SYS_BOOT_DOWNLOAD_FRAME_MAX_LEN);
						if(crc1 == value)
						{
							Sys_AlarmLedOn();
							Sys_RunLedOn();
							if(BL_WriteImagePage(g_sBootInfo.addr, g_sBootInfo.frame, SYS_BOOT_DOWNLOAD_FRAME_MAX_LEN))					//index复位
							{
								errInfo = CAN_OD_VALUE_SUCC;
								memset(&g_sBootInfo, 0, sizeof(BOOT_INFO));
							}
							Sys_AlarmLedOff();
							Sys_RunLedOff();
						}
					}
				}
			}
			Can_FormatFrame(CAN_RSP_INFO_LEN, (u8 *)&errInfo);
			break;
		case UART_FRAME_CMD_RCVINFO:
			if(a_CheckStateBit(g_sBootInfo.flag, SYS_BOOT_DOWNLOAD_FLAG_STRAT))
			{
				if(g_sBootInfo.index < SYS_BOOT_DOWNLOAD_FRAME_MAX_LEN)
				{
					memcpy(&g_sBootInfo.frame[g_sBootInfo.index], pPras, SYS_BOOT_DOWNLOAD_FRAME_LEN);
					g_sBootInfo.index += SYS_BOOT_DOWNLOAD_FRAME_LEN;
					errInfo = CAN_OD_VALUE_SUCC;
				}
				else
				{
					a_SetStateBit(g_sBootInfo.flag, SYS_BOOT_DOWNLOAD_FLAG_OVER);
				}
			}
			Can_FormatFrame(CAN_RSP_INFO_LEN, (u8 *)&errInfo);
			break;
		case UART_FRAME_CMD_TVER:
			Can_FormatFrame(SYS_VER_TYPE_SIZE, (u8 *)&SYS_BOOT_VERSION);
			break;
		case UART_FRAME_CMD_SVER:
			Can_FormatFrame(SYS_VER_SOFT_SIZE, (u8 *)(SYS_BOOT_VERSION + SYS_VER_TYPE_SIZE + 1));
			break;
		case UART_FRAME_CMD_HVER:
			Can_FormatFrame(SYS_VER_HARD_SIZE, (u8 *)(SYS_BOOT_VERSION + SYS_VER_TYPE_SIZE + 1 + SYS_VER_SOFT_SIZE + 1));
			break;
		default:
			errInfo = CAN_OD_VALUE_RANGE_EXCEEDED;
			Can_FormatFrame(CAN_RSP_INFO_LEN, (u8 *)&errInfo);
			break;
	}
}

void Sys_UsbTask()
{




}

#define SYS_BOOT_CHK_APP_LEN               128
BOOL Sys_CheckAppEmpty(void)
{
    BOOL b = FALSE;
    u16 i = 0;
    u32 *p = (u32 *)(SYS_APP_START_ADDR + 512); //检查第一个扇区的最后512直接
    for(i = 0; i < SYS_BOOT_CHK_APP_LEN; i++)
    {
        if(p[i] != 0xFFFFFFFF)
        {
            break;
        }
    }
    if(i == SYS_BOOT_CHK_APP_LEN)
    {
        b = TRUE;
    }
    
    return b;
}

BOOL Sys_CheckVersion(void)
{
    BOOL b = FALSE;
    u8 *p = (u8 *)SYS_BOOT_VER_ADDR;
    u8 i = 0, c = 0;
        
    if(memcmp(p, SYS_VER_HEAD, SYS_VER_HEAD_SIZE) == 0) //设备型号正确
    {
        for(i = SYS_VER_HEAD_SIZE; i < SYS_VERSION_SIZE; i++)
        {
            c = *p++;
            if((c < ' ' || c > 127) && (c != 0x00))
            {
                break;
            }
        }
        if(i == SYS_VERSION_SIZE)
        {
            b = TRUE;
        }
    }
    
    return b;
}


u8 Sys_ProcessBootFrame(UART_RCVFRAME *pRcvFrame, u8 com)
{
    u8 cmd = 0;
    u16 crc1 = 0, crc2 = 0;
    u16 devAddr = 0;
    memcpy(&g_sUartTempRcvFrame, pRcvFrame, sizeof(UART_RCVFRAME));


    crc1 = Uart_GetFrameCrc(g_sUartTempRcvFrame.buffer, g_sUartTempRcvFrame.index);
    crc2 = a_GetCrc(g_sUartTempRcvFrame.buffer + UART_FRAME_POS_LEN, g_sUartTempRcvFrame.index - 4);
    devAddr = *((u16 *)(g_sUartTempRcvFrame.buffer + UART_FRAME_POS_DESTADDR));
    cmd = g_sUartTempRcvFrame.buffer[UART_FRAME_POS_CMD];
    g_sUartTxFrame.len = 0;
    
    if((devAddr != g_sFramBootParamenter.addr) && (devAddr != 0xFFFF))
    {
        return 0;
    }
    if(crc1 == crc2)
    {
        switch(cmd)
        {
            case UART_FRAME_CMD_RESET:
                g_nSysTick = 0;
                g_sUartTxFrame.len = Uart_RspReset();
                break;
            case UART_FRAME_CMD_ERASE:
                if(a_CheckStateBit(g_nSysState, SYS_STAT_DOWNLOAD))
                {
                    BOOL bOk = FALSE;
                    u32 addr = 0;
                    u8 sector = 0;
                    
                    sector = g_sUartTempRcvFrame.buffer[UART_FRAME_POS_PAR];
                    addr = SYS_APP_START_ADDR + (sector << 10);         //每个扇区1K
                    
                    if(addr >= SYS_APP_START_ADDR)
                    {
                        if(g_nDeviceNxtEraseAddr == addr)               //擦除地址必须是连续的，否则会有区域未擦除
                        {
                            g_nDeviceNxtEraseAddr = addr + (1 << 10);   //每个扇区1K
                            
                            bOk = Uart_EraseFlash(addr); 
                            g_sUartTxFrame.len = Uart_RspErase(bOk);
                        }
                    }
                }
                break;
            case UART_FRAME_CMD_DL:
                if(a_CheckStateBit(g_nSysState, SYS_STAT_DOWNLOAD))
                {
                    BOOL bCheck = FALSE;
                    u32 addr = 0;
                    u32 size = 0;

                    if(g_sUartTempRcvFrame.buffer[UART_FRAME_POS_LEN] == 0x00)
                    {
                        bCheck = (BOOL)(g_sUartTempRcvFrame.buffer[UART_FRAME_POS_PAR + 0]);
                        addr = *((u32 *)(g_sUartTempRcvFrame.buffer + UART_FRAME_POS_PAR + 1));
                        size = *((u32 *)(g_sUartTempRcvFrame.buffer + UART_FRAME_POS_PAR + 5));
                        if(addr >= SYS_APP_START_ADDR)
                        {
                            //第一次不需要判定地址连续问题，因为boot程序是从后向前写数据，第一次不知道地址是什么
                            if(addr + size == g_nDeviceNxtDownloadAddr || g_nDeviceNxtDownloadAddr == 0)
                            {
                                g_nDeviceNxtDownloadAddr = addr;
                                //帧参数之前部分 + 基本参数(1 + 4 + 4) + size + crclen;
                                //frameLen = UART_FRAME_POS_PAR + 9 + size + 2;
                                Sys_AlarmLedOn();
                                Sys_RunLedOn();
                                if(BL_WriteImagePage(addr, g_sUartTempRcvFrame.buffer + UART_FRAME_POS_PAR + 9, size))
                                {
                                    g_sUartTxFrame.len = Uart_RspDownload(bCheck, addr, size);
                                }
                                Sys_AlarmLedOff();
                                Sys_RunLedOff();
                            }
                        }
                    }
                }
                break;
            case UART_FRAME_CMD_BOOT:
                g_sUartTxFrame.len = Uart_RspBoot();
                if(g_sUartTxFrame.len)      //下面擦除操作消耗时间较长
                {
                    if(com == SYS_COM_UART)
                    {
                        //Uart_WriteBuffer(g_sUartTxFrame.frame, g_sUartTxFrame.len);
                    }
                    else if(com == SYS_COM_R485)
                    {
                        R485_WriteBuffer(g_sUartTxFrame.frame, g_sUartTxFrame.len);
                    }
                    else if(com == SYS_COM_USB)
                    {
                        //USB_TxFrame(g_sUartTxFrame.frame, g_sUartTxFrame.len);
                    }
                    g_sUartTxFrame.len = 0;
                }
                a_ClearStateBit(g_nSysState, SYS_STAT_IDLE);
                a_SetStateBit(g_nSysState, SYS_STAT_DOWNLOAD);
                g_sFramBootParamenter.appState = FRAM_BOOT_APP_FAIL;
                Fram_WriteBootParamenter();
                Fram_WriteBackupBootParamenter();
                FLASH_Unlock();
                
                Uart_EraseFlash(SYS_BOOT_VER_ADDR);                 //版本信息区域擦除
                g_nDeviceNxtEraseAddr = SYS_APP_START_ADDR;
                g_nDeviceNxtDownloadAddr = 0;                       //boot是由后向前写入数据
                break;
            case UART_FRAME_CMD_JMP:
                if(Sys_CheckVersion() == TRUE)
                {
                    g_sUartTxFrame.len = Uart_RspJmp();
                    a_SetStateBit(g_nSysState, SYS_STAT_JMP);
                    g_sFramBootParamenter.appState = FRAM_BOOT_APP_OK;
                    Fram_WriteBootParamenter();
                    Fram_WriteBackupBootParamenter();
                    FLASH_Lock();
                }
                break;
            case UART_FRAME_CMD_VER:
                g_sUartTxFrame.len = Uart_RspFrame(g_sUartTxFrame.frame, cmd, (u8 *)SYS_BOOT_VERSION, SYS_BOOT_VER_SIZE, UART_FRAME_FLAG_OK, UART_FRAME_RSP_NOERR);
                break;
            default:
                break;
        }
    }

    if(g_sUartTxFrame.len)
    {
        if(com == SYS_COM_UART)
        {
            //Uart_WriteBuffer(g_sUartTxFrame.frame, g_sUartTxFrame.len);
        }
        else if(com == SYS_COM_R485)
        {
            R485_WriteBuffer(g_sUartTxFrame.frame, g_sUartTxFrame.len);
        }
        else if(com == SYS_COM_USB)
        {
            //USB_TxFrame(g_sUartTxFrame.frame, g_sUartTxFrame.len);
        }
        g_sUartTxFrame.len = 0;
        Sys_Delayms(2);         //等待最后一个字节发送完成
    }
}

