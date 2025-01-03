#include "AnyID_CanFestival_Periph_HL.h"

const PORT_INF PERIPH_LED_1_PORT = {GPIOC, GPIO_Pin_8};
const PORT_INF PERIPH_LED_2_PORT = {GPIOC, GPIO_Pin_9};
const PORT_INF PERIPH_LED_3_PORT = {GPIOC, GPIO_Pin_7};
const PORT_INF PERIPH_LED_4_PORT = {GPIOC, GPIO_Pin_6};
const PORT_INF PERIPH_LED_5_PORT = {GPIOB, GPIO_Pin_14};
const PORT_INF PERIPH_LED_6_PORT = {GPIOB, GPIO_Pin_15};

const PORT_INF PERIPH_LED[PERIPH_LED_NUM] = {PERIPH_LED1, PERIPH_LED2, PERIPH_LED3, PERIPH_LED4, PERIPH_LED5, PERIPH_LED6};
const PORT_INF PERIPH_STATE_LED[PERIPH_STATE_LED_NUM] = {PERIPH_STATE_LED1, PERIPH_STATE_LED2, PERIPH_STATE_LED3, PERIPH_STATE_LED4};

const PORT_INF PERIPH_BUZZER_PORT = {GPIOC, GPIO_Pin_15};
u8 g_nPeriphLedStatus = 0;
u8 g_nPeriphStateLedStatus = 0;

PERIPH_INFO g_sPeriphInfo = {0};
void Periph_Delayms(u32 n)
{
    n *= 0x3800;
    n++;
    while(n--);
}

void Periph_InitInterface()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    for(u8 i = 0; i < PERIPH_LED_NUM; i++)
    {
        GPIO_InitStructure.GPIO_Pin = PERIPH_LED[i].Pin;
        GPIO_Init(PERIPH_LED[i].Port, &GPIO_InitStructure);
    }
    
    for(u8 i = 0; i < PERIPH_STATE_LED_NUM; i++)
    {
        GPIO_InitStructure.GPIO_Pin = PERIPH_STATE_LED[i].Pin;
        GPIO_Init(PERIPH_STATE_LED[i].Port, &GPIO_InitStructure);
    }
    
    GPIO_InitStructure.GPIO_Pin = PERIPH_BUZZER_PORT.Pin;
    GPIO_Init(PERIPH_BUZZER_PORT.Port, &GPIO_InitStructure);

}


void Periph_Test()
{
    u8 index = 0;

    index  =  rand() % (PERIPH_STATE_LED_NUM);

    if((1 << index) & g_nPeriphStateLedStatus)
    {
        Periph_StateLedoff(index);
    }
    else
    {
        Periph_StateLedon(index);
    }
}

void Periph_Ctr(u8 flag, BOOL op)
{
    if(op)
    {
        if(flag & PERIPH_OUPUT_BUZZER_ENABLE)
        {
            Periph_BuzzerOn();
        }
        
        if(flag & PERIPH_OUPUT_LIGHT1_ENABLE)
        {
            Periph_StateLedon(PERIPH_STATE_LEDINDEX1);
        }
        
        if(flag & PERIPH_OUPUT_LIGHT2_ENABLE)
        {
            Periph_StateLedon(PERIPH_STATE_LEDINDEX2);
        }
        
        if(flag & PERIPH_OUPUT_LIGHT3_ENABLE)
        {
            Periph_StateLedon(PERIPH_STATE_LEDINDEX3);
        }
        
        if(flag & PERIPH_OUPUT_LIGHT4_ENABLE)
        {
            Periph_StateLedon(PERIPH_STATE_LEDINDEX4);
        }
    }
    else
    {
        if(flag & PERIPH_OUPUT_BUZZER_ENABLE)
        {
            Periph_BuzzerOff();
        }
        
        if(flag & PERIPH_OUPUT_LIGHT1_ENABLE)
        {
            Periph_StateLedoff(PERIPH_STATE_LEDINDEX1);
        }
        
        if(flag & PERIPH_OUPUT_LIGHT2_ENABLE)
        {
            Periph_StateLedoff(PERIPH_STATE_LEDINDEX2);
        }
        
        if(flag & PERIPH_OUPUT_LIGHT3_ENABLE)
        {
            Periph_StateLedoff(PERIPH_STATE_LEDINDEX3);
        }
        
        if(flag & PERIPH_OUPUT_LIGHT4_ENABLE)
        {
            Periph_StateLedoff(PERIPH_STATE_LEDINDEX4);
        }
    }
}