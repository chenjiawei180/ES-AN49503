 /******************************************************************************
  * @file       USER/main.c
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the source of main function.
  ******************************************************************************/
	
#define OS_GLOBALS
/* system headers */

#include "common.h"
#include "ics.h"
#include "sysinit.h"

/* user headers */
#include "global.h"
#include "led.h"
#include "wdt.h"
#include "spi_app.h"
#include "eeprom_24c.h"
#include "sleep.h"

int main(void)
{
    uint32_t i;
    uint16_t data=0;
    uint8_t i2c_buff[3]={0x04,0x05,0x06};
    uint8_t buff[3];
    /* Set the system clock */
    sysinit();

    /* WDT init */
    WDT_Init(1000);

    /* Led init */
    RED_Init();
    GREEN_Init();

    /* SPI0 init */
    SIM_RemapSPI0ToPTE_0_12_3(); //    remap SPI0 to pin PTE01/2/3
    SPI_APP_Init();

    /* KBI(sleep wake up pin) init */
    Sleep_Pin_Init();

#if 1
    SPI_Write(DEV_ADD, LOCK_ADDR, 0xe3b5);
    SPI_Write(DEV_ADD, GPIOSEL_ADDR, 0X2040);  //set the funtion of gpio4
    SPI_Write(DEV_ADD, GPIO_CTRL1_ADDR, 0X3700);    // set the gpio4 output
    SPI_Write(DEV_ADD, ADCTRL2_ADDR, 0X1032);    // set the low speed current adc enable
    SPI_Write(DEV_ADD, LOCK_ADDR, 0X0000);
    //GPIO_Init(GPIOA, GPIO_PTC3_MASK,GPIO_PinInput);
#endif

    /* I2C init */
    IIC_Init();

    while(1)
    {
        //for(i=0;i<0xffff;i++) WDT_Clear();
        WDT_Clear();
        //wait();    //enter the sleep mode
        stop();  // enter the deep sleep mode
#if 0
        if( GPIO_Read(GPIOA) & GPIO_PTC3_MASK)
        {
            SPI_Write(DEV_ADD, LOCK_ADDR, 0xe3b5);
            SPI_Write(DEV_ADD, STAT_ADDR, 0X0004);
            SPI_Write(DEV_ADD, OP_MODE_ADDR, 0X4004);
            SPI_Write(DEV_ADD, LOCK_ADDR, 0X0000);
        }
#endif
    }
}
