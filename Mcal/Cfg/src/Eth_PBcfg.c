/**
*   @file       Eth_PBcfg.c
*   @implements Eth_PBcfg_c_Artifact
*   @version 1.0.1
*
*   @brief   AUTOSAR Eth driver Post-Build configuration file
*   @details File containing the configuration for the Ethernet driver
*   @note    This file was generated on 2023-06-09.
*
*   @addtogroup ETH_DRIVER_CONFIGURATION
*   @{
*/
/*==================================================================================================
*   Project              : AUTOSAR 4.2 MCAL
*   Platform             : ARM
*   Peripheral           : ENET
*   Dependencies         : 
*
*   Autosar Version      : 4.2.2
*   Autosar Revision     : ASR_REL_4_2_REV_0002
*   Autosar Conf.Variant :
*   SW Version           : 1.0.1
*   Build Version        : S32K14x_MCAL_1_0_1_RTM_ASR_REL_4_2_REV_0002_20180713
*
*   (c) Copyright 2006-2016 Freescale Semiconductor, Inc. 
*       Copyright 2017-2018 NXP
*   All Rights Reserved.
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/
#ifdef __cplusplus
extern "C"{
#endif

/**
* @page misra_violations MISRA-C:2004 violations
*
* @section Eth_PBcfg_c_REF_1
* Violates MISRA 2004 Advisory Rule 19.1, #include statements should only be preceded
* by other preprocessor directives or comments.
* The usage of the MemMap.h file violates this advisory rule, but this
* mechanism is used to properly place the code and variables into their
* sections.
*
* @section Eth_PBcfg_c_REF_2
* Violates MISRA 2004 Required Rule 19.15, Repeated include file. Precautions shall be taken in
* order to prevent the contents of a header file being included twice. This is an approved violation
* of usage MemMap.h file used to create linkage sections.
*
* @section Eth_PBcfg_c_REF_3
* Violates MISRA 2004 Required Rule 1.4, A compiler/linker shall be checked to ensure that 31
* character significance and case sensitivity are supported for external identifiers.
* Name length of the macro is unfortunately longer than 31 characters required by the agreed file
* template and cannot be changed. Authors of the template have not taken the MISRA rules into
* account.
*
* @section Eth_PBcfg_c_REF_4
* Violates MISRA 2004 Required Rule 5.1, Identifiers shall not rely on the significance of more
* than 31 characters. Name length of the macros is unfortunately longer than 31 characters 
* which is required by the agreed macros format and cannot be changed. 
*
* @section Eth_PBcfg_c_REF_5
* Violates MISRA 2004 Required Rule 8.10, All declarations and definitions
* of objects or functions at file scope shall have internal linkage unless
* external linkage is required. The APIs always require the external
* linkage. MISRA analysis tool does not see the linkage from other files
* therefore there is no violation.
*
* @section Eth_PBcfg_c_REF_7
* Violates MISRA 2004 Required Rule 11.1, Pointer to a function shall not be converted to anything
* else than integral type. This rule is not violated however the tool used for checks reports it
* therefore it must be commented here.
*
* @section [global]
* Violates MISRA 2004 Required Rule 20.2, The names of standard library macros,
* objects and functions shall not be reused. All defines starting with upper
* letter E are reserved for future use inside <error.h> header file.
* All module define options and enumerations are restricted by JDP coding rules
* 5.6 and 5.8 and must follow format <MSN>_<NAME> where MSN is equal to ETH.
* It was agreed that this violation will not be commented above each line but
* only in header of affected file.
*/
/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/* ETH001 */
#include "Eth.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_VENDOR_ID_C                      43
#define ETH_AR_RELEASE_MAJOR_VERSION_C       4
#define ETH_AR_RELEASE_MINOR_VERSION_C       2
/** @violates @ref Eth_PBcfg_c_REF_3 MISRA rule 1.4 
* @violates @ref Eth_PBcfg_c_REF_4 MISRA rule 5.1 */
#define ETH_AR_RELEASE_REVISION_VERSION_C    2
#define ETH_SW_MAJOR_VERSION_C               1
#define ETH_SW_MINOR_VERSION_C               0
#define ETH_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and ETH header file are of the same vendor */
#if (ETH_VENDOR_ID_C != ETH_VENDOR_ID)
    #error "Eth_PBcfg.c and Eth.h have different vendor ids"
