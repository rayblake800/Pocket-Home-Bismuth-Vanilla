#include "Hardware_Battery.h"
#include "Util_Math.h"
#ifdef CHIP_FEATURES
#include "Hardware_I2CBus.h"
#endif

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Hardware::Battery::";
#endif

#ifdef CHIP_FEATURES
/* Battery status files:
 *  These files are created and actively updated by the pocketCHIP's battery 
 * monitor program, pocketchipp-batt. If these files exist and are being 
 * actively updated, they will be used as a source of battery info. Otherwise 
 * the I2C-bus will need to be directly queried. */

/* Holds the battery percentage as reported by the I2C bus battery gauge. You'd
 * think this would be the most accurate measurement, but it often varies wildly
 * from moment to moment. This file source is only available when using the 
 * PocketCHIP battery script from https://github.com/aleh/pocketchip-batt */
static constexpr const char* gaugePath = "/usr/lib/pocketchip-batt/gauge";

/* Holds the battery voltage level, which can be used to calculate battery
 * percentage. */
static constexpr const char* voltagePath = "/usr/lib/pocketchip-batt/voltage";

/* Holds 1 if the battery is charging, 0 if it is not charging. */
static constexpr const char* chargingPath = "/usr/lib/pocketchip-batt/charging";

/* Longest amount of time since last modification time (in seconds) before a
 * battery gauge or voltage file is judged an invalid data source: */
static const constexpr int validFilePeriod = 600;

/*
 * If the voltage file is being used for battery percentage, these values 
 * will be needed to calculate the battery percentage from the voltage data.
 */
static constexpr const int maxVoltage = 4250;
static constexpr const int minVoltage = 3275;
#endif

/*
 * Determines the most appropriate way to monitor battery state on construction.
 */
Hardware::Battery::Battery()
{
    // TODO: Provide battery checking command accessed through Util::Commands,
    //       and a way of determining whether that command should be used.
#ifdef CHIP_FEATURES
    /* Checks if a battery file was modified recently enough for it to qualify
     * as a valid data source. */
    const std::function<bool(const char*)> validFile = [](const char* filename)
    {
        const juce::File testFile(filename);
        const juce::Time oldestValidTime = juce::Time::getCurrentTime()
                - juce::RelativeTime(validFilePeriod);
        return testFile.existsAsFile() &&
                testFile.getLastModificationTime() >= oldestValidTime;
    };
    if (validFile(chargingPath))
    {
        if (validFile(gaugePath))
        {
            dataSource = gaugeFile;
            DBG(dbgPrefix << __func__ << ": data source set to gauge file.");
            return;
        }
        else if (validFile(voltagePath))
        {
            dataSource = voltageFile;
            DBG(dbgPrefix << __func__ << ": data source set to voltage file.");
            return;
        }
    }
    else //Check if valid i2cBus data is available:
    {
        try
        {
            I2CBus i2c;
            i2c.batteryGaugePercent();
            DBG(dbgPrefix << __func__ << ": data source set to i2c bus.");
            dataSource = i2cBus;
        }
        catch (I2CBus::I2CException e)
        {
            DBG(dbgPrefix << __func__ << ": no available battery source.");
            dataSource = noBattery;
        }
    }
#else
    dataSource = noBattery;
#endif
}

/*
 * Checks if the Battery object is able to detect and read battery information.
 */
bool Hardware::Battery::isBatteryAvailable() const
{
    return dataSource != noBattery;
}

/*
 * Find the current battery charge percentage and charging state
 */
Hardware::Battery::Status Hardware::Battery::getBatteryStatus()
{
    Status currentStatus;
    currentStatus.percent = -1;
    if (dataSource == noBattery)
    {
        return currentStatus;
    }
#ifdef CHIP_FEATURES
    if (dataSource == i2cBus)
    {
        try
        {
            I2CBus i2c;
            currentStatus.isCharging = i2c.batteryIsCharging();
            currentStatus.percent = i2c.batteryGaugePercent();
        }
        catch (I2CBus::I2CException e)
        {
            DBG(e.getErrorMessage());
            DBG(dbgPrefix << __func__ << ": I2C bus access failed, disabling "
                    << "battery monitoring.");
            dataSource = noBattery;
        }
        return currentStatus;
    }
    else
    {
        juce::String chargeFile 
                = juce::File(chargingPath).loadFileAsString().substring(0, 1);
        if (chargeFile.isNotEmpty())
        {
            currentStatus.isCharging = (chargeFile == "1");
        }
        else
        {
            DBG(dbgPrefix << __func__ << ": Failed to read charging file");
        }
        if (dataSource == voltageFile)
        {
            int voltage = juce::File(voltagePath).loadFileAsString()
                    .getIntValue();
            int voltageRange = maxVoltage - minVoltage;
            int voltageFraction = voltage - minVoltage;
            currentStatus.percent = voltageFraction * 100 / voltageRange;
        }
        else // data source is the gauge file:
        {
            currentStatus.percent 
                    = juce::File(gaugePath).loadFileAsString().getIntValue();
        }
    }
#endif
    currentStatus.percent = Util::Math::median<int>(0, currentStatus.percent,
            100);
    return currentStatus;
}
