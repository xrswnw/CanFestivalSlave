#include "AnyID_CanFestival_RTC.h"

RTC_TIME g_sRtcTime = {0, 1, 1, 0, 0, 0};

void RTC_Init(void)
{
    // Allow access to BKP Domain
    PWR_BackupAccessCmd(ENABLE);

    // Reset Backup Domain
    BKP_DeInit();

    // Enable LSE
    RCC_LSEConfig(RCC_LSE_ON);
   // RCC_LSICmd(ENABLE);

    // Wait till LSI is ready
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
    //while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    // Select LSI as RTC Clock Source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    // Enable RTC Clock
    RCC_RTCCLKCmd(ENABLE);

    // Wait for RTC registers synchronization
    RTC_WaitForSynchro();

    // Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();

    // Disable the RTC Second
    RTC_ITConfig(RTC_IT_SEC, DISABLE);

    // Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();

    // Set RTC prescaler: set RTC period to 1sec

    RTC_SetPrescaler(32767);

    // Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();
}

void RTC_ConfigInt(FunctionalState state)
{
    RTC_WaitForLastTask();             //等待写操作完成
    RTC_WaitForSynchro();//等待RTC寄存器和APB时钟同步
    RTC_WaitForLastTask();             //等待写操作完成
    RTC_ITConfig(RTC_IT_SEC, state);   //使能RTC秒中断
    RTC_WaitForLastTask();             //等待写操作完成
}

void RTC_Check(void)
{
    if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        //重新配置RTC
        RTC_Init();
        //配置完成后，向后备寄存器中写特殊字符0xA5A5
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);

        RTC_SetTime(0);
    }
    else
    {
        //清除RCC中复位标志
        RCC_ClearFlag();

        RCC_LSEConfig(RCC_LSE_ON);
        //RCC_LSEConfig(RCC_LSE_ON);

        // Wait till LSI is ready
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

        // Select LSI as RTC Clock Source
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

        // Enable RTC Clock
        RCC_RTCCLKCmd(ENABLE);

        // Wait for RTC registers synchronization
        RTC_WaitForSynchro();
    }
}

const u32 g_nRtcDayPerMonthLeapYear[12] = {RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH29_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH30_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH30_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH30_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND,
                                            RTC_MONTH30_PER_SECOND,
                                            RTC_MONTH31_PER_SECOND};
const u32 g_nRtcDayPerMonthNormalYear[12] = {RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH28_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH30_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH30_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH30_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND,
                                              RTC_MONTH30_PER_SECOND,
                                              RTC_MONTH31_PER_SECOND};
void RTC_SetTime(u32 count)
{
    PWR_BackupAccessCmd(ENABLE);
    //Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();
    //Change the current time
    RTC_SetCounter(count);
    //Wait until last write operation on RTC registers has finished
    RTC_WaitForLastTask();
}

u32 RTC_CalCount(RTC_TIME *pRtcTime)
{
    u32 count = 0;
    u8 i = 0;

    //year
    for(i = 0; i < pRtcTime->year; i++)
    {
        if(RTC_IsLeapYear(i))
        {
            count += RTC_YEAR366_PER_SECOND;
        }
        else
        {
            count += RTC_YEAR365_PER_SECOND;
        }
    }

    //month
    if(RTC_IsLeapYear(pRtcTime->year))
    {
        for(i = 0; i < pRtcTime->month - 1; i++)
        {
            count += g_nRtcDayPerMonthLeapYear[i];
        }
    }
    else
    {
        for(i = 0; i < pRtcTime->month - 1; i++)
        {
            count += g_nRtcDayPerMonthNormalYear[i];
        }
    }

    //day
    count += (pRtcTime->day - 1) * RTC_DAY_PER_SECOND;

    //hour
    count += (pRtcTime->hour) * RTC_HOUR_PER_SECOND;

    //minute
    count += (pRtcTime->minute) * RTC_MINUTE_PER_SECOND;

    //second
    count += pRtcTime->second;

    return count;
}

void RTC_FormatTime(u32 count)
{
    u8 y = 0, mth = 0, d = 0, h = 0, m = 0, s = 0;

    //year
    for(y = 0; y < 100; y++)
    {
        if(RTC_IsLeapYear(y))
        {
            if(count >= RTC_YEAR366_PER_SECOND)
            {
                count -= RTC_YEAR366_PER_SECOND;
            }
            else
            {
                break;
            }
        }
        else
        {
            if(count >= RTC_YEAR365_PER_SECOND)
            {
                count -= RTC_YEAR365_PER_SECOND;
            }
            else
            {
                break;
            }
        }
    }

    //month
    if(RTC_IsLeapYear(y))
    {
        for(mth = 0; mth < 12; mth++)
        {
            if(count >= g_nRtcDayPerMonthLeapYear[mth])
            {
                count -= g_nRtcDayPerMonthLeapYear[mth];
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        for(mth = 0; mth < 12; mth++)
        {
            if(count >= g_nRtcDayPerMonthNormalYear[mth])
            {
                count -= g_nRtcDayPerMonthNormalYear[mth];
            }
            else
            {
                break;
            }
        }
    }
    mth++;

    //day
    d = count / RTC_DAY_PER_SECOND;
    count -= d * RTC_DAY_PER_SECOND;
    d++;

    //hour
    h = count / RTC_HOUR_PER_SECOND;
    count -= h * RTC_HOUR_PER_SECOND;

    //minute
    m = count / RTC_MINUTE_PER_SECOND;
    count -= m * RTC_MINUTE_PER_SECOND;

    //second
    s = count;

    g_sRtcTime.year = y;
    g_sRtcTime.month = mth;
    g_sRtcTime.day = d;

    g_sRtcTime.hour = h;
    g_sRtcTime.minute = m;
    g_sRtcTime.second = s;
}

void Rtc_ConfigAlarmInt(FunctionalState state)
{
    NVIC_InitTypeDef NVIC_InitStructure;


    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}