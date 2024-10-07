/*
 * data_acquisitiom_th.c
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "data_acquisition_th.h"
#include "core_library/debug_log/debug_log.h"
#include "core_library/i2c/i2c_helper.h"
#include "bmi270.h"
#include "common.h"
#include "coines.h"

// Thread period, frequency control frequency.
#define DATA_AQUASITION_FREQ_PERIOD        10U  // 10 ms (100 Hz).

// BMI I2C device parameters.
#define BMI_I2C_ADDRESS          0x01
#define BMI_I2C_MAX_TIMEOUT      100

// Thread data.
osThreadId_t dataAquisitionTaskHandle;
const osThreadAttr_t dataAquisitionTaskAttributes = {
    .name = "dataAquasitionTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t) osPriorityNormal,
};

// Data protecting mutex.
static SemaphoreHandle_t mtx;

// I2C device.
static i2c_device_type bmi_i2c;

// Interrupt flag, new data.
static bool bmi_ist_flag = false;

// IMU data readings.
static acc_gyr_data_type imu_data = 0;

// Sensor data.
static struct bmi2_sens_data sensor_data = {{0}};
static struct bmi2_dev bmi;



// Setup imu sensor.
static HAL_StatusTypeDef imu_sensor_init() {
	HAL_StatusTypeDef res = HAL_OK;
    int8_t rslt = 0;

    struct bmi2_dev bmi;

    struct bmi2_sens_data sensor_data = {{0}};

    uint8_t indx = 1;
    uint8_t board = 0;
    struct bmi2_sens_config config;

    // Set I2C interface.
    rslt = bmi2_interface_init(&bmi, BMI2_I2C_INTF);
    bmi2_error_codes_print_result(rslt);

    // Initialise bmi270.
    rslt = bmi270_init(&bmi);
    bmi2_error_codes_print_result(rslt);

    if (rslt != BMI2_OK) {
    	debug_log_info("IMU init : Error");
    	return HAL_ERROR;
    }

    // Accel and gyro configuration settings.
    rslt = set_accel_gyro_config(&bmi);
    bmi2_error_codes_print_result(rslt);

    get_board_info(&board);

    config.type = BMI2_ACCEL;

    rslt = bmi2_get_sensor_config(&config, 1, &bmi);
    bmi2_error_codes_print_result(rslt);

    if (rslt != BMI2_OK) {
        debug_log_info("IMU init : Error"); 
        return HAL_ERROR;
    }

    return res;
}


/**
 * @brief Interrupt service routine, should be triggered by IMU sensor 
 *        when new data is available.
 */
void data_aquisition_bmi_isr(void) {
	bmi_ist_flag = true;
}

/**
 * @brief Initilise data aqualisition thread, needs to be called before 
 *        starting thread.
 */
HAL_StatusTypeDef data_aquisition_int(I2C_HandleTypeDef *i2c) {
    HAL_StatusTypeDef res = HAL_OK;

    // Configure.
	bmi_i2c.i2c = i2c;
	bmi_i2c.address = BMI_I2C_ADDRESS;
	bmi_i2c.timeout = BMI_I2C_MAX_TIMEOUT;

	if(i2c_helper_init(&bmi_i2c) == HAL_OK) {
		debug_log_debug("I2C BMI Init: OK");
	} else {
		debug_log_debug("I2C BMI Init: Failed");
	}

    mtx = xSemaphoreCreateMutex();
    if(mtx == NULL) {
        debug_log_error("Failed to create mutex for data aquasition!");
        return HAL_ERROR;
    }

    res = imu_sensor_init();
    return res;
}

/**
 * @brief Data aqualisition thread.
 */
void data_aquisition(void *arg) {
	int8_t rslt = 0;
    
    while(1) {
        if(bmi_ist_flag) {
            // Read and assign data.
            if( xSemaphoreTake( mtx,(TickType_t) 10) == pdTRUE) {
            	bmi_ist_flag = false;

            	rslt = bmi2_get_sensor_data(&sensor_data, &bmi);
            	if ((rslt == BMI2_OK) &&
            			(sensor_data.status & BMI2_DRDY_ACC) &&
						(sensor_data.status & BMI2_DRDY_GYR)) {

            		// Converting lsb to meter per second squared for 16 bit accelerometer at 2G range.
            		imu_data.acc_x = lsb_to_mps2(sensor_data.acc.x, (float)2, bmi.resolution);
            		imu_data.acc_y = lsb_to_mps2(sensor_data.acc.y, (float)2, bmi.resolution);
            		imu_data.acc_z = lsb_to_mps2(sensor_data.acc.z, (float)2, bmi.resolution);

            		// Converting lsb to degree per second for 16 bit gyro at 2000dps range.
            		imu_data.gyr_x = lsb_to_dps(sensor_data.gyr.x, (float)2000, bmi.resolution);
            		imu_data.gyr_y = lsb_to_dps(sensor_data.gyr.y, (float)2000, bmi.resolution);
            		imu_data.gyr_z = lsb_to_dps(sensor_data.gyr.z, (float)2000, bmi.resolution);
            	}
                xSemaphoreGive(mtx);
            } else {
            	debug_log_error("Failed to acquire IMU data");
                // TODO Trigger fail-safe.
            }
        } else {
            // // TODO.
            // Fail-save routine.
        }

		HAL_GPIO_TogglePin(heart_beat_led_GPIO_Port, heart_beat_led_Pin);
		osDelay(pdMS_TO_TICKS(DATA_AQUASITION_FREQ_PERIOD));
	}
}

/**
 * @brief Provide lates obtained data from IMU sensor.
 */
HAL_StatusTypeDef data_aquisition_get_data(acc_gyr_data_type *data) {
    HAL_StatusTypeDef res = HAL_OK;

    if( xSemaphoreTake(mtx,(TickType_t) 10) == pdTRUE) {
    	*data = *imu_data;
        xSemaphoreGive(mtx);
    } else {
        return HAL_ERROR;
    }

    return res;
}

