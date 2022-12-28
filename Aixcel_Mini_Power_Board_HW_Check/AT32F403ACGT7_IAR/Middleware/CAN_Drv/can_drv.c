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

#include "can_drv.h"

static uint32_t CAN_Packet_Tx_Time_ms = 0;
static uint32_t CAN_Packet_Rx_Time_ms = 0;

#ifdef CAN1_ENABLE
CAN_ObjectTypeDef CAN1_OBJ;

static CAN_Rx_Packet_TypeDef CAN1_Rx_Buf_Packet[CAN1_RX_BUF_PACKET_CNT];
static CAN_Tx_Packet_TypeDef CAN1_Tx_Buf_Packet[CAN1_TX_BUF_PACKET_CNT];
#endif

#ifdef CAN2_ENABLE
CAN_ObjectTypeDef CAN2_OBJ;

static CAN_Rx_Packet_TypeDef CAN2_Rx_Buf_Packet[CAN2_RX_BUF_PACKET_CNT];
static CAN_Tx_Packet_TypeDef CAN2_Tx_Buf_Packet[CAN2_TX_BUF_PACKET_CNT];
#endif

#ifdef CAN1_ENABLE
bool CAN1_Object_Init(void)
{
    CAN1_OBJ.CAN_Periph = CAN1;
    
    if(CAN1_OBJ.CAN_Periph == NULL)
        return false;
    
    CAN1_OBJ.CAN_Rx_Buf_Packet_Cnt = CAN1_RX_BUF_PACKET_CNT;
    CAN1_OBJ.CAN_Tx_Buf_Packet_Cnt = CAN1_TX_BUF_PACKET_CNT;
    
    CAN1_OBJ.p_CAN_Rx_Buf_Packet = CAN1_Rx_Buf_Packet;
    CAN1_OBJ.p_CAN_Tx_Buf_Packet = CAN1_Tx_Buf_Packet;
    
    return true;
}
#endif

#ifdef CAN2_ENABLE
bool CAN2_Object_Init(void)
{
    CAN2_OBJ.CAN_Periph = CAN2;
    
    if(CAN2_OBJ.CAN_Periph == NULL)
        return false;
    
    CAN2_OBJ.CAN_Rx_Buf_Packet_Cnt = CAN2_RX_BUF_PACKET_CNT;
    CAN2_OBJ.CAN_Tx_Buf_Packet_Cnt = CAN2_TX_BUF_PACKET_CNT;
    
    CAN2_OBJ.p_CAN_Rx_Buf_Packet = CAN2_Rx_Buf_Packet;
    CAN2_OBJ.p_CAN_Tx_Buf_Packet = CAN2_Tx_Buf_Packet;
  
    return true;
}
#endif

void CAN_Rx_Ring_Buffer_Init(CAN_ObjectTypeDef *can_obj)
{
    can_obj->CAN_Rx_Ring_Buffer.pBuffer = (CAN_Rx_Packet_TypeDef*)(can_obj->p_CAN_Rx_Buf_Packet);
    can_obj->CAN_Rx_Ring_Buffer.Head = 0;
    can_obj->CAN_Rx_Ring_Buffer.Tail = 0;
    can_obj->CAN_Rx_Ring_Buffer.Buffer_Size = can_obj->CAN_Rx_Buf_Packet_Cnt - 1;      //Buffer_Size必须为实际大小 - 1
}

void CAN_Tx_Ring_Buffer_Init(CAN_ObjectTypeDef *can_obj)
{
    can_obj->CAN_Tx_Ring_Buffer.pBuffer = (CAN_Tx_Packet_TypeDef*)(can_obj->p_CAN_Tx_Buf_Packet);
    can_obj->CAN_Tx_Ring_Buffer.Head = 0;
    can_obj->CAN_Tx_Ring_Buffer.Tail = 0;
    can_obj->CAN_Tx_Ring_Buffer.Buffer_Size = can_obj->CAN_Tx_Buf_Packet_Cnt - 1;      //Buffer_Size必须为实际大小 - 1
}

static bool CAN_Rx_Ring_Enqueue(CAN_Rx_Packet_TypeDef **p_packet, CAN_Rx_Ring_Buffer_TypeDef *p_ring)
{
    if((p_packet == NULL) || (p_ring == NULL))
        return false;
    
    if(((p_ring->Tail+1) == p_ring->Head) || ((p_ring->Tail == p_ring->Buffer_Size) && (p_ring->Head==0))) 		
    {       
        return false;
    }
    else
    {
        *p_packet = &(p_ring->pBuffer[p_ring->Tail]);
        //p_ring->pBuffer[p_ring->Tail] = *p_packet;    //真正的入列操作在CAN接收中断中完成，真正的入列完成后才能修改队列尾巴
        //p_ring->Tail++;								               
        //if(p_ring->Tail > p_ring->Buffer_Size)	               
        //    p_ring->Tail = 0;							           
        return true;
    }
}

