#ifndef __ADXL345_H
#define __ADXL345_H

#include "stm32f1xx_hal.h"


// Registers' Address 

#define ADXL_ADR            (0x53 << 1)
#define ADXL_ERR            0
//Скорость передачи выходных данных (Гц)
#define BWRATE_6_25 	 0x06
#define BWRATE_12_5 	 0x07
#define BWRATE_25 		 0x08
#define BWRATE_50 		 0x09
#define BWRATE_100		 0x0A
#define BWRATE_200		 0x0B
#define BWRATE_400		 0x0C
#define BWRATE_800		 0x0D
#define BWRATE_1600  	 0x0E
#define BWRATE_3200  	 0x0F
//Формата выходных данных
#define RANGE_2G       0x00
#define RANGE_4G       0x01
#define RANGE_8G       0x02
#define RANGE_16G      0x03
//перевод от формата выходных данных
#define SCALE_FACTOR_2G         3.9
#define SCALE_FACTOR_4G         7.8
#define SCALE_FACTOR_8G         15.6
#define SCALE_FACTOR_16G        31.2
//Настройка режима энергосбережения
#define POWER_CTL_Wakeup        0x01
#define POWER_CTL_Sleep         0x04 
#define POWER_CTL_Measure       0x08
#define POWER_CTL_AUTO_SLEEP    0x10
#define POWER_CTL_Link          0x20
//Назначение выходных линий для прерываний флагов прерываний
#define INACT_X                 0x01
#define INACT_Y                 0x02 
#define INACT_Z                 0x04
#define INACT_acdc              0x08
#define ACT_X                   0x10
#define ACT_Y                   0x20 
#define ACT_Z                   0x40
#define ACT_acdc                0x80
//Регистр флагов прерываний
#define Overrun                 0x01
#define Wetermark               0x02
#define FREE_FALL               0x04
#define Inactivity              0x08
#define Activity                0x10
#define DOUBLE_TAB              0x20
#define SINGLE_FALL             0x40
#define DATA_READY              0x80
//Выбор оси
#define TAP_X                   0x01
#define TAP_Y                   0x02 
#define TAP_Z                   0x04
#define Suppress                0x08

struct AdxlCommands
{
	uint8_t DEVID;	           //Номер ID
	uint8_t	THRESH_TAP;        //Значение уставки для толчка
	uint8_t	OFSX;	             //Смещение для оси X
	uint8_t	OFSY;	             //Смещение для оси Y
	uint8_t	OFSZ;	             //Смещение для оси Z
	uint8_t	DUR;	             //Длительность толчка
	uint8_t	LATENT;	           //Задержка перед ожиданием второго толчка
	uint8_t	WINDOW;	           //Время для обнаружения второго толчка
	uint8_t	THRESH_ACT;	       //Значение уставки функции активности
	uint8_t	THRESH_INACT;	     //Значение уставки функции бездействия
	uint8_t	TIME_INACT;	       //Время бездействия
	uint8_t	ACT_INACT_CTL;	   //Привязка событий активности/бездействия на оси
	uint8_t	THRESH_FF;	       //Значение уставки свободного падения
	uint8_t	TIME_FF;	         //Время свободного падения
	uint8_t	TAP_AXES;	         //Привязка событий толчка на оси
	uint8_t	ACT_TAP_STATUS;	   //Регистр флагов событий толчка или активности
	uint8_t	BW_RATE;	         //Настройка частоты дискретизации
	uint8_t	POWER_CTL;	       //Настройка режима энергосбережения
	uint8_t	INT_ENABLE;	       //Регистр управления прерываниями
	uint8_t	INT_MAP;	         //Назначение выходных линий для прерываний
	uint8_t	INT_SOURCE;	       //Регистр флагов прерываний
	uint8_t	DATA_FORMAT;	     //Настройка формата выходных данных
	uint8_t	DATAX0;	           //Выходные данные для оси X (младший байт)
	uint8_t	DATAX1;	           //Выходные данные для оси X (старший байт)
	uint8_t	DATAY0;	           //Выходные данные для оси Y (старший байт)
	uint8_t	DATAY1;            //Выходные данные для оси Y (старший байт)
	uint8_t	DATAZ0;            //Выходные данные для оси Z (старший байт)
	uint8_t	DATAZ1;	           //Выходные данные для оси Z (старший байт)
	uint8_t	FIFO_CTL;	         //Настройка буфера FIFO
	uint8_t	FIFO_STATUS;	     //Регистр состояния буфера FIFO
};

extern struct Adxl_Data
{
	uint8_t id;       // id устройства
	int16_t x, y, z;  // Данные ускорений по X, Y, Z в виде кода
	float xg, yg, zg; // единицы ускорения по X, Y, Z 
	uint8_t raw[6];
	uint8_t activity;
	uint8_t int_src;
	uint8_t status;
}Adxl345;

extern I2C_HandleTypeDef hi2c1;

extern uint8_t i2c_tx[2];

int adxl345_init(void);
void SendCommand(uint8_t, uint8_t);
void Read_Data(void);
void ActivWichInterruption(void);
void DuobleTap(void);
void FreeFall(void);


#endif