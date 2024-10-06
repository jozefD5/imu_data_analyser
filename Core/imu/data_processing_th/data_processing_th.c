/*
 * data_processing_th.c
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include <stdio.h>
#include "data_processing_th.h"
#include <core_library/debug_log/debug_log.h>
#include <core_library/i2c/i2c_helper.h>
//#include <BMI270_SensorAPI/bmi270.h>
//#include <BMI270_SensorAPI/bmi270_examples/common/common.h>


// BMI I2C device parameters.
#define BMI_I2C_ADDRESS          0x01
#define BMI_I2C_MAX_TIMEOUT      100
#define BMI_INTERRUPT_PIN

// Thread data.
osThreadId_t dataProcessingTaskHandle;
const osThreadAttr_t dataProcessingTaskAttributes = {
    .name = "dataProcessingTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};


// I2C device.
static i2c_device_type bmi_i2c;

// Interrupt flag, new data.
static uint8_t bmi_ist_flag = 0;


/**
 * @brief Initialise required components required for threads' correct operation.
 *        Needs to be called before thread is started.
 */
void data_processing_init(I2C_HandleTypeDef *i2c) {
	// Configure.
	bmi_i2c.i2c = i2c;
	bmi_i2c.address = BMI_I2C_ADDRESS;
	bmi_i2c.timeout = BMI_I2C_MAX_TIMEOUT;

	if(i2c_helper_init(&bmi_i2c) == HAL_OK) {
		debug_log_debug("I2C BMI Init: OK");
	} else {
		debug_log_debug("I2C BMI Init: Failed");
	}
}


void data_bmi_isr(void) {
	bmi_ist_flag = 1;
}




/**
 * @brief data processing thread.
 */
void dataProcessing(void *arg) {





	while(1) {

		HAL_GPIO_TogglePin(heart_beat_led_GPIO_Port, heart_beat_led_Pin);
		osDelay(pdMS_TO_TICKS(500));
	}
}


