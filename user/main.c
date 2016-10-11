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
#include "can_app.h"

#if 0

#include "rtc.h"


void RTC_Task(void);
#endif

#if 1
int main(void)
{
    uint32_t i;
    //uint16_t data=0;
    uint8_t i2c_buff[24]={0x30,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
    //uint8_t buff[3];
    /* Set the system clock */
    sysinit();

    /* WDT init */
    WDT_Init(1000);

    /* Led init */
    RED_Init();
    GREEN_Init();
    //GREEN_On();
    //RED_On();

    /* SPI0 init */
    SIM_RemapSPI0ToPTE_0_12_3(); //    remap SPI0 to pin PTE01/2/3
    SPI_APP_Init();

    /* KBI(sleep wake up pin) init */
    Sleep_Pin_Init();

    /* Can init */
    CAN_APP_Init();

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
        //for(i=0;i<0xffff;i++); WDT_Clear();
        WDT_Clear();
	 //RED_Toggle();
        CAN_APP_Send(1, i2c_buff);
        //CAN_APP_Rive();
        //wait();    //enter the sleep mode
        //stop();  // enter the deep sleep mode
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
#endif

#if 0

int main (void)
{
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;  



    uint8_t i;
    volatile uint32_t u32Delay;

    /* Perform processor initialization */
	sysinit();

    GREEN_Init();

    /* configure RTC to 1Hz interrupt frequency */
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/

    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);
    

    



    while(1)
    {
        CAN_CheckSendBufferFrame(MSCAN,&sCAN_TxBuff);
        if(CAN_ReadOneFramefromBufferQueue(&sRxFrameInfo))
        {
            sCANTxItemInfo.ID_Type.ID = NODE_ID2;
            sCANTxItemInfo.u32DataLength = sRxFrameInfo.DLR;
            sCANTxItemInfo.bIsExtOrStand = sRxFrameInfo.bIsExtOrStand;
            sCANTxItemInfo.bIsRemoteFrame = 0;
            memcpy((void *)&sCANTxItemInfo.u8DataBuff[0],
                   (void *)&sRxFrameInfo.DSR[0],
                   sRxFrameInfo.DLR);
            CAN_SendItemToBuffer(MSCAN,&sCANTxItemInfo,&sCAN_TxBuff);
        }
        //CAN_SendItemToBuffer(MSCAN,&sCANTxItemInfo,&sCAN_TxBuff);
    } 
}

void RTC_Task(void)
{
    /* toggle LED1 */
    GREEN_Toggle();
}
#endif


