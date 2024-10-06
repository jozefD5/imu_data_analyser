/*
 * data_processing_th.c
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include <core_library/debug_log/debug_log.h>
#include "data_processing_th.h"

osThreadId_t dataProcessingTaskHandle;
const osThreadAttr_t dataProcessingTaskAttributes = {
    .name = "dataProcessingTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};


void dataProcessing(void *arg) {
	debug_log_info("Data processing task is running...");
	while(1) {

		HAL_GPIO_TogglePin(heart_beat_led_GPIO_Port, heart_beat_led_Pin);
		osDelay(pdMS_TO_TICKS(500));
	}
}


