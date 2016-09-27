 /******************************************************************************
  * @file       USER/eeprom.h
  * @author  cjw
  * @date     2016.9.26
  * @brief     This file contains the headers of eeprom(24c02).
  ******************************************************************************/

#ifndef __EEPROM_24C_H
#define __EEPROM_24C_H

#include "global.h"
#include "gpio.h"

#define IIC_OK    0x00    //I2C success flag
#define IIC_ERR    0x01    //I2C erroe flag

#define IIC_ADD 0xA0

#define SDA_SetHigh()    GPIO_PinSet(GPIO_PTA3)
#define SDA_SetLow()    GPIO_PinClear(GPIO_PTA3)
#define SDA_GetValue() ( GPIO_Read(GPIOA)&GPIO_PTA3_MASK )
#define SDA_SetDigitalInput() GPIO_Init(GPIOA, GPIO_PTA3_MASK , GPIO_PinInput)
#define SDA_SetDigitalOutput() GPIO_Init(GPIOA, GPIO_PTA3_MASK , GPIO_PinOutput)
#define SCL_SetHigh()    GPIO_PinSet(GPIO_PTA2)
#define SCL_SetLow()    GPIO_PinClear(GPIO_PTA2)
#define WP_SetHigh()    GPIO_PinSet(GPIO_PTA1)
#define WP_SetLow()    GPIO_PinClear(GPIO_PTA1)

void IIC_Delay(void);
void IIC_Init(void);
void IIC_Start(void);
uint8_t IIC_Send_Byte(uint8_t Data);
uint8_t IIC_Receive_Byte(void);
void IIC_Stop(void);
uint8_t IIC_Read_Buffer(uint16_t ADD,uint8_t *buff,uint8_t Length);
uint8_t IIC_Send_Buffer(uint16_t ADD,uint8_t *buff,uint8_t Length);

#endif
