#ifndef CLOCKMONITOR_H
#define CLOCKMONITOR_H

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Configuration/Configurables/ConfigurableLabel.h"


using namespace juce;

class ClockMonitor : public Thread, public Configurable {
public:
    ClockMonitor();
    ~ClockMonitor();

    void run() override;
    ConfigurableLabel& getLabel();
protected:
    /**
     * Receives notification whenever clock configuration values change
     * @param config the main config file
     * @param key either the key for the clock visibility setting, or the key 
     * for the 12h/24h mode toggle. 
     */
    void loadConfigProperties(ConfigFile * config,String key);
private:
    bool use24HrMode;
    ConfigurableLabel clockLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClockMonitor)
};

#endif
