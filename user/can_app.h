 /******************************************************************************
  * @file       USER/can_app.h
  * @author  cjw
  * @date     2016.10.8
  * @brief     This file contains the headers of Can_app.
  ******************************************************************************/

#ifndef __CAN_APP_H
#define __CAN_APP_H

#include "global.h"
#include "mscan.h"
#include "mscan_api.h"

#define NODE_ID1			0x801
#define NODE_ID2			0x802
#define NODE_ID3			0x803

/* define MACRO for filter,here define a dual filter to receive 
   extend and standard frame from ID(NODE_ID2&NODE_ID3) */
#define CAN_IDAR0			NODE_ID2<<1
#define CAN_IDAR1			NODE_ID1<<1
#define CAN_IDMR0			0x1|(uint32_t)0x18<<16 /* received all standard and extend frame from ID(NODE_ID2) */
#define CAN_IDMR1			0x1|(uint32_t)0x18<<16 


void CAN_APP_Init(void);
void CAN_APP_Send(uint8_t length ,  uint8_t* data);
void CAN_APP_Rive(void);


#endif
