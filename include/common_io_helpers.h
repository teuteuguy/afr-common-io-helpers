/**
 * common_io_helpers.h - Helper library to interact with common-io
 *
 * Include this header file to use the component.
 *
 * (C) 2019 - Timothee Cruse <timothee.cruse@gmail.com>
 * This code is licensed under the MIT License.
 */

#ifndef _COMMON_IO_HELPERS_H_
#define _COMMON_IO_HELPERS_H_

#include "freertos/FreeRTOS.h"
#include "iot_i2c.h"

typedef int32_t common_io_err_t;

/**
 * @brief   DHT12 FAILURE CODES
 */
#define COMMON_IO_SUCCESS                    ( 0 )     /** Operation completed successfully. */
#define COMMON_IO_FAIL                       ( 1 )     /** Operation failed. */
#define COMMON_IO_CHECKSUM_ERROR             ( 2 )     /** Operation failed. */
#define COMMON_IO_OTHER_ERROR                ( 3 )     /** Other error. */

/**
 * @brief   Read an array of bytes from an I2C slave
 * 
 * @param   I2CHandle:  external I2C handle from Common IO I2C init
 * @param   address:    I2C slave device address (not the register address to read from)
 * @param   value:      external array to hold data. Put starting register in values[0]
 * @param   length:     number of bytes to read
 * @return  COMMON_IO_SUCCESS success
 *          COMMON_IO_FAIL errors found
*/
common_io_err_t eReadI2CBytes( IotI2CHandle_t I2CHandle, uint8_t address, uint8_t * values, uint8_t length );
			
/**
 * @brief   Write a number of bytes to an I2C slave
 * 
 * @param   I2CHandle:  external I2C handle from Common IO I2C init
 * @param   address:    I2C slave device address (not the register address to read from)
 * @param   value:      external array of data to write. Put starting register in values[0]
 * @param   length:     number of bytes to write
 * @return  COMMON_IO_SUCCESS success
 *          COMMON_IO_FAIL errors found
*/
common_io_err_t eWriteI2CBytes(  IotI2CHandle_t I2CHandle, uint8_t address, uint8_t * values, uint8_t length );

#define LIBRARY_LOG_LEVEL IOT_LOG_INFO
#define LIBRARY_LOG_NAME  "common-io-helpers"
#include "iot_logging_setup.h"

#endif // _COMMON_IO_HELPERS_H_
