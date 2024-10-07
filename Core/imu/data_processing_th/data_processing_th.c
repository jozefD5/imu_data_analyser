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
#define DATA_PROCESSING_PERIOD_SEC         (DATA_PROCESSING_FREQ_PERIOD / 1000.0f)
#define COMPLEMENTARY_ALPHA                0.98f

// Thread data.
osThreadId_t dataProcessingTaskHandle;
const osThreadAttr_t dataProcessingTaskAttributes = {
    .name = "dataProcessingTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

// Data protecting mutex.
static SemaphoreHandle_t mtx;

// IMU data readings.
static acc_gyr_data_type imu_data = 0;



/**
 * @brief Initialise required components required for threads' correct operation.
 *        Needs to be called before thread is started.
 */
void data_processing_init(void) {
	mtx = xSemaphoreCreateMutex();
	if(mtx == NULL) {
	    debug_log_error("Failed to create mutex for data processing!");
	    return HAL_ERROR;
	}
}

/**
 * @brief data processing thread.
 */
void data_processing(void *arg) {
	while(1) {
		if( xSemaphoreTake( mtx,(TickType_t) 10) == pdTRUE) {
			if(data_aquisition_get_data(&imu_data) == HAL_OK) {

				// Convert to angels.
				imu_data.pitch = atan2(imu_data.acc_y, sqrt(imu_data.acc_x^2 + imu_data.acc_z^2));
				imu_data.roll  = atan2(imu_data.acc_x, sqrt(imu_data.acc_y^2 + imu_data.acc_z^2));

				// Apply complementary filter with time interval between readings.
				imu_data.yaw   += (imu_data.gyr_z + COMPLEMENTARY_ALPHA * (imu_data.yaw_acc - imu_data.yaw)) * DATA_PROCESSING_PERIOD_SEC;
				imu_data.pitch += (imu_data.gyr_x + COMPLEMENTARY_ALPHA * (imu_data.pitch_acc - imu_data.pitch)) * DATA_PROCESSING_PERIOD_SEC;
				imu_data.roll  += (imu_data.gyr_y + COMPLEMENTARY_ALPHA * (imu_data.roll_acc - imu_data.roll)) * DATA_PROCESSING_PERIOD_SEC;

			} else {
				// TODO. Fail safe routine.
				debug_log_error("Failed to data processing!");
			}

			xSemaphoreGive(mtx);
		} else {
			// TODO. Trigger fail-safe.
			debug_log_error("Failed to data processing!");
	    }

		osDelay(pdMS_TO_TICKS(DATA_PROCESSING_FREQ_PERIOD));
	}
}

/**
 * @brief Provide processed accelerometer and gyroscope data.
 */
HAL_StatusTypeDef data_processing_get_data(acc_gyr_data_type *data) {
    HAL_StatusTypeDef res = HAL_OK;

    if( xSemaphoreTake(mtx,(TickType_t) 10) == pdTRUE) {
    	*data = *imu_data;
        xSemaphoreGive(mtx);
    } else {
        return HAL_ERROR;
    }

    return res;
}
