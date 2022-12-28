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

#include "common.h"
#include "hwconf.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 
PUTCHAR_PROTOTYPE
{
  #ifdef __DEBUG
    USART_SendData(USART1, (uint8_t)ch);
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TDE));
  #endif
    return ch;
}

#define cli __disable_irq
#define sei __enable_irq

/*进入临界区*/
inline unsigned int enter_critical_section(void)
{
    unsigned int context=__get_PRIMASK();
    cli();
    return context;
}

/*退出临界区*/
inline void leave_critical_section(unsigned int context)
{
    __set_PRIMASK(context);
}

uint8_t SPI_Read_Write_Byte(SPI_Type* handle, uint8_t ch)
{
    uint16_t tmp;
    
    while(SPI_I2S_GetFlagStatus(handle, SPI_I2S_FLAG_TE) == RESET);
    SPI_I2S_TxData(handle, ch);
    
    while(SPI_I2S_GetFlagStatus(handle, SPI_I2S_FLAG_RNE) == RESET);
    tmp = SPI_I2S_RxData(handle);
    
    return ((uint8_t)(tmp & 0xff));
}

int32_t SPI_Write_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len, uint32_t timeout)
{
    uint16_t i = 0U;
    
    while(len > 0)
    {
        SPI_Read_Write_Byte(handle, bufp[i]);
        i++;
        len--;
    }
    
    return i;
}

int32_t SPI_Read_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len, uint32_t timeout)
{
    uint16_t i = 0U;
    
    while(len > 0)
    {
        bufp[i] = SPI_Read_Write_Byte(handle, bufp[i]);
        i++;
        len--;
    }
    
    return i;
}

int32_t SPI_Transmmit_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len, uint32_t timeout)
{
    uint16_t i = 0U;
    uint16_t tmp = 0U;
    uint32_t tickstart = 0U;
    
    tickstart = Systick_Get();
  
    while(len > 0)
    {
        if(SPI_I2S_GetFlagStatus(handle, SPI_I2S_FLAG_TE))
        {
            (handle->DT) = bufp[i];
            
            while(SPI_I2S_GetFlagStatus(handle, SPI_I2S_FLAG_RNE) == RESET);
            tmp = (handle->DT)&0xFF;
            
            i++;
            len--;
        }
        
        if((timeout == 0) || ((Systick_Get() - tickstart) >=  timeout))
        {
            break;
        }
    }
    
    return i;
}

int32_t SPI_Receive_Data(SPI_Type* handle, uint8_t* bufp, uint16_t len, uint32_t timeout)
{
    uint16_t i = 0U;
    uint32_t tickstart = 0U;
    
    tickstart = Systick_Get();
  
    while(len > 0)
    {
        /* Send Dummy data on Tx line and generate clock on CLK line */
        if(SPI_I2S_GetFlagStatus(handle, SPI_I2S_FLAG_TE))
        {
            (handle->DT) = 0xA5;
            
            while(SPI_I2S_GetFlagStatus(handle, SPI_I2S_FLAG_RNE) == RESET);
            bufp[i] = (handle->DT);
            
            i++;
            len--;
        }
        
        if((timeout == 0) ||(Systick_Get() - tickstart) >=  timeout)
        {
            break;
        }
    }
    
    return i;
}

