/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file gpio.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun 12, 2013
*
* @brief providing common gpio API. 
*
******************************************************************************/
#include "gpio.h"

/******************************************************************************
* Local variables
******************************************************************************/


/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local functions
*****************************************************************************/

/******************************************************************************
* Global functions
******************************************************************************/

/******************************************************************************
* define GPIO APIs
*
*//*! @addtogroup gpio_api_list
* @{
*******************************************************************************/

/*****************************************************************************//*!
* @brief    Initialize the GPIO registers to the default reset values.
*        
* @param[in] pGPIO      Pointer to GPIO module, can be GPIOA/GPIOB.
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_DeInit(GPIO_Type *pGPIO)
{
    /* Sanity check */
#if defined(CPU_KE02)
     ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));
#endif
#if defined(CPU_KE04)
    ASSERT(pGPIO == GPIOA);
#endif
#if defined(CPU_KE06)
      ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB) || (pGPIO == GPIOC));
#endif

    pGPIO->PCOR = 0x00000000;   /* Port Clear Output Register */
    pGPIO->PDDR = 0x00000000;   /* Port Data Direction */
    //pGPIO->PDIR = 0x00000000;   /* Port Data Input Register */
    pGPIO->PDOR = 0x00000000;   /* Port Data Output Register */
    pGPIO->PIDR = 0xFFFFFFFF;   /* Port Input Disable Register */
    pGPIO->PSOR = 0x00000000;   /* Port Set Output Register */
    pGPIO->PTOR = 0x00000000;   /* Port Toggle Output Register */
}

/*****************************************************************************//*!
* @brief    Initialize GPIO pins which are specified by u32PinMask
*        
* @param[in] pGPIO      Pointer to GPIO module, can be GPIOA/GPIOB.
* @param[in] u32PinMask GPIO pin mask need to be set
* @param[in] sGpioType  pin attribute 
*
* @return   none
*
* @Note
*   . High-current drive function is disabled, if the pin is configured as an input
*   . Internal pullup is disabled if the pin is configured as an output
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_Init(GPIO_Type *pGPIO, uint32_t u32PinMask, GPIO_PinConfigType sGpioType)
{
    /* Sanity check */
#if defined(CPU_KE02)
     ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));
#endif
#if defined(CPU_KE04)
    ASSERT(pGPIO == GPIOA);
#endif
#if defined(CPU_KE06)
      ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB) || (pGPIO == GPIOC));
#endif
    
    /* Config GPIO for Input or Output */
    if ((sGpioType == GPIO_PinOutput) || (sGpioType == GPIO_PinOutput_HighCurrent))
    {
        pGPIO->PDDR |= u32PinMask;      /* Enable Port Data Direction Register */
        pGPIO->PIDR |= u32PinMask;      /* Set Port Input Disable Register */   
    }
    else if ((sGpioType == GPIO_PinInput) || (sGpioType == GPIO_PinInput_InternalPullup))
    {
        pGPIO->PDDR &= ~u32PinMask;   /* Disable Port Data Direction Register */
        pGPIO->PIDR &= ~u32PinMask;   /* Clear Port Input Disable Register */
    }
    /* Config PORT Pull select for GPIO */
#if defined(CPU_KE02)
    switch((uint32_t)pGPIO)
    {
        case GPIOA_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUEL |= u32PinMask):(PORT->PUEL &= ~u32PinMask);
            break;
        case GPIOB_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUEH |= u32PinMask):(PORT->PUEH &= ~u32PinMask);
            break;
        default:
            break;
     }
#endif
    
#if defined(CPU_KE04)
    switch((uint32_t)pGPIO)
    {
        case GPIOA_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUEL |= u32PinMask):(PORT->PUEL &= ~u32PinMask);
            break;
        default:
            break;
     }
#endif
    
#if defined(CPU_KE06)
    switch((uint32_t)pGPIO)
    {
        case GPIOA_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUE0 |= u32PinMask):(PORT->PUE0 &= ~u32PinMask);
            break;
        case GPIOB_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUE1 |= u32PinMask):(PORT->PUE1 &= ~u32PinMask);
            break;
        case GPIOC_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUE2 |= u32PinMask):(PORT->PUE2 &= ~u32PinMask);
            break;
        default:
            break;
     }
