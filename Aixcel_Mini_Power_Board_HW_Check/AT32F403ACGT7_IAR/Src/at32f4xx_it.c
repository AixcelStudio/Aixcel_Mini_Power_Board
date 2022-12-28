/**
  ******************************************************************************
  * File   : Templates/at32f4xx_it.c
  * Version: V1.2.7
  * Date   : 2020-11-10
  * Brief  : Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"
#include "main.h"
#include "hwconf.h"
#include "common.h"
#include "systick.h"
#include "uart_drv.h"
#include "can_drv.h"
/** @addtogroup AT32F403A_StdPeriph_Templates
  * @{
  */

/** @addtogroup GPIO_LED_Toggle
  * @{
  */

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    Systick_Inc();
}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
	if (RTC_GetINTStatus(RTC_INT_PACE) != RESET)
	{
		/* Clear Interrupt pending bit */
		RTC_ClearINTPendingBit(RTC_FLAG_PACE);
	}
}

/*!
    \brief      this function handles DMA1_Channel2_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA1_Channel2_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_INT_TC2))
    {
        DMA_ClearITPendingBit(DMA1_INT_GL2);
        
        #ifdef USE_DMA_TX_USART3_DATA
        while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TRAC));  //DMA传输完成后必须等到串口发送完成标志TC置起才意味着数据彻底完成传输!!!
        UART_TxCpltCallback(USART3);
        #endif
    }
}

/*!
    \brief      this function handles DMA1_Channel3_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA1_Channel3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_INT_TC3))
    {
        DMA_ClearITPendingBit(DMA1_INT_GL3);
        
        #ifdef USE_DMA_RX_USART3_DATA
        UART_DMA_RxCpltCallback(USART3);
        #endif
    }
}

/*!
    \brief      this function handles DMA1_Channel4_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_INT_TC4))
    {
        while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TRAC));  //DMA传输完成后必须等到串口发送完成标志TC置起才意味着数据彻底完成传输!!!
        UART_TxCpltCallback(USART1);
        DMA_ClearITPendingBit(DMA1_INT_GL4);
    }
}

/*!
    \brief      this function handles DMA1_Channel5_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA1_Channel5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_INT_TC5))
    {
        DMA_ClearITPendingBit(DMA1_INT_GL5);
        
        #ifdef USE_DMA_RX_USART1_DATA
        UART_DMA_RxCpltCallback(USART1);
        #endif
    }
}

/*!
    \brief      this function handles DMA1_Channel6_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA1_Channel6_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_INT_TC6))
    {
        DMA_ClearITPendingBit(DMA1_INT_GL6);
        
        #ifdef USE_DMA_RX_USART2_DATA
        UART_DMA_RxCpltCallback(USART2);
        #endif
    }
}

/*!
    \brief      this function handles DMA1_Channel7_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_INT_TC7))
    {
        DMA_ClearITPendingBit(DMA1_INT_GL7);
        
        #ifdef USE_DMA_TX_USART2_DATA
        while(RESET == USART_GetFlagStatus(USART2, USART_FLAG_TRAC));  //DMA传输完成后必须等到串口发送完成标志TC置起才意味着数据彻底完成传输!!!
        UART_TxCpltCallback(USART2);
        #endif
    }
}

/*!
    \brief      this function handles USB_HP_CAN1_TX_IRQn interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USB_HP_CAN1_TX_IRQHandler(void)
{
    ;
}

/*!
    \brief      this function handles USB_LP_CAN1_RX0_IRQn interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    CAN_IRQHandler(CAN1);//CAN_RxFifo0MsgPendingCallback(CAN1);
}  

/**
  * @brief  This function handles USB WakeUp interrupt request.
  * @param  None
  * @retval None
  */
void USBWakeUp_IRQHandler(void)
{
    ;
}

/**
  * @brief  This function handles ACC interrupt request.
  * @param  None
  * @retval None
  */
void ACC_IRQHandler(void)
{
    if(ACC_GetFlagStatus(ACC_FLAG_CALRDY) == SET)
    {
        /*Claer ACC Calibration ready flag*/
        ACC_ClearFlag(ACC_FLAG_CALRDY);
    }
    if(ACC_GetFlagStatus(ACC_FLAG_RSLOST) == SET)
    {
        /*Claer ACC Reference Signal Lost flag*/
        ACC_ClearFlag(ACC_FLAG_RSLOST);
    }
}