uint8_t System_I2C_Master_Transmit(I2C_Type* i2c_periph, uint8_t addr, uint8_t *data, uint8_t len)
{
    uint8_t err = 0;
    uint16_t time_cnt = 0;
    uint16_t time_out = 1024;
    
    /* wait until I2C bus is idle */
    while(I2C_GetFlagStatus(i2c_periph, I2C_FLAG_BUSYF))
    {
        time_cnt++;
        
        if(time_cnt >= time_out)
        {
            return (err | (1<<0));
        }
    }
    
    /* Disable Pos */
    I2C_NACKPositionConfig(i2c_periph, I2C_NACKPosition_Current);
  
    /* send a start condition to I2C bus */
    I2C_GenerateSTART(i2c_periph, ENABLE);
    
    /* wait until SBSEND bit is set */
    while(!I2C_GetFlagStatus(i2c_periph, I2C_FLAG_STARTF))
    {
        time_cnt++;
        
        if(time_cnt >= time_out)
        {
            /* send a stop condition to I2C bus */
            I2C_GenerateSTOP(i2c_periph, ENABLE);
            return (err | (1<<1));
        }
    }
    
    /* send slave address to I2C bus */
    I2C_Send7bitAddress(i2c_periph, addr, I2C_Direction_Transmit);
    
    /* wait until ADDSEND bit is set */
    while(!I2C_GetFlagStatus(i2c_periph, I2C_FLAG_ADDRF))
    {
        time_cnt++;
        
        if(time_cnt >= time_out)
        {
            /* send a stop condition to I2C bus */
            I2C_GenerateSTOP(i2c_periph, ENABLE);
            return (err | (1<<2));
        }
    }
    
    /* clear ADDSEND bit */
    __IO uint32_t tmpreg;  
  
    tmpreg = i2c_periph->STS1; 
  
    tmpreg = i2c_periph->STS2; 

    for(uint8_t i = 0; i < len; i++)
    {
        /* wait until the TBE bit is set */
        while(!I2C_GetFlagStatus(i2c_periph, I2C_FLAG_TDE))
        {
            time_cnt++;
        
            if(time_cnt >= time_out)
            {
                /* send a stop condition to I2C bus */
                I2C_GenerateSTOP(i2c_periph, ENABLE);
                return (err | (1<<3));
            }
        }
        
        /* data transmission */
        I2C_SendData(i2c_periph, data[i]);
    }
    
    /* wait until the BTFF bit is set */
    while(!I2C_GetFlagStatus(i2c_periph, I2C_FLAG_BTFF))
    {
        time_cnt++;
    
        if(time_cnt >= time_out)
        {
            /* send a stop condition to I2C bus */
            I2C_GenerateSTOP(i2c_periph, ENABLE);
            return (err | (1<<4));
        }
    }
    
    /* send a stop condition to I2C bus */
    I2C_GenerateSTOP(i2c_periph, ENABLE);
    
    /* wait until stop condition generate */ 
    while((i2c_periph->CTRL1)&0x0200)
    {
        time_cnt++;
        
        if(time_cnt >= time_out)
        {
            return (err | (1<<5));
        }
    }
    
    return err;
}

uint8_t System_I2C_Master_Receive(I2C_Type* i2c_periph, uint8_t addr, uint8_t *data, uint8_t len)
{
    uint8_t err = 0;
    uint16_t time_cnt = 0;
    uint16_t time_out = 1024;
    
    /* wait until I2C bus is idle */
    while(I2C_GetFlagStatus(i2c_periph, I2C_FLAG_BUSYF))
    {
        time_cnt++;
        
        if(time_cnt >= time_out)
        {
            return (err | (1<<0));
        }
    }
    
    /* Disable Pos */
    I2C_NACKPositionConfig(i2c_periph, I2C_NACKPosition_Current);
  
    /* send a start condition to I2C bus */
    I2C_GenerateSTART(i2c_periph, ENABLE);
    
    /* wait until SBSEND bit is set */
    while(!I2C_GetFlagStatus(i2c_periph, I2C_FLAG_STARTF))
    {
        time_cnt++;
        
        if(time_cnt >= time_out)
        {
            /* send a stop condition to I2C bus */
            I2C_GenerateSTOP(i2c_periph, ENABLE);
            return (err | (1<<1));
        }
    }
    
    /* send slave address to I2C bus */
    I2C_Send7bitAddress(i2c_periph, addr, I2C_Direction_Receive);

    /* N=1,reset ACKEN bit before clearing ADDRSEND bit */
    I2C_AcknowledgeConfig(i2c_periph, DISABLE);
    
    /* wait until ADDSEND bit is set */
    while(!I2C_GetFlagStatus(i2c_periph, I2C_FLAG_ADDRF))
    {
        time_cnt++;
        
        if(time_cnt >= time_out)
        {
            /* send a stop condition to I2C bus */
            I2C_GenerateSTOP(i2c_periph, ENABLE);
            return (err | (1<<2));
        }
    }
    
    /* clear ADDSEND bit */
    __IO uint32_t tmpreg;  
  
    tmpreg = i2c_periph->STS1; 
  
    tmpreg = i2c_periph->STS2; 
    
    for(uint8_t i = 0; i < len; i++)
    {
        /* wait until the RBNE bit is set */
        while(!I2C_GetFlagStatus(i2c_periph, I2C_FLAG_RDNE))
        {
            time_cnt++;
        
            if(time_cnt >= time_out)
            {
                /* send a stop condition to I2C bus */
                I2C_GenerateSTOP(i2c_periph, ENABLE);
                return (err | (1<<3));
            }
        }
        
        /* read a data from I2C_DATA */
        data[i] = I2C_ReceiveData(i2c_periph);
    }
    
    /* N=1,send stop condition after clearing ADDRSEND bit */
    I2C_GenerateSTOP(i2c_periph, ENABLE);
    
    /* wait until stop condition generate */ 
    while((i2c_periph->CTRL1)&0x0200)
    {
        time_cnt++;
        
        if(time_cnt >= time_out)
        {
            return (err | (1<<4));
        }
    }
    
    /* Enable Acknowledge */
    I2C_AcknowledgeConfig(i2c_periph, ENABLE);
    
    return err;
}
