/*
 * debug_log.h
 *
 *  Created on: Oct 6, 2024
 *      Author: jduda
 */
#include "main.h"

#ifndef CORELIBRARY_DEBUG_LOG_DEBUG_LOG_H_
#define CORELIBRARY_DEBUG_LOG_DEBUG_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Log level used to set message level.
 */
typedef enum {
	LOG_LEVEL_INFO = 0,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,
}log_level_type;


HAL_StatusTypeDef debug_log_init(UART_HandleTypeDef *huart, log_level_type level);
void debug_log(log_level_type level, const char *format, ...);
void debug_log_info(const char *format, ...);
void debug_log_info(const char *format, ...);
void debug_log_debug(const char *format, ...);
void debug_log_warning(const char *format, ...);
void debug_log_error(const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif /* CORELIBRARY_DEBUG_LOG_DEBUG_LOG_H_ */
