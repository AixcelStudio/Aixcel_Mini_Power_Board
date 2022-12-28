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

#ifndef HWCONF_H
#define HWCONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at32f4xx.h"
#include <stdio.h>
#include "common.h"

//#define  WWDG_ENABLE
#define  TIM2_ENABLE

#define  USART1_ENABLE
#define  USART3_ENABLE
#define  ADC1_ENABLE
#define  SPI2_ENABLE
#define  I2C1_ENABLE
#define  CAN1_ENABLE

#ifdef USART1_ENABLE
#define  USART1_RX_BUF_CELL_CNT           32U
#define  USART1_RX_BUF_CELL_SIZE          16U
 
#define  USART1_TX_BUF_CELL_CNT           32U
#define  USART1_TX_BUF_CELL_SIZE          16U
#endif

#ifdef USART2_ENABLE
#define  USART2_RX_BUF_CELL_CNT           32U
#define  USART2_RX_BUF_CELL_SIZE          16U

#define  USART2_TX_BUF_CELL_CNT           32U
#define  USART2_TX_BUF_CELL_SIZE          16U
#endif

#ifdef USART3_ENABLE
#define  USART3_RX_BUF_CELL_CNT           32U
#define  USART3_RX_BUF_CELL_SIZE          16U

#define  USART3_TX_BUF_CELL_CNT           32U
#define  USART3_TX_BUF_CELL_SIZE          16U
#endif

#ifdef UART4_ENABLE
#define  UART4_RX_BUF_CELL_CNT            32U
#define  UART4_RX_BUF_CELL_SIZE           16U

#define  UART4_TX_BUF_CELL_CNT            64U
#define  UART4_TX_BUF_CELL_SIZE           16U
#endif

#ifdef UART5_ENABLE
#define  UART5_RX_BUF_CELL_CNT            32U
#define  UART5_RX_BUF_CELL_SIZE           16U

#define  UART5_TX_BUF_CELL_CNT            32U
#define  UART5_TX_BUF_CELL_SIZE           16U
#endif
    
#define  NVIC_PRIORITYGROUP_0     0x00000007U /*!< 0 bits for pre-emption priority
                                                   4 bits for subpriority */
#define  NVIC_PRIORITYGROUP_1     0x00000006U /*!< 1 bits for pre-emption priority
                                                   3 bits for subpriority */
#define  NVIC_PRIORITYGROUP_2     0x00000005U /*!< 2 bits for pre-emption priority
                                                   2 bits for subpriority */
#define  NVIC_PRIORITYGROUP_3     0x00000004U /*!< 3 bits for pre-emption priority
                                                   1 bits for subpriority */
#define  NVIC_PRIORITYGROUP_4     0x00000003U /*!< 4 bits for pre-emption priority
                                                   0 bits for subpriority */

#define  FREQ_1MHz                   1000000U

#define  FREQ_8MHz                   8000000U

#define  FREQ_12MHz                 12000000U

#define  FREQ_20MHz                 20000000U

#define  FREQ_100KHz                  100000U

#define  FREQ_400KHz                  400000U


//#define  USE_DMA_TX_USART1_DATA

/* USE_DMA_RX_USART1_DATA 和 USE_PWM_CTRL_RGB_STRIP_1ST 只能二选一*/
//#define  USE_DMA_RX_USART1_DATA

/* USE_DMA_TX_USART2_DATA 和 USE_PWM_CTRL_RGB_STRIP_2ND 只能二选一*/
//#define  USE_DMA_TX_USART2_DATA
                                                       
/* USE_DMA_TX_USART3_DATA 和 USE_PWM_CTRL_RGB_STRIP_3RD 只能二选一*/
//#define  USE_DMA_TX_USART3_DATA
                                                       
/* USE_DMA_RX_USART3_DATA 和 USE_PWM_CTRL_RGB_STRIP_4TH 只能二选一*/
//#define  USE_DMA_RX_USART3_DATA

  
/* USART Pin defines ---------------------------------------------------------*/
#define  USART1_Tx_Pin            GPIO_Pins_9
#define  USART1_Rx_Pin            GPIO_Pins_10
#define  USART1_GPIO_Port         GPIOA


#define  USART3_Tx_Pin            GPIO_Pins_10
#define  USART3_Rx_Pin            GPIO_Pins_11
#define  USART3_GPIO_Port         GPIOB

