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

#include "simulate_i2c.h"
#include "hwconf.h"

#define MCU_GPIO_SUPPORT_OPEN_DRAIN                1

/* GPIO mode values set */
#define GPIO_MODE_SET(n, mode)           ((uint32_t)((uint32_t)(mode) << (4U * (n))))
#define GPIO_MODE_MASK(n)                (0xFU << (4U * (n)))

/* Set Simulate I2C SCL/SDA Pin Direction */
#if MCU_GPIO_SUPPORT_OPEN_DRAIN
#define SIMULATE_SDA_IN(I2C)
#define SIMULATE_SDA_OUT(I2C)
#else
#define SIMULATE_SDA_IN(I2C)   {(I2C->SDA_Port->CTRLL) &= ~GPIO_MODE_MASK(I2C->SDA_Pin_No); (I2C->SDA_Port->CTRLL) |= GPIO_MODE_SET(I2C->SDA_Pin_No, 0x4);}//0x4: GPIO_MODE_IN_FLOATING
#define SIMULATE_SDA_OUT(I2C)  {(I2C->SDA_Port->CTRLL) &= ~GPIO_MODE_MASK(I2C->SDA_Pin_No); (I2C->SDA_Port->CTRLL) |= GPIO_MODE_SET(I2C->SDA_Pin_No, 0x3);}//0x3: GPIO_MODE_OUT_PP  //0x7: GPIO_MODE_OUT_OD
#endif


/* Set Simulate I2C SCL/SDA Pin Level */
#define SIMULATE_I2C_SCL_H(I2C) (I2C->SCL_Port->BSRE = I2C->SCL_Pin)
#define SIMULATE_I2C_SCL_L(I2C) (I2C->SCL_Port->BRE = I2C->SCL_Pin)

#define SIMULATE_I2C_SDA_H(I2C) (I2C->SDA_Port->BSRE = I2C->SDA_Pin)
#define SIMULATE_I2C_SDA_L(I2C) (I2C->SDA_Port->BRE = I2C->SDA_Pin)


/* Get Simulate I2C SDA Level */
#define SIMULATE_I2C_READ_SDA(I2C) (GPIO_ReadInputDataBit(I2C->SDA_Port, I2C->SDA_Pin))


void Simulate_I2C_Init(Simulate_I2C_Bus_Typedef *I2C)
{
  #if MCU_GPIO_SUPPORT_OPEN_DRAIN
    gpio_init(I2C->SCL_Port, GPIO_Mode_OUT_OD, GPIO_MaxSpeed_50MHz, I2C->SCL_Pin);
    gpio_init(I2C->SDA_Port, GPIO_Mode_OUT_OD, GPIO_MaxSpeed_50MHz, I2C->SDA_Pin);
  #else
    gpio_init(I2C->SCL_Port, GPIO_Mode_OUT_PP, GPIO_MaxSpeed_50MHz, I2C->SCL_Pin);
    gpio_init(I2C->SDA_Port, GPIO_Mode_OUT_PP, GPIO_MaxSpeed_50MHz, I2C->SDA_Pin);
  #endif
    
    GPIO_SetBits(I2C->SCL_Port, I2C->SCL_Pin);
	GPIO_SetBits(I2C->SDA_Port, I2C->SDA_Pin);
}

void Simulate_I2C_Start(Simulate_I2C_Bus_Typedef *I2C)
{
	SIMULATE_SDA_OUT(I2C);
    
	SIMULATE_I2C_SDA_H(I2C);
	SIMULATE_I2C_SCL_H(I2C);
	Delay_us(I2C->Delay);	
    
	SIMULATE_I2C_SDA_L(I2C);
	Delay_us(I2C->Delay); 
    
	SIMULATE_I2C_SCL_L(I2C);
    Delay_us(I2C->Delay); 
}

void Simulate_I2C_Stop(Simulate_I2C_Bus_Typedef *I2C)
{
	SIMULATE_SDA_OUT(I2C);
    
	SIMULATE_I2C_SCL_L(I2C);
	SIMULATE_I2C_SDA_L(I2C);
	Delay_us(I2C->Delay);
    
	SIMULATE_I2C_SCL_H(I2C);
	Delay_us(I2C->Delay);
    
	SIMULATE_I2C_SDA_H(I2C);
	Delay_us(I2C->Delay);
}

void Simulate_I2C_ACK(Simulate_I2C_Bus_Typedef *I2C)
{
	SIMULATE_SDA_OUT(I2C);
    
	SIMULATE_I2C_SCL_L(I2C);
	Delay_us(I2C->Delay);
    
	SIMULATE_I2C_SDA_L(I2C);
	Delay_us(I2C->Delay);
    
	SIMULATE_I2C_SCL_H(I2C);
	Delay_us(I2C->Delay);
    
	SIMULATE_I2C_SCL_L(I2C);
	Delay_us(I2C->Delay);
}

