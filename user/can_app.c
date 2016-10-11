 /******************************************************************************
  * @file       USER/can_app.c
  * @author  cjw
  * @date     2016.10.8
  * @brief     This file contains the source of the Can_app.
  ******************************************************************************/

#include "can_app.h"
#include "gpio.h"

/**
  * @brief  This function is Init CAN.
  * @param  None
  * @retval None
  */

void CAN_APP_Init(void)
{
    MSCAN_ConfigType sMSCANConfig = {0};

    GPIO_Init(GPIOA, GPIO_PTD0_MASK, GPIO_PinOutput);
    GPIO_PinClear(GPIO_PTD0);

    //CAN_DeInit(MSCAN);
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
    sMSCANConfig.sSetting.bTxEmptyIEn=1;
    CAN_Init(MSCAN,&sMSCANConfig);

}

/**
  * @brief  This function is CAN_APP_Send.
  * @param  uint8_t length ,  uint8_t* data
  * @retval None
  */

void CAN_APP_Send(uint8_t length ,  uint8_t* data)
{
    ItemInfoType     sCANTxItemInfo;

    //Initialize the Tx information
    sCANTxItemInfo.bIsExtOrStand = 1;
    sCANTxItemInfo.bIsRemoteFrame = 0;
    sCANTxItemInfo.ID_Type.ID = NODE_ID2;
    sCANTxItemInfo.u32DataLength = length;
    memcpy(sCANTxItemInfo.u8DataBuff , data , length);
    sCANTxItemInfo.u8BPR = 0x80;

    CAN_SendItemToBuffer(MSCAN,&sCANTxItemInfo,&sCAN_TxBuff);

    CAN_CheckSendBufferFrame(MSCAN,&sCAN_TxBuff);

}

/**
  * @brief  This function is CAN_APP_Rive.
  * @param  uint8_t* data
  * @retval None
  */

void CAN_APP_Rive(void)
{
    MSCAN_FrameType  sRxFrameInfo;
    ItemInfoType     sCANTxItemInfo;

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

	 CAN_CheckSendBufferFrame(MSCAN,&sCAN_TxBuff);
    }
}


