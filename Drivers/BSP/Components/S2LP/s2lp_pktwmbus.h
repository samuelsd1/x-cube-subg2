/**
 * @file    S2LP_PktWMbus.h
 * @author  LowPower RF BU - AMG
 * @version 1.3.0
 * @date    10-July-2017
 * @brief   Configuration and management of S2-LP WMbus packets.
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
 * This module can be used to manage the configuration of S2-LP WMbus
 * packets.
 * The user can obtain a packet configuration filling the structure
 * <i>@ref PktWMbusInit</i>, defining in it some general parameters
 * for the S2-LP WMbus packet format.
 * Since the WMbus protocol is a standard, the configuration of a WMbus
 * packet is very simple to do.
 *
 * <b>Example:</b>
 * @code
 *
 * PktWMbusInit mbusInit={
 *   WMbus_SUBMODE_S1_S2_LONG_HEADER,    // WMbus submode selection
 *   36,                                // added "01" chips on preamble
 *   16                                 // postamble length in "01" chips
 * };
 *
 * ...
 *
 * S2LP_PACKET_WMBUS_Init(&mbusInit);
 *
 * ...
 *
 * @endcode
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __S2LP_PACKET_WMBUS_H
#define __S2LP_PACKET_WMBUS_H


/* Includes ------------------------------------------------------------------*/

#include "s2lp_regs.h"
#include "s2lp_types.h"
#include "s2lp_packethandler.h"

#ifdef __cplusplus
 extern "C" {
#endif



/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @defgroup S2LP_PktWMbus     Pkt WMbus
 * @brief Configuration and management of S2LP WMbus packets.
 * @details See the file <i>@ref S2LP_PktWMbus.h</i> for more details.
 * @{
 */

/**
 * @defgroup PktWMbus_Exported_Types     Pkt WMbus Exported Types
 * @{
 */



/**
 * @brief  WMbus submode enumeration.
 */

typedef enum {
  WMBUS_SUBMODE_NOT_CONFIGURED            = 0,   /*!< WMBUS submode S1, S2 (long header) - Header length = WMBUS_prmbl_ctrl + 279 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits) */
  WMBUS_SUBMODE_S1_S2_LONG_HEADER,               /*!< WMBUS submode S1, S2 (long header) - Header length = WMBUS_prmbl_ctrl + 279 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits) */
  WMBUS_SUBMODE_S1_M_S2_T2_OTHER_TO_METER,       /*!< WMBUS submode S1-m, S2, T2 (other to meter) - Header length = WMBUS_prmbl_ctrl + 15 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits)*/
  WMBUS_SUBMODE_T1_T2_METER_TO_OTHER,            /*!< WMBUS submode T1, T2 (meter to other) - Header length = WMBUS_prmbl_ctrl + 19 (in "01" bit pairs) ,  Sync word = 0x3D (length 10 bits)*/
  WMBUS_SUBMODE_R2_SHORT_HEADER,                 /*!< WMBUS submode R2, short header - Header length = WMBUS_prmbl_ctrl + 39 (in "01" bit pairs) , Sync word = 0x7696 (length 18 bits)*/
} WMbusSubmode;


/**
 * @brief  S2LP WMBUS Packet Init structure definition
 */
typedef struct {
  WMbusSubmode   xWMbusSubmode;      /*!< Set the WMBUS submode. @ref WMbusSubmode */
  uint8_t        cPreambleLength;    /*!< Set the preamble length in chip sequence */
  uint8_t        cPostambleLength;   /*!< Set the postamble length in chip sequence */
} PktWMbusInit;

/**
 *@}
 */



/**
 * @defgroup PktWMbus_Exported_Functions                 Pkt WMbus Exported Functions
 * @{
 */

/**
 * @brief  Sets the PREAMBLE field length for S2LP Basic packets.
 * @param  xPreambleLength length of PREAMBLE field in bytes.
 *         This parameter can be any value of @ref BasicPreambleLength.
 * @retval None.
 */
#define S2LP_PACKET_WMBUS_SetPreambleLength(xPreambleLength)                S2LPPktCommonSetPreambleLength((PktPreambleLength)xPreambleLength)


/**
 * @brief  Returns the PREAMBLE field length mode for S2LP Basic packets.
 * @param  None.
 * @retval uint8_t Preamble field length in bytes.
 */
#define S2LP_PACKET_WMBUS_GetPreambleLength()                               S2LPPktCommonGetPreambleLength()


void S2LP_PACKET_WMBUS_Init(PktWMbusInit* pxPktWMbusInit);
void S2LP_PACKET_WMBUS_GetInfo(PktWMbusInit* pxPktWMbusInit);
void S2LP_PACKET_WMBUS_SetFormat(void);
void S2LP_PACKET_WMBUS_SetPostamble(uint8_t cPostamble);
uint8_t S2LP_PACKET_WMBUS_GetPostamble(void);
void S2LP_PACKET_WMBUS_SetPostamblePattern(uint8_t cPostamble);
uint8_t S2LP_PACKET_WMBUS_GetPostamblePattern(void);
void S2LP_PACKET_WMBUS_SetSubmode(WMbusSubmode xWMbusSubmode);
WMbusSubmode S2LP_PACKET_WMBUS_GetSubmode(void);
void S2LP_PACKET_WMBUS_SetPayloadLength(uint16_t nPayloadLength);
uint16_t S2LP_PACKET_WMBUS_GetPayloadLength(void);


/**
 *@}
 */

/**
 *@}
 */


/**
 *@}
 */


#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