/* SPI Pin defines -----------------------------------------------------------*/
#define  SPI2_SCK_Pin             GPIO_Pins_13
#define  SPI2_MISO_Pin            GPIO_Pins_14
#define  SPI2_MOSI_Pin            GPIO_Pins_15
#define  SPI2_GPIO_Port           GPIOB

/* I2C Pin defines -----------------------------------------------------------*/
#define  I2C1_SCL_Pin             GPIO_Pins_6
#define  I2C1_SDA_Pin             GPIO_Pins_7
#define  I2C1_SCL_Port            GPIOB
#define  I2C1_SDA_Port            GPIOB

#define  I2C1_OWN_ADDRESS0        0x45
#define  I2C1_OWN_ADDRESS1        0x46

#define  USE_SIMULATE_I2C1

/* CAN Pin defines -----------------------------------------------------------*/
#define  CAN1_RX_Pin              GPIO_Pins_11
#define  CAN1_TX_Pin              GPIO_Pins_12
#define  CAN1_GPIO_Port           GPIOA

/* Pin defines ---------------------------------------------------------------*/
#define  U_PWR_I_Pin              GPIO_Pins_0    //ADC0_IN0
#define  U_PWR_I_GPIO_Port        GPIOA

#define  M_PWR_I_Pin              GPIO_Pins_1    //ADC0_IN1
#define  M_PWR_I_GPIO_Port        GPIOA

#define  U_PWR_V_Pin              GPIO_Pins_2    //ADC0_IN2
#define  U_PWR_V_GPIO_Port        GPIOA

#define  M_PWR_V_Pin              GPIO_Pins_3    //ADC0_IN3
#define  M_PWR_V_GPIO_Port        GPIOA

#define  BAT_OUT_V_Pin            GPIO_Pins_4    //ADC0_IN4
#define  BAT_OUT_V_GPIO_Port      GPIOA

#define  BAT_OUT_I_Pin            GPIO_Pins_5    //ADC0_IN5
#define  BAT_OUT_I_GPIO_Port      GPIOA

#define  BAT_CHG_V_Pin            GPIO_Pins_6    //ADC0_IN6
#define  BAT_CHG_V_GPIO_Port      GPIOA

#define  BAT_CHG_I_Pin            GPIO_Pins_7    //ADC0_IN7
#define  BAT_CHG_I_GPIO_Port      GPIOA

#define  INPUT_CHG_V_Pin          GPIO_Pins_0    //ADC0_IN8
#define  INPUT_CHG_V_GPIO_Port    GPIOB

#define  VCC_5V_I_Pin             GPIO_Pins_1    //ADC0_IN9
#define  VCC_5V_I_GPIO_Port       GPIOB

#define  WIRE_SIG_Pin             GPIO_Pins_8
#define  WIRE_SIG_GPIO_Port       GPIOA

#define  SW_STATE_Pin             GPIO_Pins_3
#define  SW_STATE_GPIO_Port       GPIOB

#define  VBAT_EN_Pin              GPIO_Pins_4
#define  VBAT_EN_GPIO_Port        GPIOB

#define  CHG_EN_Pin               GPIO_Pins_5
#define  CHG_EN_GPIO_Port         GPIOB

#define  M_PWR_EN_Pin             GPIO_Pins_8
#define  M_PWR_EN_GPIO_Port       GPIOB

#define  U_PWR_EN_Pin             GPIO_Pins_9
#define  U_PWR_EN_GPIO_Port       GPIOB

#define  SPI2_CS_Pin              GPIO_Pins_12
#define  SPI2_CS_GPIO_Port        GPIOB

#define  M485_DE_Pin              GPIO_Pins_13
#define  M485_DE_GPIO_Port        GPIOC

#define  SYS_LED_Pin              GPIO_Pins_15
#define  SYS_LED_GPIO_Port        GPIOA


/* GPIO Pin function defines -------------------------------------------------*/
#define  SYS_LED_ON()             (SYS_LED_GPIO_Port->BRE = SYS_LED_Pin)
#define  SYS_LED_OFF()            (SYS_LED_GPIO_Port->BSRE = SYS_LED_Pin)
#define  SYS_LED_TOGGLE()         (SYS_LED_GPIO_Port->OPTDT ^= SYS_LED_Pin)

#define  M485_DE_Pin_PULL_DOWN()  (M485_DE_GPIO_Port->BRE = M485_DE_Pin)
#define  M485_DE_Pin_PULL_UP()    (M485_DE_GPIO_Port->BSRE = M485_DE_Pin)
#define  M485_DE_Pin_TOGGLE()     (M485_DE_GPIO_Port->OPTDT ^= M485_DE_Pin)

