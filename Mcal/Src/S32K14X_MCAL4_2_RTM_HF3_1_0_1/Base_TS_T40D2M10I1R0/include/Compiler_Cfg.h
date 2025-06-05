/**
*   @file    Compiler_Cfg.h
*   @implements     Compiler_Cfg.h_Artifact
*   @version 1.0.1
*
*   @brief   AUTOSAR Base - Module specific memory and pointer classes
*   @details The file Compiler_Cfg.h contains the module specific parameters (ptrclass and 
*            memclass) that are passed to the macros defined in Compiler.h
*            This file contains sample code only. It is not part of the production code deliverables
*
*   @addtogroup BASE_COMPONENT
*   @{
*/
/*==================================================================================================
*   Project              : AUTOSAR 4.2 MCAL
*   Platform             : ARM
*   Peripheral           : S32K14X
*   Dependencies         : none
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

#ifndef COMPILER_CFG_H
#define COMPILER_CFG_H

#ifdef __cplusplus
#define BEGIN_C_DECLS         extern "C" {
#define END_C_DECLS           }
#else
#define BEGIN_C_DECLS
#define END_C_DECLS
#endif

#ifdef __cplusplus
extern "C"{
#endif

/**
* @page misra_violations MISRA-C:2004 violations
*
* @section [global]
* Violates MISRA 2004 Required Rule 20.2, The names of standard library macros, objects and functions
* shall not be reused. The driver needs to use defines starting with letter E.
*/

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
/** 
@{
* @brief ADC memory and pointer classes.
*/
#define ADC_CODE
#define ADC_CONST
#define ADC_APPL_DATA
#define ADC_APPL_CONST
#define ADC_APPL_CODE
#define ADC_CALLOUT_CODE
#define ADC_VAR_NOINIT
#define ADC_VAR_POWER_ON_INIT
#define ADC_VAR_FAST
#define ADC_VAR
/**@}*/

/** 
@{
* @brief CAN memory and pointer classes.
*/
#define CAN_CODE
#define CAN_CONST
#define CAN_APPL_DATA
#define CAN_APPL_CONST
#define CAN_APPL_CODE
#define CAN_CALLOUT_CODE
#define CAN_VAR_NOINIT
#define CAN_VAR_POWER_ON_INIT
#define CAN_VAR_FAST
#define CAN_VAR
/**@}*/

/** 
@{
* @brief CANIF memory and pointer classes.
*/
#define CANIF_CODE
#define CANIF_CONST
#define CANIF_APPL_DATA
#define CANIF_APPL_CONST
#define CANIF_APPL_CODE
#define CANIF_CALLOUT_CODE
#define CANIF_VAR_NOINIT
#define CANIF_VAR_POWER_ON_INIT
#define CANIF_VAR_FAST
#define CANIF_VAR
/**@}*/

/** 
@{
* @brief CRCU memory and pointer classes.
*/
#define CRCU_CODE
#define CRCU_CONST
#define CRCU_APPL_DATA
#define CRCU_APPL_CONST
#define CRCU_APPL_CODE
#define CRCU_CALLOUT_CODE
#define CRCU_VAR_NOINIT
#define CRCU_VAR_POWER_ON_INIT
#define CRCU_VAR_FAST
#define CRCU_VAR
/**@}*/

/** 
@{
* @brief CSEC memory and pointer classes.
*/
#define CSEC_CODE
#define CSEC_CONST
#define CSEC_APPL_DATA
#define CSEC_APPL_CONST
#define CSEC_APPL_CODE
#define CSEC_CALLOUT_CODE
#define CSEC_VAR_NOINIT
#define CSEC_VAR_POWER_ON_INIT
#define CSEC_VAR_FAST
#define CSEC_VAR
/**@}*/

/** 
@{
* @brief DEM memory and pointer classes.
*/
#define DEM_CODE
#define DEM_CONST
#define DEM_APPL_DATA
#define DEM_APPL_CONST
#define DEM_APPL_CODE
#define DEM_CALLOUT_CODE
#define DEM_VAR_NOINIT
#define DEM_VAR_POWER_ON_INIT
#define DEM_VAR_FAST
#define DEM_VAR
/**@}*/

