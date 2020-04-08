/**
 * common_io_helpers.c - Helper library to interact with common-io
 *
 * Include this header file to use the component.
 *
 * (C) 2019 - Timothee Cruse <timothee.cruse@gmail.com>
 * This code is licensed under the MIT License.
 */

#include "common_io_helpers.h"

#define LIBRARY_LOG_LEVEL IOT_LOG_INFO
#define LIBRARY_LOG_NAME  "common-io-helpers"
#include "iot_logging_setup.h"

#define COMMON_IO_CHECK_HANDLER( handler ) if ( handler == NULL ) { return COMMON_IO_FAIL; }

/*-----------------------------------------------------------*/

common_io_err_t eReadI2CBytes( IotI2CHandle_t const pxI2CPeripheral, uint8_t address, uint8_t * values, uint8_t length )
{
    // Return value of I2C functions.
    common_io_err_t res = COMMON_IO_FAIL;

    COMMON_IO_CHECK_HANDLER( pxI2CPeripheral );
 
    // Number of read/write bytes.
    uint16_t usReadBytes = 0;
    uint16_t usWriteBytes = 0;

    // Set slave address.
    IotLogDebug( "eReadI2CBytes: Set slave address to: %#04x", address );
    res = iot_i2c_ioctl( pxI2CPeripheral, eI2CSetSlaveAddr, &address );
    if ( res != IOT_I2C_SUCCESS )
    {
        IotLogError( "eReadI2CBytes: Setting the slave address for %#04x on I2C failed.", address );
        return COMMON_IO_FAIL;
    }
    // assert(lRetVal == IOT_I2C_SUCCESS);

    // Write the register address as single byte, in a transaction.
    uint8_t reg = values[ 0 ];
    IotLogDebug( "eReadI2CBytes: Write the start register: %#04x", reg );
    res = iot_i2c_write_sync( pxI2CPeripheral, &reg, 1 );
    if ( res != IOT_I2C_SUCCESS )
    {
        IotLogError( "eReadI2CBytes: Writing %#04x on I2C failed.", reg );
        return COMMON_IO_FAIL;
    }

    // Get the number of written bytes in last transaction.
    res = iot_i2c_ioctl( pxI2CPeripheral, eI2CGetTxNoOfbytes, &usWriteBytes );
    if ( res != IOT_I2C_SUCCESS || usWriteBytes != 1 ) 
    {
        IotLogError( "eReadI2CBytes: Failed to check the number of written bytes %u vs. %u.", usWriteBytes, 1 );
        return COMMON_IO_FAIL;
    }
    IotLogDebug( "eReadI2CBytes: Wrote %u bytes.", usWriteBytes );

    // Read length bytes of data to allocated buffer, in a transaction.
    res = iot_i2c_read_sync( pxI2CPeripheral, values, length );
    if ( res != IOT_I2C_SUCCESS )
    {
        IotLogError( "eReadI2CBytes: Failed to Read %u bytes at %#04x on I2C.", length, reg );
        return COMMON_IO_FAIL;
    }

    // Get the number of read bytes in last transaction.
    res = iot_i2c_ioctl( pxI2CPeripheral, eI2CGetRxNoOfbytes, &usReadBytes ); 
    if ( res != IOT_I2C_SUCCESS || usReadBytes != length ) 
    {
        IotLogError( "eReadI2CBytes: Failed to check the number of read bytes %u vs. %u.", usReadBytes, length );
        return COMMON_IO_FAIL;
    }
    IotLogDebug( "eReadI2CBytes: Read %u bytes @ %#04x", length, reg );

    return COMMON_IO_SUCCESS;
}

/*-----------------------------------------------------------*/

common_io_err_t eWriteI2CBytes( IotI2CHandle_t const pxI2CPeripheral, uint8_t address, uint8_t * values, uint8_t length )
{
    // Return value of I2C functions.
    common_io_err_t res = COMMON_IO_FAIL;

    COMMON_IO_CHECK_HANDLER( pxI2CPeripheral );
 
    // Number of write bytes.
    uint16_t usWriteBytes = 0;

    // Set slave address.
    IotLogDebug( "eWriteI2CBytes: Set slave address to: %#04x", address );
    res = iot_i2c_ioctl( pxI2CPeripheral, eI2CSetSlaveAddr, &address );
    if ( res != IOT_I2C_SUCCESS )
    {
        IotLogError( "eWriteI2CBytes: Setting the slave address for %#04x on I2C failed.", address );
        return COMMON_IO_FAIL;
    }
    // assert(lRetVal == IOT_I2C_SUCCESS);

    // Write the register address as single byte, in a transaction.
    IotLogDebug( "eWriteI2CBytes: Write the start register: %#04x and length: %u", values[ 0 ], length );
    res = iot_i2c_write_sync( pxI2CPeripheral, values, length );
    if ( res != IOT_I2C_SUCCESS )
    {
        IotLogError( "eWriteI2CBytes: Writing %#04x on I2C failed.", values[ 0 ] );
        return COMMON_IO_FAIL;
    }

    // Get the number of written bytes in last transaction.
    res = iot_i2c_ioctl( pxI2CPeripheral, eI2CGetTxNoOfbytes, &usWriteBytes );
    if ( res != IOT_I2C_SUCCESS || usWriteBytes != length ) 
    {
        IotLogError( "eWriteI2CBytes: Failed to check the number of written bytes %u vs. %u.", usWriteBytes, length );
        return COMMON_IO_FAIL;
    }
    IotLogDebug( "eWriteI2CBytes: Wrote %u bytes.", usWriteBytes );

    return COMMON_IO_SUCCESS;
}