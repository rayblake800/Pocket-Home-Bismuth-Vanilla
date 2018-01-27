#pragma once

#include "../../../JuceLibraryCode/JuceHeader.h"

struct BatteryStatus {
  int percentage;
  int isCharging;
};
class BatteryMonitor: public Thread {
private:
  BatteryStatus status;
  
  Array<int> percentages;
  int addAndCalculateAverage(int);
  
public:
  BatteryMonitor();
  ~BatteryMonitor();
  
  const BatteryStatus& getCurrentStatus();
  void updateStatus();
  
  virtual void run();
};