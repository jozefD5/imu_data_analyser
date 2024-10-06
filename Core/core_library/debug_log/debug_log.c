/*
 * debug_log.c
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include <core_library/debug_log/debug_log.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

// UART driver to be used for debugging.
static UART_HandleTypeDef *uart_driver;

// Log level to be used to control message printed to terminal.
static log_level_type set_log_level;


/**
 * @brief Re-targets  C library printf function to the STM32 USART.
 */
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(uart_driver, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

/**
 * @brief Initialise debug print driver.
 * @param *huart    uart/usart driver to be used for print to terminal
 * @param level     log level will used to set what debug prints will be printed.
 */
HAL_StatusTypeDef debug_log_init(UART_HandleTypeDef *huart, log_level_type level) {
	HAL_StatusTypeDef res = HAL_OK;

	if(huart == NULL) {
		return HAL_ERROR;
	} else {
		uart_driver = huart;
	}

	set_log_level = level;

	debug_log(LOG_LEVEL_INFO, "Debug Log init completed.");

	return res;
}

/**
 * @brief Log message to terminal.
 * @param level     log level of the message.
 * @param format    message and format to be printed.
 * @param ...       additional arguments such as integers or strings.
 */
void debug_log(log_level_type level, const char *format, ...) {
	if (level >= set_log_level) {
	    va_list args;
	    va_start(args, format);
	    printf("[%s] ", level == LOG_LEVEL_INFO ? "INFO" : level == LOG_LEVEL_DEBUG ? "DEBUG" : level == LOG_LEVEL_WARNING ? "WARNING" : "ERROR");
	    vprintf(format, args);
	    printf("\r\n");
	    va_end(args);
	}
}

/**
 * @brief Log info message to terminal.
 * @param format    message and format to be printed.
 * @param ...       additional arguments such as integers or strings.
 */
void debug_log_info(const char *format, ...) {
	va_list args;
	va_start(args, format);
	debug_log(LOG_LEVEL_INFO, format, args);
	va_end(args);
}

/**
 * @brief Log debug message to terminal.
 * @param format    message and format to be printed.
 * @param ...       additional arguments such as integers or strings.
 */
void debug_log_debug(const char *format, ...) {
	va_list args;
	va_start(args, format);
	debug_log(LOG_LEVEL_DEBUG, format, args);
	va_end(args);
}

/**
 * @brief Log warning message to terminal.
 * @param format    message and format to be printed.
 * @param ...       additional arguments such as integers or strings.
 */
void debug_log_warning(const char *format, ...) {
	va_list args;
	va_start(args, format);
	debug_log(LOG_LEVEL_WARNING, format, args);
	va_end(args);
}

/**
 * @brief Log error message to terminal.
 * @param format    message and format to be printed.
 * @param ...       additional arguments such as integers or strings.
 */
void debug_log_error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	debug_log(LOG_LEVEL_ERROR, format, args);
	va_end(args);
}