#endif
/* Check if current file and ETH header file are of the same Autosar version */
#if ((ETH_AR_RELEASE_MAJOR_VERSION_C    != ETH_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_AR_RELEASE_MINOR_VERSION_C    != ETH_AR_RELEASE_MINOR_VERSION) || \
     (ETH_AR_RELEASE_REVISION_VERSION_C != ETH_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Eth_PBcfg.c and Eth.h are different"
#endif
/* Check if current file and ETH header file are of the same Software version */
#if ((ETH_SW_MAJOR_VERSION_C != ETH_SW_MAJOR_VERSION) || \
     (ETH_SW_MINOR_VERSION_C != ETH_SW_MINOR_VERSION) || \
     (ETH_SW_PATCH_VERSION_C != ETH_SW_PATCH_VERSION))
    #error "Software Version Numbers of Eth_PBcfg.c and Eth.h are different"
#endif
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
/** @violates @ref Eth_PBcfg_c_REF_3 MISRA rule 1.4 
* @violates @ref Eth_PBcfg_c_REF_4 MISRA rule 5.1 */
#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED

/** 
* @violates @ref Eth_PBcfg_c_REF_1 MISRA rule 19.1
* @violates @ref Eth_PBcfg_c_REF_2 MISRA rule 19.15
*/
#include "Eth_MemMap.h"
/*******************************************************************************
* Multiple configurations
*******************************************************************************/


/**
* @var Specified for EthCtrlConfig: EthCtrlConfig_0
* @par Memory usage:
* - RX buffers area: Occupied space: 24064 Bytes
* - TX buffers area: Occupied space: 24064 Bytes
*/
static CONST(Eth_CtrlCfgType, ETH_APPL_CONST) EthConfigSet_EthCtrlConfig_0 =
{
    /* Configured MAC Address is 11:22:33:44:55:66 */
    0x11223344U, /**< First 32 bits of MAC address */
    0x55668808U, /**< Last 16 bits of MAC address<<16 | 0x8808 */
    0x00000104U, /**< Value stored to TCR during initialization - full duplex mode selected */
    /* RCR value:
    - Maximal received frame length is 1472
    - MII or RMII mode is selected
    - CRC is not forwarded to application
    - Payload length check is enable
    - Broadcast is accepted
    - Promiscuous mode is disabled
    - RMII 100Mbs mode enabled */
    (((VAR(uint32, AUTOMATIC))1522U)<<16U) | 0x40004104U, /**< Value stored to RCR during initialization */
    (((VAR(uint32, AUTOMATIC))32U)<<1U) | ((VAR(uint32, AUTOMATIC))0U<<8U), /**< Value stored to MSCR during initialization, F_sys,max = 320 MHz */
    0x000005f2U, /* FTRL value */
    0x00000040U, /**< Value stored to RACC during initialization - frames with errors at MAC layer discarded (CRC,length,PHY) */
    12U,  /**< Value stored to TIPG during initialization */


#if STD_ON == ETH_DEM_EVENT_DETECT

    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},
    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},
    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},
    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},
    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},
    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},
    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},
    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},
    {(VAR(uint32, AUTOMATIC))STD_OFF, 0U},  
#endif /* ETH_DEM_EVENT_DETECT */
#if (STD_ON == ETH_GLOBALTIME_SUPPORT)
    ((VAR(uint32, AUTOMATIC))1000000U),
#endif

    1472U, /**< Maximum length of one Rx buffer (header, data, crc, 64-byte alignment) */
    1472U, /**< Maximum length of one Tx buffer (header, data, 64-byte alignment) */
    16U, /**< Number of Rx buffers */
    16U, /**< Number of Tx buffers */

};


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
/**
* @brief         Driver configuration to be passed to Eth_Init function
* @details       This array contains references to all multiple configurations
*                to be used for the Eth driver configuration.
*/
/** @violates @ref Eth_PBcfg_c_REF_5 MISRA rule 8.10  */
CONST(Eth_ConfigType, ETH_CONST) EthConfigSet= 
{
 {
      &EthConfigSet_EthCtrlConfig_0
 }
};

/** @violates @ref Eth_PBcfg_c_REF_3 MISRA rule 1.4 
* @violates @ref Eth_PBcfg_c_REF_4 MISRA rule 5.1 */
#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/** 
* @violates @ref Eth_PBcfg_c_REF_1 MISRA rule 19.1
* @violates @ref Eth_PBcfg_c_REF_2 MISRA rule 19.15
*/
#include "Eth_MemMap.h"

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif
/** @} */
