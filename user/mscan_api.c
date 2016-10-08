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
* @file mscan_api.c
*
* @author Freescale
*
* @version 0.0.1
*
* @date Oct. 25, 2013
*
* @brief providing APIs for configuring MSCAN module (MSCAN). 
*
*******************************************************************************
*
* provide APIs for MSCAN application (MSCAN APIs).
******************************************************************************/
#include "common.h"
#include "mscan.h"
#include "mscan_api.h"
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

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


/******************************************************************************
* Global functions
******************************************************************************/
FrameBufferInfoType	sCAN_TxBuff;
//static FrameBufferInfoType	sCAN_RxBuff;
MSCAN_FrameType      sRxFrame[CAN_BUFFER_LENGTH];
uint8_t u8RxFrameBufferIndex;
uint8_t u8RxFrameHeader;
uint8_t u8RxFrameBufferFreeLength;

uint32_t u32RxInterruptCounter=0;

/******************************************************************************
* define MSCAN APIs
*
*//*! @addtogroup mscan_api_list
* @{
*******************************************************************************/



/*****************************************************************************//*!
   *
   * @brief Write a frame data to buffers
   *        
   * @param[in] pCANx      point to CAN module type.
   * @param[in] pTxItemInfo point to CAN transmitting data information
   * @param[in] pTxBuffInfo  point to CAN transmitting Buffer information
   *
   * @return send status, fail or success
   *
   * @ Pass/ Fail criteria: none
*****************************************************************************/
uint8_t CAN_SendItemToBuffer(MSCAN_Type *pCANx,ItemInfoPtr pTxItemInfo,FrameBufferInfoPtr pTxBuffInfo)
{
	IDR1_3_UNION sIDR1;
	IDR1_3_UNION sIDR3;
	uint32_t u32Index = 0;

	if((pTxBuffInfo->u8FreeLength == 0)||		
	  ((pTxBuffInfo->u8FreeLength*8)<pTxItemInfo->u32DataLength))
	{
		// no free space for save
		return FALSE;
	}
    // disable interrupt
    DisableInterrupts;
    
	if(pTxBuffInfo->u8Index == CAN_BUFFER_LENGTH)
	{
		pTxBuffInfo->u8Index = 0;
	}
	
	if(pTxItemInfo->bIsExtOrStand)
	{
		sIDR1.IDR1.EID20_18_OR_SID2_0 = pTxItemInfo->ID_Type.ExtID.EID20_18;
		sIDR1.IDR1.R_TSRR = 1;
		sIDR1.IDR1.R_TEIDE = 1;
		sIDR1.IDR1.EID17_15 = pTxItemInfo->ID_Type.ExtID.EID17_15;
		sIDR3.IDR3.EID6_0 = pTxItemInfo->ID_Type.ExtID.EID6_0;
		if(pTxItemInfo->bIsRemoteFrame)
		{
			sIDR3.IDR3.ERTR = 1;

			// don't need to send data frame
			u32Index = pTxItemInfo->u32DataLength;
		}
		else
		{
			sIDR3.IDR3.ERTR = 0;
		}
		
	}
	else
	{
		sIDR1.IDR1.EID20_18_OR_SID2_0 = pTxItemInfo->ID_Type.ExtID.EID20_18;
		sIDR1.IDR1.R_TSRR = 0;
		sIDR1.IDR1.R_TEIDE = 0;
		sIDR1.IDR1.EID17_15 = pTxItemInfo->ID_Type.ExtID.EID17_15;
	}

	while(u32Index < pTxItemInfo->u32DataLength)
	{
		// save EIDR0-EIDR3
		pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].EIDR0 = 
					pTxItemInfo->ID_Type.ExtID.EID28_21;
		pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].EIDR1 = sIDR1.Bytes;
		if(pTxItemInfo->bIsExtOrStand)
		{
			pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].EIDR2 = 
						pTxItemInfo->ID_Type.ExtID.EID14_7;
			pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].EIDR3 = sIDR3.Bytes;
		}
		
		if((u32Index + 8)<= pTxItemInfo->u32DataLength)
		{
			memcpy((void *)&pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].EDSR[0],
					(void *)&pTxItemInfo->u8DataBuff[u32Index],8);
					pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].DLR = 8;
			u32Index += 8;	
		}
		else
		{
			memcpy((void *)&pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].EDSR[0],
					(void *)&pTxItemInfo->u8DataBuff[u32Index],
                      pTxItemInfo->u32DataLength - u32Index );
					pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].DLR = 
					                          pTxItemInfo->u32DataLength - u32Index;
			u32Index = pTxItemInfo->u32DataLength;	
		}
		pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Index].BPR = pTxItemInfo->u8BPR; 

		pTxBuffInfo->u8FreeLength --;
		pTxBuffInfo->u8Index ++;
		if(pTxBuffInfo->u8Index == CAN_BUFFER_LENGTH)
		{
			pTxBuffInfo->u8Index = 0;
		}
	}
    // enable interrupt
    EnableInterrupts;
    
	return TRUE;
}
/*****************************************************************************//*!
   *
   * @brief Write a frame data to buffers
   *        
   * @param[in] pCANx      point to CAN module type.
   * @param[in] pTxItemInfo point to CAN transmitting data information
   * @param[in] pTxBuffInfo  point to CAN transmitting Buffer information
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
*****************************************************************************/
uint8_t CAN_TransmitItemByInt(MSCAN_Type *pCANx,ItemInfoPtr pTxItemInfo,FrameBufferInfoPtr pTxBuffInfo)
{
    if(!CAN_SendItemToBuffer(pCANx,pTxItemInfo,pTxBuffInfo)) 
	{
		// no information in buffer pools
		return FALSE;
	}
    
    // disable interrupt
    DisableInterrupts;
    
    CAN_TransmitterEmptyIntEn(MSCAN);
    // enable interrupt
    EnableInterrupts;
    
    return TRUE;
}
/*****************************************************************************//*!
   *
   * @brief Check buffer status and send data frame to transmitting buffer of MSCAN
   *        
   * @param[in] pCANx      point to CAN module type.
   * @param[in] pTxBuffInfo  point to CAN transmitting Buffer information
   *
   * @return false- no buffer transmitted, true -transmitting is busy
   *
   * @ Pass/ Fail criteria: none
*****************************************************************************/
uint8_t CAN_CheckSendBufferFrame(MSCAN_Type *pCANx,FrameBufferInfoPtr pTxBuffInfo)
{

	if(pTxBuffInfo->u8FreeLength==CAN_BUFFER_LENGTH) 
	{
		// no information in buffer pools
		return FALSE;
	}
    // disable interrupt
    DisableInterrupts;
	// check transmitter Buffer of MSCAN
	while((pTxBuffInfo->u8Head != pTxBuffInfo->u8Index)||(pTxBuffInfo->u8FreeLength == 0))
	{
		if(CAN_LoadOneFrameToBuff(pCANx,&pTxBuffInfo->sFrameRegisterBuffer[pTxBuffInfo->u8Head]))
		{
			pTxBuffInfo->u8Head++;
			if(pTxBuffInfo->u8Head >= CAN_BUFFER_LENGTH)
			{
				pTxBuffInfo->u8Head = 0;
			}
			pTxBuffInfo->u8FreeLength++;
		}
		else
		{
            // enable interrupt
            EnableInterrupts;
			return TRUE;
		}
	}
    
    // enable interrupt
    EnableInterrupts;
    
	return TRUE;
}

