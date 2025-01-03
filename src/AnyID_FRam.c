#include "AnyID_FRam.h"

BOOL FRam_ReadBuffer(u16 addr, u16 len, u8 *pBuffer)
{
    BOOL b = TRUE;
    u16 i = 0;

    if(len)
    {
        //开始
        if(!FRam_I2CStart())
        {
            b = FALSE;
        }

        //设备地址
        if(b)
        {
            FRam_I2CSendByte(FRAM_ADDR_WRITE);
            if(!FRam_I2CWaitAck())
            {
                b = FALSE;
            }
        }

        //数据高地址
        if(b)
        {
            FRam_I2CSendByte((addr >> 8) & 0xFF);
            b = FRam_I2CWaitAck();
        }

        //数据低地址
        if(b)
        {
            FRam_I2CSendByte((addr >> 0) & 0xFF);
            b = FRam_I2CWaitAck();
        }

        //开始
    	if(b)
    	{
    		if(!FRam_I2CStart())
    		{
    			b = FALSE;
            }
    	}

        //设备地址
        if(b)
        {
            FRam_I2CSendByte(FRAM_ADDR_READ);
            if(!FRam_I2CWaitAck())
            {
                b = FALSE;
            }
        }

        //读出数据数据，最后一个数据要以NOACK结尾
    	len--;
    	if(b)
    	{
    		for(i = 0; i < len; i++)
    		{
    			pBuffer[i] = FRam_I2CReceiveByte();
    			FRam_I2CAck();
    		}

    		pBuffer[i] = FRam_I2CReceiveByte();
    		FRam_I2CNoAck();
    	}

        FRam_I2CStop();
    }

    return b;
}

BOOL FRam_WritePage(u16 addr, u16 len, u8 *pBuffer)
{
    u16 i = 0;
    BOOL b = TRUE;

    if(len)
    {
        //开始
        if(!FRam_I2CStart())
        {
            b = FALSE;
        }

        //设备地址
        if(b)
        {
            FRam_I2CSendByte(FRAM_ADDR_WRITE);
            if(!FRam_I2CWaitAck())
            {
                b = FALSE;
            }
        }

        //数据高地址
        if(b)
        {
            FRam_I2CSendByte((addr >> 8) & 0xFF);
            b = FRam_I2CWaitAck();
        }

        //数据低地址
        if(b)
        {
            FRam_I2CSendByte((addr >> 0) & 0xFF);
            b = FRam_I2CWaitAck();
        }

        //写入数据
        for(i = 0; i < len; i++)
        {
            if(b)
            {
                FRam_I2CSendByte(pBuffer[i]);
                b = FRam_I2CWaitAck();
            }
        }

        //停止
        FRam_I2CStop();
    }
    FRam_Delayms(5);
    return b;
}

BOOL FRam_WriteBuffer(u16 addr, u16 len, u8 *pBuffer)
{
    u16 size = 0;
    u16 pos = 0;
    BOOL b = FALSE;

    //pageSize = 32
    if(addr & FRAM_PAGE_MASK)
    {
        size = FRAM_PAGE_SIZE - (addr & FRAM_PAGE_MASK);
        if(len < size)
        {
            size = len;
        }
        b = FRam_WritePage(addr + pos, size, pBuffer + pos);
        len -= size;
        pos += size;
    }
    else
    {
        b = TRUE;
    }

    if(b)
    {
        while(len > 0)
        {
            size = FRAM_PAGE_SIZE;
            if(len < size)
            {
                size = len;
            }
            b = FRam_WritePage(addr + pos, size, pBuffer + pos);
            len -= size;
            pos += size;

            if(b == FALSE)
            {
                break;
            }
        }
    }

    return b;
}