#define  SW_STATE_Pin_PULL_DOWN() (SW_STATE_GPIO_Port->BRE = SW_STATE_Pin)
#define  SW_STATE_Pin_PULL_UP()   (SW_STATE_GPIO_Port->BSRE = SW_STATE_Pin)
#define  SW_STATE_Pin_TOGGLE()    (SW_STATE_GPIO_Port->OPTDT ^= SW_STATE_Pin)

#define  SPI_Flash_SELECT()       (SPI2_CS_GPIO_Port->BRE = SPI2_CS_Pin)
#define  SPI_Flash_DESELECT()     (SPI2_CS_GPIO_Port->BSRE = SPI2_CS_Pin)
#define  SPI_Flash_TOGGLE()       (SPI2_CS_GPIO_Port->OPTDT ^= SPI2_CS_Pin)

#define  VBAT_EN_PULL_DOWN()      (VBAT_EN_GPIO_Port->BRE = VBAT_EN_Pin)
#define  VBAT_EN_PULL_UP()        (VBAT_EN_GPIO_Port->BSRE = VBAT_EN_Pin)
#define  VBAT_EN_TOGGLE()         (VBAT_EN_GPIO_Port->OPTDT ^= VBAT_EN_Pin)

#define  CHG_EN_PULL_DOWN()       (CHG_EN_GPIO_Port->BRE = CHG_EN_Pin)
#define  CHG_EN_PULL_UP()         (CHG_EN_GPIO_Port->BSRE = CHG_EN_Pin)
#define  CHG_EN_TOGGLE()          (CHG_EN_GPIO_Port->OPTDT ^= CHG_EN_Pin)

#define  M_PWR_EN_PULL_DOWN()     (M_PWR_EN_GPIO_Port->BRE = M_PWR_EN_Pin)
#define  M_PWR_EN_PULL_UP()       (M_PWR_EN_GPIO_Port->BSRE = M_PWR_EN_Pin)
#define  M_PWR_EN_TOGGLE()        (M_PWR_EN_GPIO_Port->OPTDT) ^= M_PWR_EN_Pin)

#define  U_PWR_EN_PULL_DOWN()     (U_PWR_EN_GPIO_Port->BRE = U_PWR_EN_Pin)
#define  U_PWR_EN_PULL_UP()       (U_PWR_EN_GPIO_Port->BSRE = U_PWR_EN_Pin)
#define  U_PWR_EN_TOGGLE()        (U_PWR_EN_GPIO_Port->OPTDT ^= U_PWR_EN_Pin)


void gpio_init(GPIO_Type* GPIOx, GPIOMode_Type GPIO_Mode, GPIOMaxSpeed_Type GPIO_MaxSpeed, uint16_t GPIO_Pins);

void System_GPIO_Init(void);

void System_NVIC_Init(void);

void System_Misc_Init(void);

void System_EXTI_Init(void);

void System_DMA_Init(void);

void System_RTC_Init(void);

void System_USART1_Init(uint32_t baud_rate);

void System_USART2_Init(uint32_t baud_rate);

void System_USART3_Init(uint32_t baud_rate);

void USART3_EN_M485_Tx(void);

void USART3_EN_M485_Rx(void);

void System_TMR1_Init(void);

void System_TMR2_Init(void);

void System_TMR3_Init(void);

void System_TMR6_Init(void);

void System_ADC1_Init(void);

void System_ADC2_Init(void);

void System_SPI1_Init(uint32_t baud_rate);

void System_SPI2_Init(uint32_t baud_rate);

void System_I2C1_Init(uint32_t baud_rate);

void System_CAN1_Init(CAN_Baudrate_Typedef baudrate);

void System_CAN2_Init(CAN_Baudrate_Typedef baudrate);

uint8_t System_CAN_Start(CAN_Type* can_periph);

uint8_t System_CAN_Stop(CAN_Type* can_periph);

void System_WWDG_Init(void);

void System_WWDG_Disable(void);

void EXTI0_IRQ_Callback(void);

void EXTI1_IRQ_Callback(void);

void EXTI3_IRQ_Callback(void);

void EXTI4_IRQ_Callback(void);

void EXTI7_IRQ_Callback(void);

void EXTI8_IRQ_Callback(void);

void EXTI15_10_IRQ_Callback(void);


#ifdef __cplusplus
}
#endif

#endif /* HWCONF_H */
