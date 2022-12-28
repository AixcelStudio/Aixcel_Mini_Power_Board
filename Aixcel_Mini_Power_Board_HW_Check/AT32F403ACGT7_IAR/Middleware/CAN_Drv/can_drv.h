/*
******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2022 Aixcel Co.,Ltd</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of Aixcel nor the names of its contributors
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
  ******************************************************************************
  */

#ifndef CAN_DRV_H
#define CAN_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "hwconf.h"

/**
  * @brief  CAN handle Structure definition
  */
typedef struct __CAN_HandleTypeDef
{
  uint32_t    CAN;

} CAN_HandleTypeDef;

typedef __packed struct __CAN_Rx_Packet_TypeDef
{
    CanRxMsg                       packet;
    
    uint32_t                       timestamp;

} CAN_Rx_Packet_TypeDef;

typedef __packed struct __CAN_Tx_Packet_TypeDef
{
    CanTxMsg                       packet;
    
    uint32_t                       timestamp;

} CAN_Tx_Packet_TypeDef;  

#ifdef CAN1_ENABLE
#define    CAN1_RX_BUF_PACKET_CNT               20
#define    CAN1_RX_BUF_PACKET_SIZE              sizeof(CAN_Rx_Packet_TypeDef)
 
#define    CAN1_TX_BUF_PACKET_CNT               64
#define    CAN1_TX_BUF_PACKET_SIZE              sizeof(CAN_Tx_Packet_TypeDef) 
#endif

#ifdef CAN2_ENABLE  
#define    CAN2_RX_BUF_PACKET_CNT               20
#define    CAN2_RX_BUF_PACKET_SIZE              sizeof(CAN_Rx_Packet_TypeDef)
 
#define    CAN2_TX_BUF_PACKET_CNT               64
#define    CAN2_TX_BUF_PACKET_SIZE              sizeof(CAN_TxPacket_TypeDef)
#endif 

typedef struct __CAN_Rx_Ring_Buffer_TypeDef
{
    CAN_Rx_Packet_TypeDef          *pBuffer;
    
    uint32_t                       Head; 
    
    uint32_t                       Tail; 
    
    uint32_t                       Buffer_Size;

} CAN_Rx_Ring_Buffer_TypeDef;

typedef struct __CAN_Tx_Ring_Buffer_TypeDef
{
    CAN_Tx_Packet_TypeDef          *pBuffer;
    
    uint32_t                       Head; 
    
    uint32_t                       Tail; 
    
    uint32_t                       Buffer_Size;

} CAN_Tx_Ring_Buffer_TypeDef;

typedef struct __CAN_ObjectTypeDef
{
    CAN_Type*                      CAN_Periph;
    
    uint16_t                       CAN_Rx_Buf_Packet_Cnt;
    uint16_t                       CAN_Tx_Buf_Packet_Cnt;
    
    CAN_Rx_Packet_TypeDef          *p_CAN_Rx_Buf_Packet;
    CAN_Tx_Packet_TypeDef          *p_CAN_Tx_Buf_Packet;
  
    CAN_Rx_Ring_Buffer_TypeDef     CAN_Rx_Ring_Buffer;
    CAN_Tx_Ring_Buffer_TypeDef     CAN_Tx_Ring_Buffer;
    
} CAN_ObjectTypeDef;

#ifdef CAN1_ENABLE
extern CAN_ObjectTypeDef CAN1_OBJ;
#endif

#ifdef CAN2_ENABLE
extern CAN_ObjectTypeDef CAN2_OBJ;
#endif

#ifdef CAN1_ENABLE
bool CAN1_Object_Init(void);
#endif

#ifdef CAN2_ENABLE
bool CAN2_Object_Init(void);
#endif

void CAN_Rx_Ring_Buffer_Init(CAN_ObjectTypeDef *can_obj);

void CAN_Tx_Ring_Buffer_Init(CAN_ObjectTypeDef *can_obj);

void CAN_RxFifo0FullCallback(CAN_Type* can_periph);

void CAN_RxFifo0MsgPendingCallback(CAN_Type* can_periph);

void CAN_RxFifo1FullCallback(CAN_Type* can_periph);

void CAN_RxFifo1MsgPendingCallback(CAN_Type* can_periph);

void CAN_IRQHandler(CAN_Type* can_periph);

HAL_StatusTypeDef Send_Packet_From_CAN(CAN_ObjectTypeDef *can_obj, CAN_Tx_Packet_TypeDef *p_packet);

HAL_StatusTypeDef Recv_Packet_From_CAN(CAN_ObjectTypeDef *can_obj, CAN_Rx_Packet_TypeDef *p_packet);

HAL_StatusTypeDef Loop_Send_CAN_Packet(CAN_ObjectTypeDef *can_obj);

HAL_StatusTypeDef CAN_Rx_ID_Filter_All(CAN_ObjectTypeDef *can_obj);

void CAN_Packet_Loopback(CAN_ObjectTypeDef *can_obj);

#ifdef __cplusplus
}
#endif

#endif /* CAN_DRV_H */
