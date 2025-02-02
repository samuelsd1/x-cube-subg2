/**
 * @file    S2LP_PktBasic.c
 * @author  LowPower RF BU - AMG
 * @version 1.3.0
 * @date    10-July-2017
 * @brief   Configuration and management of S2-LP Basic packets.
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
 */


/* Includes ------------------------------------------------------------------*/
#include "s2lp_pktbasic.h"
#include "s2lp.h"
#include "s2lp_pktwmbus.h"


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_PktBasic
 * @{
 */



/**
 * @defgroup PktBasic_Private_Defines           Pkt Basic Private Defines
 * @{
 */

#define PKT_FORMAT_BASIC_CODE                   0x00
  
/**
 *@}
 */


/**
 * @defgroup PktBasic_Private_Macros            Pkt Basic Private Macros
 * @{
 */
  
/**
 *@}
 */



/**
 * @defgroup PktBasic_Private_Functions                 Pkt Basic Private Functions
 * @{
 */

/**
 * @brief  Initialize the S2LP Basic packet according to the specified parameters in the PktBasicInit struct.
 *         Notice that this function sets the autofiltering option on CRC if it is set to any value different from BASIC_NO_CRC.
 * @param  pxPktBasicInit Basic packet init structure.
 *         This parameter is a pointer to @ref PktBasicInit.
 * @retval None.
 */
void S2LP_PCKT_BASIC_Init(PktBasicInit* pxPktBasicInit)
{
  uint8_t tmpBuffer[6];

  S2LP_PACKET_WMBUS_SetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);
  
  /* Always set the automatic packet filtering */
  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);
  tmpBuffer[0] |= AUTO_PCKT_FLT_REGMASK;
  S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);

  tmpBuffer[0] = ((pxPktBasicInit->xSyncLength)<<2) | (uint8_t)((pxPktBasicInit->xPreambleLength)>>8);
  tmpBuffer[1] = (uint8_t)(pxPktBasicInit->xPreambleLength);
  tmpBuffer[2] = (((uint8_t)pxPktBasicInit->xAddressField)<<3);
  
  if((pxPktBasicInit->cExtendedPktLenField)==S_ENABLE)
  {
    tmpBuffer[2]|=0x80;
  }
  
  S2LP_ReadRegister(PCKTCTRL3_ADDR, 1, &tmpBuffer[3]);
  tmpBuffer[3] &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmpBuffer[3] |= PKT_FORMAT_BASIC_CODE;
  
  S2LP_ReadRegister(PCKTCTRL2_ADDR, 2, &tmpBuffer[4]);
  
  if(pxPktBasicInit->xFixVarLength == S_ENABLE) {
     tmpBuffer[4] |= FIX_VAR_LEN_REGMASK;
  }
  else {
     tmpBuffer[4] &= ~FIX_VAR_LEN_REGMASK;
  }
  tmpBuffer[4] &= ~(MANCHESTER_EN_REGMASK | MBUS_3OF6_EN_REGMASK);
  
  tmpBuffer[5] &= ~(CRC_MODE_REGMASK | TXSOURCE_REGMASK);
  tmpBuffer[5] |= (uint8_t)pxPktBasicInit->xCrcMode;
  
  if(pxPktBasicInit->xDataWhitening == S_ENABLE) {
     tmpBuffer[5] |= WHIT_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~WHIT_EN_REGMASK;
  }

  if(pxPktBasicInit->xFec == S_ENABLE)
  {
     tmpBuffer[5] |= FEC_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~FEC_EN_REGMASK;
  }
  
  S2LP_WriteRegister(PCKTCTRL6_ADDR, 6, tmpBuffer);  
  
  /* SYNC word */
  for(uint8_t i=0 ; i<4 ; i++) {
    tmpBuffer[i] = (uint8_t)(pxPktBasicInit->lSyncWords>>(8*i));
  }
  g_xStatus = S2LP_WriteRegister(SYNC3_ADDR, 4, tmpBuffer);
  
  /* Sets CRC check bit */
  if(pxPktBasicInit->xCrcMode == PKT_NO_CRC) {
    S2LP_PCKT_BASIC_FilterOnCrc(S_DISABLE);
  }
  else {
    S2LP_PCKT_BASIC_FilterOnCrc(S_ENABLE);
  }
  
    /* Constellation map setting */
  S2LP_ReadRegister(MOD1_ADDR, 1, tmpBuffer);
  tmpBuffer[0] &= ~G4FSK_CONST_MAP_REGMASK;
  S2LP_WriteRegister(MOD1_ADDR, 1, tmpBuffer); 
}


