/**
 * @file    S2LP_Radio.h
 * @author  LowPower RF BU - AMG
 * @version 1.3.0
 * @date    10-July-2017
 * @brief   Configuration and management of S2-LP RF Analog and Digital part.
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
 * In order to configure the Radio main parameters, the user can
 * fit <i>SRadioInit</i> structure the and call the <i>S2LP_RADIO_Init()</i>
 * function passing its pointer as an argument.
 *
 * <b>Example:</b>
 * @code
 *
 * SRadioInit radioInit = {
 *     433400000,                 // base frequency
 *     MOD_2FSK,                     // modulation select
 *     38400,                   // datarate
 *     20000,                     // frequency deviation
 *     100000                     // channel filter bandwidth
 * };
 *
 * ...
 *
 * S2LP_RADIO_Init(&radioInit);
 * @endcode
 *
 * Another important parameter for the radio configuration is the
 * transmission power.
 * The user is allowed to configure it using the function <i>S2LP_RADIO_SetPALeveldBm()</i>
 * which sets the PA LEVEL specified by the first argument to the
 * power expressed in dBm by the second parameter.
 *
 * <b>Example:</b>
 * @code
 *
 *  S2LP_RADIO_SetPALeveldBm(0 , 10.0);
 *
 * @endcode
 *
 *
 * @note The effective power that is set can be a little different from the
 * passed argument in dBm because the function performs an approximation.
 *
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2LP_RADIO_H
#define __S2LP_RADIO_H


/* Includes ------------------------------------------------------------------*/
#include "s2lp_regs.h"
#include "s2lp_types.h"