/*!
    \brief      this function handles CAN1_RX1_IRQn interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN1_RX1_IRQHandler(void)
{
    CAN_IRQHandler(CAN1);//CAN_RxFifo1MsgPendingCallback(CAN1);
}

/*!
    \brief      this function handles CAN1_SCE_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN1_SCE_IRQHandler(void)
{
    ;
}

/*!
    \brief      this function handles CAN2_TX_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN2_TX_IRQHandler(void)
{
    ;
}

/*!
    \brief      this function handles CAN2_RX0_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN2_RX0_IRQHandler(void)
{
    //CAN_IRQHandler(CAN2);//CAN_RxFifo0MsgPendingCallback(CAN2);
}

/*!
    \brief      this function handles CAN2_RX1_IRQn interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN2_RX1_IRQHandler(void)
{
    //CAN_IRQHandler(CAN2);//CAN_RxFifo1MsgPendingCallback(CAN2);
}

/*!
    \brief      this function handles CAN1_SCE_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void CAN2_SCE_IRQHandler(void)
{
    ;
}

/*!
    \brief      this function handles SPI1_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SPI1_IRQHandler(void)
{
    ;
}

/*!
    \brief      this function handles SPI2_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SPI2_IRQHandler(void)
{
    ;
}

/*!
    \brief      this function handles USART1_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{
    if(RESET != USART_GetITStatus(USART1, USART_INT_RDNE))
    {
        UART_IT_CONT_RxCpltCallback(USART1);
    }
}

/*!
    \brief      this function handles USART2_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART2_IRQHandler(void)
{
    if(RESET != USART_GetITStatus(USART2, USART_INT_RDNE))
    {
        UART_IT_CONT_RxCpltCallback(USART2);
    }
}

/*!
    \brief      this function handles USART3_IRQHandler interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART3_IRQHandler(void)
{
    if(RESET != USART_GetITStatus(USART3, USART_INT_RDNE))
    {
        UART_IT_CONT_RxCpltCallback(USART3);
    }
}

/*!
    \brief      this function handles external lines 0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI0_IRQHandler(void)
{
    if(RESET != EXTI_GetIntStatus(EXTI_Line0))
    {
        EXTI_ClearIntPendingBit(EXTI_Line0);
        EXTI0_IRQ_Callback();
    }
}

/*!
    \brief      this function handles external lines 1 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI1_IRQHandler(void)
{
    if(RESET != EXTI_GetIntStatus(EXTI_Line1))
    {
        EXTI_ClearIntPendingBit(EXTI_Line1);
        EXTI1_IRQ_Callback();
    }
}

/*!
    \brief      this function handles external lines 3 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI3_IRQHandler(void)
{
    if(RESET != EXTI_GetIntStatus(EXTI_Line3))
    {
        EXTI_ClearIntPendingBit(EXTI_Line3);
        EXTI3_IRQ_Callback();
    }
}

/*!
    \brief      this function handles external lines 4 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI4_IRQHandler(void)
{
    if(RESET != EXTI_GetIntStatus(EXTI_Line4))
    {
        EXTI_ClearIntPendingBit(EXTI_Line4);
        EXTI4_IRQ_Callback();
    }
}

/*!
    \brief      this function handles external lines 9_5 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI9_5_IRQHandler(void)
{
    if(RESET != EXTI_GetIntStatus(EXTI_Line8))
    {
        EXTI_ClearIntPendingBit(EXTI_Line8);
        EXTI8_IRQ_Callback();
    }
}

/*!
    \brief      this function handles external lines 5 to 9 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI15_10_IRQHandler(void)
{
    if(RESET != EXTI_GetIntStatus(EXTI_Line14))
    {
        EXTI_ClearIntPendingBit(EXTI_Line14);
        EXTI15_10_IRQ_Callback();
    }
}

/*!
    \brief      this function handles DMA2_Channel3_IRQHandler interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DMA2_Channel3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_INT_TC3))
    {
        DMA_ClearITPendingBit(DMA2_INT_GL3);
    }
}

/******************************************************************************/
/*                 at32f4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f403_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

