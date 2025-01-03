/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "AnyID_CanFestival_Can_HL.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval Noned
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    g_nSysTick ++;
  //心跳时钟调整，设置5ms心跳     

    if((g_nSysTick % 21) == 0)
    {
        a_SetStateBit(g_nSysState, SYS_STAT_RUNLED );  
    }
    
    Uart_IncIdleTime(STICK_TIME_MS, g_sRS485RcvFrame);
}

 
void Uart_IRQHandler(void)
{
    if(USART_GetITStatus(UART_PORT, USART_IT_RXNE) != RESET)
    {
        u8 byte = 0;
        USART_ClearITPendingBit(UART_PORT, USART_IT_RXNE);
        byte = Uart_ReadByte();
        Uart_ReceiveFrame(byte, &g_sRS485RcvFrame);
    }
    else if(USART_GetITStatus(UART_PORT, USART_IT_ORE) != RESET)
    {
        Uart_ReadByte();
    }
    UART_PORT->SR &= (~0x3FF);
}

void CAN_IRQHandler(void)
{

    if(SET == CAN_GetITStatus(CAN_HARDPORT, CAN_IT_ERR))//出错中断挂号 
    {
        CAN_ClearITPendingBit(CAN_HARDPORT, CAN_IT_ERR);
        //此处可重新初始化硬件层
    }
    
	// 没有接收到数据,直接退出 
	if(CAN_MessagePending(CAN_HARDPORT,CAN_FIFO0))
	{
		CanRxMsg rxMsg = {0};
		
		CAN_Receive(CAN_HARDPORT, CAN_FIFO0, &rxMsg);   // 读取数据	
		if(rxMsg.IDE == CAN_ID_STD)
		{
			g_sCanFrame.rxFrame.cob_id = rxMsg.StdId;
			g_sCanFrame.extId = 0;
		}
		else
		{
			g_sCanFrame.extId = rxMsg.ExtId;
			g_sCanFrame.rxFrame.cob_id = 0;
		}
		g_sCanFrame.rxFrame.rtr = rxMsg.RTR == (CAN_RTR_DATA ? 0 : 1);
		g_sCanFrame.rxFrame.len = rxMsg.DLC;
		a_SetState(g_sCanFrame.state, CAN_STATE_RX_OVER);
		memcpy(g_sCanFrame.rxFrame.data, rxMsg.Data, rxMsg.DLC);	
        canDispatch(&slaveNormal_Data, &g_sCanFrame.rxFrame);
	}
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