/** 
@{
* @brief DET memory and pointer classes.
*/
#define DET_CODE
#define DET_CONST
#define DET_APPL_DATA
#define DET_APPL_CONST
#define DET_APPL_CODE
#define DET_CALLOUT_CODE
#define DET_VAR_NOINIT
#define DET_VAR_POWER_ON_INIT
#define DET_VAR_FAST
#define DET_VAR
/**@}*/

/** 
@{
* @brief DIO memory and pointer classes.
*/
#define DIO_CODE
#define DIO_CONST
#define DIO_APPL_DATA
#define DIO_APPL_CONST
#define DIO_APPL_CODE
#define DIO_CALLOUT_CODE
#define DIO_VAR_NOINIT
#define DIO_VAR_POWER_ON_INIT
#define DIO_VAR_FAST
#define DIO_VAR
/**@}*/

/** 
@{
* @brief EEP memory and pointer classes.
*/

#define EEP_CODE
#define EEP_CONST
#define EEP_APPL_DATA
#define EEP_APPL_CONST
#define EEP_APPL_CODE
#define EEP_CALLOUT_CODE
#define EEP_VAR_NOINIT
#define EEP_VAR_POWER_ON_INIT
#define EEP_VAR_FAST
#define EEP_VAR
/**@}*/

/**
@{
* @brief ETH memory and pointer classes.
*/

#define ETH_CODE
#define ETH_CONST
#define ETH_APPL_DATA
#define ETH_APPL_CONST
#define ETH_APPL_CODE
#define ETH_CALLOUT_CODE
#define ETH_VAR_NOINIT
#define ETH_VAR_POWER_ON_INIT
#define ETH_VAR_FAST
#define ETH_VAR
/**@}*/



/** 
@{
* @brief FEE memory and pointer classes.
*/
#define FEE_CODE
#define FEE_CONST
#define FEE_APPL_DATA
#define FEE_APPL_CONST
#define FEE_APPL_CODE
#define FEE_CALLOUT_CODE
#define FEE_VAR_NOINIT
#define FEE_VAR_POWER_ON_INIT
#define FEE_VAR_FAST
#define FEE_VAR
/**@}*/

/** 
@{
* @brief FLS memory and pointer classes.
*/
#define FLS_CODE
#define FLS_CONST
#define FLS_APPL_DATA
#define FLS_APPL_CONST
#define FLS_APPL_CODE
#define FLS_CALLOUT_CODE
#define FLS_VAR_NOINIT
#define FLS_VAR_POWER_ON_INIT
#define FLS_VAR_FAST
#define FLS_VAR
/**@}*/

/** 
@{
* @brief FlexRay memory and pointer classes.
*/
#define FR_CODE
#define FR_CONST
#define FR_APPL_DATA
#define FR_APPL_CONST
#define FR_APPL_CODE
#define FR_CALLOUT_CODE
#define FR_VAR_NOINIT
#define FR_VAR_POWER_ON_INIT
#define FR_VAR_FAST
#define FR_VAR
/**@}*/

/** 
@{
* @brief GPT memory and pointer classes.
*/
#define GPT_CODE
#define GPT_CONST
#define GPT_APPL_DATA
#define GPT_APPL_CONST
#define GPT_APPL_CODE
#define GPT_CALLOUT_CODE
#define GPT_VAR_NOINIT
#define GPT_VAR_POWER_ON_INIT
#define GPT_VAR_FAST
#define GPT_VAR
/**@}*/

/** 
@{
* @brief ICU memory and pointer classes.
*/
#define ICU_CODE
#define ICU_CONST
#define ICU_APPL_DATA
#define ICU_APPL_CONST
#define ICU_APPL_CODE
#define ICU_CALLOUT_CODE
#define ICU_VAR_NOINIT
#define ICU_VAR_POWER_ON_INIT
#define ICU_VAR_FAST
#define ICU_VAR
/**@}*/

/** 
@{
* @brief I2C memory and pointer classes.
*/
#define I2C_CODE
#define I2C_CONST
#define I2C_APPL_DATA
#define I2C_APPL_CONST
#define I2C_APPL_CODE
#define I2C_CALLOUT_CODE
#define I2C_VAR_NOINIT
#define I2C_VAR_POWER_ON_INIT
#define I2C_VAR_FAST
#define I2C_VAR
/**@}*/