static bool CAN_Rx_Ring_Dequeue(CAN_Rx_Packet_TypeDef *p_packet, CAN_Rx_Ring_Buffer_TypeDef *p_ring)
{
    if((p_packet == NULL) || (p_ring == NULL))
        return false;
    
    if(p_ring->Head == p_ring->Tail)					          
    {
        return false;                                        	 
    }
    else
    {
        *p_packet = p_ring->pBuffer[p_ring->Head];
        p_ring->Head++;							            	  
        if(p_ring->Head > p_ring->Buffer_Size)        
            p_ring->Head = 0;                          
        return true;
    }
}

static bool CAN_Tx_Ring_Enqueue(CAN_Tx_Packet_TypeDef *p_packet, CAN_Tx_Ring_Buffer_TypeDef *p_ring)
{
    if((p_packet == NULL) || (p_ring == NULL))
        return false;
    
    if(((p_ring->Tail+1) == p_ring->Head) || ((p_ring->Tail == p_ring->Buffer_Size) && (p_ring->Head==0))) 		
    {       
        return false;
    }
    else
    {
        p_ring->pBuffer[p_ring->Tail] = *p_packet;         
        p_ring->Tail++;								               
        if(p_ring->Tail > p_ring->Buffer_Size)	               
            p_ring->Tail = 0;							           
        return true;
    }
}

static bool CAN_Tx_Ring_Dequeue(CAN_Tx_Packet_TypeDef **p_packet, CAN_Tx_Ring_Buffer_TypeDef *p_ring)
{
    if((p_packet == NULL) || (p_ring == NULL))
        return false;
    
    if(p_ring->Head == p_ring->Tail)					          
    {
        return false;                                        	 
    }
    else
    {
        *p_packet = &(p_ring->pBuffer[p_ring->Head]);
        //*p_packet = p_ring->pBuffer[p_ring->Head];    //真正的出列操作在CAN发送函数中完成，真正的出列完成后才能修改队列头
        //p_ring->Head++;							            	  
        //if(p_ring->Head > p_ring->Buffer_Size)        
        //    p_ring->Head = 0;                          
        return true;
    }
}

void CAN_RxFifo0FullCallback(CAN_Type* can_periph)
{
    CAN_ObjectTypeDef *can_obj = NULL;
    
    CanRxMsg temp_packet;
    
    if(can_periph == CAN1)
    {
      #ifdef CAN1_ENABLE
        can_obj = &CAN1_OBJ;
      #endif
    }
    else if(can_periph == CAN2)
    {
      #ifdef CAN2_ENABLE
        can_obj = &CAN2_OBJ;
      #endif
    }
    else
        return;
    
    CAN_Rx_Packet_TypeDef *p_packet = NULL;
    
    CAN_Rx_Ring_Buffer_TypeDef *p_ring = &(can_obj->CAN_Rx_Ring_Buffer);
    
    if(true == CAN_Rx_Ring_Enqueue(&p_packet, p_ring))
    {
        CAN_Receive(can_periph, CAN_FIFO0, (CanRxMsg*)&(p_packet->packet));
        {
            p_ring->Tail++;								               
            if(p_ring->Tail > p_ring->Buffer_Size)	               
                p_ring->Tail = 0;	
        }
    }
    else
    {
        CAN_Receive(can_periph, CAN_FIFO0, &temp_packet);
    }
}

void CAN_RxFifo0MsgPendingCallback(CAN_Type* can_periph)
{
    CAN_ObjectTypeDef *can_obj = NULL;
    
    CanRxMsg temp_packet;
    
    if(can_periph == CAN1)
    {
      #ifdef CAN1_ENABLE
        can_obj = &CAN1_OBJ;
      #endif
    }
    else if(can_periph == CAN2)
    {
      #ifdef CAN2_ENABLE
        can_obj = &CAN2_OBJ;
      #endif
    }
    else
        return;
    
    CAN_Rx_Packet_TypeDef *p_packet = NULL;
    
    CAN_Rx_Ring_Buffer_TypeDef *p_ring = &(can_obj->CAN_Rx_Ring_Buffer);
    
    if(true == CAN_Rx_Ring_Enqueue(&p_packet, p_ring))
    {
        CAN_Receive(can_periph, CAN_FIFO0, (CanRxMsg*)&(p_packet->packet));
        {
            p_ring->Tail++;								               
            if(p_ring->Tail > p_ring->Buffer_Size)	               
                p_ring->Tail = 0;	
        }
    }
    else
    {
        CAN_Receive(can_periph, CAN_FIFO0, &temp_packet);
    }
}

