#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Configuration/Configurable.h"

#ifndef CLOCKMONITOR_H
#define CLOCKMONITOR_H
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>

using namespace juce;

class ClockMonitor : public Thread, public Configurable {
public:
    ClockMonitor();
    ~ClockMonitor();

    void run() override;
    Label& getLabel();
protected:
    /**
     * Receives notification whenever clock configuration values change
     * @param config the main config file
     * @param key either the key for the clock visibility setting, or the key 
     * for the 12h/24h mode toggle. 
     */
    void loadConfigProperties(ConfigFile * config,String key);
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClockMonitor)
    char formatted[10];
    bool use24HrMode;
    ScopedPointer<Label> clock;
};

#endif
