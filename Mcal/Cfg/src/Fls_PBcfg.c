/**
*   @file    Fls_PBcfg.c
*   @implements Fls_PBcfg.c_Artifact
*   @version 1.0.1
*
*   @brief   AUTOSAR Fls - AUTOSAR Module Flash Driver.
*   @details Definitions of all post-build configuration structures.
*
*   @addtogroup FLS
*   @{
*/
/*==================================================================================================
*   Project              : AUTOSAR 4.2 MCAL
*   Platform             : ARM
*   Peripheral           : IPV_FTFE
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
* @section fls_pbcfg_c_REF_1
* Violates MISRA 2004 Advisory Rule 19.1, Only preprocessor statements and comments before 
* '#include'
* Because of Autosar requirement MEMMAP003 of memory mapping.
* 
* @section fls_pbcfg_c_REF_2
* Violates MISRA 2004 Required Rule 11.1, Conversions involing function pointers must be to/from integral types.
* This violation is due to FLS213.
* 
* @section fls_pbcfg_c_REF_3
* Violates MISRA 2004 Advisory Rule 11.4, A cast should not be performed between a pointer to object type
* and a different pointer to object type.
* This violation is due to FLS213.
* 
* @section fls_pbcfg_c_REF_4
* Violates MISRA 2004 Required Rule 8.8, Object/function previously declared.
* The rule 8.8 requires that 'An external object or function shall be declared in one and only one file'.
* This requirement is fulfilled since the function is declared as external in and only in one configuration 
* C file (Fls_Cfg.c or Fls_PBcfg.c). There is no need to put the declaration into Fls_Cfg.h and made it 
* accessible for all modules which includes Fls.h/Fls_Cfg.h.
* 
* @section fls_pbcfg_c_REF_5
* Violates MISRA 2004 Required Rule 8.10, Could be made static.  
* Making FlsConfigSet_0 static would make it unaccessible to the application.
*
* @section fls_pbcfg_c_REF_6
* Violates MISRA 2004 Required Rule 20.2, Re-use of C90 identifier pattern.
* This is just a matter of a symbol specified in the configuration.
* 
* @section fls_pbcfg_c_REF_7
* Violates MISRA 2004 Required Rule 8.8,
* An external object or function shall be declared in one and only one file.
* Possible MISRA error if the same callback function or variable name is used in multiple configuration variants. 
* Callback function/variable name is set by the driver user so this cannot be fixed by updating the driver code.
*
* @section fls_pbcfg_c_REF_8
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be performed between a pointer type and
* an integral type.
* This violation is due to FLS213.
* 
* @section [global]
* Violates MISRA 2004 Required Rule 5.1,
* Identifiers (internal and external) shall not rely on the significance of more than 31 characters. 
* The used compilers use more than 31 chars for identifiers.
*
* @section [global]
* Violates MISRA 2004 Required Rule 1.4, The compiler/linker shall be checked to ensure that
* 31 character significance and case sensitivity are supported for external identifiers
* This violation is due to the requirement that requests to have a file version check.
*
* @section [global]
* Violates MISRA 2004 Required Rule 19.15, Repeated include files, Precautions shall be taken in
* order to prevent the contents of a header file being included twice This is not a violation since
* all header files are protected against multiple inclusions
*  
*/

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Fls.h"
#include "Reg_eSys_QSPI.h"
#include "Reg_eSys_FLASHC.h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLS_VENDOR_ID_CFG_C                      43
#define FLS_AR_RELEASE_MAJOR_VERSION_CFG_C       4
#define FLS_AR_RELEASE_MINOR_VERSION_CFG_C       2
#define FLS_AR_RELEASE_REVISION_VERSION_CFG_C    2
#define FLS_SW_MAJOR_VERSION_CFG_C               1
#define FLS_SW_MINOR_VERSION_CFG_C               0
#define FLS_SW_PATCH_VERSION_CFG_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Fls header file are of the same vendor */
#if (FLS_VENDOR_ID_CFG_C != FLS_VENDOR_ID)
    #error "Fls_PBcfg.c and Fls.h have different vendor ids"