/**
 * @brief  Return the S2LP Basic packet structure according to the specified parameters in the registers.
 * @param  pxPktBasicInit Basic packet init structure.
 *         This parameter is a pointer to @ref PktBasicInit.
 * @retval None.
 */
void S2LP_PCKT_BASIC_GetInfo(PktBasicInit* pxPktBasicInit)
{
  uint8_t tmpBuffer[6];

  S2LP_ReadRegister(PCKTCTRL6_ADDR, 6, tmpBuffer);
  
  /* Sync length */
  pxPktBasicInit->xSyncLength = ((tmpBuffer[0] & SYNC_LEN_REGMASK)>>2);
  
  /* Preamble length */
  pxPktBasicInit->xPreambleLength = (((uint16_t)(tmpBuffer[0] & PREAMBLE_LEN_9_8_REGMASK))<<8) | ((uint16_t)tmpBuffer[1]);
  
  /* Length width */
  pxPktBasicInit->cExtendedPktLenField = (SFunctionalState)((tmpBuffer[2] & LEN_WID_REGMASK)>>7);
  
  /* Address field */
  pxPktBasicInit->xAddressField = (SFunctionalState)((tmpBuffer[2] & ADDRESS_LEN_REGMASK)>>3);

  /* FIX or VAR bit */
  pxPktBasicInit->xFixVarLength = (SFunctionalState)(tmpBuffer[4] & FIX_VAR_LEN_REGMASK);

  /* CRC mode */
  pxPktBasicInit->xCrcMode = (BasicCrcMode)(tmpBuffer[5] & CRC_MODE_REGMASK);
  
  /* Whitening */
  pxPktBasicInit->xDataWhitening = (SFunctionalState)((tmpBuffer[5] & WHIT_EN_REGMASK)>> 4);

  /* FEC */
  pxPktBasicInit->xFec = (SFunctionalState)(tmpBuffer[5] & FEC_EN_REGMASK);
  
  g_xStatus = S2LP_ReadRegister(SYNC3_ADDR, 4, tmpBuffer);
  
  /* SYNC word */
  pxPktBasicInit->lSyncWords = 0;
  for(uint8_t i=0 ; i<4 ; i++) {
    pxPktBasicInit->lSyncWords |= ((uint32_t)tmpBuffer[i])<<(8*i);
  }
}


/**
 * @brief  Initialize the S2LP Basic packet addresses according to the specified
 *         parameters in the PktBasicAddressesInit struct.
 * @param  pxPktBasicAddresses Basic packet addresses init structure.
 *         This parameter is a pointer to @ref PktBasicAddresses.
 * @retval None.
 */
void S2LP_PCKT_BASIC_AddressesInit(PktBasicAddressesInit* pxPktBasicAddresses)
{
  uint8_t tmpBuffer[3];

  /* Reads the PCKT_FLT_OPTIONS ragister */
  S2LP_ReadRegister(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);
  
  /* Enables or disables filtering on my address */
  if(pxPktBasicAddresses->xFilterOnMyAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_SOURCE_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_SOURCE_ADDR_REGMASK;
  }
  
  /* Enables or disables filtering on multicast address */
  if(pxPktBasicAddresses->xFilterOnMulticastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_MULTICAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_MULTICAST_ADDR_REGMASK;
  }
  
  /* Enables or disables filtering on broadcast address */
  if(pxPktBasicAddresses->xFilterOnBroadcastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_BROADCAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_BROADCAST_ADDR_REGMASK;
  }
  
  S2LP_WriteRegister(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);

  /* Fills the array with the addresses passed in the structure */
  tmpBuffer[0] = pxPktBasicAddresses->cBroadcastAddress;
  tmpBuffer[1] = pxPktBasicAddresses->cMulticastAddress;
  tmpBuffer[2] = pxPktBasicAddresses->cMyAddress;
  g_xStatus = S2LP_WriteRegister(PCKT_FLT_GOALS2_ADDR, 3, tmpBuffer);
}


/**
 * @brief  Return the S2LP Basic packet addresses structure according to the specified
 *         parameters in the registers.
 * @param  pxPktBasicAddresses Basic packet addresses init structure.
 *         This parameter is a pointer to @ref PktBasicAddresses.
 * @retval None.
 */
