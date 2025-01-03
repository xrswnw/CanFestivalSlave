#ifndef _ANYID_CANFESTIVAL_PERIPH_HL_H
#define _ANYID_CANFESTIVAL_PERIPH_HL_H

#include "AnyID_CanFestival_Config.h"
#include "stdlib.h"

extern const PORT_INF PERIPH_BUZZER_PORT;
#define Periph_BuzzerOn()                 (PERIPH_BUZZER_PORT.Port->BSRR = PERIPH_BUZZER_PORT.Pin)
#define Periph_BuzzerOff()                 (PERIPH_BUZZER_PORT.Port->BRR = PERIPH_BUZZER_PORT.Pin)

extern const PORT_INF PERIPH_LED_1_PORT;
#define Periph_Led1On()                 (PERIPH_LED_1_PORT.Port->BSRR = PERIPH_LED_1_PORT.Pin)
#define Periph_Led1Off()                 (PERIPH_LED_1_PORT.Port->BRR = PERIPH_LED_1_PORT.Pin)

extern const PORT_INF PERIPH_LED_2_PORT;
#define Periph_Led2On()                 (PERIPH_LED_2_PORT.Port->BSRR = PERIPH_LED_2_PORT.Pin)
#define Periph_Led2Off()                 (PERIPH_LED_2_PORT.Port->BRR = PERIPH_LED_2_PORT.Pin)

extern const PORT_INF PERIPH_LED_3_PORT;
#define Periph_Led3On()                 (PERIPH_LED_3_PORT.Port->BSRR = PERIPH_LED_3_PORT.Pin)
#define Periph_Led3Off()                 (PERIPH_LED_3_PORT.Port->BRR = PERIPH_LED_3_PORT.Pin)

extern const PORT_INF PERIPH_LED_4_PORT;
#define Periph_Led4On()                 (PERIPH_LED_4_PORT.Port->BSRR = PERIPH_LED_4_PORT.Pin)
#define Periph_Led4Off()                 (PERIPH_LED_4_PORT.Port->BRR = PERIPH_LED_4_PORT.Pin)

extern const PORT_INF PERIPH_LED_5_PORT;
#define Periph_Led5On()                 (PERIPH_LED_5_PORT.Port->BSRR = PERIPH_LED_5_PORT.Pin)
#define Periph_Led5Off()                 (PERIPH_LED_5_PORT.Port->BRR = PERIPH_LED_5_PORT.Pin)

extern const PORT_INF PERIPH_LED_6_PORT;
#define Periph_Led6On()                 (PERIPH_LED_6_PORT.Port->BSRR = PERIPH_LED_6_PORT.Pin)
#define Periph_Led6Off()                 (PERIPH_LED_6_PORT.Port->BRR = PERIPH_LED_6_PORT.Pin)


#define PERIPH_LED_NUM                  6

#define PERIPH_LED1                 {GPIOC, GPIO_Pin_12}
#define PERIPH_LED2                 {GPIOD, GPIO_Pin_2}
#define PERIPH_LED3                 {GPIOB, GPIO_Pin_3}
#define PERIPH_LED4                 {GPIOB, GPIO_Pin_4}
#define PERIPH_LED5                 {GPIOB, GPIO_Pin_5}
#define PERIPH_LED6                 {GPIOB, GPIO_Pin_6}

#define PERIPH_LEDINDEX1            0
#define PERIPH_LEDINDEX2            1
#define PERIPH_LEDINDEX3            2
#define PERIPH_LEDINDEX4            3
#define PERIPH_LEDINDEX5            4
#define PERIPH_LEDINDEX6            5

#define PERIPH_STATE_LED_NUM        4

#define PERIPH_STATE_LED1                   {GPIOB, GPIO_Pin_15}
#define PERIPH_STATE_LED2                   {GPIOB, GPIO_Pin_14}
#define PERIPH_STATE_LED3                   {GPIOC, GPIO_Pin_7}
#define PERIPH_STATE_LED4                   {GPIOC, GPIO_Pin_6}

#define PERIPH_STATE_LEDINDEX1            0
#define PERIPH_STATE_LEDINDEX2            1
#define PERIPH_STATE_LEDINDEX3            2
#define PERIPH_STATE_LEDINDEX4            3


extern u8 g_nPeriphLedStatus;
extern const PORT_INF PERIPH_LED[PERIPH_LED_NUM];
#define Periph_Ledon(index)         do{PERIPH_LED[index].Port->BSRR = PERIPH_LED[index].Pin; g_nPeriphLedStatus |= (1 << index);}while(0)
#define Periph_Ledoff(index)        do{PERIPH_LED[index].Port->BRR = PERIPH_LED[index].Pin; g_nPeriphLedStatus &= ~(1 << index);}while(0)

extern u8 g_nPeriphStateLedStatus;
extern const PORT_INF PERIPH_STATE_LED[PERIPH_STATE_LED_NUM];
#define Periph_StateLedon(index)         do{PERIPH_STATE_LED[index].Port->BSRR = PERIPH_STATE_LED[index].Pin;}while(0)
#define Periph_StateLedoff(index)        do{PERIPH_STATE_LED[index].Port->BRR = PERIPH_STATE_LED[index].Pin;}while(0)


#define PERIPH_OUTPUT_PERIPH_STATUS_IDLE            0x00
#define PERIPH_OUTPUT_PERIPH_STATUS_BUSY            0x01

#define PERIPH_OUTPUT_PERIPH_STATE_IDLE             0x00
#define PERIPH_OUTPUT_PERIPH_STATE_START            0x01
#define PERIPH_OUTPUT_PERIPH_STATE_OPEN             0x02
#define PERIPH_OUTPUT_PERIPH_STATE_STOP             0x04
#define PERIPH_OUTPUT_PERIPH_STATE_CLOSE            0x08



#define PERIPH_OUPUT_OPEN                           TRUE
#define PERIPH_OUPUT_CLOSE                          FALSE

#define PERIPH_OUPUT_BUZZER_ENABLE                  0x01
#define PERIPH_OUPUT_LIGHT1_ENABLE                  0x02
#define PERIPH_OUPUT_LIGHT2_ENABLE                  0x04
#define PERIPH_OUPUT_LIGHT3_ENABLE                  0x08
#define PERIPH_OUPUT_LIGHT4_ENABLE                  0x10

#define PERIPH_OUPUT_CMD_CTR                        0x40
#define PERIPH_OUPUT_CMD_ENABLE                     0x80

typedef struct outPutPeriph
{
    u8 state;
    u8 status;
    u8 outFlag;
    u8 openTime;
    u8 closeTime;
    u8 opNum;
    u32 time;
}OUTOUT_PERIPH;


typedef struct periphInfo{
    OUTOUT_PERIPH oPeriph;


}PERIPH_INFO;
extern PERIPH_INFO g_sPeriphInfo;

void Periph_InitInterface();
void Periph_Test();
void Periph_Ctr(u8 flag, BOOL op);
#endif 

