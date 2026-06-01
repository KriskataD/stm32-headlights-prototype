/*
 * i2c_ina219.c
 *
 *  Created on: Jul 4, 2024
 *      Author: 35988
 */
#include "i2c_ina219.h"

#define RxSIZE 2
static uint8_t RxData[RxSIZE];
static uint8_t Register[] = {INA219_REG_SHUNTVOLTAGE,
							INA219_REG_BUSVOLTAGE,
							INA219_REG_CURRENT,
							INA219_REG_POWER};
static uint8_t registerIndex = 0;
static INA219_t ina219;

extern I2C_HandleTypeDef hi2c1;

//void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c1)
//{
//	HAL_I2C_EnableListen_IT(&hi2c1);
//}

//Called when the address of the slave(INA) matches the one sent by the master(STM)
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	if (TransferDirection == I2C_DIRECTION_TRANSMIT)
	{
		//Master send address and register to slave
		for (int i = 0; i < 4; i++)
		{
			HAL_I2C_Master_Transmit_IT(hi2c, AddrMatchCode, &Register[i], 1);
		}
	}
	else
	{
		Error_Handler();
	}
}

//Called when teh slave receive the message - address and register
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == hi2c1.Instance)
	    {
	        // Process received data here
	        uint8_t receivedRegister = Register[registerIndex];
	        registerIndex++;
	        uint16_t regValue = Read16(&ina219, receivedRegister);
	        RxData[0] = (regValue >> 8) & 0xff;
	        RxData[1] = regValue & 0xff;


	        // Store or process RxData/regValue as needed for each register
	        switch (receivedRegister)
	        {
	            case INA219_REG_SHUNTVOLTAGE:
	            	uint16_t shuntVoltage = regValue * 0.01;
	            	printf("Shunt Voltage: %u\n", shuntVoltage);
	                break;
	            case INA219_REG_BUSVOLTAGE:
	            	uint16_t result = regValue;
	            	uint16_t busVoltage = (result >> 3  ) * 4;
	            	printf("Bus Voltage: %u\n", busVoltage);
	                 break;
	            case INA219_REG_CURRENT:
	            	uint16_t current = regValue / 10;
	            	printf("Current: %u\n", current);
	                 break;
	            case INA219_REG_POWER:
	            	uint16_t power = regValue * 2;
	            	printf("Power: %u\n", power);
	                 break;
	            default:
	                 Error_Handler();
	                 break;
	         }

	        //Send the information from the register back to the master
	        HAL_I2C_Slave_Transmit_IT(hi2c, RxData, 2);
	    }
	    else
	    {
	        Error_Handler();
	    }
}

//Called when the master receive the data from the register
//void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
//{
//	if (hi2c->Instance == hi2c1.Instance)
//		{
//			uint16_t receivedData = (RxData[0] << 8) | RxData[1];
//		}
//	else
//		{
//		    Error_Handler();
//		}
//}

