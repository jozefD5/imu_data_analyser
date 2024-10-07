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
#define DATA_PROCESSING_FREQ_PERIOD        10U
#define DATA_PROCESSING_PERIOD_SEC         (DATA_PROCESSING_FREQ_PERIOD / 1000.0f)

#define GYRO_SENSITIVITY 1.0          // Adjust sensitivity as needed
#define ACCEL_SENSITIVITY 1.0         // Adjust sensitivity as needed
#define KF_ALPHA 0.98                 // Filter alpha (adjust for desired balance)
#define M_PI    3.14



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


static float calculate_pitch_accel(acc_gyr_data_type *data) {
    return atan2(data->acc_y, sqrt(data->acc_x*data->acc_x + data->acc_z*data->acc_z)) * 180.0 / M_PI;
}

static float calculate_roll_accel(acc_gyr_data_type *data) {
    return atan2(data->acc_x, sqrt(data->acc_y*data->acc_y + data->acc_z*data->acc_z)) * 180.0 / M_PI;
}

static void update_pitch_roll(acc_gyr_data_type *data) {
    float pitch_accel = calculate_pitch_accel(data);
    float roll_accel = calculate_roll_accel(data);

    data->pitch = data->pitch + (data->gyr_x * GYRO_SENSITIVITY * DATA_PROCESSING_PERIOD_SEC) + KF_ALPHA * (pitch_accel - data->pitch);
    data->roll  = data->roll + (data->gyr_y * GYRO_SENSITIVITY * DATA_PROCESSING_PERIOD_SEC) + KF_ALPHA * (roll_accel - data->roll);
}

void update_yaw(acc_gyr_data_type *data) {
    data->yaw += data->gyr_z * GYRO_SENSITIVITY * DATA_PROCESSING_PERIOD_SEC;
    if (data->yaw > 180.0) {
    	data->yaw -= 360.0;
    } else if (data->yaw < -180.0) {
    	data->yaw += 360.0;
    }
}






/**
 * @brief Initialise required components required for threads' correct operation.
 *        Needs to be called before thread is started.
 */
HAL_StatusTypeDef data_processing_init(void) {
	HAL_StatusTypeDef res = HAL_OK;

	mtx = xSemaphoreCreateMutex();
	if(mtx == NULL) {
	    debug_log_error("Failed to create mutex for data processing!");
	    return HAL_ERROR;
	}

	return res;
}

/**
 * @brief data processing thread.
 */
void data_processing(void *arg) {
	while(1) {
		if( xSemaphoreTake( mtx,(TickType_t) 10) == pdTRUE) {
			if(data_aquisition_get_data(&imu_data) == HAL_OK) {

				// Update pitch and roll using complementary filter.
				update_pitch_roll(&imu_data);

				// Update yaw.
				update_yaw(&imu_data);

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
