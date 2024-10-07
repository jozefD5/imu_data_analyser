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
#include "imu/data_acquisition/data_acquisition_th.h"

// Thread period, frequency control frequency.
#define DATA_PROCESSING_FREQ_PERIOD        10U  // 10 ms (100 Hz).

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

		if(data_aquisition_get_data() == HAL_OK) {

		} else {
			// TODO.
			// Failsafe routine.
		}

		HAL_GPIO_TogglePin(heart_beat_led_GPIO_Port, heart_beat_led_Pin);
		osDelay(pdMS_TO_TICKS(DATA_PROCESSING_FREQ_PERIOD));
	}
}