#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup S2LP_Libraries
 * @{
 */


/** @defgroup S2LP_Radio          Radio
 * @brief Configuration and management of S2LP RF Analog and Digital part.
 * @details See the file <i>@ref S2LP_Radio.h</i> for more details.
 * @{
 */



/** @defgroup Radio_Exported_Types      Radio Exported Types
 * @{
 */


/**
 * @brief  S2LP Modulation enumeration
 */
typedef enum {
  MOD_NO_MOD       = 0x70, /*!< CW modulation selected */
  MOD_2FSK         = 0x00, /*!< 2-FSK modulation selected */
  MOD_4FSK         = 0x10, /*!< 4-FSK modulation selected */
  MOD_2GFSK_BT05   = 0xA0, /*!< G2FSK modulation selected with BT = 0.5 */
  MOD_2GFSK_BT1    = 0x20, /*!< G2FSK modulation selected with BT = 1 */
  MOD_4GFSK_BT05   = 0xB0, /*!< G4FSK modulation selected with BT = 0.5 */
  MOD_4GFSK_BT1    = 0x30, /*!< G4FSK modulation selected with BT = 1 */
  MOD_ASK_OOK      = 0x50, /*!< OOK modulation selected. */
  MOD_POLAR        = 0x60, /*!< OOK modulation selected. */
} ModulationSelect;


/**
 * @brief  S2LP Radio Init structure definition
 */
typedef struct {
  uint32_t          lFrequencyBase;     /*!< Specifies the base carrier frequency (in Hz),
                                        i.e. the carrier frequency of channel #0.
                                        This parameter can be in one of the following ranges:
                                        High_Band: from 860 MHz to 940 MHz
                                        Middle Band: from 430 MHz to 470 MHz */
  ModulationSelect  xModulationSelect;  /*!< Specifies the modulation @ref ModulationSelect */
  uint32_t          lDatarate;          /*!< Specifies the datarate expressed in bps.
                                        This parameter can be in the range
                                        between 100 bps and 500 kbps */
  uint32_t          lFreqDev;           /*!< Specifies the frequency deviation expressed in Hz.
                                        This parameter can be in the range: 
                                        [F_Xo*8/2^18, F_Xo*7680/2^18] Hz */
  uint32_t          lBandwidth;         /*!< Specifies the channel filter bandwidth
                                        expressed in Hz. This parameter can be
                                        in the range between 1100 and 800100 Hz */
} SRadioInit;


/**
 * @brief  S2LP AFC mode enumeration
 */
typedef enum {
  AFC_MODE_LOOP_CLOSED_ON_SLICER = 0,     /* loop closed on slicer */
  AFC_MODE_LOOP_CLOSED_ON_2ND_CONV_STAGE  /* loop closed on 2nd conversion stage */
} SAfcMode;


/**
 * @brief  S2LP AFC Init structure definition
 */
typedef struct {
  SFunctionalState  xAfcEnable;       /*!< AFC enable */
  SFunctionalState  xAfcFreezeOnSync; /*!< Freeze the parameters on SYNC word detect */
  SAfcMode          xAfcMode;         /*!< Specify the AFC mode. @ref SAfcMode */
  uint8_t          cAfcFastPeriod;    /*!< Fast period duration */
  uint8_t          cAfcFastGain;      /*!< Gain used during fast loop */
  uint8_t          cAfcSlowGain;      /*!< Gain used during slow loop */
} SAfcInit;


/**
 * @brief  S2LP ISI Equalization type enumeration
 */
typedef enum {
  ISI_EQUALIZATION_DISABLED = 0,  /* static mode */
  ISI_EQUALIZATION_SINGLE_PASS,   /* dynamic mode 6 dB above threshold*/
  ISI_EQUALIZATION_DUAL_PASS      /* dynamic mode 18 dB above threshold */
} SIsiEqu;


/**
 * @brief  S2LP Clock Recovery mode enumeration
 */
typedef enum {
  CLKREC_DLL_MODE = 0, /* DLL mode */
  CLKREC_PLL_MODE,     /* PLL mode */
} SClkRecMode;


/**
 * @brief  S2LP Clock Recovery Init structure definition
 */
typedef struct {
  SClkRecMode      xSClkRecMode;          /*!< Select the clock recovery mode. @ref SClkRecMode */
  uint8_t          cClkRecPGainSlow;      /*!< Gain P for slow stage. From 0 to 7. */
  uint8_t          cClkRecPGainFast;      /*!< Gain P for fast stage. From 0 to 7. */
  uint8_t          cClkRecIGainSlow;      /*!< Gain I for slow stage. From 0 to 15. */
  uint8_t          cClkRecIGainFast;      /*!< Gain I for fast stage. From 0 to 15. */
  SFunctionalState cClkRec16SymPostFlt;   /*!< Set the post filter to 16 symbols. Default is 8 symbols */
} SSymClkRecInit;


/**
 * @}
 */



/** @defgroup Radio_Exported_Functions                          Radio Exported Functions
 * @{
 */

uint8_t S2LP_RADIO_Init(SRadioInit* pxSRadioInitStruct);
void S2LP_RADIO_GetInfo(SRadioInit* pxSRadioInitStruct);
void S2LP_RADIO_SetSynthWord(uint32_t lSynthWord);
uint32_t S2LP_RADIO_GetSynthWord(void);
void S2LP_RADIO_SetChannel(uint8_t cChannel);
uint8_t S2LP_RADIO_GetChannel(void);
void S2LP_RADIO_SetRefDiv(SFunctionalState xNewState);
SFunctionalState S2LP_RADIO_GetRefDiv(void);
void S2LP_RADIO_SetDigDiv(SFunctionalState xNewState);
SFunctionalState S2LP_RADIO_GetDigDiv(void);
void S2LP_RADIO_SetChannelSpace(uint32_t lChannelSpace);
uint32_t S2LP_RADIO_GetChannelSpace(void);
uint8_t S2LP_RADIO_SetFrequencyBase(uint32_t lFBase);
uint32_t S2LP_RADIO_GetFrequencyBase(void);
void S2LP_RADIO_SetDatarate(uint32_t lDatarate);
uint32_t S2LP_RADIO_GetDatarate(void);
void S2LP_RADIO_SetFrequencyDev(uint32_t lFDev);
uint32_t S2LP_RADIO_GetFrequencyDev(void);
void S2LP_RADIO_SetChannelBW(uint32_t lBandwidth);
uint32_t S2LP_RADIO_GetChannelBW(void);
void S2LP_RADIO_SetModulation(ModulationSelect xModulation);
ModulationSelect S2LP_RADIO_GetModulation(void);
void S2LP_RADIO_SetXtalFrequency(uint32_t lXtalFrequency);
uint32_t S2LP_RADIO_GetXtalFrequency(void);
void S2LP_RADIO_SetIsiEqualizationMode(SIsiEqu xSIsiMode);
SIsiEqu S2LP_RADIO_GetIsiEqualizationMode(void);
void S2LP_RADIO_CalibrationVco(SFunctionalState xAmplitudeCalibration, SFunctionalState xFrequencyCalibration);
void S2LP_RADIO_SetTxCalibVcoAmpWord(uint8_t value);
void S2LP_RADIO_SetRxCalibVcoAmpWord(uint8_t value);
void S2LP_RADIO_SetTxCalibVcoFreqWord(uint8_t value);
void S2LP_RADIO_SetRxCalibVcoFreqWord(uint8_t value);

void S2LP_RADIO_SetMaxPALevel(SFunctionalState xNewState);
void S2LP_RADIO_SetPALeveldBm(uint8_t cIndex, int32_t wPowerdBm);
int32_t S2LP_RADIO_GetPALeveldBm(uint8_t cIndex);
uint8_t S2LP_RADIO_GetPALevelMaxIndex(void);
void S2LP_RADIO_SetPALevelMaxIndex(uint8_t cIndex);
void S2LP_RADIO_SetManualRampingMode(SFunctionalState xNewState);
void S2LP_RADIO_SetAutoRampingMode(SFunctionalState xNewState);

void S2LP_RADIO_AfcInit(SAfcInit* xSAfcInit);
void S2LP_RADIO_GetAfcInfo(SAfcInit* xSAfcInit);
void S2LP_RADIO_SymClkRecoverInit(SSymClkRecInit* xSSymClkRecInit);
void S2LP_RADIO_GetSymClkRecoverInfo(SSymClkRecInit* xSSymClkRecInit);


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