#endif   
    
    /* Config PORT GPIO_PinOutput_HighCurrent for GPIO */
#if defined(CPU_KE04)
    if (u32PinMask & GPIO_PTC5_MASK)
    {   
        PORT->HDRVE |= PORT_HDRVE_PTC5_MASK;
    }   
    if (u32PinMask & GPIO_PTC1_MASK)
    {   
        PORT->HDRVE |= PORT_HDRVE_PTC1_MASK;
    }   
    if (u32PinMask & GPIO_PTB5_MASK)
    {   
        PORT->HDRVE |= PORT_HDRVE_PTB5_MASK;
    }   
#endif
    
#if defined(CPU_KE02) | defined(CPU_KE06)
    if (pGPIO == GPIOA)
    {
        if (u32PinMask & GPIO_PTB4_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTB4_MASK;
        }   
        if (u32PinMask & GPIO_PTB5_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTB5_MASK;
        }   
        if (u32PinMask & GPIO_PTD0_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTD0_MASK;
        }   
        if (u32PinMask & GPIO_PTD1_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTD1_MASK;
        }   
    }
    if (pGPIO == GPIOB)
    {
        if (u32PinMask & GPIO_PTE0_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTE0_MASK;
        }   
        if (u32PinMask & GPIO_PTE1_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTE1_MASK;
        }   
        if (u32PinMask & GPIO_PTH0_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTH0_MASK;
        }   
        if (u32PinMask & GPIO_PTH1_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTH1_MASK;
        }   
    }

#endif
    
}

/*****************************************************************************//*!
* @brief    Toggle the pins which are specified by u32PinMask
*        
* @param[in] pGPIO       Pointer to GPIO module, can be GPIOA/GPIOB.
* @param[in] u32PinMask  Specify GPIO pin need to be toggled
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_Toggle(GPIO_Type *pGPIO, uint32_t u32PinMask)
{
    /* Sanity check */
#if defined(CPU_KE02)
     ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));
#endif
#if defined(CPU_KE04)
    ASSERT(pGPIO == GPIOA);
#endif
#if defined(CPU_KE06)
      ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB) || (pGPIO == GPIOC));
#endif

    pGPIO->PTOR = u32PinMask;   /* Toggle the pins specified by u32PinMask */
}

/*****************************************************************************//*!
* @brief Read input data from GPIO which is specified by pGPIO
*        
* @param[in] pGPIO       Pointer to GPIO module, can be GPIOA/GPIOB.
*
* @return   GPIO input value unsigned int 32-bit
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
uint32_t GPIO_Read(GPIO_Type *pGPIO)
{
    /* Sanity check */
#if defined(CPU_KE02)
     ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));
#endif
#if defined(CPU_KE04)
    ASSERT(pGPIO == GPIOA);
#endif
#if defined(CPU_KE06)
      ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB) || (pGPIO == GPIOC));
#endif

    return (pGPIO->PDIR);   /* Read Port Data Input Register */
  
}

/*****************************************************************************//*!
* @brief    Write output data to GPIO which is specified by pGPIO
*        
* @param[in] pGPIO       Pointer to GPIO module, can be GPIOA/GPIOB.
* @param[in] u32Value    value to output
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_Write(GPIO_Type *pGPIO, uint32_t u32Value)
{
    /* Sanity check */
#if defined(CPU_KE02)
     ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB));
#endif
#if defined(CPU_KE04)
    ASSERT(pGPIO == GPIOA);
#endif
#if defined(CPU_KE06)
      ASSERT((pGPIO == GPIOA) || (pGPIO == GPIOB) || (pGPIO == GPIOC));
#endif
    
    pGPIO->PDOR = u32Value;    /* Write Port Ouput Data Register */
  
}

