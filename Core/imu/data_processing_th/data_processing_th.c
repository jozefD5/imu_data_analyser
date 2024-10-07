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


/**
 * @brief Initialise required components required for threads' correct operation.
 *        Needs to be called before thread is started.
 */
void data_processing_init(void) {
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


