 /******************************************************************************
  * @file       USER/sleep.c
  * @author  cjw
  * @date     2016.9.29
  * @brief     This file contains the source of the sleep.
  ******************************************************************************/

#include "sleep.h"
#include "spi_app.h"
#include "led.h"
#include "gpio.h"

/**
  * @brief  This function is Sleep_Pin_Init.
  * @param  None
  * @retval None
  */

void Sleep_Pin_Init(void)
{
    uint32_t i;
    KBI_ConfigType sKBIConfig={0};

    /* Disable all the KBI pins */
    for (i = 0; i < KBI_MAX_PINS_PER_PORT; i++)
    {
        sKBIConfig.sPin[i].bEn   = 0;
    }
    sKBIConfig.sBits.bRstKbsp   = 1;/*Writing a 1 to RSTKBSP is to clear the KBIxSP Register*/
    sKBIConfig.sBits.bKbspEn   = 1;/*The latched value in KBxSP register while interrupt flag occur to be read.*/
    sKBIConfig.sBits.bMode   = KBI_MODE_EDGE_ONLY;
    sKBIConfig.sBits.bIntEn  = 1;

    sKBIConfig.sPin[GPIO_PTC3].bEdge = KBI_RISING_EDGE_HIGH_LEVEL;
    sKBIConfig.sPin[GPIO_PTC3].bEn   = 1;

    KBI_Init(KBI0, &sKBIConfig);

    KBI_SetCallback(KBI0, &Sleep_Wake_Up);

}

/**
  * @brief  This function is Sleep_Wake_Up.
  * @param  None
  * @retval None
  */
  
void Sleep_Wake_Up(void)
{
    if(KBI_GetSP(KBI0)== GPIO_PTC3_MASK)
    {
        SPI_Write(DEV_ADD, LOCK_ADDR, 0xe3b5);
        SPI_Write(DEV_ADD, STAT_ADDR, 0X0004);
        SPI_Write(DEV_ADD, OP_MODE_ADDR, 0X4004);
        SPI_Write(DEV_ADD, LOCK_ADDR, 0X0000);
        GREEN_Toggle();    
    }
}


