#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "I2CBus.h"

const std::vector<std::pair<uint8_t, char>> I2CBus::felModeSequence = {
    {0x4, 'f'},
    {0x5, 'b'},
    {0x6, '0'},
    {0x7, 0x0}
};

I2CBus::I2CBus()
{
}

/**
 * If the i2c bus file is still open at this point, it will be closed.
 */
I2CBus::~I2CBus()
{
    i2cClose();
}

/**
 * Read battery charge state from the i2c bus
 */
bool I2CBus::batteryIsCharging()
{
    i2cWriteByte(regCharging, 1);
    char isCharging = i2cReadByte(regCharging);
    return isCharging != '0';
}

/**
 * Read battery charge percentage from the i2c bus
 */
int I2CBus::batteryGaugePercent()
{
    i2cWriteByte(regBattGauge, 1);
    return i2cReadByte(regBattGauge);
}

/**
 * Writes a series of bytes to the i2c bus that will force the system to
 * enter Fel (Flashing) mode after the next restart.
 */
void I2CBus::enableFelMode()
{
    for (const std::pair<uint8_t, char>& regBytePair : felModeSequence)
    {
        i2cWriteByte(regBytePair.first, regBytePair.second);
    }
}

/**
 * Throw I2C exceptions when i2c bus access fails
 */
I2CBus::I2CException::I2CException(String errorMessage)
: errorMessage(errorMessage)
{
}

/**
 * @return a short message explaining the i2c bus access failure.
 */
String I2CBus::I2CException::getErrorMessage()
{
    return errorMessage;
}

/**
 * Opens access to the i2c bus file
 * @post i2cFileDescriptor will be set to a valid file descriptor, if
 * the file could be opened.
 * @throws I2CException if unable to access the i2c bus
 */
void I2CBus::i2cOpen()
{
    i2cFileDescriptor = open(i2cPath, O_RDWR);
    if (i2cFileDescriptor < 0)
    {
        throw I2CException("Failed to open i2c bus");
    }
    if (ioctl(i2cFileDescriptor, I2C_SLAVE_FORCE, 0x34) < 0)
    {
        throw I2CException("Failed to set slave address");
    }
}

/**
 * If the i2c bus file was opened, this will close it. Otherwise, nothing
 * will happen.
 */
void I2CBus::i2cClose()
{
    if (i2cFileDescriptor > 0)
    {
        close(i2cFileDescriptor);
        i2cFileDescriptor = -1;
    }
}

/**
 * Reads one byte from an i2c bus register.
 */
uint8_t I2CBus::i2cReadByte(uint8_t regAddr)
{
    if (i2cFileDescriptor < 0)
    {
        i2cOpen();
    }
    int data = i2c_smbus_read_byte_data(i2cFileDescriptor, regAddr);
    if (data < 0)
    {
        throw I2CException(String("Failed to read from register ")
                + String(regAddr));
    }
    return (uint8_t) data;
}

/**
 * Writes a byte to an i2c bus register.
 */
void I2CBus::i2cWriteByte(uint8_t regAddr, uint8_t byte)
{
    if (i2cFileDescriptor < 0)
    {
        i2cOpen();
    }
    uint8_t res = i2c_smbus_write_byte_data(i2cFileDescriptor, regAddr, byte);
    if (res < 0)
    {
        throw I2CException(String("Failed to write to register ")
                + String(regAddr));
    }
}
