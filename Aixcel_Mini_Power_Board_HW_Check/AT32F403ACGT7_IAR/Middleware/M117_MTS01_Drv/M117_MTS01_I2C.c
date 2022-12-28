/* Includes ------------------------------------------------------------------*/
#include "M117_MTS01_I2C.h"

#define I2C_ADDR 		   0x45  /* Addr */

/**
  * @brief  把16位二进制补码表示的温度输出转换为以摄氏度为单位的温度读数
  * @param  out：有符号的16位二进制温度输出
  * @retval 以摄氏度为单位的浮点温度
*/
float MY_OutputtoTemp(int16_t out)
{
	return ((float)out/256.0 + 40.0);	
}

/**
  * @brief  把以摄氏度为单位的浮点温度值转换为16位二进制补码表示的温度值
  * @param  以摄氏度为单位的浮点温度值
  * @retval 有符号的16位二进制温度值
*/
int16_t MY_TemptoOutput(float Temp)
{
	return (int16_t)((Temp-40.0)*256.0);	
}

/**
  * @brief  启动温度测量
  * @param  无
  * @retval I2C发送状态
*/
bool MY_ConvertTemp(void)
{	 
	uint8_t cmd[2]={(uint8_t)(CONVERT_T >>8), (uint8_t)(CONVERT_T & 0xFF)};

	/*主发送16位命令，从以ACK响应命令的最后字节*/
	if(MY_TRANSPORT_DATA_WRITE(I2C_ADDR, cmd, 2) != GPIOI2C_XFER_LASTACK)
	{
		return false;
	}
	
    return true;	 
}

/**
  * @brief  等待转换结束后读测量结果
  * @param  iTemp：返回的16位温度测量结果
  * @retval I2C接收状态
*/
bool MY_ReadTempWaiting(uint16_t *iTemp)
{
	uint8_t data[3];

	/*读3个字节。前两个是温度转换结果，最后字节是前两个的校验和--CRC。*/	
	if(MY_TRANSPORT_DATA_READ(I2C_ADDR, data, 3) != GPIOI2C_XFER_LASTNACK)
	{		
		return false;  /*I2C地址头应答为NACK*/
	}

	/*计算接收的前两个字节的校验和，并与接收的第3个CRC字节比较。*/	
    if(data[2] != MY_CRC8(data, 2))
    {	
		return false;  /*CRC验证失败*/
    }
			
	*iTemp=(uint16_t)data[0]<<8 | data[1];
	
    return true;		
}

/**
  * @brief  查询是否转换结束，然后读测量结果
  * @param  iTemp：返回的16温度测量结果
  * @retval I2C接收状态
*/
bool MY_ReadTempPolling(uint16_t *iTemp)
{ 
    int timeout=0; uint8_t data[3];

	MY_DELAY_MS(1);   /*minimum waiting time of 1ms. after convert*/
	
	/*尝试读3个字节。如果转换还没结束，芯片对地址头的应答为NACK。如果转换结束，应答为ACK。
	前两个字节是温度转换结果，最后字节是前两个的校验和CRC。*/		
	while (MY_TRANSPORT_DATA_READ(I2C_ADDR, data, 3) == GPIOI2C_XFER_ADDRNACK)
	{	
		MY_DELAY_MS(1);	
        timeout++; 
		if(timeout > 50) 
		{				
			return false;				/*超时错误*/
		}
	}

	/*计算接收的前两个字节的校验和，并与接收的第3个字节比较。*/		
    if(data[2] != MY_CRC8(data,2))
    {	
		return false;
    }
			
	*iTemp=(uint16_t)data[0]<<8 | data[1];

    return true;		
}

/**
  * @brief  设置周期测量频率和重复性
  * @param  mps 要设置的周期测量频率（每秒测量次数），可能为下列其一
	 *@arg CFG_MPS_Single		：每执行ConvertTemp一次，启动一次温度测量
	 *@arg CFG_MPS_Half			：每执行ConvertTemp一次，启动每秒0.5次重复测量
	 *@arg CFG_MPS_1			：每执行ConvertTemp一次，启动每秒1次重复测量
	 *@arg CFG_MPS_2			：每执行ConvertTemp一次，启动每秒2次重复测量
	 *@arg CFG_MPS_4			：每执行ConvertTemp一次，启动每秒4次重复测量
	 *@arg CFG_MPS_10			：每执行ConvertTemp一次，启动每秒10次重复测量
   * @param  repeatability：要设置的重复性值，可能为下列其一
	 *@arg CFG_Repeatbility_Low			：设置低重复性
	 *@arg CFG_Repeatbility_Medium		：设置中重复性
	 *@arg CFG_Repeatbility_High			：设置高重复性
  * @retval 无
*/
void MY_SetConfig(uint8_t mps, uint8_t repeatability)
{
	uint8_t scr_rd[3],scr_wr[3];
	MY_TRANSPORT_READ(I2C_ADDR,READ_STATUSCONFIG,scr_rd,3);
	
	scr_wr[0]=(scr_rd[1]&~CFG_Repeatbility_Mask);
	scr_wr[0]|=repeatability;
	scr_wr[0]=(scr_wr[0]&~CFG_MPS_Mask);
	scr_wr[0]|=mps;
	scr_wr[1]=0xFF;
	scr_wr[2]=MY_CRC8(scr_wr,2);
	
	MY_TRANSPORT_WRITE(I2C_ADDR,WRITE_CONFIG,scr_wr,3);
}

