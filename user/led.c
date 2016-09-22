 /******************************************************************************
  * @file       USER/led.c
  * @author  cjw
  * @date     2016.9.22
  * @brief     This file contains the source of the Led.
  ******************************************************************************/

#include "led.h"

/**
  * @brief  This function is Init Red led and default is led off.
  * @param  None
  * @retval None
  */

void RED_Init(void)
{
    GPIO_Init(GPIOA, GPIO_PTB3_MASK, GPIO_PinOutput);
    GPIO_PinClear(GPIO_PTB3);
}

/**
  * @brief  This function is red led on.
  * @param  None
  * @retval None
  */
  
void RED_On(void)
{
    GPIO_PinSet(GPIO_PTB3);
}

/**
  * @brief  This function is red led off.
  * @param  None
  * @retval None
  */
  
void RED_Off(void)
{
    GPIO_PinClear(GPIO_PTB3);
}

/**
  * @brief  This function is red led toggle.
  * @param  None
  * @retval None
  */
  
void RED_Toggle(void)
{
    GPIO_Toggle(GPIOA, GPIO_PTB3_MASK);
}

/**
  * @brief  This function is Init Green led and default is led off.
  * @param  None
  * @retval None
  */
  
void GREEN_Init(void)
{
    GPIO_Init(GPIOA, GPIO_PTB2_MASK, GPIO_PinOutput);
    GPIO_PinClear(GPIO_PTB2);
}

/**
  * @brief  This function is Green led on.
  * @param  None
  * @retval None
  */
  
void GREEN_On(void)
{
    GPIO_PinSet(GPIO_PTB2);
}

/**
  * @brief  This function is Green led off.
  * @param  None
  * @retval None
  */
  
void GREEN_Off(void)
{
    GPIO_PinClear(GPIO_PTB2);
}

/**
  * @brief  This function is Green led toggle.
  * @param  None
  * @retval None
  */
  
void GREEN_Toggle(void)
{
    GPIO_Toggle(GPIOA, GPIO_PTB2_MASK);
}