#endif
/* Check if current file and Fls header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG_C    != FLS_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_AR_RELEASE_MINOR_VERSION_CFG_C    != FLS_AR_RELEASE_MINOR_VERSION) || \
     (FLS_AR_RELEASE_REVISION_VERSION_CFG_C != FLS_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Fls_PBcfg.c and Fls.h are different"
#endif
/* Check if current file and Fls header file are of the same Software version */
#if ((FLS_SW_MAJOR_VERSION_CFG_C != FLS_SW_MAJOR_VERSION) || \
     (FLS_SW_MINOR_VERSION_CFG_C != FLS_SW_MINOR_VERSION) || \
     (FLS_SW_PATCH_VERSION_CFG_C != FLS_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls_PBcfg.c and Fls.h are different"
#endif

/* Check if current file and Reg_eSys_QSPI header file are of the same vendor */
#if (FLS_VENDOR_ID_CFG_C != ESYS_QSPI_VENDOR_ID)
    #error "Fls_PBcfg.c and Reg_eSys_QSPI.h have different vendor ids"
#endif
/* Check if current file and Reg_eSys_QSPI header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG_C    != ESYS_QSPI_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_AR_RELEASE_MINOR_VERSION_CFG_C    != ESYS_QSPI_AR_RELEASE_MINOR_VERSION) || \
     (FLS_AR_RELEASE_REVISION_VERSION_CFG_C != ESYS_QSPI_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Fls_PBcfg.c and Reg_eSys_QSPI.h are different"
#endif
/* Check if current file and Reg_eSys_QSPI header file are of the same Software version */
#if ((FLS_SW_MAJOR_VERSION_CFG_C != ESYS_QSPI_SW_MAJOR_VERSION) || \
     (FLS_SW_MINOR_VERSION_CFG_C != ESYS_QSPI_SW_MINOR_VERSION) || \
     (FLS_SW_PATCH_VERSION_CFG_C != ESYS_QSPI_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls_PBcfg.c and Reg_eSys_QSPI.h are different"
#endif

/* Check if current file and Reg_eSys_FLASHC header file are of the same vendor */
#if (FLS_VENDOR_ID_CFG_C != ESYS_FLASHC_VENDOR_ID)
    #error "Fls_PBcfg.c and Reg_eSys_FLASHC.h have different vendor ids"
#endif
/* Check if current file and Reg_eSys_FLASHC header file are of the same Autosar version */
#if ((FLS_AR_RELEASE_MAJOR_VERSION_CFG_C    != ESYS_FLASHC_AR_RELEASE_MAJOR_VERSION) || \
     (FLS_AR_RELEASE_MINOR_VERSION_CFG_C    != ESYS_FLASHC_AR_RELEASE_MINOR_VERSION) || \
     (FLS_AR_RELEASE_REVISION_VERSION_CFG_C != ESYS_FLASHC_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Fls_PBcfg.c and Reg_eSys_FLASHC.h are different"
#endif
/* Check if current file and Reg_eSys_FLASHC header file are of the same Software version */
#if ((FLS_SW_MAJOR_VERSION_CFG_C != ESYS_FLASHC_SW_MAJOR_VERSION) || \
     (FLS_SW_MINOR_VERSION_CFG_C != ESYS_FLASHC_SW_MINOR_VERSION) || \
     (FLS_SW_PATCH_VERSION_CFG_C != ESYS_FLASHC_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Fls_PBcfg.c and Reg_eSys_FLASHC.h are different"
#endif


/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
#define FLS_START_SEC_CODE
/* 
* @violates @ref fls_pbcfg_c_REF_1 Only preprocessor statements
* and comments before '#include'
*/
#include "Fls_MemMap.h"        
/* Declaration of Fls notification function (Fee_JobEndNotification())*/
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern FUNC( void, FLS_CODE ) Fee_JobEndNotification( void );
/* Declaration of Fls notification function (Fee_JobErrorNotification())*/
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern FUNC( void, FLS_CODE ) Fee_JobErrorNotification( void );
#define FLS_STOP_SEC_CODE
/* 
* @violates @ref fls_pbcfg_c_REF_1 Only preprocessor statements
* and comments before '#include'
*/
#include "Fls_MemMap.h"

#define FLS_START_SEC_CODE_AC
/* 
* @violates @ref fls_pbcfg_c_REF_1 Only preprocessor statements
* and comments before '#include'
*/
#include "Fls_MemMap.h"

#ifdef _LINARO_C_S32K14x_ 
#if (FLS_HW_OPERATION_ABORT_SUPPORTED == STD_ON)
#if (FLS_TIMEOUT_HANDLING == STD_ON)
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern void Fls_Flash_AccessCode( CONST(uint32, AUTOMATIC) u32RegBaseAddr, P2FUNC(void, FLS_CODE, CallBack)( void ), P2VAR(uint32, AUTOMATIC, FLS_CODE) pTimerCounterAC, P2VAR(uint32, AUTOMATIC, FLS_CODE) pTimerCounterAbort ) __attribute__ ((section (".acfls_code_rom")));
#else
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern void Fls_Flash_AccessCode( CONST(uint32, AUTOMATIC) u32RegBaseAddr, P2FUNC(void, FLS_CODE, CallBack)( void ) ) __attribute__ ((section (".acfls_code_rom")));
#endif /* FLS_TIMEOUT_HANDLING == STD_ON */
#else   /* FLS_HW_OPERATION_ABORT_SUPPORTED == STD_OFF */
#if (FLS_TIMEOUT_HANDLING == STD_ON)
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern void Fls_Flash_AccessCode( CONST(uint32, AUTOMATIC) u32RegBaseAddr, P2FUNC(void, FLS_CODE, CallBack)( void ), P2VAR(uint32, AUTOMATIC, FLS_CODE) pTimerCounterAC ) __attribute__ ((section (".acfls_code_rom")));
#else
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern void Fls_Flash_AccessCode( CONST(uint32, AUTOMATIC) u32RegBaseAddr, P2FUNC(void, FLS_CODE, CallBack)( void ) ) __attribute__ ((section (".acfls_code_rom")));
#endif /* FLS_TIMEOUT_HANDLING == STD_ON */
#endif  /* FLS_HW_OPERATION_ABORT_SUPPORTED == STD_ON */
#else
#if (FLS_HW_OPERATION_ABORT_SUPPORTED == STD_ON)
#if (FLS_TIMEOUT_HANDLING == STD_ON)
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern void Fls_Flash_AccessCode(CONST(uint32, AUTOMATIC) u32RegBaseAddr, P2FUNC(void, FLS_CODE, CallBack)( void ), P2VAR(uint32, AUTOMATIC, FLS_CODE) pTimerCounterAC, P2VAR(uint32, AUTOMATIC, FLS_CODE) pTimerCounterAbort );
#else
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern void Fls_Flash_AccessCode( CONST(uint32, AUTOMATIC) u32RegBaseAddr, P2FUNC(void, FLS_CODE, CallBack)( void ) );
#endif /* FLS_TIMEOUT_HANDLING == STD_ON */
#else   /* FLS_HW_OPERATION_ABORT_SUPPORTED == STD_OFF */
#if (FLS_TIMEOUT_HANDLING == STD_ON)
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern void Fls_Flash_AccessCode(CONST(uint32, AUTOMATIC) u32RegBaseAddr, P2FUNC(void, FLS_CODE, CallBack)( void ), P2VAR(uint32, AUTOMATIC, FLS_CODE) pTimerCounterAC );
#else
/* @violates @ref fls_pbcfg_c_REF_4 Object/function previously declared */
extern void Fls_Flash_AccessCode( CONST(uint32, AUTOMATIC) u32RegBaseAddr, P2FUNC(void, FLS_CODE, CallBack)( void ) );
#endif /* FLS_TIMEOUT_HANDLING == STD_ON */
#endif  /* FLS_HW_OPERATION_ABORT_SUPPORTED == STD_ON */
#endif /* #ifdef _LINARO_C_S32K14x_ */

#define FLS_STOP_SEC_CODE_AC
/* 
* @violates @ref fls_pbcfg_c_REF_1 Only preprocessor statements
* and comments before '#include'
*/
#include "Fls_MemMap.h"

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/


                                                                                                                                                                                
            
                



#define FLS_START_SEC_CONFIG_DATA_8
/* 
* @violates @ref fls_pbcfg_c_REF_1 Only preprocessor statements
* and comments before '#include'
*/
#include "Fls_MemMap.h"
/* aFlsSectorFlags[] (FlsConfigSet) */
static CONST(uint8, FLS_CONST) FlsConfigSet_aFlsSectorFlags[16] =
{
    0U, /* (FlsSector_0) */
    0U, /* (FlsSector_1) */
    0U, /* (FlsSector_2) */
    0U, /* (FlsSector_3) */
    0U, /* (FlsSector_4) */
    0U, /* (FlsSector_5) */
    0U, /* (FlsSector_6) */
    0U, /* (FlsSector_7) */
    0U, /* (FlsSector_8) */
    0U, /* (FlsSector_9) */
    0U, /* (FlsSector_10) */
    0U, /* (FlsSector_11) */
    0U, /* (FlsSector_12) */
    0U, /* (FlsSector_13) */
    0U, /* (FlsSector_14) */
    0U /* (FlsSector_15) */
};
/* aFlsSectorUnlock[] (FlsConfigSet) */
static CONST(uint8, FLS_CONST) FlsConfigSet_aFlsSectorUnlock[16] =
{
    1U, /* (FlsSector_0) */
    1U, /* (FlsSector_1) */
    1U, /* (FlsSector_2) */
    1U, /* (FlsSector_3) */
    1U, /* (FlsSector_4) */
    1U, /* (FlsSector_5) */
    1U, /* (FlsSector_6) */
    1U, /* (FlsSector_7) */
    1U, /* (FlsSector_8) */
    1U, /* (FlsSector_9) */
    1U, /* (FlsSector_10) */
    1U, /* (FlsSector_11) */
    1U, /* (FlsSector_12) */
    1U, /* (FlsSector_13) */
    1U, /* (FlsSector_14) */
    1U /* (FlsSector_15) */
};
#define FLS_STOP_SEC_CONFIG_DATA_8
/* 
* @violates @ref fls_pbcfg_c_REF_1 Only preprocessor statements
* and comments before '#include'
*/
#include "Fls_MemMap.h"

#define FLS_START_SEC_CONFIG_DATA_UNSPECIFIED
/* 
* @violates @ref fls_pbcfg_c_REF_1 Only preprocessor statements
* and comments before '#include'
*/
#include "Fls_MemMap.h"
/* aFlsSectorEndAddr[] (FlsConfigSet) */
static CONST(Fls_AddressType, FLS_CONST) FlsConfigSet_aFlsSectorEndAddr[16] =
{
    (Fls_AddressType)4095U, /* FlsSectorEndAddr (FlsSector_0)*/
    (Fls_AddressType)8191U, /* FlsSectorEndAddr (FlsSector_1)*/
    (Fls_AddressType)12287U, /* FlsSectorEndAddr (FlsSector_2)*/
    (Fls_AddressType)16383U, /* FlsSectorEndAddr (FlsSector_3)*/
    (Fls_AddressType)20479U, /* FlsSectorEndAddr (FlsSector_4)*/
    (Fls_AddressType)24575U, /* FlsSectorEndAddr (FlsSector_5)*/
    (Fls_AddressType)28671U, /* FlsSectorEndAddr (FlsSector_6)*/
    (Fls_AddressType)32767U, /* FlsSectorEndAddr (FlsSector_7)*/
    (Fls_AddressType)36863U, /* FlsSectorEndAddr (FlsSector_8)*/
    (Fls_AddressType)40959U, /* FlsSectorEndAddr (FlsSector_9)*/
    (Fls_AddressType)45055U, /* FlsSectorEndAddr (FlsSector_10)*/
    (Fls_AddressType)49151U, /* FlsSectorEndAddr (FlsSector_11)*/
    (Fls_AddressType)53247U, /* FlsSectorEndAddr (FlsSector_12)*/
    (Fls_AddressType)57343U, /* FlsSectorEndAddr (FlsSector_13)*/
    (Fls_AddressType)61439U, /* FlsSectorEndAddr (FlsSector_14)*/
    (Fls_AddressType)65535U /* FlsSectorEndAddr (FlsSector_15)*/
};

/* paSectorProgSize[] (FlsConfigSet) */
static CONST(Fls_LengthType, FLS_CONST) FlsConfigSet_aFlsProgSize[16] =
{
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_0) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_1) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_2) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_3) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_4) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_5) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_6) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_7) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_8) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_9) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_10) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_11) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_12) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_13) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD, /* FlsProgrammingSize (FlsSector_14) */
    (Fls_LengthType)FLS_WRITE_DOUBLE_WORD /* FlsProgrammingSize (FlsSector_15) */
};


