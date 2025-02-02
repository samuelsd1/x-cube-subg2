/**
 * @file    S2LP_Gpio.c
 * @author  LowPower RF BU - AMG
 * @version 1.3.0
 * @date    10-July-2017
 * @brief   This file provides all the low level API to manage S2-LP GPIO.
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
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "s2lp_gpio.h"
#include "s2lp.h"

/** @addtogroup S2LP_Libraries
 * @{
 */


/** @addtogroup S2LP_Gpio
 * @{
 */




/** @defgroup Gpio_Private_Macros               GPIO Private Macros
 * @{
 */


#define IS_S2LP_GPIO(PIN)  ((PIN == S2LP_GPIO_0) || \
                              (PIN == S2LP_GPIO_1) || \
                              (PIN == S2LP_GPIO_2) || \
                              (PIN == S2LP_GPIO_3))


#define IS_S2LP_GPIO_MODE(MODE)   ((MODE == S2LP_GPIO_MODE_DIGITAL_INPUT) || \
                                   (MODE == S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP) || \
                                   (MODE == S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP))

#define IS_S2LP_GPIO_IO(IO_SEL)        ((IO_SEL == S2LP_GPIO_DIG_OUT_IRQ) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_POR_INV) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WUT_EXP) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_LBD) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TX_DATA) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TX_STATE) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_EMPTY) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_FULL) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_RX_DATA) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_RX_CLOCK) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_RX_STATE) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_NOT_STANDBY_SLEEP) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_STANDBY) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_ANTENNA_SWITCH) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_VALID_PREAMBLE) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_SYNC_DETECTED) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_RSSI_THRESHOLD) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_MCU_CLOCK) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TX_RX_MODE) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_VDD) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_GND) || \
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_SMPS_EXT) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_SLEEP) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_READY) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_LOCK) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WAIT_FOR_LOCK_SIG) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_TX_DATA_OOK_SIGNAL) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WAIT_FOR_READY2_SIG) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_PM_SET) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_WAIT_VCO_CALIBRATION) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_OUT_ENABLE_SYNTH_FULL_CIRCUIT) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_TX_COMMAND) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_RX_COMMAND) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_TX_DATA_INPUT_FOR_DIRECTRF) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_DATA_WAKEUP) ||\
                                          (IO_SEL == S2LP_GPIO_DIG_IN_EXT_CLOCK_AT_34_7KHZ))


#define IS_S2LP_GPIO_LEVEL(LEVEL)      ((LEVEL == LOW) || \
                                          (LEVEL == HIGH))

#define IS_S2LP_CLOCK_OUTPUT_XO(RATIO) ((RATIO == XO_RATIO_1) || \
                                           (RATIO == XO_RATIO_1_2) || \
                                           (RATIO == XO_RATIO_1_4) || \
                                           (RATIO == XO_RATIO_1_8) || \
                                           (RATIO == XO_RATIO_1_16) || \
                                           (RATIO == XO_RATIO_1_32) || \
                                           (RATIO == XO_RATIO_1_64) || \
                                           (RATIO == XO_RATIO_1_128) || \
                                           (RATIO == XO_RATIO_1_256))

#define IS_S2LP_CLOCK_OUTPUT_RCO(RATIO) ((RATIO == RCO_RATIO_1) || \
                                           (RATIO == RCO_RATIO_1_128))


#define IS_S2LP_CLOCK_OUTPUT_EXTRA_CYCLES(CYCLES) ((CYCLES == EXTRA_CLOCK_CYCLES_0) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_128) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_256) || \
                                                      (CYCLES == EXTRA_CLOCK_CYCLES_512))


