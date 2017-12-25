#include "ClockMonitor.hpp"
#include "PokeLookAndFeel.h"

ClockMonitor::ClockMonitor() :
Thread("Clock"), 
clock(new Label("clock")),
ampm(false)
{
  clock->setFont(Font(16.5f));
  clock->setColour(Label::backgroundColourId,PokeLookAndFeel::chipPurple);
  clock->setColour(Label::textColourId,Colours::white);
  clock->setAlwaysOnTop(true);
}

ClockMonitor::~ClockMonitor(){ }

void ClockMonitor::setAmMode(bool mode){
  ampm = mode;
}

void ClockMonitor::run(){
  while(!threadShouldExit()){
    struct timeval tv;
    int error = gettimeofday(&tv, NULL);
    if(error) perror("Time of the day");
    struct tm res;
    localtime_r(&tv.tv_sec, &res);
    if(!ampm)
      sprintf(formatted, "%02d:%02d", res.tm_hour, res.tm_min);
    else{
      const char* moment = (res.tm_hour>12)?"pm":"am";
      int hour = (res.tm_hour>12)?res.tm_hour-12:res.tm_hour;
      sprintf(formatted, "%02d:%02d %s", hour, res.tm_min, moment);
    }
    lock.enter();
    clock->setText(String(formatted),
                   NotificationType::dontSendNotification);
    lock.exit();
    int width = Font(16.5f).getStringWidth(clock->getText(false));
    int height = Font(16.5f).getHeight();
    Rectangle<int> bounds = clock->getBounds();
    clock->setBounds(bounds.withSizeKeepingCentre(width,height));
    //Thread::sleep(10000);
    Thread::sleep(1000);
  }
}

Label& ClockMonitor::getLabel(){
  return *clock;
}
