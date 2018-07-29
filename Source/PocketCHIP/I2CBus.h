#pragma once
#include <exception>
#include "JuceHeader.h"

/**
 * @file I2CBus.h
 * 
 * @brief Accesses the i2c bus on PocketCHIP to read information on battery 
 *        state or put the system in FEL mode.  
 */

class I2CBus
{
public:

    I2CBus() { }

    /**
     * If the i2c bus file is still open at this point, it will be closed.
     */
    virtual ~I2CBus();

    /**
     * Reads the battery charge state from the i2c bus.
     * 
     * @return true iff the battery is currently charging.
     * 
     * @throws I2CException if unable to access the i2c bus
     */
    bool batteryIsCharging();

    /**
     * Reads the battery charge percentage from the i2c bus.
     * 
     * @return the percent of battery life remaining, between 0-100.
     * 
     * @throws I2CException if unable to access the i2c bus.
     */
    int batteryGaugePercent();

    /**
     * Writes a series of bytes to the i2c bus that will force the system to
     * enter Fel (Flashing) mode after the next restart.
     * 
     * @throws I2CException if unable to access the i2c bus.
     */
    void enableFelMode();

    /**
     * If the i2c bus file was opened, this will close it. Otherwise, nothing
     * will happen.
     */
    void i2cClose();

    /**
     * I2CException is thrown whenever accessing the i2c bus fails.
     */
    struct I2CException : public std::exception
    {
    public:

        /**
         * Throw I2C exceptions when i2c bus access fails
         * @param errorMessage a short error message describing the failure.
         */
        I2CException(juce::String errorMessage);

        /**
         * @return a short message explaining the i2c bus access failure.
         */
        juce::String getErrorMessage();
    private:

        juce::String errorMessage;
    };

private:

    /**
     * Opens access to the i2c bus file
     * 
     * @post i2cFileDescriptor will be set to a valid file descriptor, if
     *       the file could be opened.
     * 
     * @throws I2CException if unable to access the i2c bus
     */
    void i2cOpen();

    /**
     * Reads one byte from an i2c bus register.
     * 
     * @param regAddr  The address of the register to access.
     * 
     * @param buf      A buffer to hold data copied from the register.
     * 
     * @return         The byte read from the register.
     * 
     * @throws         I2CException if unable to access the i2c bus
     */
    uint8_t i2cReadByte(uint8_t regAddr);

    /**
     * Writes a byte to an i2c bus register.
     * 
     * @param regAddr The address of the register to access.
     * 
     * @param byte    The byte to write to the register.
     * 
     * @throws        I2CException if unable to access the i2c bus.
     */
    void i2cWriteByte(uint8_t regAddr, uint8_t byte);

    int i2cFileDescriptor = -1;

    //Path to the i2c bus device file. 
    static constexpr const char* i2cPath = "/dev/i2c-0";
    //charge state register
    static constexpr const uint8_t regCharging = 0x01;
    //battery percent gauge register
    static constexpr const uint8_t regBattGauge = 0xB9;

    /**
     * a series of <register,byte> pairs.
     * When each byte is written to its corresponding register, the PocketCHIP 
     * system will enter fel/flashing mode on the next reboot.
     */
    static const std::vector<std::pair<uint8_t, char>> felModeSequence;

};