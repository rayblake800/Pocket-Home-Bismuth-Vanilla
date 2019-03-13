#include "Hardware_I2CBus.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

/* Path to the I2C bus device file: */ 
static constexpr const char* i2cPath = "/dev/i2c-0";

/* Charge state register: */
static constexpr const uint8_t chargingRegister = 0x01;

/* Battery percent gauge register: */
static constexpr const uint8_t gaugeRegister = 0xB9;

/*  FEL mode sequence stored as <register,byte> pairs:
 * When each byte is written to its corresponding register, the PocketCHIP 
 * system will enter fel/flashing mode on the next reboot. */
static const std::vector<std::pair<uint8_t, char>> felModeSequence= 
{
    {0x4, 'f'},
    {0x5, 'b'},
    {0x6, '0'},
    {0x7, 0x0}
};

#ifdef __x86_64
/* Provide fake versions of I2C I/O functions on systems where they're not
 * defined. */
int i2c_smbus_read_byte_data(int a, int b) { return 0; }
int i2c_smbus_write_byte_data(int a, int b, int c) { return 0; }
#endif

/*
 * Ensures the connection to the I2C bus file is closed when the object is 
 * destroyed.
 */
Hardware::I2CBus::~I2CBus()
{
    closeBus();
}

/*
 * Reads the battery charge state from the I2C bus.
 */
bool Hardware::I2CBus::batteryIsCharging()
{
    writeByte(chargingRegister, 1);
    char isCharging = readByte(chargingRegister);
    return isCharging != '0';
}

/*
 * Reads the battery charge percentage from the I2C bus.
 */
int Hardware::I2CBus::batteryGaugePercent()
{
    writeByte(gaugeRegister, 1);
    return readByte(gaugeRegister);
}

/*
 * Writes a series of bytes to the i2c bus that will force the system to enter 
 * Fel (Flashing) mode after the next restart.
 */
void Hardware::I2CBus::enableFelMode()
{
    for(const std::pair<uint8_t, char>& regBytePair : felModeSequence)
    {
        writeByte(regBytePair.first, regBytePair.second);
    }
}

/*
 * If the I2C bus file was opened, this will close it. Otherwise, nothing will 
 * happen.
 */
void Hardware::I2CBus::closeBus()
{
    if (busFileDescriptor > 0)
    {
        close(busFileDescriptor);
        busFileDescriptor = -1;
    }
}

/*
 * Throw I2C exceptions when I2C bus access fails.
 */
Hardware::I2CBus::I2CException::I2CException(const juce::String errorMessage) : 
errorMessage(errorMessage) { }

/*
 * Gets the exception's stored error message.
 */
juce::String Hardware::I2CBus::I2CException::getErrorMessage()
{
    return errorMessage;
}

/*
 * Opens access to the I2C bus file.
 */
void Hardware::I2CBus::openBus()
{
    busFileDescriptor = open(i2cPath, O_RDWR);
    if(busFileDescriptor < 0)
    {
        throw I2CException("Failed to open i2c bus");
    }
    if(ioctl(busFileDescriptor, I2C_SLAVE_FORCE, 0x34) < 0)
    {
        throw I2CException("Failed to set slave address");
    }
}

/*
 * Reads one byte from an i2c bus register.
 */
uint8_t Hardware::I2CBus::readByte(const uint8_t address)
{
    using juce::String;
    if(busFileDescriptor < 0)
    {
        openBus();
    }
    int data = i2c_smbus_read_byte_data(busFileDescriptor, address);
    if(data < 0)
    {
        throw I2CException(String("Failed to read from register ")
                + String(address));
    }
    return (uint8_t) data;
}

/*
 * Writes a byte to an I2C bus register.
 */
void Hardware::I2CBus::writeByte(const uint8_t address, const uint8_t byte)
{
    using juce::String;
    if (busFileDescriptor < 0)
    {
        openBus();
    }
    int result = i2c_smbus_write_byte_data(busFileDescriptor, address, byte);
    if (result < 0)
    {
        throw I2CException(String("Failed to write to register ")
                + String(address));
    }
}