/* paSectorPageSize[] (FlsConfigSet) */
static CONST(Fls_LengthType, FLS_CONST) FlsConfigSet_aFlsSectorPageSize[16] =
{
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_0) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_1) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_2) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_3) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_4) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_5) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_6) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_7) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_8) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_9) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_10) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_11) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_12) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_13) */
    (Fls_LengthType)8U, /* FlsPageSize (FlsSector_14) */
    (Fls_LengthType)8U /* FlsPageSize (FlsSector_15) */
};



static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_0_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x000000UL),    /* sectorStartAddressPtr */
    0U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_1_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x001000UL),    /* sectorStartAddressPtr */
    1U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_2_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x002000UL),    /* sectorStartAddressPtr */
    2U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_3_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x003000UL),    /* sectorStartAddressPtr */
    3U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_4_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x004000UL),    /* sectorStartAddressPtr */
    4U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_5_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x005000UL),    /* sectorStartAddressPtr */
    5U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_6_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x006000UL),    /* sectorStartAddressPtr */
    6U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_7_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x007000UL),    /* sectorStartAddressPtr */
    7U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_8_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x008000UL),    /* sectorStartAddressPtr */
    8U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_9_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x009000UL),    /* sectorStartAddressPtr */
    9U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_10_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x00a000UL),    /* sectorStartAddressPtr */
    10U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_11_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x00b000UL),    /* sectorStartAddressPtr */
    11U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_12_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x00c000UL),    /* sectorStartAddressPtr */
    12U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_13_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x00d000UL),    /* sectorStartAddressPtr */
    13U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_14_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x00e000UL),    /* sectorStartAddressPtr */
    14U  /* Sector location to calculate cfgCRC */              
};
static CONST(Fls_Flash_InternalSectorInfoType, FLS_CONST) FlsSector_15_FlsConfigSet_sInternalSectorInfo =
{
    /* 
    * @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types 
    * @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.
    */
    (volatile uint32*)(D_FLASH_BASE_ADDR + 0x00f000UL),    /* sectorStartAddressPtr */
    15U  /* Sector location to calculate cfgCRC */              
};