#define IS_S2LP_IRQ_LIST(VALUE)   ((VALUE == RX_DATA_READY) || \
                                     (VALUE == RX_DATA_DISC)  || \
                                     (VALUE == TX_DATA_SENT)  || \
                                     (VALUE == MAX_RE_TX_REACH)  || \
                                     (VALUE == CRC_ERROR)  || \
                                     (VALUE == TX_FIFO_ERROR)  || \
                                     (VALUE == RX_FIFO_ERROR)  || \
                                     (VALUE == TX_FIFO_ALMOST_FULL)  || \
                                     (VALUE == TX_FIFO_ALMOST_EMPTY)  || \
                                     (VALUE == RX_FIFO_ALMOST_FULL)  || \
                                     (VALUE == RX_FIFO_ALMOST_EMPTY)  || \
                                     (VALUE == MAX_BO_CCA_REACH)  || \
                                     (VALUE == VALID_PREAMBLE)  || \
                                     (VALUE == VALID_SYNC)  || \
                                     (VALUE == RSSI_ABOVE_TH)  || \
                                     (VALUE == WKUP_TOUT_LDC)  || \
                                     (VALUE == READY)  || \
                                     (VALUE == STANDBY_DELAYED)  || \
                                     (VALUE == LOW_BATT_LVL)  || \
                                     (VALUE == POR)  || \
                                     (VALUE == BOR)  || \
                                     (VALUE == LOCK)  || \
                                     (VALUE == VCO_CALIBRATION_END)  || \
                                     (VALUE == PA_CALIBRATION_END)  || \
                                     (VALUE == PM_COUNT_EXPIRED)  || \
                                     (VALUE == XO_COUNT_EXPIRED)  || \
                                     (VALUE == TX_START_TIME)  || \
                                     (VALUE == RX_START_TIME)  || \
                                     (VALUE == RX_TIMEOUT)  || \
                                     (VALUE == RX_SNIFF_TIMEOUT)   || \
				                              (VALUE == ALL_IRQ ))



/**
 * @}
 */



/** @defgroup Gpio_Private_Functions            GPIO Private Functions
 * @{
 */

/**
 * @brief  Initialize the S2LP GPIOx according to the specified
 *         parameters in the pxGpioInitStruct.
 * @param  pxGpioInitStruct pointer to a SGpioInit structure that
 *         contains the configuration information for the specified S2LP GPIO.
 * @retval None.
 */
void S2LP_GPIO_Init(SGpioInit* pxGpioInitStruct)
{
  uint8_t tmp;

  tmp = ((uint8_t)(pxGpioInitStruct->xS2LP_GPIO_Mode) | (uint8_t)(pxGpioInitStruct->xS2LP_GPIO_IO));
  g_xStatus = S2LP_WriteRegister(pxGpioInitStruct->xS2LP_GPIO_Pin, 1, &tmp);

}



/**
 * @brief  Force S2LP GPIO_x configured as digital output, to VDD or GND.
 * @param  xGpioX Specifies the GPIO to be configured.
 *   This parameter can be one of following parameters:
 *     @arg S2LP_GPIO_0: S2LP GPIO_0
 *     @arg S2LP_GPIO_1: S2LP GPIO_1
 *     @arg S2LP_GPIO_2: S2LP GPIO_2
 *     @arg S2LP_GPIO_3: S2LP GPIO_3
 * @param  xLevel Specifies the level.
 *   This parameter can be: HIGH or LOW.
 * @retval None.
 */
void S2LP_GPIO_SetLevel(S2LP_GPIO_Pin xGpioX, OutputLevel xLevel)
{
  uint8_t tmp;

  /* Sets the value of the S2LP GPIO register according to the specified level */
  if(xLevel == HIGH) {
    tmp = (uint8_t)S2LP_GPIO_DIG_OUT_VDD | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP;
  }
  else {
    tmp = (uint8_t)S2LP_GPIO_DIG_OUT_GND | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP;
  }

  g_xStatus = S2LP_WriteRegister(xGpioX, 1, &tmp);

}


/**
 * @brief  Return output value (VDD or GND) of S2LP GPIO_x, when it is configured as digital output.
 * @param  xGpioX Specifies the GPIO to be read.
 *         This parameter can be one of following parameters:
 *         @arg S2LP_GPIO_0: S2LP GPIO_0
 *         @arg S2LP_GPIO_1: S2LP GPIO_1
 *         @arg S2LP_GPIO_2: S2LP GPIO_2
 *         @arg S2LP_GPIO_3: S2LP GPIO_3
 * @retval OutputLevel Logical level of selected GPIO configured as digital output.
 *         This parameter can be: HIGH or LOW.
 */
