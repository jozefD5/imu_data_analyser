/*
 * bmi270.c
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include <core_library/bmi270/bmi270.h>
#include <core_library/debug_log/debug_log.h>
#include <core_library/i2c/i2c_helper.h>


typedef struct{
	I2C_HandleTypeDef *i2c;
	uint8_t address;
};