/*  FLASH physical sectorization description */
static CONSTP2CONST(Fls_Flash_InternalSectorInfoType, FLS_VAR, FLS_APPL_CONST) FlsConfigSet_aSectorList[16] =
{
    /* FLS_DATA_ARRAY_0_BLOCK_3_S000 */
    &FlsSector_0_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S001 */
    &FlsSector_1_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S002 */
    &FlsSector_2_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S003 */
    &FlsSector_3_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S004 */
    &FlsSector_4_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S005 */
    &FlsSector_5_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S006 */
    &FlsSector_6_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S007 */
    &FlsSector_7_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S008 */
    &FlsSector_8_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S009 */
    &FlsSector_9_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S010 */
    &FlsSector_10_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S011 */
    &FlsSector_11_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S012 */
    &FlsSector_12_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S013 */
    &FlsSector_13_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S014 */
    &FlsSector_14_FlsConfigSet_sInternalSectorInfo
    ,    /* FLS_DATA_ARRAY_0_BLOCK_3_S015 */
    &FlsSector_15_FlsConfigSet_sInternalSectorInfo
};


/* External QSPI flash parameters. */

/* paHwCh[] (FlsConfigSet) */
static CONST(Fls_HwChType, FLS_CONST) FlsConfigSet_paHwCh[16] =
{
    FLS_CH_INTERN, /* (FlsSector_0) */
    FLS_CH_INTERN, /* (FlsSector_1) */
    FLS_CH_INTERN, /* (FlsSector_2) */
    FLS_CH_INTERN, /* (FlsSector_3) */
    FLS_CH_INTERN, /* (FlsSector_4) */
    FLS_CH_INTERN, /* (FlsSector_5) */
    FLS_CH_INTERN, /* (FlsSector_6) */
    FLS_CH_INTERN, /* (FlsSector_7) */
    FLS_CH_INTERN, /* (FlsSector_8) */
    FLS_CH_INTERN, /* (FlsSector_9) */
    FLS_CH_INTERN, /* (FlsSector_10) */
    FLS_CH_INTERN, /* (FlsSector_11) */
    FLS_CH_INTERN, /* (FlsSector_12) */
    FLS_CH_INTERN, /* (FlsSector_13) */
    FLS_CH_INTERN, /* (FlsSector_14) */
    FLS_CH_INTERN /* (FlsSector_15) */
};