/*****************************************************************************//*!
* @brief    Initialize GPIO single pin which is specified by GPIO_Pin
*        
* @param[in] GPIO_Pin        GPIO pin name, can be GPIO_PTA0,1 ...
* @param[in] GPIO_PinConfig  Config output or input
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_PinInit(GPIO_PinType GPIO_Pin, GPIO_PinConfigType GPIO_PinConfig)
{
    /* Sanity check */
    ASSERT(GPIO_Pin <= GPIO_PTI7);
      
    /* Config GPIO and pull select*/
#if defined(CPU_KE02)
      if (GPIO_Pin < GPIO_PTE0)
      {
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOA->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOA->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUEL &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput:
            GPIOA->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOA->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUEL &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput_InternalPullup:
            GPIOA->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOA->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUEL |= (1<<GPIO_Pin);    /* Enable Pullup */
          break;
        case GPIO_PinOutput_HighCurrent:
            GPIOA->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOA->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUEL &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        }
      }
      else if (GPIO_Pin < GPIO_PTI0)
      {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 32);
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOB->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOB->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUEH &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput:
            GPIOB->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOB->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUEH &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput_InternalPullup:
            GPIOB->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOB->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUEH |= (1<<GPIO_Pin);    /* Enable Pullup */
          break;
        case GPIO_PinOutput_HighCurrent:
            GPIOB->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOB->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUEH &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        }
      }
#endif
      
#if defined(CPU_KE04)
      if (GPIO_Pin < GPIO_PTE0)
      {
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOA->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOA->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUEL &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput:
            GPIOA->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOA->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUEL &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput_InternalPullup:
            GPIOA->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOA->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUEL |= (1<<GPIO_Pin);    /* Enable Pullup */
          break;
        case GPIO_PinOutput_HighCurrent:
            GPIOA->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOA->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUEL &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        }
      }
#endif
    
    
#if defined(CPU_KE06)
      if (GPIO_Pin < GPIO_PTE0)
      {
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOA->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOA->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUE0 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput:
            GPIOA->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOA->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUE0 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput_InternalPullup:
            GPIOA->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOA->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUE0 |= (1<<GPIO_Pin);    /* Enable Pullup */
          break;
        case GPIO_PinOutput_HighCurrent:
            GPIOA->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOA->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUE0 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        }
      }
      else if (GPIO_Pin < GPIO_PTI0)
      {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 32);
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOB->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOB->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUE1 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput:
            GPIOB->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOB->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUE1 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput_InternalPullup:
            GPIOB->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOB->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUE1 |= (1<<GPIO_Pin);    /* Enable Pullup */
          break;
        case GPIO_PinOutput_HighCurrent:
            GPIOB->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOB->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUE1 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        }
      }
      else
      {
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 64);
        switch (GPIO_PinConfig)
        {
        case GPIO_PinOutput:
            GPIOC->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOC->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUE2 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput:
            GPIOC->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOC->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUE2 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        case GPIO_PinInput_InternalPullup:
            GPIOC->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
            GPIOC->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
            PORT->PUE2 |= (1<<GPIO_Pin);    /* Enable Pullup */
          break;
        case GPIO_PinOutput_HighCurrent:
            GPIOC->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
            GPIOC->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
            PORT->PUE2 &= ~(1<<GPIO_Pin);    /* Disable Pullup */
          break;
        }
      }
#endif
      
    /* Config GPIO HDRV */
    if(GPIO_PinConfig == GPIO_PinOutput_HighCurrent)
    {
#if defined(CPU_KE04)
        switch (GPIO_Pin)
        {
            case GPIO_PTB5:
                PORT->HDRVE |= PORT_HDRVE_PTB5_MASK;
                break;
            case GPIO_PTC1:
                PORT->HDRVE |= PORT_HDRVE_PTC1_MASK;
                break;
            case GPIO_PTC5:
                PORT->HDRVE |= PORT_HDRVE_PTC5_MASK;
                break;
            default:
                break;
        }
#endif
    
#if defined(CPU_KE06) | defined(CPU_KE02)
        switch (GPIO_Pin)
        {
            case GPIO_PTB4:
                PORT->HDRVE |= PORT_HDRVE_PTB4_MASK;
                break;
            case GPIO_PTB5:
                PORT->HDRVE |= PORT_HDRVE_PTB5_MASK;
                break;
            case GPIO_PTD0:
                PORT->HDRVE |= PORT_HDRVE_PTD0_MASK;
                break;
            case GPIO_PTD1:
                PORT->HDRVE |= PORT_HDRVE_PTD1_MASK;
                break;
            case GPIO_PTE0:
                PORT->HDRVE |= PORT_HDRVE_PTE0_MASK;
                break;
            case GPIO_PTE1:
                PORT->HDRVE |= PORT_HDRVE_PTE1_MASK;
                break;
            case GPIO_PTH0:
                PORT->HDRVE |= PORT_HDRVE_PTH0_MASK;
                break;
            case GPIO_PTH1:
                PORT->HDRVE |= PORT_HDRVE_PTH1_MASK;
                break;
            default:
                break; 
        }
#endif
    }
}