OutputLevel S2LP_GPIO_GetLevel(S2LP_GPIO_Pin xGpioX)
{
  uint8_t tmp = 0x00;

  g_xStatus = S2LP_ReadRegister(xGpioX, 1, &tmp);

  /* Mask the GPIO_SELECT field and returns the value according */
  tmp &= GPIO_SELECT_REGMASK;
  if(tmp == S2LP_GPIO_DIG_OUT_VDD) {
    return HIGH;
  }
  else {
    return LOW;
  }

}


/**
 * @brief  Deinit the S2LPIrqs structure setting all the bitfield to 0.
 *         Moreover, it sets the IRQ mask registers to 0x00000000, disabling all IRQs.
 * @param  pxIrqInit pointer to a variable of type @ref S2LPIrqs, in which all the
 *         bitfields will be settled to zero.
 * @retval None.
 */
void S2LP_GPIO_IrqDeInit(S2LPIrqs* pxIrqInit)
{
  uint8_t tmp[4] = {0x00,0x00,0x00,0x00};

  if(pxIrqInit!=NULL) {
    *pxIrqInit = (*(S2LPIrqs*)tmp);
  }

  g_xStatus = S2LP_WriteRegister(IRQ_MASK3_ADDR, 4, tmp);
}


/**
 * @brief  Enable the IRQs according to the user defined pxIrqInit structure.
 * @param  pxIrqInit pointer to a variable of type @ref S2LPIrqs, through which the
 *         user enable specific IRQs. This parameter is a pointer to a S2LPIrqs.
 *         For example suppose to enable only the two IRQ Low Battery Level and Tx Data Sent:
 * @code
 * S2LPIrqs myIrqInit = {0};
 * myIrqInit.IRQ_LOW_BATT_LVL = 1;
 * myIrqInit.IRQ_TX_DATA_SENT = 1;
 * S2LP_GPIO_IrqInit(&myIrqInit);
 * @endcode
 * @retval None.
 */
void S2LP_GPIO_IrqInit(S2LPIrqs* pxIrqInit)
{
  uint8_t tmp[4];
  uint8_t* tmpPoint;

  tmpPoint = (uint8_t*)(pxIrqInit);
  for(int i=0; i<4; i++) {
    tmp[3-i]= tmpPoint[i];
  }

  g_xStatus = S2LP_WriteRegister(IRQ_MASK3_ADDR, 4, tmp);

}


/**
 * @brief  Enable or disables a specific IRQ.
 * @param  xIrq IRQ to enable or disable.
 *         This parameter can be any value of @ref IrqList.
 * @param  xNewState new state for the IRQ.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LP_GPIO_IrqConfig(IrqList xIrq, SFunctionalState xNewState)
{
  uint8_t tmpBuffer[4];
  uint32_t tempValue = 0;
/* All the 4 bytes of IRQ Status register is being read */
  S2LP_ReadRegister(IRQ_MASK3_ADDR, 4, tmpBuffer);

  /* Build the IRQ mask word */
  for(int i=0; i<4; i++) {
    tempValue += ((uint32_t)tmpBuffer[i])<<(8*(3-i));
  }
  
  /* Rebuild the new mask according to user request */
  if(xNewState == S_DISABLE) {
    tempValue &= (~xIrq);
  }
  else {
    tempValue |= (xIrq);
  }

  /* Build the array of bytes to write in the IRQ_MASK registers */
  for(int j=0; j<4; j++) {
    tmpBuffer[j] = (uint8_t)(tempValue>>(8*(3-j)));
  }
  
  g_xStatus = S2LP_WriteRegister(IRQ_MASK3_ADDR, 4, tmpBuffer);

}