/* paSectorHwAddress[] (FlsConfigSet) */
static CONST(Fls_AddressType, FLS_CONST) FlsConfigSet_paSectorHwAddress[16] =
{
    (Fls_AddressType)0U, /* (FlsSector_0) */
    (Fls_AddressType)0U, /* (FlsSector_1) */
    (Fls_AddressType)0U, /* (FlsSector_2) */
    (Fls_AddressType)0U, /* (FlsSector_3) */
    (Fls_AddressType)0U, /* (FlsSector_4) */
    (Fls_AddressType)0U, /* (FlsSector_5) */
    (Fls_AddressType)0U, /* (FlsSector_6) */
    (Fls_AddressType)0U, /* (FlsSector_7) */
    (Fls_AddressType)0U, /* (FlsSector_8) */
    (Fls_AddressType)0U, /* (FlsSector_9) */
    (Fls_AddressType)0U, /* (FlsSector_10) */
    (Fls_AddressType)0U, /* (FlsSector_11) */
    (Fls_AddressType)0U, /* (FlsSector_12) */
    (Fls_AddressType)0U, /* (FlsSector_13) */
    (Fls_AddressType)0U, /* (FlsSector_14) */
    (Fls_AddressType)0U /* (FlsSector_15) */
};




/**
* @brief        Structure used to set function pointers notification, working mode
*/
/* Fls module initialization data (FlsConfigSet)*/
/* @violates @ref fls_pbcfg_c_REF_5 Could be made static */

