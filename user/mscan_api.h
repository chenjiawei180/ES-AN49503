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
* @file mscan_api.h
*
* @author Freescale
*
* @version 0.0.1
*
* @date Oct. 25, 2013
*
* @brief header file for MSCAN application utilities (MSCAN). 
*
*******************************************************************************
*
* provide APIs for MSCAN application (MSCAN APIs).
******************************************************************************/
#ifndef _MSCAN_API_H__
#define _MSCAN_API_H__
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
/******************************************************************************
* define MSCAN Frame Data Length Definition 
*
*//*! @addtogroup frame_data_length_list
* @{
*******************************************************************************/
#define MAX_DATA_LENGTH				64
#define MAX_SINGLE_FRAME_LENGTH		8

/*! @} End of frame_data_length_list											*/

/******************************************************************************
* define MSCAN Frame Buffer Length
*
*//*! @addtogroup frame_buffer_length_list
* @{
*******************************************************************************/
#define CAN_BUFFER_LENGTH		16	/*!< MSCAN Tx or Rx buffer max length */

/*! @} End of frame_buffer_length_list											*/


/******************************************************************************
* Local types
******************************************************************************/
/******************************************************************************
*
*//*! @addtogroup item_info_List
* @{
*******************************************************************************/
/*!
 * @brief Tx or RX data information by CAN
 *
 */
typedef struct
{
	union{
	MSCAN_StandardIDType StandID;
	MSCAN_ExtendIDType	 ExtID;
	uint32_t ID;
	}ID_Type;
	uint32_t u32DataLength;
	uint8_t u8DataBuff[MAX_DATA_LENGTH];	/*!>data buffer*/
	uint8_t u8BPR;							/*!>frame priority*/
	uint8_t bIsRemoteFrame;
	uint8_t bIsExtOrStand;
	uint8_t u8TSRH;
	uint8_t u8TSRL;
}ItemInfoType,*ItemInfoPtr;
/*! @} End of item_info_List									*/

/******************************************************************************
*
*//*! @addtogroup frame_buffer_info_List
* @{
*******************************************************************************/
/*!
 * @brief save tx data buffer information for transmitting or save the receiving data buffer 
 *
 */
typedef struct
{
	MSCAN_RegisterFrameType	sFrameRegisterBuffer[CAN_BUFFER_LENGTH];
	uint8_t u8Head;
	uint8_t u8Index;
	uint8_t u8FreeLength;
}FrameBufferInfoType,*FrameBufferInfoPtr;
/*! @} End of frame_buffer_info_List									*/


/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/

extern FrameBufferInfoType	sCAN_TxBuff;
//static FrameBufferInfoType	sCAN_RxBuff;
extern MSCAN_FrameType      sRxFrame[CAN_BUFFER_LENGTH];
extern uint8_t u8RxFrameBufferIndex;
extern uint8_t u8RxFrameHeader;
extern uint8_t u8RxFrameBufferFreeLength;
extern uint32_t u32RxInterruptCounter;
/******************************************************************************
* inline functions
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
uint8_t CAN_SendItemToBuffer(MSCAN_Type *pCANx,ItemInfoPtr pTxItemInfo,FrameBufferInfoPtr pTxBuffInfo);
uint8_t CAN_CheckSendBufferFrame(MSCAN_Type *pCANx,FrameBufferInfoPtr pTxBuffInfo);
uint8_t CAN_TransmitItemByInt(MSCAN_Type *pCANx,ItemInfoPtr pTxItemInfo,FrameBufferInfoPtr pTxBuffInfo);
uint8_t CAN_ReadOneFramefromBufferQueue(MSCAN_FramePtr pRxFrameInfo);
void MSCAN_GlobeVaribleInit( MSCAN_Type *pCANx );
void MSCAN_TxProcessing( void );
void MSCAN_RxProcessing( void );

#ifdef __cplusplus
}
#endif
#endif //