/**
 * @brief  Fill a pointer to a structure of S2LPIrqs type reading the IRQ_MASK registers.
 * @param  pxIrqMask pointer to a variable of type @ref S2LPIrqs, through which the
 *         user can read which IRQs are enabled. All the bitfields equals to zero correspond
 *         to enabled IRQs, while all the bitfields equals to one correspond to disabled IRQs.
 *         This parameter is a pointer to a S2LPIrqs.
 *         For example suppose that the Power On Reset and RX Data ready are the only enabled IRQs.
 * @code
 * S2LPIrqs myIrqMask;
 * S2LPIrqGetStatus(&myIrqMask);
 * @endcode
 * Then
 * myIrqMask.IRQ_POR and myIrqMask.IRQ_RX_DATA_READY are equal to 0
 * while all the other bitfields are equal to one.
 * @retval None.
 */
void S2LP_GPIO_IrqGetMask(S2LPIrqs* pxIrqMask)
{
  uint8_t tmp[4];
  uint8_t* pIrqPointer = (uint8_t*)pxIrqMask;

  g_xStatus = S2LP_ReadRegister(IRQ_MASK3_ADDR, 4, tmp);

  for(char i=0; i<4; i++) {
    *pIrqPointer = tmp[3-i];
    pIrqPointer++;
  }

}


/**
 * @brief  Fill a pointer to a structure of S2LPIrqs type reading the IRQ_STATUS registers.
 * @param  pxIrqStatus pointer to a variable of type @ref S2LPIrqs, through which the
 *         user can read the status of all the IRQs. All the bitfields equals to one correspond
 *         to the raised interrupts. This parameter is a pointer to a S2LPIrqs.
 *         For example suppose that the XO settling timeout is raised as well as the Sync word
 *         detection.
 * @code
 * S2LPIrqs myIrqStatus;
 * S2LP_GPIO_IrqGetStatus(&myIrqStatus);
 * @endcode
 * Then
 * myIrqStatus.IRQ_XO_COUNT_EXPIRED and myIrqStatus.IRQ_VALID_SYNC are equals to 1
 * while all the other bitfields are equals to zero.
 * @retval None.
 */
void S2LP_GPIO_IrqGetStatus(S2LPIrqs* pxIrqStatus)
{
  uint8_t tmp[4];
  uint8_t* pIrqPointer = (uint8_t*)pxIrqStatus;
  
  /* all the 4 bytes of irq status register is being read */
  g_xStatus = S2LP_ReadRegister(IRQ_STATUS3_ADDR, 4, tmp);

  /* Build the IRQ Status word */
  for(uint8_t i=0; i<4; i++) {
    *pIrqPointer = tmp[3-i];
    pIrqPointer++;
  }
}


/**
 * @brief  Clear the IRQ status registers.
 * @param  None.
 * @retval None.
 */
void S2LP_GPIO_IrqClearStatus(void)
{
  uint8_t tmp[4];
  g_xStatus = S2LP_ReadRegister(IRQ_STATUS3_ADDR, 4, tmp);

}


/**
 * @brief  Verifie if a specific IRQ has been generated.
 *         The call resets all the IRQ status, so it can't be used in case of multiple raising interrupts.
 * @param  xFlag IRQ flag to be checked.
 *         This parameter can be any value of @ref IrqList.
 * @retval SBool S_TRUE or S_FALSE.
 */
SBool S2LP_GPIO_IrqCheckFlag(IrqList xFlag)
{
  uint8_t tmp[4];
  uint32_t tempValue = 0;
  SBool flag;

  g_xStatus = S2LP_ReadRegister(IRQ_STATUS3_ADDR, 4, tmp);
  for(uint8_t i=0; i<4; i++) {
    tempValue += ((uint32_t)tmp[i])<<(8*(3-i));
  }
  
  if(tempValue & xFlag) {
    flag = S_TRUE;
  }
  else {
    flag = S_FALSE;
  }

  return flag;

}

/**
 * @}
 */


/**
 * @}
 */


/**
 * @}
 */



/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
