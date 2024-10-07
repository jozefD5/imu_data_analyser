/*
 * data_acquisitiom_th.c
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include <stdio.h>
#include <stdbool.h>
#include "data_acquisition_th.h"
#include <core_library/debug_log/debug_log.h>
#include <core_library/i2c/i2c_helper.h>

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

// Data protecting mutex;
static SemaphoreHandle_t mtx;

// I2C device.
static i2c_device_type bmi_i2c;

// Interrupt flag, new data.
static bool bmi_ist_flag = false;

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
}

/**
 * @brief Data aqualisition thread.
 */
void data_aquisition(void *arg) {
    
    while(1) {
        if(bmi_ist_flag) {
            // Read data.

            // Assign data.
            if( xSemaphoreTake( mtx,(TickType_t) 10) == pdTRUE) {


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
HAL_StatusTypeDef data_aquisition_get_data(void) {
    HAL_StatusTypeDef res = HAL_OK;

    if( xSemaphoreTake(mtx,(TickType_t) 10) == pdTRUE) {


        xSemaphoreGive(mtx);
    } else {
        return HAL_ERROR;
    }

    return res;
}

