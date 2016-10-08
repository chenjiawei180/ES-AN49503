
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
* @file CAN_Node1_demo.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Oct. 15, 2013
*
* @brief providing framework of demo cases for MCU. 
*
*******************************************************************************/

#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "sim.h"
#include "mscan.h"
#include "mscan_api.h"
#include "sysinit.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define NODE_ID1			0x801
#define NODE_ID2			0x802
#define NODE_ID3			0x803

/* define MACRO for filter,here define a dual filter to receive 
   extend and standard frame from ID(NODE_ID2&NODE_ID3) */
#define CAN_IDAR0			NODE_ID2<<1
#define CAN_IDAR1			NODE_ID3<<1
#define CAN_IDMR0			0x1|(uint32_t)0x18<<16 /* received all standard and extend frame from ID(NODE_ID2) */
#define CAN_IDMR1			0x1|(uint32_t)0x18<<16 
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void RTC_Task(void);
/******************************************************************************
* Global functions
******************************************************************************/
int main (void)
{
    RTC_ConfigType  sRTCConfig;
    RTC_ConfigType  *pRTCConfig = &sRTCConfig;  
    MSCAN_ConfigType sMSCANConfig = {0};
    ItemInfoType     sCANTxItemInfo;
    MSCAN_FrameType  sRxFrameInfo;
    uint8_t i;
    volatile uint32_t u32Delay;

    /* Perform processor initialization */
	sysinit();

    printf("\nRunning the KE06 CAN_Node1_demo project.\r\n");
    LED0_Init();        //red led
    LED1_Init();        //green led
    LED2_Init();        //blue led
    
    LED0_Off();
    LED1_Off();
    LED2_Off();

    /* configure RTC to 1Hz interrupt frequency */
    pRTCConfig->u16ModuloValue = 9;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
    pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/

    RTC_SetCallback(RTC_Task);
    RTC_Init(pRTCConfig);
    
    MSCAN_GlobeVaribleInit(MSCAN);
    
    SIM->PINSEL1 |= SIM_PINSEL1_MSCANPS_MASK;
    
    sMSCANConfig.sBaudRateSetting.SJW = BAUD_RATE_SJW;
    sMSCANConfig.sBaudRateSetting.BRP = BAUD_RATE_BRP;
    sMSCANConfig.sBaudRateSetting.SAMP = BAUD_RATE_SAMP;
	sMSCANConfig.sBaudRateSetting.TSEG1= BAUD_RATE_TSEG1;
	sMSCANConfig.sBaudRateSetting.TSEG2= BAUD_RATE_TSEG2;
	sMSCANConfig.u32IDAR0 = (CAN_IDAR0);
	sMSCANConfig.u32IDAR1 = (CAN_IDAR1);
	sMSCANConfig.u32IDMR0 = (CAN_IDMR0);
	sMSCANConfig.u32IDMR1 = (CAN_IDMR1);
	sMSCANConfig.u8IDARMode = ID_ACCEPT_MODE_TWO32;
	sMSCANConfig.sSetting.bCanEn = 1;
    sMSCANConfig.sSetting.bCLKSRC = 1;
  //  sMSCANConfig.sSetting.bLoopModeEn = 1;
	sMSCANConfig.sSetting.bRxFullIEn=1;
	sMSCANConfig.sSetting.bTimerEn=1;
	sMSCANConfig.sSetting.bOverRunIEn=1;
	sMSCANConfig.sSetting.bStatusChangeIEn=1;
	//sMSCANConfig.sSetting.bTxEmptyIEn=1;
	CAN_Init(MSCAN,&sMSCANConfig);

	//Initialize the Tx information
	sCANTxItemInfo.bIsExtOrStand = 1;
	sCANTxItemInfo.bIsRemoteFrame = 0;
	sCANTxItemInfo.ID_Type.ID = NODE_ID1;
	sCANTxItemInfo.u32DataLength = 24;
	for(i=0;i<24;i++)
	{
		sCANTxItemInfo.u8DataBuff[i] = i;
	}
	sCANTxItemInfo.u8BPR = 0x80;

	CAN_SendItemToBuffer(MSCAN,&sCANTxItemInfo,&sCAN_TxBuff);

    while(1)
    {
        CAN_CheckSendBufferFrame(MSCAN,&sCAN_TxBuff);
        while(CAN_ReadOneFramefromBufferQueue(&sRxFrameInfo))
        {
			printf("receive a frame data!\r\n");
			printf("ID is 0x%x!\r\n",sRxFrameInfo.ID_Type);
			printf("Data length is %d\r\n",sRxFrameInfo.DLR);
			for(i=0;i<sRxFrameInfo.DLR;i++)
			{
				printf("0x%x,",sRxFrameInfo.DSR[i]);
			}
			printf("\r\n");
        }
        
        for(u32Delay=0;u32Delay<0xFFFFF;u32Delay++);
        sCANTxItemInfo.u8DataBuff[7]++;
        
        CAN_SendItemToBuffer(MSCAN,&sCANTxItemInfo,&sCAN_TxBuff);
    } 
}

/*****************************************************************************//*!
*
* @brief callback routine of RTC driver which does what you want to do at 
*        every RTC period.
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void RTC_Task(void)
{
    /* toggle LED1 */
    LED0_Toggle();
}
/********************************************************************/


