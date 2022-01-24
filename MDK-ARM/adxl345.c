#include "adxl345.h"





struct Adxl_Data Adxl345;
struct AdxlCommands AdxlReg = {0x00, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22,
	0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
	0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
  

enum AdxlBitNum{D0, D1, D2, D3, D4, D5, D6, D7 };

uint8_t i2c_tx[2];



int adxl345_init()
{
	/* ID */
	HAL_I2C_Master_Transmit(&hi2c1, ADXL_ADR, (uint8_t*)&AdxlReg.DEVID , 1, 1000);
	HAL_Delay(100);
	HAL_I2C_Master_Receive(&hi2c1, ADXL_ADR, (uint8_t*)&Adxl345.id, 1, 1000);
	
	Adxl345.status = 1;
	
	if(Adxl345.id != 0xE5)
  {return Adxl345.status = ADXL_ERR;}
	
	//Получение ускорения по трем осям XYZ
	SendCommand(AdxlReg.BW_RATE, BWRATE_100);
	SendCommand(AdxlReg.POWER_CTL, POWER_CTL_Measure);
	SendCommand(AdxlReg.DATA_FORMAT, RANGE_16G);

}
void SendCommand(uint8_t reg,uint8_t data)
{
  i2c_tx[0] = reg;
  i2c_tx[1] = data;
  HAL_I2C_Master_Transmit(&hi2c1, ADXL_ADR, (uint8_t*)i2c_tx, 2, 1000);
}
 

void Read_Data()
{
	HAL_I2C_Master_Transmit(&hi2c1, ADXL_ADR, (uint8_t*)&AdxlReg.DATAX0, 1, 100);
	HAL_Delay(100);
	HAL_I2C_Master_Receive(&hi2c1, ADXL_ADR, (uint8_t*)&Adxl345.raw, 6, 100);
	
	Adxl345.x = ((int16_t)(Adxl345.raw[1] << 8) | Adxl345.raw[0]);
	Adxl345.y = ((int16_t)(Adxl345.raw[3] << 8) | Adxl345.raw[2]);
	Adxl345.z = ((int16_t)(Adxl345.raw[5] << 8) | Adxl345.raw[4]);
	HAL_Delay(50);
	Adxl345.xg = (SCALE_FACTOR_16G *Adxl345.x)/1000;
	Adxl345.yg = (SCALE_FACTOR_16G *Adxl345.y)/1000;
	Adxl345.zg = (SCALE_FACTOR_16G *Adxl345.z)/1000;
}
