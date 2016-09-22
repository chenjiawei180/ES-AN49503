 /******************************************************************************
  * @file       USER/wdt.c
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the source of the Watch Dog Timer.
  ******************************************************************************/

#include "wdt.h"

/**
  * @brief  This function is Init Watch Dog Timer and default is 1s reset without wdt clear.
  * @param  None
  * @retval None
  */

void WDT_Init(uint16_t timeout)
{
    WDOG_ConfigType sWDOGConfig = {0};    /*! < watchdog configuration structure */

    sWDOGConfig.sBits.bWaitEnable   = TRUE;
    sWDOGConfig.sBits.bStopEnable   = TRUE;
    sWDOGConfig.sBits.bDbgEnable    = TRUE;
    sWDOGConfig.sBits.bUpdateEnable = FALSE;
    sWDOGConfig.sBits.bDisable      = FALSE;        /* enable WDOG */
    sWDOGConfig.sBits.bClkSrc       = WDOG_CLK_INTERNAL_1KHZ;
    sWDOGConfig.u16TimeOut          = timeout;  /*< 1s */
    sWDOGConfig.u16WinTime          = 0;  

    WDOG_Init(&sWDOGConfig);
}

/**
  * @brief  This function is Clear Watch Dog Timer.
  * @param  None
  * @retval None
  */
  
void WDT_Clear(void)
{
    WDOG_Feed();
}


/**
  * @brief  This function is only stays in endless loop so as to generate watchdog reset .
  * @param  None
  * @retval None
  */
  
void NMI_Isr(void)
{
    /*!
     * endless loop to generate watchdog reset
     */
    while(1)
    {
        //printf("Running in NMI_Isr()!\n");
    }
}

