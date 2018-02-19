#include "MainConfigFile.h"
#include "../Utils.h"

MainConfigFile::MainConfigFile() : ConfigFile(filenameConst)
{
    const ScopedLock readLock(lock);
    File configFile = File(getHomePath() + String(CONFIG_PATH) + filename);
    var jsonConfig = JSON::parse(configFile);
    var defaultConfig = var::null;
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

MainConfigFile::~MainConfigFile()
{
}

//############################ String Data #####################################
const String MainConfigFile::backgroundKey = "background";
const String MainConfigFile::menuTypeKey = "app menu type";
const String MainConfigFile::shutdownCommandKey = "shutdown command";
const String MainConfigFile::restartCommandKey = "restart command";
const String MainConfigFile::sleepCommandKey = "sleep command";
const String MainConfigFile::termLaunchCommandKey = "terminal launch command";

Array<String> MainConfigFile::getStringKeys() const
{
    return {backgroundKey,
        menuTypeKey,
        shutdownCommandKey,
        restartCommandKey,
        termLaunchCommandKey};
}
//############################ Boolean Data ####################################
const String MainConfigFile::showCursorKey = "cursor";

Array<String> MainConfigFile::getBoolKeys() const
{
    return {showCursorKey};
}
