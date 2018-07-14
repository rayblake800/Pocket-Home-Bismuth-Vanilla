#include "AssetFiles.h"
#include "MainConfigFile.h"
#include "Utils.h"


ScopedPointer<ResourceManager::SharedResource>
        MainConfigFile::sharedResource = nullptr;

ReadWriteLock MainConfigFile::configLock;

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
        StringArray trackedKeys)
{
    const ScopedWriteLock writeLock(configLock);
    MainJson* config = static_cast<MainJson*> (sharedResource.get());
    config->addListener(listener, trackedKeys);
}

MainConfigFile::MainJson::MainJson() :
ConfigFile(filenameConst)
{
    var jsonConfig = AssetFiles::loadJSONAsset
            (String(configPath) + filenameConst, true);
    var defaultConfig;
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

//menu types
const StringArray MainConfigFile::menuTypes = {
                                               "Scrolling menu",
                                               "Paged menu"
};


//#### Integer value keys #######
const String MainConfigFile::maxRowsKey = "max menu row count";
const String MainConfigFile::maxColumnsKey = "max menu column count";
//string
const String MainConfigFile::menuTypeKey = "app menu type";
const String MainConfigFile::shutdownCommandKey = "shutdown command";
const String MainConfigFile::restartCommandKey = "restart command";
const String MainConfigFile::sleepCommandKey = "sleep command";
const String MainConfigFile::termLaunchCommandKey = "terminal launch command";
const String MainConfigFile::backgroundKey = "background";
const String MainConfigFile::wifiInterfaceKey = "Wifi interface";
//boolean
const String MainConfigFile::showCursorKey = "cursor";
const String MainConfigFile::showClockKey = "show clock";
const String MainConfigFile::use24HrModeKey = "use 24h mode";

std::vector<ConfigFile::DataKey> MainConfigFile::MainJson::getDataKeys() const
{
    return {
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
        {use24HrModeKey, boolType}};
}

