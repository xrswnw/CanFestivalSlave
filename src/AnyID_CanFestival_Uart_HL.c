#include "AnyID_CanFestival_Uart_HL.h"

#define UART_INT_CHANNEL        USART2_IRQn
const PORT_INF UART_PORT_TX = {GPIOA, GPIO_Pin_2};
const PORT_INF UART_PORT_RX = {GPIOA, GPIO_Pin_3};
const PORT_INF UART_485_CTRL = {GPIOA, GPIO_Pin_1};
void Uart_InitInterface(u32 baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    USART_DeInit(UART_PORT);

    GPIO_InitStructure.GPIO_Pin = UART_PORT_TX.Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART_PORT_TX.Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART_PORT_RX.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_PORT_RX.Port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART_485_CTRL.Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(UART_485_CTRL.Port, &GPIO_InitStructure);
    Uart_Enable485Rx();

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    //Configure USART
    USART_Init(UART_PORT, &USART_InitStructure);
    // Enable the USART
    USART_Cmd(UART_PORT, ENABLE);
}


void Uart_ConfigInt(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = UART_INT_CHANNEL;

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_UART_RX >> 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_UART_RX & 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

void Uart_EnableInt(FunctionalState rxState, FunctionalState txState)
{
    USART_ClearITPendingBit(UART_PORT, USART_IT_RXNE | USART_IT_TC);

    USART_ITConfig(UART_PORT, USART_IT_RXNE, rxState);
    USART_ITConfig(UART_PORT, USART_IT_TC, txState);
}

void Uart_WriteByte(u8 ch)
{
    while(((UART_PORT)->SR & USART_FLAG_TXE) == (u16)RESET);
	(UART_PORT)->DR = (ch & (u16)0x01FF);
}

void Uart_Delayms(u32 n)
{
    n *= 0x3800;
    n++;
    while(n--);
}

void Uart_WriteBuffer(u8 *pFrame, u16 len)
{
    u16 i = 0;
    Uart_Enable485Tx();
    for(i = 0; i < len; i++)
    {
		Uart_WriteByte(pFrame[i]);
    }
    Uart_Chk485TxOver();
    Uart_Enable485Rx();
}




