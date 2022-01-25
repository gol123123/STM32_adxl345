#include "adxl345.h"

// Настройка режима работы, по умолчанию чтение данных
#define READ_DATA     //Получение ускорения по трем осям XYZ
//#define AWI           //прерывание для обнаружения активности
//#define _DOUBLE_TAP   //Настройка засекания двойного удара
//#define _FREE_FALL    //Свободное падение


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
	
#ifdef READ_DATA	//Получение ускорения по трем осям XYZ
	SendCommand(AdxlReg.BW_RATE, BWRATE_100);
	SendCommand(AdxlReg.POWER_CTL, POWER_CTL_Measure);
	SendCommand(AdxlReg.DATA_FORMAT, RANGE_16G);
#endif
	
#ifdef AWI        //прерывание для обнаружения активности
	SendCommand(AdxlReg.ACT_INACT_CTL, (ACT_X | ACT_Y | ACT_Z | ACT_acdc));
	SendCommand(AdxlReg.THRESH_ACT, 10);
	SendCommand(AdxlReg.INT_MAP, ~(Activity));
	SendCommand(AdxlReg.INT_ENABLE, Activity);
	SendCommand(AdxlReg.POWER_CTL, POWER_CTL_Measure);
#endif 
	
#ifdef _DOUBLE_TAP	//Настройка засекания двойного удара
	SendCommand(AdxlReg.TAP_AXES, (TAP_X | TAP_Y | TAP_Z));
	SendCommand(AdxlReg.THRESH_ACT, 60);
	SendCommand(AdxlReg.DUR, 40);
	SendCommand(AdxlReg.LATENT, 80);
	SendCommand(AdxlReg.WINDOW, 200);
  SendCommand(AdxlReg.INT_ENABLE, DOUBLE_TAB);
  SendCommand(AdxlReg.INT_MAP, ~(DOUBLE_TAB));
  SendCommand(AdxlReg.POWER_CTL, POWER_CTL_Measure);
#endif

#ifdef _FREE_FALL	//Свободное падение
	SendCommand(AdxlReg.THRESH_FF, 3);
	SendCommand(AdxlReg.TIME_FF, 2);
	SendCommand(AdxlReg.INT_ENABLE, FREE_FALL);
	SendCommand(AdxlReg.INT_MAP, ~(FREE_FALL));
	SendCommand(AdxlReg.POWER_CTL, POWER_CTL_Measure);
#endif 
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

void ActivWichInterruption()
{
  HAL_I2C_Mem_Read(&hi2c1, ADXL_ADR, AdxlReg.INT_SOURCE, 1, &Adxl345.int_src, 1, 100);
  if(Adxl345.int_src & (1 << D4))
  {
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	  Adxl345.int_src = 0;
  }
  HAL_Delay(50);
}

void DuobleTap()
{
	HAL_I2C_Mem_Read(&hi2c1, ADXL_ADR, AdxlReg.INT_SOURCE, 1, &Adxl345.int_src, 1, 100);
	if(Adxl345.int_src & (1 << D5))
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		Adxl345.int_src = 0;
	}
	HAL_Delay(50);
}

void FreeFall()
{
HAL_I2C_Mem_Read(&hi2c1, ADXL_ADR, AdxlReg.INT_SOURCE, 1, &Adxl345.int_src, 1, 100);
if(Adxl345.int_src & (1 << D2))
{
  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  Adxl345.int_src = 0;
}
HAL_Delay(50);
}