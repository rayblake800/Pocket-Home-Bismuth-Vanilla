/**
 * @file BatteryMonitor.h
 * 
 * BatteryMonitor finds and shares the current charge percentage and charging
 * state of the PocketCHIP battery.
 */
#pragma once
#include "../../../JuceLibraryCode/JuceHeader.h"

class BatteryMonitor {
public:
    BatteryMonitor();
    ~BatteryMonitor();
    
    /**
     * Stores battery charge percentage and if the battery is currently
     *  charging.
     */
    struct BatteryStatus{
        int percent=0;
        bool isCharging=0;
    };

    /**
     * Find the current battery charge percentage and charging state
     * @return an integer between 0 and 100 representing battery charge state,
     * and a boolean indicating if the battery is charging.  If battery
     * information can't be found, percent is set to -1.
     */
    BatteryStatus getBatteryStatus();
private:
    
    /**
     * If this doesn't exist, then the program isn't running on PocketCHIP
     * or any similar device, and reading battery status isn't supported.
     */
    static constexpr const char* i2cPath = "/dev/i2c-0";
    
    /**
     * If these files exist and are being actively updated, they will be used
     * as a source of battery info.  Otherwise the i2c-bus will need to be
     * directly queried.
     * This is the most accurate source, but is only available when using the 
     * custom battery script at https://github.com/aleh/pocketchip-batt
     */
    static constexpr const char* gaugePath = "/usr/lib/pocketchip-batt/gauge";
    
    /**
     * These two files will be available as long as some form of pocketchip-batt
     * is running, although the voltage file isn't needed if the gauge file
     * exists.
     */
    static constexpr const char* voltagePath = "/usr/lib/pocketchip-batt/voltage";  
    static constexpr const char* chargingPath = "/usr/lib/pocketchip-batt/charging";
    
    
    /**
     * If the voltage file is being used for battery percentage, these values 
     * will be needed to calculate the battery percentage from the voltage data.
     */
    static constexpr const int maxVoltage=4250;
    static constexpr const int minVoltage=3275;
    
    //The most appropriate data source will be set by the constructor.
    enum DataSource{
        gaugeFile,
        voltageFile,
        i2cBus,
        noBattery
    };
    DataSource dataSource;
    
    //i2c bus access methods:
    //i2c access will be needed iff there's no battery script updating files
    
    /**
     * Opens access to battery registers on the i2c bus. 
     * Don't forget to close the file when finished.
     * @return the file descriptor int for the i2c bus, or -1 if the operation
     * failed.
     */
    int i2cBatteryOpen();
    
    /**
     * Copies data from i2c bus registers into a buffer.
     * @param i2cFileDesc a file descriptor for the i2c bus returned by i2cOpen 
     * @param regAddr the address of the register to access
     * @param buf the address to copy data from the register
     * @param length number of bytes to copy
     * @return true iff reading data succeeded.
     */
    bool i2cRead(int i2cFileDesc,uint8_t regAddr,uint8_t * buf, int length);
};