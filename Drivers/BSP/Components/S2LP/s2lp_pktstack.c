/**
 * @file    S2LP_PktStack.c
 * @author  LowPower RF BU - AMG
 * @version 1.3.0
 * @date    10-July-2017
 * @brief   Configuration and management of S2-LP STack packets.
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
 *
 * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "s2lp_pktstack.h"
#include "s2lp.h"
#include "s2lp_pktwmbus.h"


/**
 * @addtogroup S2LP_Libraries
 * @{
 */


/**
 * @addtogroup S2LP_PktStack
 * @{
 */

/**
 * @defgroup PktStack_Private_Defines                   Pkt STack Private Defines
 * @{
 */
#define PKT_FORMAT_STACK_CODE           (uint8_t)3

/**
 *@}
 */


/**
 * @defgroup PktStack_Private_Macros                    Pkt STack Private Macros
 * @{
 */

/**
 *@}
 */


/**
 * @defgroup PktStack_Private_Functions                 Pkt STack Private Functions
 * @{
 */


/**
 * @brief  Initialize the S2LP STack packet according to the specified
 *         parameters in the PktStackInit.
 * @param  pxPktStackInit STack packet init structure.
 *         This parameter is a pointer to @ref PktStackInit.
 * @retval None.
 */
void S2LP_PCKT_STACK_Init(PktStackInit* pxPktStackInit)
{
  uint8_t tmpBuffer[6];

  S2LP_PACKET_WMBUS_SetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);
  
  /* Always set the automatic packet filtering */
  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);
  tmpBuffer[0] |= AUTO_PCKT_FLT_REGMASK;
  S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);

  tmpBuffer[0] = ((pxPktStackInit->xSyncLength)<<2) | (uint8_t)((pxPktStackInit->xPreambleLength)>>8);
  tmpBuffer[1] = (uint8_t)(pxPktStackInit->xPreambleLength);
  tmpBuffer[2] = ((uint8_t)pxPktStackInit->cExtendedPktLenField)<<7 | ADDRESS_LEN_REGMASK;
  
  S2LP_ReadRegister(PCKTCTRL3_ADDR, 1, &tmpBuffer[3]);
  tmpBuffer[3] &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmpBuffer[3] |= PKT_FORMAT_STACK_CODE<<6;
  
  S2LP_ReadRegister(PCKTCTRL2_ADDR, 2, &tmpBuffer[4]);
  
  if(pxPktStackInit->xFixVarLength == S_ENABLE) {
     tmpBuffer[4] |= FIX_VAR_LEN_REGMASK;
  }
  else {
     tmpBuffer[4] &= ~FIX_VAR_LEN_REGMASK;
  }
  tmpBuffer[4] &= ~(MANCHESTER_EN_REGMASK | MBUS_3OF6_EN_REGMASK);
  
  tmpBuffer[5] &= ~(CRC_MODE_REGMASK | TXSOURCE_REGMASK);
  tmpBuffer[5] |= (uint8_t)pxPktStackInit->xCrcMode;
  
  if(pxPktStackInit->xDataWhitening == S_ENABLE) {
     tmpBuffer[5] |= WHIT_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~WHIT_EN_REGMASK;
  }

  if(pxPktStackInit->xFec == S_ENABLE) {
     tmpBuffer[5] |= FEC_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~FEC_EN_REGMASK;
  }
  
  S2LP_WriteRegister(PCKTCTRL6_ADDR, 6, tmpBuffer);  

  /* SYNC word */
  for(uint8_t i=0 ; i<4 ; i++) {
    tmpBuffer[i] = (uint8_t)(pxPktStackInit->lSyncWords>>(8*i));
  }
  g_xStatus = S2LP_WriteRegister(SYNC3_ADDR, 4, tmpBuffer);
  
  /* Sets CRC check bit */
  if(pxPktStackInit->xCrcMode == PKT_NO_CRC) {
    S2LP_PCKT_STACK_FilterOnCrc(S_DISABLE);
  }
  else {
    S2LP_PCKT_STACK_FilterOnCrc(S_ENABLE);
  }
    /* Constellation map setting */
  S2LP_ReadRegister(MOD1_ADDR, 1, tmpBuffer);
  tmpBuffer[0] &= ~G4FSK_CONST_MAP_REGMASK;
  S2LP_WriteRegister(MOD1_ADDR, 1, tmpBuffer); 
}


