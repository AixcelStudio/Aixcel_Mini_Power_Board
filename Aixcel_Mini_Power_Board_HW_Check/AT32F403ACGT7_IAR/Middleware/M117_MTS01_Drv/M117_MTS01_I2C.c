/* Includes ------------------------------------------------------------------*/
#include "M117_MTS01_I2C.h"

#define I2C_ADDR 		   0x45  /* Addr */

/**
  * @brief  ��16λ�����Ʋ����ʾ���¶����ת��Ϊ�����϶�Ϊ��λ���¶ȶ���
  * @param  out���з��ŵ�16λ�������¶����
  * @retval �����϶�Ϊ��λ�ĸ����¶�
*/
float MY_OutputtoTemp(int16_t out)
{
	return ((float)out/256.0 + 40.0);	
}

/**
  * @brief  �������϶�Ϊ��λ�ĸ����¶�ֵת��Ϊ16λ�����Ʋ����ʾ���¶�ֵ
  * @param  �����϶�Ϊ��λ�ĸ����¶�ֵ
  * @retval �з��ŵ�16λ�������¶�ֵ
*/
int16_t MY_TemptoOutput(float Temp)
{
	return (int16_t)((Temp-40.0)*256.0);	
}

/**
  * @brief  �����¶Ȳ���
  * @param  ��
  * @retval I2C����״̬
*/
bool MY_ConvertTemp(void)
{	 
	uint8_t cmd[2]={(uint8_t)(CONVERT_T >>8), (uint8_t)(CONVERT_T & 0xFF)};

	/*������16λ�������ACK��Ӧ���������ֽ�*/
	if(MY_TRANSPORT_DATA_WRITE(I2C_ADDR, cmd, 2) != GPIOI2C_XFER_LASTACK)
	{
		return false;
	}
	
    return true;	 
}

/**
  * @brief  �ȴ�ת����������������
  * @param  iTemp�����ص�16λ�¶Ȳ������
  * @retval I2C����״̬
*/
bool MY_ReadTempWaiting(uint16_t *iTemp)
{
	uint8_t data[3];

	/*��3���ֽڡ�ǰ�������¶�ת�����������ֽ���ǰ������У���--CRC��*/	
	if(MY_TRANSPORT_DATA_READ(I2C_ADDR, data, 3) != GPIOI2C_XFER_LASTNACK)
	{		
		return false;  /*I2C��ַͷӦ��ΪNACK*/
	}

	/*������յ�ǰ�����ֽڵ�У��ͣ�������յĵ�3��CRC�ֽڱȽϡ�*/	
    if(data[2] != MY_CRC8(data, 2))
    {	
		return false;  /*CRC��֤ʧ��*/
    }
			
	*iTemp=(uint16_t)data[0]<<8 | data[1];
	
    return true;		
}

/**
  * @brief  ��ѯ�Ƿ�ת��������Ȼ����������
  * @param  iTemp�����ص�16�¶Ȳ������
  * @retval I2C����״̬
*/
bool MY_ReadTempPolling(uint16_t *iTemp)
{ 
    int timeout=0; uint8_t data[3];

	MY_DELAY_MS(1);   /*minimum waiting time of 1ms. after convert*/
	
	/*���Զ�3���ֽڡ����ת����û������оƬ�Ե�ַͷ��Ӧ��ΪNACK�����ת��������Ӧ��ΪACK��
	ǰ�����ֽ����¶�ת�����������ֽ���ǰ������У���CRC��*/		
	while (MY_TRANSPORT_DATA_READ(I2C_ADDR, data, 3) == GPIOI2C_XFER_ADDRNACK)
	{	
		MY_DELAY_MS(1);	
        timeout++; 
		if(timeout > 50) 
		{				
			return false;				/*��ʱ����*/
		}
	}

	/*������յ�ǰ�����ֽڵ�У��ͣ�������յĵ�3���ֽڱȽϡ�*/		
    if(data[2] != MY_CRC8(data,2))
    {	
		return false;
    }
			
	*iTemp=(uint16_t)data[0]<<8 | data[1];

    return true;		
}