/*****************************************************************************//*!
   *
   * @brief Check buffer status and receive data frame from receiver buffer of MSCAN
   *        
   * @param[in] pTxBuffInfo  point to CAN transmitting Buffer information
   *
   * @return false- no buffer transmitted, true -transmitting is busy
   *
   * @ Pass/ Fail criteria: none
*****************************************************************************/
uint8_t CAN_ReadOneFramefromBufferQueue(MSCAN_FramePtr pRxFrameInfo)
{
    // disable interrupt
    DisableInterrupts;
    if( (u8RxFrameHeader!=u8RxFrameBufferIndex)||
        (u8RxFrameBufferFreeLength == 0) )
    {
        *pRxFrameInfo = sRxFrame[u8RxFrameHeader++];
        if(u8RxFrameHeader >= CAN_BUFFER_LENGTH)
        {
            u8RxFrameHeader = 0;
        }
        u8RxFrameBufferFreeLength ++;
    }
    else
    {
        // enable interrupt
        EnableInterrupts;
        return FALSE;
    }
    // enable interrupt
    EnableInterrupts;
    return TRUE;
}

/*****************************************************************************//*!
   *
   * @brief Initialize the globe variable for CAN buffer and buffer Queue
   *        
   * @param[in] pCANx      point to CAN module type.
   *
   * @return none.
   *
   * @ Pass/ Fail criteria:  none.
*****************************************************************************/