/** 
@{
* @brief ISELED memory and pointer classes.
*/
#define ISELED_CODE
#define ISELED_CONST
#define ISELED_APPL_DATA
#define ISELED_APPL_CONST
#define ISELED_APPL_CODE
#define ISELED_CALLOUT_CODE
#define ISELED_VAR_NOINIT
#define ISELED_VAR_POWER_ON_INIT
#define ISELED_VAR_FAST
#define ISELED_VAR
/**@}*/

/** 
@{
* @brief LIN memory and pointer classes.
*/
#define LIN_CODE
#define LIN_CONST
#define LIN_APPL_DATA
#define LIN_APPL_CONST
#define LIN_APPL_CODE
#define LIN_CALLOUT_CODE
#define LIN_VAR_NOINIT
#define LIN_VAR_POWER_ON_INIT
#define LIN_VAR_FAST
#define LIN_VAR
/**@}*/

/** 
@{
* @brief MCEM memory and pointer classes.
*/
#define MCEM_CODE
#define MCEM_CONST
#define MCEM_APPL_DATA
#define MCEM_APPL_CONST
#define MCEM_APPL_CODE
#define MCEM_CALLOUT_CODE
#define MCEM_VAR_NOINIT
#define MCEM_VAR_POWER_ON_INIT
#define MCEM_VAR_FAST
#define MCEM_VAR
/**@}*/

/** 
@{
* @brief MCL memory and pointer classes.
*/
#define MCL_CODE
#define MCL_CONST
#define MCL_APPL_DATA
#define MCL_APPL_CONST
#define MCL_APPL_CODE
#define MCL_CALLOUT_CODE
#define MCL_VAR_NOINIT
#define MCL_VAR_POWER_ON_INIT
#define MCL_VAR_FAST
#define MCL_VAR
/**@}*/

/** 
@{
* @brief MCU memory and pointer classes.
*/
#define MCU_CODE
#define MCU_CONST
#define MCU_APPL_DATA
#define MCU_APPL_CONST
#define MCU_APPL_CODE
#define MCU_CALLOUT_CODE
#define MCU_VAR_NOINIT
#define MCU_VAR_POWER_ON_INIT
#define MCU_VAR_FAST
#define MCU_VAR
/**@}*/

/** 
@{
* @brief OCU memory and pointer classes.
*/
#define OCU_CODE
#define OCU_CONST
#define OCU_APPL_DATA
#define OCU_APPL_CONST
#define OCU_APPL_CODE
#define OCU_CALLOUT_CODE
#define OCU_VAR_NOINIT
#define OCU_VAR_POWER_ON_INIT
#define OCU_VAR_FAST
#define OCU_VAR
/**@}*/

/** 
@{
* @brief PORT memory and pointer classes.
*/
#define PORT_CODE
#define PORT_CONST
#define PORT_APPL_DATA
#define PORT_APPL_CONST
#define PORT_APPL_CODE
#define PORT_CALLOUT_CODE
#define PORT_VAR_NOINIT
#define PORT_VAR_POWER_ON_INIT
#define PORT_VAR_FAST
#define PORT_VAR
/**@}*/

/** 
@{
* @brief PWM memory and pointer classes.
*/
#define PWM_CODE
#define PWM_CONST
#define PWM_APPL_DATA
#define PWM_APPL_CONST
#define PWM_APPL_CODE
#define PWM_CALLOUT_CODE
#define PWM_VAR_NOINIT
#define PWM_VAR_POWER_ON_INIT
#define PWM_VAR_FAST
#define PWM_VAR
/**@}*/


/** 
@{
* @brief RamTST memory and pointer classes.
*/
#define RAMTST_CODE
#define RAMTST_CONST
#define RAMTST_APPL_DATA
#define RAMTST_APPL_CONST
#define RAMTST_APPL_CODE
#define RAMTST_CALLOUT_CODE
#define RAMTST_VAR_NOINIT
#define RAMTST_VAR_POWER_ON_INIT
#define RAMTST_VAR_FAST
#define RAMTST_VAR
/**@}*/

