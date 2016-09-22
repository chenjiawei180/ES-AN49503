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
* @file common.h
*
* @author Freescale
*
* @version 0.0.1
*
* @date Jun. 25, 2013
*
* @brief provide header files to be included by all project files. 
*
*******************************************************************************/


#ifndef _COMMON_H_
#define _COMMON_H_

#define swap_bytes(ptrWord)   *ptrWord = (*ptrWord >>8) | (*ptrWord<<8)
typedef unsigned long   dword;
typedef unsigned short  word;

/********************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */

#define DEBUG
#define DEBUG_PRINT

/* 
 * Include the generic CPU header file 
 */
#include "arm_cm0.h"

/* 
 * Include the platform specific header file 
 */
#if (defined(FRDM_KE02))
 #include  "ke02_config.h"
#elif (defined(FRDM_KE04))
 #include  "ke04_config.h"
#elif (defined(FRDM_KE06))
 #include  "ke06_config.h"
#else
  #error "No valid board defined"
#endif

/* 
 * Include the cpu specific header file 
*/
#if (defined(CPU_KE02))
 #include "MKE02Z2.h"
#elif (defined(CPU_KE04))
 #include "MKE04Z4.h"
#elif (defined(CPU_KE06))
 #include "MKE06Z4.h"
#else
  #error "No valid CPU defined"
#endif


/* 
 * Include any toolchain specfic header files 
 */
#if (defined(__MWERKS__))
  #include "mwerks.h"
#elif (defined(__DCC__))
  #include "build/wrs/diab.h"
#elif (defined(__ghs__))
  #include "build/ghs/ghs.h"
#elif (defined(__GNUC__))
  #if (defined(IAR))
    #include "build/gnu/gnu.h"
  #endif
#elif (defined(IAR))
  #include "iar.h"
#elif (defined(KEIL))
  
#else
#warning "No toolchain specific header included"
#endif

/* 
 * Include common utilities
 */
#include "assert.h"
#include "io.h"
#include "startup.h"
#include "stdlib.h"

#if (defined(IAR))
#include "intrinsics.h"
#endif
/********************************************************************/

#endif /* _COMMON_H_ */
