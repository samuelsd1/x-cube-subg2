/**
  ******************************************************************************
  * @file    button-sensor.c
  * @author  SRA Application Team
  * @brief   Enable button sensor functionality 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Software License Agreement
  * SLA0055, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0055
  *
  ******************************************************************************
  */

/**
 * @defgroup button_sensor
 * @ingroup dev
 * @{
 */

/**
 * @addtogroup button_sensor
 * @ingroup dev
 * @{
 * @file Driver for the board User Button
 */

/* Includes ------------------------------------------------------------------*/
#include "dev/button-sensor.h"
#include "lib/sensors.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
static int _active = 0 ;
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void init(void)
{
  /* See stm32**xx_it.c (**=F4,L1,L4, ...) for the callback that triggers the
   * relevant "sensors_changed" event
   */
  (void) BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
}
/*---------------------------------------------------------------------------*/
static void activate(void)
{
  _active = 1;
}
/*---------------------------------------------------------------------------*/
static void deactivate(void)
{
  _active = 0;
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
static int active(void)
{
  return _active;
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
static int value(int type)
{
  UNUSED(type);
  /*Note: 0 means pressed, 1 means not pressed.*/
  return BSP_PB_GetState(BUTTON_USER);
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
static int configure(int type, int val)
{
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
  int retval;
  
  switch(type) {
    case SENSORS_HW_INIT:
      init();
      retval = 1;
      break;
    case SENSORS_ACTIVE:
      if(val != 0) {      
        activate();
      } else {
        deactivate();
      }
      retval = 1;
      break;
    default:
      retval = 0;
      break;
  }
 
  return retval;
}
/*---------------------------------------------------------------------------*/
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
static int status(int type)
{
/* MISRA C-2012 Dir-4.6_a violation for purpose: Contiki-NG API */
  int retval;
  
  switch(type) {
    case SENSORS_READY:
      retval = active();
      break;
    default:
      retval = 0;
      break;
  }
  
  return retval;
}
/*--------------------------------------------------------------------------- */
/* Functions Definition ------------------------------------------------------*/
SENSORS_SENSOR((button_sensor), (BUTTON_SENSOR), (value), (configure), (status));
/*---------------------------------------------------------------------------*/
/** @} */
/** @} */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