/**
  * @brief  �������ڲ���Ƶ�ʺ��ظ���
  * @param  mps Ҫ���õ����ڲ���Ƶ�ʣ�ÿ�����������������Ϊ������һ
	 *@arg CFG_MPS_Single		��ÿִ��ConvertTempһ�Σ�����һ���¶Ȳ���
	 *@arg CFG_MPS_Half			��ÿִ��ConvertTempһ�Σ�����ÿ��0.5���ظ�����
	 *@arg CFG_MPS_1			��ÿִ��ConvertTempһ�Σ�����ÿ��1���ظ�����
	 *@arg CFG_MPS_2			��ÿִ��ConvertTempһ�Σ�����ÿ��2���ظ�����
	 *@arg CFG_MPS_4			��ÿִ��ConvertTempһ�Σ�����ÿ��4���ظ�����
	 *@arg CFG_MPS_10			��ÿִ��ConvertTempһ�Σ�����ÿ��10���ظ�����
   * @param  repeatability��Ҫ���õ��ظ���ֵ������Ϊ������һ
	 *@arg CFG_Repeatbility_Low			�����õ��ظ���
	 *@arg CFG_Repeatbility_Medium		���������ظ���
	 *@arg CFG_Repeatbility_High			�����ø��ظ���
  * @retval ��
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
  * @brief  ��״̬������
  * @param  status ���ص�״̬�Ĵ���ֵ
  * @param  cfg ���ص����üĴ���ֵ
  * @retval ��
*/
void MY_ReadStatusConfig(uint8_t *status, uint8_t *cfg)
{ 
	uint8_t scr_rd[3];
	
	/*��3���ֽڣ���һ�ֽ���״̬�Ĵ�����ֵ���ڶ��ֽ������üĴ�����ֵ������������ֽڵ�У��͡�*/	
	MY_TRANSPORT_READ(I2C_ADDR, READ_STATUSCONFIG, scr_rd, 3);		
	*status = scr_rd[0];
	*cfg = scr_rd[1];	
}

/**
  * @brief  ���״̬�Ĵ���ĳЩ��־
  * @param  ��
  * @retval ��
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
  * @brief  ���ñ�����
  * @param  tha_set Ҫ���õı��������¶�ֵ����λΪ���϶�
  * @param  tla_set Ҫ���õı��������¶�ֵ����λΪ���϶�
  * @retval ��
*/
void MY_SetTempAlert(float tha_set, float tla_set)
{
    uint16_t hs, ls; uint8_t scr_wr[3];

	/*���������ֵ�͵���ֵת��Ϊ��Ӧ��16λ�����Ʋ��룬��ȡ��9λ*/		
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
  * @brief  ��������
  * @param  tha_set ���صı��������¶�ֵ����λΪ���϶�
  * @param  tla_set ���صı��������¶�ֵ����λΪ���϶�
  * @retval ��
*/
void MY_GetTempAlert(float *tha_set, float *tla_set)
{
    uint16_t hs, ls; uint8_t scr_rd[3];

	/*��16λ�������޺͵��޼Ĵ���ֵ����9λΪ�����޵ĸ�9λ����ת��Ϊ��Ӧ�������϶�Ϊ��λ�ĸ��޺͵����¶�ֵ*/	
	MY_TRANSPORT_READ(I2C_ADDR, READ_ALERT_HI_SET, scr_rd, 3);
	hs = (scr_rd[0] << 8 | scr_rd[1]) << 7;
	*tha_set = MY_OutputtoTemp((int16_t)hs);

	MY_TRANSPORT_READ(I2C_ADDR, READ_ALERT_LO_SET, scr_rd, 3);
	ls = (scr_rd[0] << 8 | scr_rd[1]) << 7;
	*tla_set = MY_OutputtoTemp((int16_t)ls);		
}

/**
  * @brief  �������üĴ����ͱ������ޡ��������޵����ݵ�E2PROM
  * @param  ��
  * @retval �Ƿ�ɹ�״̬
*/
bool MY_SavetoE2prom(void)
{
	uint8_t cmd[2]={(uint8_t)(COPY_PAGE0 >>8), (uint8_t)(COPY_PAGE0 & 0xFF)};	
	bool ret= true;
	
	/*����дE2PROM����*/		
	if(MY_TRANSPORT_DATA_WRITE(I2C_ADDR, cmd, 2) != GPIOI2C_XFER_LASTACK)
		ret = false;
	
	/*�ȴ������ͱ�����*/		
	MY_DELAY_MS(50);	

    return ret;	
}
/**
  * @brief  �������ֽ����е�У���
  * @param  data���ֽ�����ָ��
  * @param  nbrOfBytes���ֽ�����ĳ���
  * @retval У��ͣ�CRC��
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