/**
 * @brief  Return the S2LP STack packet structure according to the specified parameters in the registers.
 * @param  pxPktStackInit STack packet init structure.
 *         This parameter is a pointer to @ref PktStackInit.
 * @retval None.
 */
void S2LP_PCKT_STACK_GetInfo(PktStackInit* pxPktStackInit)
{
  uint8_t tmpBuffer[6];

  S2LP_ReadRegister(PCKTCTRL6_ADDR, 6, tmpBuffer);
  
  /* Sync length */
  pxPktStackInit->xSyncLength = ((tmpBuffer[0] & SYNC_LEN_REGMASK)>>2);
  
  /* Preamble length */
  pxPktStackInit->xPreambleLength = (((uint16_t)(tmpBuffer[0] & PREAMBLE_LEN_9_8_REGMASK))<<8) | ((uint16_t)tmpBuffer[1]);
  
  /* Length width */
  pxPktStackInit->cExtendedPktLenField = (SFunctionalState)((tmpBuffer[2] & LEN_WID_REGMASK)>>7);

  /* FIX or VAR bit */
  pxPktStackInit->xFixVarLength = (SFunctionalState)(tmpBuffer[4] & FIX_VAR_LEN_REGMASK);

  /* CRC mode */
  pxPktStackInit->xCrcMode = (StackCrcMode)(tmpBuffer[5] & CRC_MODE_REGMASK);
  
  /* Whitening */
  pxPktStackInit->xDataWhitening = (SFunctionalState)((tmpBuffer[5] & WHIT_EN_REGMASK)>> 4);

  /* FEC */
  pxPktStackInit->xFec = (SFunctionalState)(tmpBuffer[5] & FEC_EN_REGMASK);
  
  g_xStatus = S2LP_ReadRegister(SYNC3_ADDR, 4, tmpBuffer);
  
  /* SYNC word */
  pxPktStackInit->lSyncWords = 0;
  for(uint8_t i=0 ; i<4 ; i++) {
    pxPktStackInit->lSyncWords |= ((uint32_t)tmpBuffer[3-i])<<(8*i);
  }
  
}


/**
 * @brief  Initialize the S2LP STack packet addresses according to the specified
 *         parameters in the PktStackAddresses struct.
 * @param  pxPktStackAddresses STack packet addresses init structure.
 *         This parameter is a pointer to @ref PktStackAddressesInit .
 * @retval None.
 */
void S2LP_PCKT_STACK_AddressesInit(PktStackAddressesInit* pxPktStackAddresses)
{
  uint8_t tmpBuffer[3];

  /* Reads the PCKT_FLT_OPTIONS ragister */
  S2LP_ReadRegister(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);
  
  /* Enables or disables filtering on my address */
  if(pxPktStackAddresses->xFilterOnMyAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_SOURCE_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_SOURCE_ADDR_REGMASK;
  }
  
  /* Enables or disables filtering on multicast address */
  if(pxPktStackAddresses->xFilterOnMulticastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_MULTICAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_MULTICAST_ADDR_REGMASK;
  }
  
  /* Enables or disables filtering on broadcast address */
  if(pxPktStackAddresses->xFilterOnBroadcastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_BROADCAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_BROADCAST_ADDR_REGMASK;
  }
  
  S2LP_WriteRegister(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);
  
  /* Fills the array with the addresses passed in the structure */
  tmpBuffer[2] = pxPktStackAddresses->cMyAddress;
  tmpBuffer[0] = pxPktStackAddresses->cBroadcastAddress;
  tmpBuffer[1] = pxPktStackAddresses->cMulticastAddress;
  g_xStatus = S2LP_WriteRegister(PCKT_FLT_GOALS2_ADDR, 3, tmpBuffer);
}