/** 
@{
* @brief SchM memory and pointer classes.
*/
#define SCHM_CODE
#define SCHM_CONST
#define SCHM_APPL_DATA
#define SCHM_APPL_CONST
#define SCHM_APPL_CODE
#define SCHM_CALLOUT_CODE
#define SCHM_VAR_NOINIT
#define SCHM_VAR_POWER_ON_INIT
#define SCHM_VAR_FAST
#define SCHM_VAR
/**@}*/

/** 
@{
* @brief SPI memory and pointer classes.
*/
#define SPI_CODE
#define SPI_CONST
#define SPI_APPL_DATA
#define SPI_APPL_CONST
#define SPI_APPL_CODE
#define SPI_CALLOUT_CODE
#define SPI_VAR_NOINIT
#define SPI_VAR_POWER_ON_INIT
#define SPI_VAR_FAST
#define SPI_VAR
/**@}*/

/** 
@{
* @brief WDG memory and pointer classes.
*/
#define WDG_CODE
#define WDG_CONST
#define WDG_APPL_DATA
#define WDG_APPL_CONST
#define WDG_APPL_CODE
#define WDG_CALLOUT_CODE
#define WDG_VAR_NOINIT
#define WDG_VAR_POWER_ON_INIT
#define WDG_VAR_FAST
#define WDG_VAR
/**@}*/

/** 
@{
* @brief WDGIF memory and pointer classes.
*/
#define WDGIF_CODE
#define WDGIF_CONST
#define WDGIF_APPL_DATA
#define WDGIF_APPL_CONST
#define WDGIF_APPL_CODE
#define WDGIF_CALLOUT_CODE
#define WDGIF_VAR_NOINIT
#define WDGIF_VAR_POWER_ON_INIT
#define WDGIF_VAR_FAST
#define WDGIF_VAR
/**@}*/
/* ethif */
#define ETHIF_CODE
#define ETHIF_VAR

#define ETHIF_APPL_DATA

/* tcpip */
#define TCPIP_CODE
#define TCPIP_VAR

#define TCPIP_APPL_CONST
#define TCPIP_APPL_VAR
#define TCPIP_CONST

#define TCPIP_CONFIG_DATA

#define TCPIP_APPL_DATA
#define TCPIP_APPL_CODE
#define TCPIP_VAR_ZERO_INIT
/* os */
#define OS_VAR
#define OS_CONST


/* ecum */
#define ECUM_CONST
#define ECUM_APPL_DATA
#define ECUM_VAR_NO_INIT
#define ECUM_DETERMINEPBCONFIGURATION_CODE
#define ECUM_CONFIG_DATA
#define ECUM_DETERMINEPBCONFIGURATION_CODE


/* nvm */
#define NVM_CODE
#define NVM_VAR
#define NVM_CONST
#define NVM_VAR_NOINIT
#define NVM_APPL_DATA
#define NVM_VAR_POWER_ON_INIT
#define NVM_APPL_CODE

/* canif */
#define CANIF_VAR_POWER_ON_INIT
#define CANIF_CONST
#define CANIF_VAR
#define CANIF_CODE
#define CANIF_APPL_DATA
#define CANIF_CONST_PBCFG
#define CANIF_APPL_CONST
#define CANIF_APPL_CODE

/* com */
#define COM_VAR_POWER_ON_INIT
#define COM_VAR
#define COM_CODE
#define COM_APPL_CONST
#define COM_APPL_DATA
#define COM_CONST_PBCFG
#define COM_CONST
#define COM_APPL_CODE

/* cantp */
#define CANTP_VAR_POWER_ON_INIT
#define CANTP_CODE
#define CANTP_CONST
#define CANTP_APPL_CONST
#define CANTP_APPL_DATA
#define CANTP_APPL_CODE
#define CANTP_VAR_NOINIT
#define CANTP_VAR

/* dcm */
#define DCM_VAR_POWER_ON_INIT
#define DCM_CODE
#define DCM_CONST_PBCFG
#define DCM_VAR
#define DCM_APPL_CODE
#define DCM_CONST
#define DCM_VAR_NOINIT
#define DCM_APPL_CONST
#define DCM_APPL_DATA

/* pdur */
#define PDUR_VAR_POWER_ON_INIT
#define PDUR_VAR
#define PDUR_APPL_DATA
#define PDUR_CODE
#define PDUR_CONST
#define PDUR_CONST_PBCFG