void CAN_RxFifo1FullCallback(CAN_Type* can_periph)
{
    CAN_ObjectTypeDef *can_obj = NULL;
    
    CanRxMsg temp_packet;
    
    if(can_periph == CAN1)
    {
      #ifdef CAN1_ENABLE
        can_obj = &CAN1_OBJ;
      #endif
    }
    else if(can_periph == CAN2)
    {
      #ifdef CAN2_ENABLE
        can_obj = &CAN2_OBJ;
      #endif
    }
    else
        return;
    
    CAN_Rx_Packet_TypeDef *p_packet = NULL;
    
    CAN_Rx_Ring_Buffer_TypeDef *p_ring = &(can_obj->CAN_Rx_Ring_Buffer);
    
    if(true == CAN_Rx_Ring_Enqueue(&p_packet, p_ring))
    {
        CAN_Receive(can_periph, CAN_FIFO1, (CanRxMsg*)&(p_packet->packet));
        {
            p_ring->Tail++;								               
            if(p_ring->Tail > p_ring->Buffer_Size)	               
                p_ring->Tail = 0;	
        }
    }
    else
    {
        CAN_Receive(can_periph, CAN_FIFO1, &temp_packet);
    }
}

void CAN_RxFifo1MsgPendingCallback(CAN_Type* can_periph)
{
    CAN_ObjectTypeDef *can_obj = NULL;
    
    CanRxMsg temp_packet;
    
    if(can_periph == CAN1)
    {
      #ifdef CAN1_ENABLE
        can_obj = &CAN1_OBJ;
      #endif
    }
    else if(can_periph == CAN2)
    {
      #ifdef CAN2_ENABLE
        can_obj = &CAN2_OBJ;
      #endif
    }
    else
        return;
    
    CAN_Rx_Packet_TypeDef *p_packet = NULL;
    
    CAN_Rx_Ring_Buffer_TypeDef *p_ring = &(can_obj->CAN_Rx_Ring_Buffer);
    
    if(true == CAN_Rx_Ring_Enqueue(&p_packet, p_ring))
    {
        CAN_Receive(can_periph, CAN_FIFO1, (CanRxMsg*)&(p_packet->packet));
        {
            p_ring->Tail++;								               
            if(p_ring->Tail > p_ring->Buffer_Size)	               
                p_ring->Tail = 0;
        }
    }
    else
    {
        CAN_Receive(can_periph, CAN_FIFO1, &temp_packet);
    }
}

void CAN_IRQHandler(CAN_Type* can_periph)
{
    if(CAN_GetINTStatus(can_periph, CAN_INT_RFOV0))
    {
        CAN_ClearINTPendingBit(can_periph, CAN_INT_RFOV0);
        
        CAN_RxFifo0FullCallback(can_periph);
    }
    
    if(CAN_GetINTStatus(can_periph, CAN_INT_RFFU0))
    {
        CAN_ClearINTPendingBit(can_periph, CAN_INT_RFFU0);
        
        CAN_RxFifo0FullCallback(can_periph);
    }
    
    if(CAN_GetINTStatus(can_periph, CAN_INT_RFP0))
    {
        while(CAN_MessagePending(can_periph, CAN_FIFO0))
        {
            CAN_RxFifo0MsgPendingCallback(can_periph);
        }
    }
    
    /**************************************************************************/
    
    if(CAN_GetINTStatus(can_periph, CAN_INT_RFOV1))
    {
        CAN_ClearINTPendingBit(can_periph, CAN_INT_RFOV1);
        
        CAN_RxFifo1FullCallback(can_periph);
    }
    
    if(CAN_GetINTStatus(can_periph, CAN_INT_RFFU1))
    {
        CAN_ClearINTPendingBit(can_periph, CAN_INT_RFFU1);
        
        CAN_RxFifo1FullCallback(can_periph);
    }
    
    if(CAN_GetINTStatus(can_periph, CAN_INT_RFP1))
    {
        while(CAN_MessagePending(can_periph, CAN_FIFO1))
        {
            CAN_RxFifo1MsgPendingCallback(can_periph);
        }
    }
}

HAL_StatusTypeDef Send_Packet_From_CAN(CAN_ObjectTypeDef *can_obj, CAN_Tx_Packet_TypeDef *p_packet)
{
    uint32_t TxMailbox;
    
    if((can_obj == NULL) || (p_packet == NULL))
    {
        return HAL_ERROR;
    }

    TxMailbox = CAN_Transmit(can_obj->CAN_Periph, (CanTxMsg*)&(p_packet->packet));

    if(TxMailbox != CAN_TxStatus_NoMailBox)
    {
        return HAL_OK;
    }
    else
    {
        if(true == CAN_Tx_Ring_Enqueue(p_packet, &(can_obj->CAN_Tx_Ring_Buffer)))
        {
            return HAL_OK;
        }
    }
    
    return HAL_ERROR;
}