/**
* @brief  Return the S2LP STack packet addresses structure according to the specified
*         parameters in the registers.
* @param  pxPktStackAddresses STack packet addresses init structure.
*         This parameter is a pointer to @ref PktStackAddresses.
* @retval None.
*/
void S2LP_PCKT_STACK_GetAddressesInfo(PktStackAddressesInit* pxPktStackAddresses)
{
  uint8_t tmpBuffer[3];

  S2LP_ReadRegister(PCKT_FLT_GOALS3_ADDR, 3, tmpBuffer);
  pxPktStackAddresses->cMyAddress = tmpBuffer[0];
  pxPktStackAddresses->cBroadcastAddress = tmpBuffer[1];
  pxPktStackAddresses->cMulticastAddress = tmpBuffer[2];
  
  g_xStatus = S2LP_ReadRegister(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);
  pxPktStackAddresses->xFilterOnBroadcastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_BROADCAST_ADDR_REGMASK) >> 3);
  pxPktStackAddresses->xFilterOnMulticastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_MULTICAST_ADDR_REGMASK) >> 2);
  pxPktStackAddresses->xFilterOnMyAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_SOURCE_ADDR_REGMASK) >> 1);
}


/**
 * @brief  Configure the STack packet format for S2LP.
 * @param  None.
 * @retval None.
 */
void S2LP_PCKT_STACK_SetFormat(void)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(PCKTCTRL3_ADDR, 1, &tmp);
  
  /* Build the new value. Also set to 0 the direct RX mode bits */
  tmp &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmp |= PKT_FORMAT_STACK_CODE;
  S2LP_WriteRegister(PCKTCTRL3_ADDR, 1, &tmp);

  S2LP_ReadRegister(PCKTCTRL1_ADDR, 1, &tmp);
  
  /* Set to 0 the direct TX mode bits */
  tmp &= ~TXSOURCE_REGMASK;
  g_xStatus = S2LP_WriteRegister(PCKTCTRL1_ADDR, 1, &tmp);

  S2LP_PACKET_WMBUS_SetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);
}



/**
 * @brief  Set the payload length for S2LP STack packets. Since the packet length
 *         depends from the address (always 2 for this packet format)
 *         and the control field size, this function reads the control length register
 *         content in order to determine the correct packet length to be written.
 * @param  nPayloadLength payload length in bytes.
 *         This parameter can be any value of uint16_t.
 * @retval None.
 */
void S2LP_PCKT_STACK_SetPayloadLength(uint16_t nPayloadLength)
{
  uint8_t tmpBuffer[2];

  nPayloadLength+=2;
  tmpBuffer[0] = (uint8_t)(nPayloadLength>>8);
  tmpBuffer[1] = (uint8_t)nPayloadLength;
  g_xStatus = S2LP_WriteRegister(PCKTLEN1_ADDR, 2, tmpBuffer);
}


/**
 * @brief  Return the payload length for S2LP STack packets. Since the
 *         packet length depends from the address and the control
 *         field size, this function reads the correspondent
 *         registers in order to determine the correct payload length
 *         to be returned.
 * @param  None.
 * @retval uint16_t Payload length.
 */
uint16_t S2LP_PCKT_STACK_GetPayloadLength(void)
{
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  g_xStatus = S2LP_ReadRegister(PCKTLEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);
  nPayloadLength-=2;

  return nPayloadLength;
}


/**
 * @brief  Return the packet length field of the received packet.
 * @param  None.
 * @retval uint16_t Packet length.
 */
