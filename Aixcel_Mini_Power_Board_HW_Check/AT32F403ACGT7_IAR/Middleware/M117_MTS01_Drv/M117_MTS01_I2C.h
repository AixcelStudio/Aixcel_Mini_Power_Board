/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef M117_MTS01_I2C_H
#define M117_MTS01_I2C_H

#include "MY_I2C_DRV.h"

typedef enum {
    CONVERT_T            	= 0xCC44,   //测量温度
    BREAK                	= 0x3093,   //中止周期测量
    READ_STATUSCONFIG     	= 0xf32d,   //读取状态和配置寄存器
    CLEAR_STATUS         	= 0x3041,   //清除状态寄存器
    WRITE_CONFIG         	= 0x5206,	//写配置寄存器
    READ_ALERT_HI_SET    	= 0xe11f,   //读取报警门限上阈值
    READ_ALERT_LO_SET    	= 0xe102,   //读取报警清除门限下阈值
    WRITE_ALERT_HI_SET   	= 0x611d,   //设置报警门限上阈值
    WRITE_ALERT_LO_SET   	= 0x6100,   //设置报警清除门限下阈值
    COPY_PAGE0           	= 0xcc48,   //保存page0到mtp
    RECALL_EE            	= 0xccb8,   //恢复ee区域
    RECALL_PAGE0_RES     	= 0xccb6,	//恢复page0的残留区域
	SOFT_RST            	= 0x30a2,	//软件复位
	WRITE_ALERT_LO_UNSET	= 0x610b,   //设置报警门限下阈值
	WRITE_ALERT_HI_UNSET	= 0x6116,   //设置报警清除门限上阈值
    READ_ALERT_HI_UNSET	    = 0xe114,   //读取报警清除门限上阈值
    READ_ALERT_LO_UNSET	    = 0xe109,   //读取报警门限下阈值

} I2C_CMD;

/*Bit definition of config register*/
#define CFG_MPS_Mask   		    (0x1C)
#define CFG_Repeatbility_Mask   (0x03)

#define CFG_MPS_Single  		(0x00)
#define CFG_MPS_Half  		    (0x04)
#define CFG_MPS_1  			    (0x08)
#define CFG_MPS_2 			    (0x0C)
#define CFG_MPS_4 			    (0x10)
#define CFG_MPS_10 			    (0x14)

#define CFG_Repeatbility_Low    (0x00)
#define CFG_Repeatbility_Medium (0x01)
#define CFG_Repeatbility_High 	(0x02)

/*Bit definition of status register*/
#define Status_Meature_Mask   	(0x81)
#define Status_WriteCrc_Mask    (0x20)
#define Status_CMD_Mask      	(0x10)
#define Status_POR_Mask       	(0x08)
#define Status_T_Alert_Mask   	(0x04)

/*Definition of conversion time corresponding to repeatability setting*/
/*默认低/中/高重复性对应转换时间分别为4/6/11ms,考虑到不同用户的单片机延时偏差，预留2ms以保证转换时间充分*/
#define tConvert_LowRepeatability   		(6)	/* ms. */
#define tConvert_MediumRepeatability   	    (8)	/* ms. */
#define tConvert_HighRepeatability   		(13)	/* ms. */


#include "systick.h"   //此处放置用户自定义延时头文件
#define MY_DELAY_MS(x) Delay_us(1000*x)
#define MY_DELAY_US Delay_us
#ifndef MY_DELAY_MS
#error "please redefine MY_DELAY as you own platform delay function!"
#endif

#include "MY_I2C_DRV.h"  //此处放置用户IIC底层驱动文件
#define MY_TRANSPORT_WRITE      GPIOI2C_Cmd_Write
#define MY_TRANSPORT_READ       GPIOI2C_Cmd_Read
#define MY_TRANSPORT_DATA_WRITE GPIOI2C_Transmit
#define MY_TRANSPORT_DATA_READ  GPIOI2C_Receive


/*Exported functions*/
float MY_OutputtoTemp(int16_t out);
int16_t MY_TemptoOutput(float Temp);
bool MY_ConvertTemp(void);
bool MY_ReadTempWaiting(uint16_t *iTemp);
bool MY_ReadTempPolling(uint16_t *iTemp);
void MY_ReadStatusConfig(uint8_t *status, uint8_t *cfg);
bool MY_ClearStatus(void);
void MY_SetConfig(uint8_t mps, uint8_t repeatability);
void MY_GetTempAlert(float *tha_set, float *tla_set);
void MY_SetTempAlert(float tha_set, float tla_set);
bool MY_SavetoE2prom(void);
uint8_t MY_CRC8(uint8_t data[], uint8_t nbrOfBytes);

#endif //M117_MTS01_IIC_H
