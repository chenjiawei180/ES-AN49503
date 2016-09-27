 /******************************************************************************
  * @file       USER/eeprom_24c.c
  * @author  cjw
  * @date     2016.9.26
  * @brief     This file contains the source of the eeprom(24c02).
  ******************************************************************************/

#include "eeprom_24c.h"
#include "led.h"

/**
  * @brief  This function is IIC_Delay.
  * @param  None .
  * @retval None
  */

void IIC_Delay(void)    // about 12us
{
    uint8_t i;
    for(i=0;i<8;i++);
}

/**
  * @brief  This function is IIC_Init.
  * @param  None .
  * @retval None
  */
  
void IIC_Init(void)
{
    GPIO_Init(GPIOA, GPIO_PTA1_MASK , GPIO_PinOutput);    //WP GPIO Init
    GPIO_Init(GPIOA, GPIO_PTA2_MASK , GPIO_PinOutput);    //SCL GPIO Init
    GPIO_Init(GPIOA, GPIO_PTA3_MASK , GPIO_PinOutput);    //SDA GPIO Init
    WP_SetHigh();
    SDA_SetHigh();
    SCL_SetLow();
}

/**
  * @brief  This function is IIC_Start.
  * @param  None .
  * @retval None
  */

void IIC_Start(void)
{
    SDA_SetHigh();
    IIC_Delay();
    SCL_SetHigh();
    IIC_Delay();
    SDA_SetLow();
    IIC_Delay();
    SCL_SetLow();
    IIC_Delay();
}

/**
  * @brief  This function is IIC_Send_Byte.
  * @param  uint8_t Data .
  * @retval 0 is success or 1 is error
  */
  
uint8_t IIC_Send_Byte(uint8_t Data)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        if(Data&0x80)
            SDA_SetHigh();
        else
            SDA_SetLow();
        IIC_Delay();
        SCL_SetHigh();
        IIC_Delay();
        Data<<=1;
        SCL_SetLow();
        IIC_Delay();
    }
    SDA_SetHigh();
    IIC_Delay();
    SCL_SetHigh();
    IIC_Delay();
    SDA_SetDigitalInput();
    if(SDA_GetValue() != 0)
    {
        SDA_SetDigitalOutput();
        return IIC_ERR;
    }
    SDA_SetDigitalOutput();
    SCL_SetLow();
    IIC_Delay();
    SDA_SetLow();
    IIC_Delay();
    return IIC_OK;
}

/**
  * @brief  This function is IIC_Receive_Byte.
  * @param  None.
  * @retval data
  */
  
uint8_t IIC_Receive_Byte(void)
{
    uint8_t i,Data=0;
    SDA_SetHigh();    //release the I2C bus for prepare receive data
    for(i=0;i<8;i++)
    {
        Data <<= 1;
        SCL_SetHigh();
        IIC_Delay();
        SDA_SetDigitalInput();
        if(SDA_GetValue())
        {
            SDA_SetDigitalOutput();
            Data |= 0x01;
        }
        SDA_SetDigitalOutput();
        SCL_SetLow();
        IIC_Delay();
    }
    return Data;
}

/**
  * @brief  This function is IIC_Stop.
  * @param  None.
  * @retval None.
  */
  
void IIC_Stop(void)
{
    SCL_SetLow();
    IIC_Delay();
    SDA_SetLow();
    IIC_Delay();
    SCL_SetHigh();
    IIC_Delay();
    SDA_SetHigh();
    IIC_Delay();
}

/**
  * @brief  This function is Read a buffer.
  * @param  ADD of I2C address ,index of data buffer,length of data buffer.
  * @retval success or error
  */
  
uint8_t IIC_Read_Buffer(uint16_t ADD,uint8_t *buff,uint8_t Length)
{
    uint8_t Flag=1;

    IIC_Start();

    Flag=IIC_Send_Byte(IIC_ADD);     //send the device address write
    if(Flag)
        return IIC_ERR;

    Flag=IIC_Send_Byte( (ADD>>8)&0x1f );     //send the first register address
    if(Flag)
        return IIC_ERR;

    Flag=IIC_Send_Byte( ADD & 0xff );     //send the second register address
    if(Flag)
        return IIC_ERR;
    
    IIC_Start();    //restart the I2C bus
    Flag=IIC_Send_Byte(IIC_ADD+1);    //send the device address read
    if(Flag)
        return IIC_ERR;

    while(Length>1)
    {
        *buff = IIC_Receive_Byte();    //receive the data

        SDA_SetLow();//send ACK to slave device
        IIC_Delay();
        SCL_SetHigh();
        IIC_Delay();
        SCL_SetLow();
        IIC_Delay();

        buff++;
        Length--;
    }
    *buff = IIC_Receive_Byte();

    SDA_SetHigh();//SEND NCAK to slave device
    IIC_Delay();
    SCL_SetHigh();
    IIC_Delay();
    SCL_SetLow();
    IIC_Delay();

    IIC_Stop();    //×ÜÏßÍ£Ö¹

    return IIC_OK;
}

/**
  * @brief  This function is Read a buffer.
  * @param  ADD of I2C address ,index of data buffer,length of data buffer.
  * @retval success or error
  */
  
uint8_t IIC_Send_Buffer(uint16_t ADD,uint8_t *buff,uint8_t Length)
{
    uint8_t Flag=1;
    uint16_t i;

    WP_SetLow();

    IIC_Start();    

    Flag=IIC_Send_Byte(IIC_ADD);     //send the device address write
    if(Flag)
        return IIC_ERR;

    Flag=IIC_Send_Byte( (ADD>>8)&0x1f );     //send the first register address
    if(Flag)
        return IIC_ERR;

    Flag=IIC_Send_Byte( ADD & 0xff );     //send the second register address
    if(Flag)
        return IIC_ERR;

    for (i = 0; i<Length; i++)
    {
        Flag=IIC_Send_Byte(*(buff+i));    //send the data
        if(Flag)
            return IIC_ERR;
    }

    IIC_Stop();  

    WP_SetHigh();

    for(i=0;i<0x7fff;i++);    //wait write complete

    return IIC_OK;
}