/*****************************************************************************//*!
* @brief    Toggle GPIO single pin which is specified by GPIO_Pin
*        
* @param[in] GPIO_Pin        GPIO pin name, can be GPIO_PTA0,1 ...
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_PinToggle(GPIO_PinType GPIO_Pin)
{
    /* Sanity check */
    ASSERT(GPIO_Pin <= GPIO_PTI7);

    if (GPIO_Pin < GPIO_PTE0)
    {
        /* PTA0-7, PTB0-7, PTC0-7, PTD0-7 */
        GPIOA->PTOR = (1<<GPIO_Pin);
    }

#if (defined(CPU_KE02) | defined(CPU_KE06))

    else if (GPIO_Pin < GPIO_PTI0)
    {
        /* PTE0-7, PTF0-7, PTH0-7, PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PTOR = (1<<GPIO_Pin);
    }
#endif

#if defined(CPU_KE06)  
    else if(GPIO_Pin < GPIO_PIN_MAX)
    {
        /* PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTI0);
        GPIOC->PTOR = (1<<GPIO_Pin);
    }
#endif
}

/*****************************************************************************//*!
* @brief    Set GPIO single pin which is specified by GPIO_Pin
*        
* @param[in] GPIO_Pin        GPIO pin name, can be GPIO_PTA0,1 ...
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_PinSet(GPIO_PinType GPIO_Pin)
{
    /* Sanity check */
    ASSERT(GPIO_Pin <= GPIO_PTI7);
    
    if (GPIO_Pin < GPIO_PTE0)
    {
        /* PTA0-7, PTB0-7, PTC0-7, PTD0-7 */
        GPIOA->PSOR = (1<<GPIO_Pin);
    }

#if (defined(CPU_KE02) | defined(CPU_KE06))

    else if (GPIO_Pin < GPIO_PTI0)
    {
        /* PTE0-7, PTF0-7, PTH0-7, PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PSOR = (1<<GPIO_Pin);
    }
#endif

#if defined(CPU_KE06) 
    else if(GPIO_Pin < GPIO_PIN_MAX)
    {
        /* PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTI0);
        GPIOC->PSOR = (1<<GPIO_Pin);
    }
#endif
}

/*****************************************************************************//*!
* @brief    Clear GPIO single pin which is specified by GPIO_Pin
*        
* @param[in] GPIO_Pin        GPIO pin name, can be GPIO_PTA0,1 ...
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_PinClear(GPIO_PinType GPIO_Pin)
{
    /* Sanity check */
    ASSERT(GPIO_Pin <= GPIO_PTI7);
    
    if (GPIO_Pin < GPIO_PTE0)
    {
        /* PTA0-7, PTB0-7, PTC0-7, PTD0-7 */
        GPIOA->PCOR = (1<<GPIO_Pin);
    }

#if (defined(CPU_KE02) | defined(CPU_KE06))

    else if (GPIO_Pin < GPIO_PTI0)
    {
        /* PTE0-7, PTF0-7, PTH0-7, PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PCOR = (1<<GPIO_Pin);
    }
#endif

#if defined(CPU_KE06)
    else if(GPIO_Pin < GPIO_PIN_MAX)
    {
        /* PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTI0);
        GPIOC->PCOR = (1<<GPIO_Pin);
    }
#endif
}

/*! @} End of gpio_api_list */