/**
  * @brief  读状态和配置
  * @param  status 返回的状态寄存器值
  * @param  cfg 返回的配置寄存器值
  * @retval 无
*/
void MY_ReadStatusConfig(uint8_t *status, uint8_t *cfg)
{ 
	uint8_t scr_rd[3];
	
	/*读3个字节，第一字节是状态寄存器的值，第二字节是配置寄存器的值，后跟这两个字节的校验和。*/	
	MY_TRANSPORT_READ(I2C_ADDR, READ_STATUSCONFIG, scr_rd, 3);		
	*status = scr_rd[0];
	*cfg = scr_rd[1];	
}

/**
  * @brief  清除状态寄存器某些标志
  * @param  无
  * @retval 无
*/
bool MY_ClearStatus(void)
{ 
	uint8_t cmd[2]={(uint8_t)(CLEAR_STATUS >>8), (uint8_t)(CLEAR_STATUS & 0xFF)};
	
	if(MY_TRANSPORT_DATA_WRITE(I2C_ADDR, cmd, 2) != GPIOI2C_XFER_LASTACK)
	{	
		return false;
	}
	
    return true;	
}

/**
  * @brief  设置报警限
  * @param  tha_set 要设置的报警高限温度值，单位为摄氏度
  * @param  tla_set 要设置的报警低限温度值，单位为摄氏度
  * @retval 无
*/
void MY_SetTempAlert(float tha_set, float tla_set)
{
    uint16_t hs, ls; uint8_t scr_wr[3];

	/*将浮点高限值和低限值转换为对应的16位二进制补码，仅取高9位*/		
	hs=MY_TemptoOutput(tha_set)>>7 & 0x01FF; 
	
	scr_wr[0] = hs >> 8;
	scr_wr[1] = hs & 0xFF;	
	scr_wr[2] = MY_CRC8(scr_wr,2);
	
	MY_TRANSPORT_WRITE(I2C_ADDR, WRITE_ALERT_HI_SET, scr_wr, 3);	

	ls=MY_TemptoOutput(tla_set)>>7 & 0x01FF;
	
	scr_wr[0] = ls >> 8;
	scr_wr[1] = ls & 0xFF;	
	scr_wr[2] = MY_CRC8(scr_wr,2);
	
	MY_TRANSPORT_WRITE(I2C_ADDR, WRITE_ALERT_LO_SET, scr_wr, 3);		
}

/**
  * @brief  读报警限
  * @param  tha_set 返回的报警高限温度值，单位为摄氏度
  * @param  tla_set 返回的报警低限温度值，单位为摄氏度
  * @retval 无
*/
void MY_GetTempAlert(float *tha_set, float *tla_set)
{
    uint16_t hs, ls; uint8_t scr_rd[3];

	/*读16位报警高限和低限寄存器值，低9位为报警限的高9位。并转换为相应的以摄氏度为单位的高限和低限温度值*/	
	MY_TRANSPORT_READ(I2C_ADDR, READ_ALERT_HI_SET, scr_rd, 3);
	hs = (scr_rd[0] << 8 | scr_rd[1]) << 7;
	*tha_set = MY_OutputtoTemp((int16_t)hs);

	MY_TRANSPORT_READ(I2C_ADDR, READ_ALERT_LO_SET, scr_rd, 3);
	ls = (scr_rd[0] << 8 | scr_rd[1]) << 7;
	*tla_set = MY_OutputtoTemp((int16_t)ls);		
}

/**
  * @brief  保存配置寄存器和报警高限、报警低限的内容到E2PROM
  * @param  无
  * @retval 是否成功状态
*/
bool MY_SavetoE2prom(void)
{
	uint8_t cmd[2]={(uint8_t)(COPY_PAGE0 >>8), (uint8_t)(COPY_PAGE0 & 0xFF)};	
	bool ret= true;
	
	/*发送写E2PROM命令*/		
	if(MY_TRANSPORT_DATA_WRITE(I2C_ADDR, cmd, 2) != GPIOI2C_XFER_LASTACK)
		ret = false;
	
	/*等待擦除和编程完成*/		
	MY_DELAY_MS(50);	

    return ret;	
}
/**
  * @brief  计算多个字节序列的校验和
  * @param  data：字节数组指针
  * @param  nbrOfBytes：字节数组的长度
  * @retval 校验和（CRC）
*/
#define POLYNOMIAL 	0x131 //100110001
uint8_t MY_CRC8(uint8_t data[], uint8_t nbrOfBytes)
{
    uint8_t bit;        // bit mask
    uint8_t crc = 0xFF; // calculated checksum
    uint8_t byteCtr;    // byte counter
  
  // calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
    {
        crc ^= (data[byteCtr]);
        for(bit = 8; bit > 0; --bit)
        {
            if(crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
            else           crc = (crc << 1);
        }
    }
  
    return crc;
}
