/**
 * @file PocketHomeApplication.h
 * 
 * PocketHomeApplication is the base application class that initializes and
 * shuts down the program.
 */

#pragma once
#include "JuceHeader.h"
#include "PocketHomeWindow.h"

class PocketHomeApplication : public JUCEApplication
{
public:
    PocketHomeApplication() { }
    virtual ~PocketHomeApplication() { }
    
private:
    
    void initialise(const String &commandLine) override;

    void shutdown() override;

    const String getApplicationName() override
    {
        return ProjectInfo::projectName;
    }

    const String getApplicationVersion() override
    {
        return ProjectInfo::versionString;
    }

    bool moreThanOneInstanceAllowed() override
    {
        return false;
    }
    ScopedPointer<PocketHomeWindow> homeWindow;
};