/**
 * @file    S2LP_Timer.h
 * @author  LowPower RF BU - AMG
 * @version 1.3.0
 * @date    10-July-2017
 * @brief   Configuration and management of S2-LP Timers.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors 
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This module provides API to configure the S2-LP timing mechanisms.
 * They allow the user to set the timer registers using raw values or
 * compute them since the desired timer value is expressed in ms.
 * Moreover the management of the S2-LP LDCR mode can be done using
 * these API.
 *
 * <b>Example:</b>
 * @code
 *   ...
 *
 *   S2LP_TIM_SetRxTimeoutMs(50.0);
 *   S2LP_TIM_SetWakeUpTimerUs(150000);
 * 
 *   // IRQ configuration for RX_TIMEOUT and WAKEUP_TIMEOUT
 *   ...
 *
 *   S2LP_TIM_LdcrMode(S_ENABLE);
 *
 *   ...
 *
 * @endcode
 *
 *
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2LP1_TIMER_H
#define __S2LP1_TIMER_H


/* Includes ------------------------------------------------------------------*/
#include "s2lp_regs.h"
#include "s2lp_types.h"


#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @defgroup S2LP_Timer               Timer
 * @brief Configuration and management of S2LP Timers.
 * @details See the file <i>@ref S2LP_Timer.h</i> for more details.
 * @{
 */


/**
 * @defgroup Timer_Exported_Types       Timer Exported Types
 * @{
 */

/**
 * @brief  All the possible RX timeout stop conditions enumeration.
 */
typedef enum {
     NO_TIMEOUT_STOP = 0x00,                /*!< Timeout never stopped */
     PQI_ABOVE_THRESHOLD = 0x01,            /*!< Timeout stopped on PQI above threshold */
     SQI_ABOVE_THRESHOLD = 0x02,            /*!< Timeout stopped on SQI above threshold */
     SQI_AND_PQI_ABOVE_THRESHOLD = 0x03,    /*!< Timeout stopped on both SQI and PQI above threshold */
     RSSI_ABOVE_THRESHOLD = 0x04,           /*!< Timeout stopped on RSSI above threshold */
     RSSI_AND_PQI_ABOVE_THRESHOLD = 0x05,   /*!< Timeout stopped on both RSSI and PQI above threshold */
     RSSI_AND_SQI_ABOVE_THRESHOLD = 0x06,   /*!< Timeout stopped on both RSSI and SQI above threshold */
     ALL_ABOVE_THRESHOLD = 0x07,            /*!< Timeout stopped only if RSSI, SQI and PQI are above threshold */
     TIMEOUT_ALWAYS_STOPPED = 0x08,         /*!< Timeout always stopped (default) */
     SQI_OR_PQI_ABOVE_THRESHOLD = 0x0B,     /*!< Timeout stopped if one between SQI or PQI are above threshold */
     RSSI_OR_PQI_ABOVE_THRESHOLD = 0x0D,    /*!< Timeout stopped if one between RSSI or PQI are above threshold */
     RSSI_OR_SQI_ABOVE_THRESHOLD = 0x0E,    /*!< Timeout stopped if one between RSSI or SQI are above threshold */
     ANY_ABOVE_THRESHOLD = 0x0F             /*!< Timeout stopped if one among RSSI, SQI or SQI are above threshold */
} RxTimeoutStopCondition;


/**
 * @}
 */


/**
 * @defgroup Timer_Exported_Constants   Timer Exported Constants
 * @{
 */


/**
 * @}
 */


/**
 * @defgroup Timer_Exported_Macros              Timer Exported Macros
 * @{
 */

#define SET_INFINITE_RX_TIMEOUT()     S2LP_TIM_SetRxTimerCounter(0)

/**
 * @}
 */


/**
 * @defgroup Timer_Exported_Functions           Timer Exported Functions
 * @{
 */

void S2LP_TIM_SetRxTimerStopCondition(RxTimeoutStopCondition xStopCondition);

void S2LP_TIM_LdcrMode(SFunctionalState xNewState);
void S2LP_TIM_LdcrAutoReload(SFunctionalState xNewState);
SFunctionalState S2LP_TIM_LdcrGetAutoReload(void);
void S2LP_TIM_FastRxTermTimer(SFunctionalState xNewState);
void S2LP_TIM_SetFastRxTermTimer(uint8_t fast_rx_word);
void S2LP_TIM_SetFastRxTermTimerUs(uint32_t fast_rx_us);

void S2LP_TIM_LdcrMode(SFunctionalState xNewState);
void S2LP_TIM_LdcrAutoReload(SFunctionalState xNewState);
SFunctionalState S2LP_TIM_LdcrGetAutoReload(void);

void S2LP_TIM_SetRxTimer(uint8_t cCounter , uint8_t cPrescaler);
void S2LP_TIM_SetRxTimerUs(uint32_t lDesiredUsec);
void S2LP_TIM_SetRxTimerCounter(uint8_t cCounter);
void S2LP_TIM_SetRxTimerPrescaler(uint8_t cPrescaler);
void S2LP_TIM_GetRxTimerUs(uint32_t* plTimeoutUsec, uint8_t* pcCounter , uint8_t* pcPrescaler);

void S2LP_TIM_SetWakeUpTimer(uint8_t cCounter , uint8_t cPrescaler);
void S2LP_TIM_SetWakeUpTimerUs(uint32_t lDesiredUsec);
void S2LP_TIM_SetWakeUpTimerCounter(uint8_t cCounter);
void S2LP_TIM_SetWakeUpTimerPrescaler(uint8_t cPrescaler);
void S2LP_TIM_SetWakeUpTimerReloadUs(uint32_t lDesiredUsec);
void S2LP_TIM_GetWakeUpTimerUs(uint32_t* plWakeUpUsec, uint8_t* pcCounter, uint8_t* pcPrescaler, uint8_t* pcMulti);
void S2LP_TIM_SetWakeUpTimerReload(uint8_t cCounter , uint8_t cPrescaler, uint8_t cMulti);
void S2LP_TIM_SetWakeUpTimerReloadCounter(uint8_t cCounter);
void S2LP_TIM_SetWakeUpTimerReloadPrescaler(uint8_t cPrescaler);
void S2LP_TIM_GetWakeUpTimerReloadUs(uint32_t* plWakeUpReloadUsec, uint8_t* pcCounter, uint8_t* pcPrescaler, uint8_t* pcMulti);

uint16_t S2LP_TIM_GetRcoFrequency(void);
void S2LP_TIM_CalibrationRco(SFunctionalState xCalibration);
void S2LP_TIM_SleepB(SFunctionalState en);
void S2LP_TIM_LdcIrqWa(SFunctionalState en);

void S2LP_TIM_SetRxTimerMs(float fDesiredMsec);
void S2LP_TIM_GetRxTimer(float* pfTimeoutMsec, uint8_t* pcCounter , uint8_t* pcPrescaler);
void S2LP_TIM_SetWakeUpTimerMs(float fDesiredMsec);
void S2LP_TIM_SetWakeUpTimerReloadMs(float fDesiredMsec);
void S2LP_TIM_GetWakeUpTimer(float* pfWakeUpMsec, uint8_t* pcCounter , uint8_t* pcPrescaler, uint8_t* pcMulti);
void S2LP_TIM_GetWakeUpTimerReload(float* pfWakeUpReloadMsec, uint8_t* pcCounter, uint8_t* pcPrescaler, uint8_t* pcMulti);
void S2LP_TIM_ComputeWakeUpValues(float fDesiredMsec , uint8_t* pcCounter , uint8_t* pcPrescaler);

/**
 * @}
 */

/**
 * @}
 */


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