void Simulate_I2C_NACK(Simulate_I2C_Bus_Typedef *I2C)
{
	SIMULATE_SDA_OUT(I2C);
    
	SIMULATE_I2C_SCL_L(I2C);
	Delay_us(I2C->Delay);
    
	SIMULATE_I2C_SDA_H(I2C);
	Delay_us(I2C->Delay);
    
	SIMULATE_I2C_SCL_H(I2C);
	Delay_us(I2C->Delay);
    
	SIMULATE_I2C_SCL_L(I2C);
	Delay_us(I2C->Delay);
}

uint8_t Simulate_I2C_Wait_ACK(Simulate_I2C_Bus_Typedef *I2C)
{
    uint8_t t = 200;
    
    SIMULATE_SDA_OUT(I2C);
    
    SIMULATE_I2C_SDA_H(I2C);
    
    SIMULATE_I2C_SCL_L(I2C);
    Delay_us(I2C->Delay); 
    
    SIMULATE_SDA_IN(I2C);
    Delay_us(I2C->Delay);
    
    while(SIMULATE_I2C_READ_SDA(I2C))
    {
		t--;
		Delay_us(I2C->Delay); 
		if(t==0)
		{
			SIMULATE_I2C_SCL_L(I2C);
			return 1;
		}
		Delay_us(I2C->Delay);
    }
    
    Delay_us(I2C->Delay);
    
    SIMULATE_I2C_SCL_H(I2C);
    Delay_us(I2C->Delay);
    
    SIMULATE_I2C_SCL_L(I2C);
    Delay_us(I2C->Delay);
    
    return 0;
}

void Simulate_I2C_Send_Byte(Simulate_I2C_Bus_Typedef *I2C, uint8_t byte)
{
	uint8_t BitCnt;
    
	SIMULATE_SDA_OUT(I2C);
    
	SIMULATE_I2C_SCL_L(I2C);
    
	for(BitCnt=0; BitCnt<8; BitCnt++)
	{
		if(byte&0x80)
            SIMULATE_I2C_SDA_H(I2C);
		else
            SIMULATE_I2C_SDA_L(I2C);
		byte<<=1;
        
		Delay_us(I2C->Delay);
        
		SIMULATE_I2C_SCL_H(I2C);
		Delay_us(I2C->Delay);
        
		SIMULATE_I2C_SCL_L(I2C);
		Delay_us(I2C->Delay);
	}
}
 
uint8_t Simulate_I2C_Recv_Byte(Simulate_I2C_Bus_Typedef *I2C)
{
	uint8_t retc;
	uint8_t BitCnt;
    
	retc=0;
    
	SIMULATE_SDA_IN(I2C);
	Delay_us(I2C->Delay);
    
	for(BitCnt=0; BitCnt<8; BitCnt++)
	{
		SIMULATE_I2C_SCL_L(I2C);
		Delay_us(I2C->Delay);
        
		SIMULATE_I2C_SCL_H(I2C);
        
		retc=retc<<1;
        
		if(SIMULATE_I2C_READ_SDA(I2C))
            retc |=1;
		Delay_us(I2C->Delay);
	}
    
	SIMULATE_I2C_SCL_L(I2C); 
   
	return(retc);
}

void Simulate_I2C_Master_Transmit(Simulate_I2C_Bus_Typedef *I2C, uint8_t addr, uint8_t *data, uint8_t len)
{
    Simulate_I2C_Start(I2C);
    {
        Simulate_I2C_Send_Byte(I2C, addr);
        
        Simulate_I2C_Wait_ACK(I2C);
        
        for(uint8_t i = 0; i < len; i++)
        {
            Simulate_I2C_Send_Byte(I2C, data[i]);
            Simulate_I2C_Wait_ACK(I2C);
        }
        
        Simulate_I2C_Stop(I2C);
    }
}

void Simulate_I2C_Master_Receive(Simulate_I2C_Bus_Typedef *I2C, uint8_t addr, uint8_t *data, uint8_t len)
{
    Simulate_I2C_Start(I2C);
    {
        Simulate_I2C_Send_Byte(I2C, (addr | 1));
        
        Simulate_I2C_Wait_ACK(I2C);
        
        for(uint8_t i = 0; i < len; i++)
        {
            data[i] = Simulate_I2C_Recv_Byte(I2C);
                
            if(i == (len-1))
            {
                Simulate_I2C_NACK(I2C);
            }
            else
            {
                Simulate_I2C_ACK(I2C);
            }
        }
        
        Simulate_I2C_Stop(I2C);
    }
}