HAL_StatusTypeDef Recv_Packet_From_CAN(CAN_ObjectTypeDef *can_obj, CAN_Rx_Packet_TypeDef *p_packet)
{
    if((can_obj == NULL) || (p_packet == NULL))
    {
        return HAL_ERROR;
    }
    
    if(true == CAN_Rx_Ring_Dequeue(p_packet, &(can_obj->CAN_Rx_Ring_Buffer)))
    {
        return HAL_OK;
    }
    
    return HAL_ERROR;
}

HAL_StatusTypeDef Loop_Send_CAN_Packet(CAN_ObjectTypeDef *can_obj)
{
    CAN_Tx_Packet_TypeDef *p_packet = NULL;
    
    CAN_Tx_Ring_Buffer_TypeDef *p_ring = &(can_obj->CAN_Tx_Ring_Buffer);
    
    if(p_ring != NULL)
    {
        while(true == CAN_Tx_Ring_Dequeue(&p_packet, p_ring))
        {
            if(CAN_TxStatus_NoMailBox == CAN_Transmit(can_obj->CAN_Periph, (CanTxMsg*)&(p_packet->packet)))
            {
                return HAL_BUSY;
            }
            else
            {
                p_ring->Head++;							            	  
                if(p_ring->Head > p_ring->Buffer_Size)        
                    p_ring->Head = 0; 
            }
        }
    }
    
    return HAL_OK;
}

HAL_StatusTypeDef CAN_Rx_ID_Filter_All(CAN_ObjectTypeDef *can_obj)//标准数据帧和扩展数据帧, 未包含远程帧!!!
{
    uint32_t i = 0;
    
    CAN_FilterInitType CAN_FilterInitStruct = {0};
    
    if(can_obj->CAN_Periph == CAN1)                                   
    {
        CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FIFO0;
    }
    else if(can_obj->CAN_Periph == CAN2)
    {
        CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FIFO1;
    }
    else
    {
        return HAL_ERROR;
    }
    
    CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
    
    CAN_FilterInitStruct.CAN_FilterIdHigh        = (0x00 & 0xFFFF0000) >> 16 << 3;
    CAN_FilterInitStruct.CAN_FilterIdLow         = (0x00 & 0x0000FFFF) << 3 | CAN_Id_Extended;
    CAN_FilterInitStruct.CAN_FilterMskIdHigh     = (0x00 & 0xFFFF0000) >> 16 << 3;
    CAN_FilterInitStruct.CAN_FilterMskIdLow      = (0x00 & 0x0000FFFF) << 3 | CAN_Id_Extended;
    CAN_FilterInitStruct.CAN_FilterActivation    = (ENABLE);
                    
    if(can_obj->CAN_Periph == CAN1)
    {
        CAN_FilterInitStruct.CAN_FilterNumber = i;
    }
    else if(can_obj->CAN_Periph == CAN2)
    {
        CAN_FilterInitStruct.CAN_FilterNumber = i + 2;
    }
    
    CAN_FilterInit(can_obj->CAN_Periph, &CAN_FilterInitStruct);
    
    i++;
    
    CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_16bit;
    
    CAN_FilterInitStruct.CAN_FilterIdLow         = (0x00 & 0x0000FFFF) << 5;
    CAN_FilterInitStruct.CAN_FilterMskIdLow      = (0x00 & 0x0000FFFF) << 5 | (1 << 3);
    CAN_FilterInitStruct.CAN_FilterActivation    = (ENABLE);
    
    if(can_obj->CAN_Periph == CAN1)
    {
        CAN_FilterInitStruct.CAN_FilterNumber = i;
    }
    else if(can_obj->CAN_Periph == CAN2)
    {
        CAN_FilterInitStruct.CAN_FilterNumber = i + 2;
    }
    
    CAN_FilterInit(can_obj->CAN_Periph, &CAN_FilterInitStruct);
    
    return HAL_OK;
}

void CAN_Packet_Loopback(CAN_ObjectTypeDef *can_obj)
{
    CAN_Rx_Packet_TypeDef can_packet;
    
    while(HAL_OK == Recv_Packet_From_CAN(can_obj, &can_packet))
    {
        Send_Packet_From_CAN(can_obj, (CAN_Tx_Packet_TypeDef*)&can_packet);
    }
    
    Loop_Send_CAN_Packet(can_obj);
}

