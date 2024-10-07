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
#define DATA_PROCESSING_PERIOD_SEC         (DATA_AQUASITION_FREQ_PERIOD / 1000.0f)

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

// Data protecting mutex;
static SemaphoreHandle_t mtx;

// I2C device.
static i2c_device_type bmi_i2c;

// Interrupt flag, new data.
static bool bmi_ist_flag = false;

// IMU data readings.
static acc_gyr_data_type imu_data = 0;





// Setup imu sensor.
static void imu_sensor_init() {
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
    }

    // Accel and gyro configuration settings.
    rslt = set_accel_gyro_config(&bmi);
    bmi2_error_codes_print_result(rslt);

    get_board_info(&board);

    coines_attach_interrupt(COINES_SHUTTLE_PIN_20, drdy_int_callback, COINES_PIN_INTERRUPT_FALLING_EDGE);

    config.type = BMI2_ACCEL;

    rslt = bmi2_get_sensor_config(&config, 1, &bmi);
    bmi2_error_codes_print_result(rslt);

    if (rslt != BMI2_OK) {
        debug_log_info("IMU init : Error"); 
    }
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

    imu_sensor_init();
}

/**
 * @brief Data aqualisition thread.
 */
void data_aquisition(void *arg) {
	int8_t rslt = 0;
    
    while(1) {
        if(bmi_ist_flag) {
            // Read data.

            // Assign data.
            if( xSemaphoreTake( mtx,(TickType_t) 10) == pdTRUE) {
            	bmi_ist_flag = false;

            	rslt = bmi2_get_sensor_data(&sensor_data, &bmi);
            	if ((rslt == BMI2_OK) &&
            		(sensor_data.status & BMI2_DRDY_ACC) &&
            	    (sensor_data.status & BMI2_DRDY_GYR)) {

            		imu_data.pitch = atan2(imu_data.acc_y, sqrt(imu_data.acc_x^2 + imu_data.acc_z^2));
            		imu_data.roll  = atan2(imu_data.acc_x, sqrt(imu_data.acc_y^2 + imu_data.acc_z^2));

            		// Apply complementary filter with time interval between readings.
            		imu_data.yaw   += (imu_data.gyr_z + alpha * (imu_data.yaw_acc - imu_data.yaw)) * DATA_PROCESSING_PERIOD_SEC;
            		imu_data.pitch += (imu_data.gyr_x + alpha * (imu_data.pitch_acc - imu_data.pitch)) * DATA_PROCESSING_PERIOD_SEC;
					imu_data.roll  += (imu_data.gyr_y + alpha * (imu_data.roll_acc - imu_data.roll)) * DATA_PROCESSING_PERIOD_SEC;

            	}
                xSemaphoreGive(mtx);
            } else {
                // TODO.
                // Trigger fail-safe.
            }
        } else {
            // // TODO.
            // Fail-save routint.
        }

		HAL_GPIO_TogglePin(heart_beat_led_GPIO_Port, heart_beat_led_Pin);
		osDelay(pdMS_TO_TICKS(DATA_AQUASITION_FREQ_PERIOD));
	}
}

/**
 * @brief Provade lates obtained data from IMU sensor.
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