CONST(Fls_ConfigType, FLS_CONST) FlsConfigSet=
{
    /* @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types */
    /* @violates @ref fls_pbcfg_c_REF_3 Cast from pointer to pointer */
    /* @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.*/
    (Fls_AcErasePtrType)&Fls_Flash_AccessCode, /* FlsAcErase */
    /* @violates @ref fls_pbcfg_c_REF_2 Conversions involing function pointers must be to/from integral types */
    /* @violates @ref fls_pbcfg_c_REF_3 Cast from pointer to pointer */
    /* @violates @ref fls_pbcfg_c_REF_8 A cast should not be performed between a pointer type and an integral type.*/
    (Fls_AcWritePtrType)&Fls_Flash_AccessCode, /* FlsAcWrite */
    NULL_PTR, /* FlsACCallback */  
    &Fee_JobEndNotification, /* FlsJobEndNotification */
    &Fee_JobErrorNotification, /* FlsJobErrorNotification */  
    NULL_PTR, /* FlsStartFlashAccessNotif */
    NULL_PTR, /* FlsFinishedFlashAccessNotif */
    MEMIF_MODE_SLOW, /* FlsDefaultMode */
    1048576U, /* FlsMaxReadFastMode */
    1024U, /* FlsMaxReadNormalMode */
    256U, /* FlsMaxWriteFastMode */
    16U, /* FlsMaxWriteNormalMode */
    16U, /* FlsSectorCount */
    &FlsConfigSet_aFlsSectorEndAddr,                /* (*paSectorEndAddr)[]  */
    &FlsConfigSet_aSectorList,                      /* (*pSectorList)[] */
    &FlsConfigSet_aFlsSectorFlags,                  /* (*paSectorFlags)[] */
    &FlsConfigSet_aFlsProgSize,                     /* (*paSectorProgSize)[] */
    &FlsConfigSet_aFlsSectorPageSize,               /* (*paSectorPageSize)[] */    
    &FlsConfigSet_aFlsSectorUnlock,                 /* (*paSectorUnlock)[] */
    &FlsConfigSet_paHwCh,                           /* (*paHwCh)[] */
    &FlsConfigSet_paSectorHwAddress,                /* (*paSectorHwAddress)[] */
    NULL_PTR, /* pFlsQspiCfgConfig */
    38992U /* configCrc */
};
#define FLS_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* 
* @violates @ref fls_pbcfg_c_REF_1 Only preprocessor statements
* and comments before '#include'
*/
#include "Fls_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @}*/