/* soad */
#define SOAD_APPL_DATA
#define SOAD_APPL_CONST
#define SOAD_VAR_POWER_ON_INIT
#define SOAD_VAR
#define SOAD_CODE
#define SOAD_CONST
#define SOAD_CALLBACK_CODE
#define SOAD_APPL_CODE
#define SOAD_CONST_PBCFG

/* bswm */
#define BSWM_CONST
#define BSWM_VAR_CLEARED


/* xcp */
#define XCP_CONST
#define XCP_CONST_PBCFG
#define XCP_VAR_INIT_UNSPECIFIED
#define XCP_VAR
#define XCP_VAR_CLEARED_UNSPECIFIED
#define XCP_VAR_INIT_8
#define XCP_VAR_CLEARED_32
#define XCP_APPL_DATA
#define XCP_CODE
#define XCP_APPL_CONST



/* fee */
#define FEE_CODE
#define FEE_CONST_PBCFG
#define FEE_CONST_PCCFG
#define FEE_VAR
#define FEE_CONST
#define FEE_APPL_DATA

/* MemIf */
#define MEMIF_CODE
#define MEMIF_VAR
#define MEMIF_CONST
#define MEMIF_VAR_NOINIT
#define MEMIF_APPL_DATA
#define MEMIF_VAR_POWER_ON_INIT
#define MEMIF_APPL_CODE


/*cannm*/
#define CANNM_APPL_CONST
#define CANNM_APPL_DATA
#define CANNM_CODE
#define CANNM_VAR
#define CANNM_VAR_POWER_ON_INIT
#define CANNM_CONST

/*cansm*/
#define CANSM_CONST
#define CANNM_VAR_POWER_ON_INIT
#define CANSM_CONFIG_DATA
#define CANSM_VAR
#define CANSM_CODE
#define CANSM_COMM_CODE
#define CANSM_ECUM_CODE
#define CANSM_CONTROLLERBUSOFF_CODE
#define CANSM_CONTROLLERNODEINDICATION_CODE
#define CANSM_TRANSCEIVERMODEINDICATION_CODE
#define CANSM_CLEARTRCVWUFFLAGINDICATION_CODE
#define CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_CODE
#define CANSM_CONFIRMPNAVAILABILITY_CODE
#define CANSM_CURRENTICOMCONFIGURATION_CODE
#define CANSM_TXTIMEOUTEXCEPTION_CODE

/* comm */
#define COMM_VAR_POWER_ON_INIT
#define COMM_APPL_DATA
#define COMM_CODE
#define COMM_CONST
#define COMM_COM_CODE
#define COMM_DCM_CODE
#define COMM_VAR
#define COMM_NM_CODE
#define ComM_CONFIG_DATA

/* iohwab */
#define IOHWAB_VAR
#define IOHWAB_CONST
#define IOHWAB_CODE

/* rte */
#define RTE_CONST
#define RTE_DATA

/* ethsm */
#define ETHSM_APPL_DATA
#define ETHSM_CODE
#define ETHSM_CONST
#define ETHSM_VAR
#define ETHSM_VAR_POWER_ON_INIT

/* ldcom */
#define LDCOM_VAR_POWER_ON_INIT
#define LDCOM_CONST
#define LDCOM_APPL_DATA
#define LDCOM_CODE
#define LDCOM_COPYTXDATA_CODE
#define LDCOM_TPTXCONFIRMATION_CODE
#define LDCOM_STARTOFRECEPTION_CODE
#define LDCOM_COPYRXDATA_CODE
#define LDCOM_TPRXINDICATION_CODE
#define LDCOM_RXINDICATION_CODE
#define LDCOM_TXCONFIRMATION_CODE
#define LDCOM_TRIGGERTRANSMIT_CODE
#define LDCOM_APPL_CODE
#define LDCOM_CONST_PBCFG

/* UdpNm */
#define UDPNM_APPL_CONST
#define UDPNM_APPL_DATA
#define UDPNM_CONST
#define UDPNM_CODE
#define UDPNM_VAR
#define UDPNM_VAR_POWER_ON_INIT





/** 
* @brief Define for ComStack Data.
*/
#define AUTOSAR_COMSTACKDATA

/*==================================================================================================
*                                            ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef COMPILER_CFG_H */

/** @} */
