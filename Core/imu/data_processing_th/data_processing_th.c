/*
 * data_processing_th.c
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include <stdio.h>
#include "data_processing_th.h"
#include "core_library/debug_log/debug_log.h"
#include "core_library/i2c/i2c_helper.h"
#include "BMI270_SensorAPI/bmi270.h"
#include "BMI270_SensorAPI/bmi270_examples/common/common.h"
#include "imu/data_acquisition/data_acquisition_th.h"

// Thread period, frequency control frequency.
#define DATA_PROCESSING_FREQ_PERIOD        10U        // 10 ms (100 Hz).
#define COMPLEMENTARY_ALPHA                0.98f

// Thread data.
osThreadId_t dataProcessingTaskHandle;
const osThreadAttr_t dataProcessingTaskAttributes = {
    .name = "dataProcessingTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

// IMU data readings.
static acc_gyr_data_type imu_data = 0;

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
			if(data_aquisition_get_data(&imu_data) == HAL_OK) {

				imu_data.yaw =   COMPLEMENTARY_ALPHA * (imu_data.yaw   + imu_data.gyr_x * dt) + (1 - COMPLEMENTARY_ALPHA) * pitch_acc;
				imu_data.pitch = COMPLEMENTARY_ALPHA * (imu_data.pitch + imu_data.gyr_y * dt) + (1 - COMPLEMENTARY_ALPHA) * roll_acc;
				imu_data.roll =  COMPLEMENTARY_ALPHA * (imu_data.roll  + imu_data.gyr_z * dt);



			} else {
				// TODO.
				// Fail safe.
			}
		} else {
			// TODO.
			// Fail safe routine.
		}

		HAL_GPIO_TogglePin(heart_beat_led_GPIO_Port, heart_beat_led_Pin);
		osDelay(pdMS_TO_TICKS(DATA_PROCESSING_FREQ_PERIOD));
	}
}


