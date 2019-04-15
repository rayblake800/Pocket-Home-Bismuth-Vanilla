#pragma once
/**
 * @file  Hardware_I2CBus.h
 *
 * @brief  Accesses the i2c bus on PocketCHIP to read information on battery
 *         state or put the system in FEL mode.
 */

#include <exception>
#include "JuceHeader.h"

namespace Hardware { class I2CBus; }


/**
 * @brief  Reads and writes information over a connection to the system's I2C
 * bus.
 *
 *  This class is designed to access the PocketCHIP's I2C bus. It will not be
 * functional on any other type of system.
 */
class Hardware::I2CBus
{
public:
    I2CBus() { }

    /**
     * @brief  Ensures the connection to the I2C bus file is closed when the
     *         object is destroyed.
     */
    virtual ~I2CBus();

    /**
     * @brief  Reads the battery charge state from the I2C bus.
     *
     * @throws I2CException  If unable to access the I2C bus.
     *
     * @return               Whether the battery is currently charging.
     */
    bool batteryIsCharging();

    /**
     * @brief  Reads the battery charge percentage from the i2c bus.
     *
     * @throws I2CException  If unable to access the I2C bus.
     *
     * @return               The percent of battery life remaining, between
     *                       zero and 100.
     */
    int batteryGaugePercent();

    /**
     * @brief  Writes a series of bytes to the I2C bus that will force the
     *         system to enter Fel(Flashing) mode after the next restart.
     *
     * @throws I2CException  If unable to access the I2C bus.
     */
    void enableFelMode();

    /**
     * @brief  If the I2C bus file was opened, this will close it. Otherwise,
     *         nothing will happen.
     */
    void closeBus();

    /**
     * @brief  I2CException is thrown whenever accessing the I2C bus fails.
     */
    struct I2CException : public std::exception
    {
    public:

        /**
         * @brief  Throw I2C exceptions when I2C bus access fails.
         *
         * @param errorMessage  A short error message describing the failure.
         */
        I2CException(const juce::String errorMessage);

        /**
         * @brief  Gets the exception's stored error message.
         *
         * @return  A short message explaining the I2C bus access failure.
         */
        juce::String getErrorMessage();

    private:
        // The I2C failure error message:
        juce::String errorMessage;
    };

private:
    /**
     * @brief  Opens access to the I2C bus file.
     *
     * @throws I2CException  If unable to access the i2c bus.
     */
    void openBus();

    /**
     * @brief  Reads one byte from an I2C bus register.
     *
     * @param address        The address of the register to access.
     *
     * @throws I2CException  If unable to access the I2C bus.
     *
     * @return               The byte read from the register.
     */
    uint8_t readByte(const uint8_t address);

    /**
     * @brief  Writes a byte to an I2C bus register.
     *
     * @param address        The address of the register to access.
     *
     * @param byte           The byte to write to the register.
     *
     * @throws I2CException  If unable to access the I2C bus.
     */
    void writeByte(const uint8_t address, const uint8_t byte);

    // Stores the file descriptor for an open I2C bus file, or -1 if the file
    // is not open:
    int busFileDescriptor = -1;
};
