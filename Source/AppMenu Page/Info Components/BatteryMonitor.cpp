#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "../../Utils.h"
#include "BatteryMonitor.h"

BatteryMonitor::BatteryMonitor()
{
    if (!File(i2cPath).existsAsFile())
    {
        DBG("BatteryMonitor: data source set to none.");
        dataSource = noBattery;
        return;
    }
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
            DBG("BatteryMonitor: data source set to gauge file.");
            return;
        } else if (validFile(voltagePath))
        {
            dataSource = voltageFile;
            DBG("BatteryMonitor: data source set to voltage file.");
            return;
        }
    }
    DBG("BatteryMonitor: data source set to i2c bus.");
    dataSource = i2cBus;
}

BatteryMonitor::~BatteryMonitor()
{
}

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
        int file_i2c = i2cBatteryOpen();
        if (file_i2c < 0)
        {
            DBG("BatteryMonitor::getBatteryStatus: Failed to open i2c bus");
            return currentStatus;
        }
        uint8_t buf = 0;
        //Read the charging status
        if (!i2cRead(file_i2c, 0x01, &buf, 1))
        {
            DBG("BatteryMonitor::getBatteryStatus: Failed to read charging status");
            return currentStatus;
        }
        currentStatus.isCharging = ((char) buf == '1');

        //Read the battery percentage  
        if (!i2cRead(file_i2c, 0xB9, &buf, 1))
        {
            DBG("BatteryMonitor::getBatteryStatus: Failed to read battery percentage");
            return currentStatus;
        }
        currentStatus.percent = buf;
        close(file_i2c);

    } else
    {
        MemoryBlock chargeFile(1, true);
        if (File(chargingPath).loadFileAsData(chargeFile))
        {
            DBG(String("Charge Status <")+String(chargeFile[0])+String(">"));
            currentStatus.isCharging = chargeFile[0] == '1';
        } else
        {
            DBG("BatteryMonitor::getBatteryStatus: Failed to read charging file");
        }
        if (dataSource == voltageFile)
        {
            int voltage = File(voltagePath).loadFileAsString().getIntValue();
            int voltageRange = maxVoltage - minVoltage;
            int voltageFraction = voltage - minVoltage;
            currentStatus.percent = voltageFraction * 100 / voltageRange;

        } else
        {//dataSource is gauge file
            currentStatus.percent =
                    File(gaugePath).loadFileAsString().getIntValue();
        }
    }
    return currentStatus;
}

/**
 * Opens access to the i2c bus. 
 * Don't forget to close the file when finished.
 */
int BatteryMonitor::i2cBatteryOpen()
{
    int file_i2c = open("/dev/i2c-0", O_RDWR);
    if (file_i2c < 0)
    {
        DBG("BatteryMonitor::i2cOpen: Failed to open i2c bus");
        return -1;
    }

    if (ioctl(file_i2c, I2C_SLAVE_FORCE, 0x34) < 0)
    {
        DBG("BatteryMonitor::i2cOpen: Failed to set slave address");
        return -1;
    }
    return file_i2c;
}

/**
 * Copies data from i2c bus registers into a buffer.
 */
bool BatteryMonitor::i2cRead(int i2cFileDesc,
        uint8_t regAddr, uint8_t * buf, int length)
{
    if (write(i2cFileDesc, &regAddr, 1) != 1)
    {
        return false;
    }
    if (read(i2cFileDesc, buf, length) != length)
    {
        return false;
    }
    return true;
}