void MSCAN_GlobeVaribleInit( MSCAN_Type *pCANx )
{
    sCAN_TxBuff.u8FreeLength = CAN_BUFFER_LENGTH;
    sCAN_TxBuff.u8Index = 0;
    sCAN_TxBuff.u8Head = 0; 
    u8RxFrameBufferFreeLength = CAN_BUFFER_LENGTH;
    u8RxFrameBufferIndex = 0;
	u8RxFrameHeader = 0;
    MSCAN_SetRxCallBack(MSCAN_RxProcessing);
    MSCAN_SetTxCallBack(MSCAN_TxProcessing);
}

/*****************************************************************************//*!
   *
   * @brief MSCAN Transmitting Callback function
   *        
   * @param  
   *
   * @return none
   *
   * @ Pass/ Fail criteria:  none
*****************************************************************************/
void MSCAN_TxProcessing( void )
{
    if(CAN_IsOverRunFlag(MSCAN))
   	{
		// overrunn error occur
		CAN_ClearOVRIF_Flag(MSCAN);
   	}
   	if(CAN_IsWakeUpIntFlag(MSCAN))
   	{
		CAN_ClearWUPIF_Flag(MSCAN);

		// user processing
		
   	}
   	if(CAN_IsStatusChangeFlag(MSCAN))
   	{
		CAN_ClearCSCIF_Flag(MSCAN);

		// Get currently status
		CAN_GetReceiverStatus(MSCAN);
		CAN_GetReceiveErrorCount(MSCAN);

		// user processing
		
   	}
	if(!CAN_CheckSendBufferFrame(MSCAN,&sCAN_TxBuff))
	{
		// no data in transmitting buffer,disbale interrupt
		CAN_TransmitterEmptyIntDisable(MSCAN);
	}
}
/*****************************************************************************//*!
   *
   * @brief MSCAN receiving Callback function
   *        
   * @param  
   *
   * @return none
   *
   * @ Pass/ Fail criteria:  none
*****************************************************************************/
void MSCAN_RxProcessing( void )
{
    u32RxInterruptCounter ++;
   	if(CAN_IsRxBuffFull(MSCAN))
   	{
   		if(u8RxFrameBufferFreeLength!=0)
        {
            CAN_ReadOneFrameFromBuff(MSCAN,&sRxFrame[u8RxFrameBufferIndex++]);	
            if(u8RxFrameBufferIndex>=CAN_BUFFER_LENGTH)
            {
                u8RxFrameBufferIndex = 0;
            }
            u8RxFrameBufferFreeLength--;

        }
        else
        {
            //receive frame buffer is full, clear buffer in MSCAN
            // clear receiver full flag
            CAN_ClearRXF_Flag(MSCAN);
        }
   	}
}





