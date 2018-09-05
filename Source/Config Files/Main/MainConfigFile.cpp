#include "AssetFiles.h"
#include "MainConfigFile.h"
#include "Utils.h"


juce::ScopedPointer<ResourceManager::SharedResource>
        MainConfigFile::sharedResource = nullptr;

juce::ReadWriteLock MainConfigFile::configLock;

MainConfigFile::MainConfigFile() : 
ResourceManager(sharedResource,configLock,
[this]()->ResourceManager::SharedResource*
{
    return new MainJson();
}) { }


/*
 * Add a listener to track component setting changes.
 */
void MainConfigFile::addListener(ConfigFile::Listener* listener,
        juce::StringArray trackedKeys)
{
    using namespace juce;
    const ScopedWriteLock writeLock(configLock);
    MainJson* config = static_cast<MainJson*> (sharedResource.get());
    config->addListener(listener, trackedKeys);
}

MainConfigFile::MainJson::MainJson() :
ConfigFile(filenameConst)
{
    loadJSONData();
}

//menu types
const juce::StringArray MainConfigFile::menuTypes = 
{
    "Scrolling menu",
     "Paged menu"
};


//#### Integer value keys #######
const juce::String MainConfigFile::maxRowsKey = "max menu row count";
const juce::String MainConfigFile::maxColumnsKey = "max menu column count";
//string
const juce::String MainConfigFile::menuTypeKey = "app menu type";
const juce::String MainConfigFile::shutdownCommandKey = "shutdown command";
const juce::String MainConfigFile::restartCommandKey = "restart command";
const juce::String MainConfigFile::sleepCommandKey = "sleep command";
const juce::String MainConfigFile::termLaunchCommandKey = "terminal launch command";
const juce::String MainConfigFile::backgroundKey = "background";
const juce::String MainConfigFile::wifiInterfaceKey = "Wifi interface";
//boolean
const juce::String MainConfigFile::showCursorKey = "cursor";
const juce::String MainConfigFile::showClockKey = "show clock";
const juce::String MainConfigFile::use24HrModeKey = "use 24h mode";

const std::vector<ConfigFile::DataKey>& 
MainConfigFile::MainJson::getDataKeys() const
{
    static const std::vector<DataKey> keys =
    {
        {maxRowsKey, intType},
        {maxColumnsKey, intType},
        {backgroundKey, stringType},
        {menuTypeKey, stringType},
        {shutdownCommandKey, stringType},
        {restartCommandKey, stringType},
        {termLaunchCommandKey, stringType},
        {wifiInterfaceKey, stringType},
        {showCursorKey, boolType},
        {showClockKey, boolType},
        {use24HrModeKey, boolType}
    };
    return keys;
}