void S2LP_PCKT_BASIC_GetAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses)
{
  uint8_t tmpBuffer[3];

  S2LP_ReadRegister(PCKT_FLT_GOALS2_ADDR, 3, tmpBuffer);
  pxPktBasicAddresses->cBroadcastAddress = tmpBuffer[0];
  pxPktBasicAddresses->cMulticastAddress = tmpBuffer[1];
  pxPktBasicAddresses->cMyAddress = tmpBuffer[2];
  
  g_xStatus = S2LP_ReadRegister(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);
  pxPktBasicAddresses->xFilterOnBroadcastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_BROADCAST_ADDR_REGMASK) >> 3);
  pxPktBasicAddresses->xFilterOnMulticastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_MULTICAST_ADDR_REGMASK) >> 2);
  pxPktBasicAddresses->xFilterOnMyAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_SOURCE_ADDR_REGMASK) >> 1);
}


/**
 * @brief  Configure the Basic packet format as packet used by S2LP.
 * @param  None.
 * @retval None.
 */
void S2LP_PCKT_BASIC_SetFormat(void)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(PCKTCTRL3_ADDR, 1, &tmp);
  
  /* Build the new value. Also set to 0 the direct RX mode bits */
  tmp &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmp |= PKT_FORMAT_BASIC_CODE;
  S2LP_WriteRegister(PCKTCTRL3_ADDR, 1, &tmp);

  S2LP_ReadRegister(PCKTCTRL1_ADDR, 1, &tmp);
  
  /* Set to 0 the direct TX mode bits */
  tmp &= ~TXSOURCE_REGMASK;
  g_xStatus = S2LP_WriteRegister(PCKTCTRL1_ADDR, 1, &tmp);

  S2LP_PACKET_WMBUS_SetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);
}


/**
 * @brief  Set the address length for S2LP Basic packets.
 * @param  xAddressField length of ADDRESS in bytes.
 *         This parameter can be: S_ENABLE or S_DISABLE.
 * @retval None.
 */
void S2LP_PCKT_BASIC_AddressField(SFunctionalState xAddressField)
{
  uint8_t tmp;

  S2LP_ReadRegister(PCKTCTRL4_ADDR, 1, &tmp);
  if(xAddressField==S_ENABLE) {
    tmp |= ADDRESS_LEN_REGMASK;
  }
  else {
    tmp &= ADDRESS_LEN_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PCKTCTRL4_ADDR, 1, &tmp);

}


/**
 * @brief  Specify if the Address field for S2LP Basic packets is enabled or disabled.
 * @param  None.
 * @retval SFunctionalState Notifies if the address field is enabled or disabled.
 */
SFunctionalState S2LP_PCKT_BASIC_GetAddressField(void)
{
  uint8_t tmp;

  g_xStatus = S2LP_ReadRegister(PCKTCTRL4_ADDR, 1, &tmp);
  if(tmp & ADDRESS_LEN_REGMASK) {
    return S_ENABLE;
  }
  else {
    return S_DISABLE;
  }

}


/**
 * @brief  Set the payload length for S2LP Basic packets. Since the packet length
 *         depends from the address and the control field size, this
 *         function reads the correspondent registers in order to determine
 *         the correct packet length to be written.
 * @param  nPayloadLength payload length in bytes.
 *         This parameter is an uint16_t.
 * @retval None.
 */
void S2LP_PCKT_BASIC_SetPayloadLength(uint16_t nPayloadLength)
{
  uint8_t tmpBuffer[2];
  
  if(S2LP_PCKT_BASIC_GetAddressField()) {
    nPayloadLength++;
  }
  tmpBuffer[0] = (uint8_t)(nPayloadLength>>8);
  tmpBuffer[1] = (uint8_t)nPayloadLength;
  g_xStatus = S2LP_WriteRegister(PCKTLEN1_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Return the payload length for S2LP Basic packets. Since the
 *         packet length depends from the address and the control
 *         field size, this function reads the correspondent
 *         registers in order to determine the correct payload length
 *         to be returned.
 * @param  None.
 * @retval uint16_t Payload length in bytes.
 */
uint16_t S2LP_PCKT_BASIC_GetPayloadLength(void)
{
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  g_xStatus = S2LP_ReadRegister(PCKTLEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);
  
  if(S2LP_PCKT_BASIC_GetAddressField()) {
    nPayloadLength--;
  }
  return nPayloadLength;
}

/**
 * @brief  Return the packet length field of the received packet.
 * @param  None.
 * @retval uint16_t Packet length.
 */
uint16_t S2LP_PCKT_BASIC_GetReceivedPktLength(void)
{
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  g_xStatus = S2LP_ReadRegister(RX_PCKT_LEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);
  
  if(S2LP_PCKT_BASIC_GetAddressField()) {
    nPayloadLength--;
  }
  return nPayloadLength;
}


/**
 *@}
 */

/**
 *@}
 */


/**
 *@}
 */



/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
