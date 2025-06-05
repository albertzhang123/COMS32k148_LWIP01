/*============================================================================*/
/*  Copyright (C) iSOFT   (2020), iSOFT INFRASTRUCTURE SOFTWARE CO.,LTD.
 *  
 *  All rights reserved. This software is iSOFT property. Duplication 
 *  or disclosure without iSOFT written authorization is prohibited.
 *  
 *  @file       <Rte_Type.h>
 *  @brief      <>
 *  
 *  <MCU:TC39X>
 *  
 *  @author     <>
 *  @date       <2022-02-24 15:36:15>
 */
/*============================================================================*/

/* Rte_Type.h */

#ifndef RTE_TYPE_H
#define RTE_TYPE_H

/*******************************************************************************
 **                        Revision Control History                           **
******************************************************************************/

/*******************************************************************************
 **                        Version Information                                **
******************************************************************************/

/*******************************************************************************
 **                        Include Section                                    **
******************************************************************************/
#include "Rte.h"

/*******************************************************************************
 **                        Global Variable Definitions                        **
******************************************************************************/

typedef boolean Boolean;



#define _DEFINED_TYPEDEF_FOR_Boolean_ 

typedef float32 Float32;



#define _DEFINED_TYPEDEF_FOR_Float32_ 

typedef float64 Float64;



#define _DEFINED_TYPEDEF_FOR_Float64_ 

typedef sint16 SInt16;
#define SInt16_LowerLimit -32768
#define SInt16_UpperLimit 32767



#define _DEFINED_TYPEDEF_FOR_SInt16_ 

typedef sint32 SInt32;
#define SInt32_LowerLimit -2147483648
#define SInt32_UpperLimit 2147483647



#define _DEFINED_TYPEDEF_FOR_SInt32_ 

typedef sint8 SInt8;
#define SInt8_LowerLimit -128
#define SInt8_UpperLimit 127



#define _DEFINED_TYPEDEF_FOR_SInt8_ 

typedef uint16 UInt16;
#define UInt16_LowerLimit 0
#define UInt16_UpperLimit 65535



#define _DEFINED_TYPEDEF_FOR_UInt16_ 

typedef uint32 UInt32;
#define UInt32_LowerLimit 0
#define UInt32_UpperLimit 4294967295



#define _DEFINED_TYPEDEF_FOR_UInt32_ 

typedef uint64 UInt64;
#define UInt64_LowerLimit 0
#define UInt64_UpperLimit 18446744073709551615



#define _DEFINED_TYPEDEF_FOR_UInt64_ 

typedef uint8 UInt8;
#define UInt8_LowerLimit 0
#define UInt8_UpperLimit 255



#define _DEFINED_TYPEDEF_FOR_UInt8_ 

typedef uint8 uint8_T;

typedef uint16 uint16_T;

typedef uint32 uint32_T;

typedef boolean boolean_T;

typedef uint16_T rt_Array_UInt16_100[100];

typedef float64 Double_T;

typedef uint32 imask_t;



#endif

