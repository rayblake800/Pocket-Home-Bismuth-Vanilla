#include "I2CBus.h"
#include "Utils.h"
#include "BatteryMonitor.h"

BatteryMonitor::BatteryMonitor()
{
    //Battery files are assumed valid if they exist and were updated in the
    //last 10 minutes.
    std::function<bool(const char*) > validFile = []
            (const char* filename)
    {
        File testFile(filename);
        return testFile.existsAsFile() &&
                testFile.getLastModificationTime() + RelativeTime(600)
                > Time::getCurrentTime();
    };
    if (validFile(chargingPath))
    {
        if (validFile(gaugePath))
        {
            dataSource = gaugeFile;
            DBG("BatteryMonitor::" << __func__
                    << ": data source set to gauge file.");
            return;
        }
        else if (validFile(voltagePath))
        {
            dataSource = voltageFile;
            DBG("BatteryMonitor::" << __func__
                    << ": data source set to voltage file.");
            return;
        }
    }
    DBG("BatteryMonitor::" << __func__
            << ": data source set to i2c bus.");
    dataSource = i2cBus;
}

BatteryMonitor::~BatteryMonitor() { }

/**
 * Find the current battery charge percentage and charging state
 */
BatteryMonitor::BatteryStatus BatteryMonitor::getBatteryStatus()
{
    BatteryStatus currentStatus;
    currentStatus.percent = -1;
    if (dataSource == noBattery)
    {
        return currentStatus;
    }
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
            DBG("BatteryMonitor::" << __func__
                    << ": I2C bus access failed, disabling battery monitoring.");
            dataSource = noBattery;
        }
        return currentStatus;

    }
    else
    {
        String chargeFile = File(chargingPath).loadFileAsString().substring(0, 1);
        if (chargeFile.isNotEmpty())
        {
            currentStatus.isCharging = (chargeFile == "1");
        }
        else
        {
            DBG("BatteryMonitor::" << __func__ 
                    << ": Failed to read charging file");
        }
        if (dataSource == voltageFile)
        {
            int voltage = File(voltagePath).loadFileAsString().getIntValue();
            int voltageRange = maxVoltage - minVoltage;
            int voltageFraction = voltage - minVoltage;
            currentStatus.percent = voltageFraction * 100 / voltageRange;

        }
        else
        {//dataSource is gauge file
            currentStatus.percent =
                    File(gaugePath).loadFileAsString().getIntValue();
        }
    }
    currentStatus.percent = median<int>(0,currentStatus.percent,100);
    return currentStatus;
}
