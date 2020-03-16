/**
 * common_io_helpers.c - Helper library to interact with common-io
 *
 * Include this header file to use the component.
 *
 * (C) 2019 - Timothee Cruse <timothee.cruse@gmail.com>
 * This code is licensed under the MIT License.
 */

#include "common_io_helpers.h"

#define COMMON_IO_CHECK_HANDLER( handler ) if ( handler == NULL ) { return COMMON_IO_FAIL; }

/*-----------------------------------------------------------*/

common_io_err_t readBytes( IotI2CHandle_t I2CHandle, uint8_t address, uint8_t * values, uint8_t length )
{
    // Return value of I2C functions.
    bmp280_err_t res = COMMON_IO_FAIL;

    COMMON_IO_CHECK_HANDLER( I2CHandle );

    // Register address on I2C slave device.
    uint16_t ioCtlBuffer = address;
 
    // Number of read/write bytes.
    uint16_t usReadBytes = 0;
    uint16_t usWriteBytes = 0;

    uint8_t address = values[0];

    // Set slave address.
    res = iot_i2c_ioctl( I2CHandle, eI2CSetSlaveAddr, &ioCtlBuffer );
    if ( res != IOT_I2C_SUCCESS )
    {
        IotLogError( "Setting the slave address for %#04X on I2C failed.", ioCtlBuffer );
        return COMMON_IO_FAIL;
    }
    // assert(lRetVal == IOT_I2C_SUCCESS);

    // Write the register address as single byte, in a transaction.
    res = iot_i2c_write_sync( I2CHandle, values, 1 );
    if ( res == IOT_I2C_SUCCESS )
    {
        // Get the number of written bytes in last transaction.
        res = iot_i2c_ioctl( I2CHandle, eI2CGetTxNoOfbytes, &usWriteBytes );
        if ( res != IOT_I2C_SUCCESS || usWriteBytes != 1 ) 
        {
            IotLogError( "Failed to check the number of written bytes %u vs. %u.", usWriteBytes, 1 );
            return COMMON_IO_FAIL;
        }

        // Read length bytes of data to allocated buffer, in a transaction.
        res = iot_i2c_read_sync( I2CHandle, values, length );
        if ( res == IOT_I2C_SUCCESS )
        {
            // Get the number of read bytes in last transaction.
            res = iot_i2c_ioctl( I2CHandle, eI2CGetRxNoOfbytes, &usReadBytes ); 
            if ( res != IOT_I2C_SUCCESS || usReadBytes != length ) 
            {
                IotLogError( "Failed to check the number of read bytes %u vs. %u.", usReadBytes, length );
                return COMMON_IO_FAIL;
            }

            IotLogDebug( "Read %u bytes @ %#04X", length, address );

            return COMMON_IO_SUCCESS;
        }
    }
    else
    {
        IotLogError( "Writing %#04X on I2C failed.", address );
    }
    
    return COMMON_IO_FAIL; 
}

/*-----------------------------------------------------------*/

common_io_err_t writeBytes(  IotI2CHandle_t I2CHandle, uint8_t address, uint8_t * values, uint8_t length )
{
    // Return value of I2C functions.
    bmp280_err_t res = COMMON_IO_FAIL;

    COMMON_IO_CHECK_HANDLER( I2CHandle );

    // Register address on I2C slave device.
    uint16_t ioCtlBuffer = address;
 
    // Number of read/write bytes.
    uint16_t usReadBytes = 0;
    uint16_t usWriteBytes = 0;

    uint8_t address = values[0];

    // Set slave address.
    res = iot_i2c_ioctl( I2CHandle, eI2CSetSlaveAddr, &ioCtlBuffer );
    if ( res != IOT_I2C_SUCCESS )
    {
        IotLogError( "Setting the slave address for %#04X on I2C failed.", ioCtlBuffer );
        return COMMON_IO_FAIL;
    }
    // assert(lRetVal == IOT_I2C_SUCCESS);

    // Write the register address as single byte, in a transaction.
    res = iot_i2c_write_sync( I2CHandle, values, length );
    if ( res == IOT_I2C_SUCCESS )
    {
        // Get the number of written bytes in last transaction.
        res = iot_i2c_ioctl( I2CHandle, eI2CGetTxNoOfbytes, &usWriteBytes );
        if ( res != IOT_I2C_SUCCESS || usWriteBytes != length )
        {
            IotLogError( "Failed to check the number of written bytes %u vs. %u.", usWriteBytes, 1 );
            return COMMON_IO_FAIL;
        }

        IotLogDebug( "Wrote %u bytes @ %#04X", length, address );

        return COMMON_IO_SUCCESS;
    }
    else
    {
        IotLogError( "Writing %u bytes on I2C failed.", length );
    }
    
    return COMMON_IO_FAIL; 
}