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
        gaugeFile,  //read from gauge file
        voltageFile,//read from voltage file
        i2cBus,     //directly query i2c bus
        noBattery   //no battery, or battery monitoring isn't supported
    };
    DataSource dataSource;
};