uint16_t S2LP_PCKT_STACK_GetReceivedPktLength(void)
{
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  g_xStatus = S2LP_ReadRegister(RX_PCKT_LEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);
    nPayloadLength--;

  return nPayloadLength;
}


/**
* @brief  Se the AUTO_ACK bit on the receiver .
* @param  xNewState if S_ENABLE, the receiver will check the NO_ACK bit to see if the ack should be sent (NO_ACK=0) or not (NO_ACK=1).
* @retval None.
*/
void S2LP_PCKT_STACK_AutoAck(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LP_ReadRegister(PROTOCOL0_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= AUTO_ACK_REGMASK;
  }
  else {
    tmp &= ~AUTO_ACK_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PROTOCOL0_ADDR, 1, &tmp);

}


/**
* @brief  Set the number of retransmissions to be done in case of ACK loss.
* @param  nRetx number of retransmissions.
* @retval None.
*/
void S2LP_PCKT_STACK_NRetx(uint8_t nRetx)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(PROTOCOL0_ADDR, 1, &tmp);
  tmp &= ~NMAX_RETX_REGMASK;
  tmp |= (nRetx<<4);
  g_xStatus = S2LP_WriteRegister(PROTOCOL0_ADDR, 1, &tmp);

}


/**
* @brief  Get the NO_ACK bit.
* @param  None.
* @retval SFlagStatus if it is S_SET, the ack will be not requested, otherwise it will be.
*/
SFlagStatus S2LP_PCKT_STACK_GetTXAckRequest(void)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(RX_PCKT_INFO_ADDR, 1, &tmp);
  tmp &= NACK_RX_REGMASK;
  tmp >>= 2;
  return (SFlagStatus)tmp;
}

/**
* @brief  This function will set the NO_ACK bit or reset it.
* @param  xNewState if it is S_DISABLE, the ack will be not request and thus the NO_ACK bit will be set to 1.
*         if this parameter is S_ENABLE, the ack will be request and thus the NO_ACK bit will be set to 0.
* @retval None.
*/
void S2LP_PCKT_STACK_AckRequest(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LP_ReadRegister(PROTOCOL0_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp &= ~NACK_TX_REGMASK;
  }
  else {
    tmp |= NACK_TX_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PROTOCOL0_ADDR, 1, &tmp);

}


/**
* @brief  Enable or Disable the piggybacking.
* @param  xNewState enable or disable.
* @retval None.
*/
void S2LP_PCKT_STACK_Piggybacking(SFunctionalState xNewState)
{
  uint8_t tmp;

  S2LP_ReadRegister(PROTOCOL1_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= PIGGYBACKING_REGMASK;
  }
  else {
    tmp &= ~PIGGYBACKING_REGMASK;
  }
  g_xStatus = S2LP_WriteRegister(PROTOCOL1_ADDR, 1, &tmp);

}


/**
* @brief  Set the reload value of the sequence number.
* @note  A SEQ_NUM_RELOAD command must be strobed to make this value available for the next packet.
* @param  cReloadValue reload value.
* @retval None.
*/
void S2LP_PCKT_STACK_SeqNumForReload(uint8_t cReloadValue)
{
  uint8_t tmp;
  
  S2LP_ReadRegister(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= ~TX_SEQ_NUM_RELOAD_REGMASK;
  tmp |= (cReloadValue<<3);
  g_xStatus = S2LP_WriteRegister(PROTOCOL2_ADDR, 1, &tmp);
}

/**
 * @brief  Returns the number of retransmission done on the transmitted packet.
 * @param  None.
 * @retval uint8_t Number of retransmissions done until now.
 */
uint8_t S2LP_PCKT_STACK_GetNReTx(void)
{
  uint8_t tempRetValue;

  /* Reads the TX_PCKT_INFO register value */
  g_xStatus = S2LP_ReadRegister(TX_PCKT_INFO_ADDR, 1, &tempRetValue);

  /* Obtains and returns the number of retransmission done */
  return (tempRetValue & 0x0F);

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
