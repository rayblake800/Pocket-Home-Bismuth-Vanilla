#include "MainConfigFile.h"
#include "../Utils.h"

CriticalSection MainConfigFile::mainConfigLock;

MainConfigFile::MainConfigFile() : ConfigFile(filenameConst)
{
    if (!fileOpened())
    {
        const ScopedLock readLock(mainConfigLock);
        var jsonConfig = openFile();
        var defaultConfig = var::null;
        readDataFromJson(jsonConfig, defaultConfig);
        writeChanges();
    }
}

MainConfigFile::~MainConfigFile() { }

//string
const String MainConfigFile::backgroundKey = "background";
const String MainConfigFile::menuTypeKey = "app menu type";
const String MainConfigFile::shutdownCommandKey = "shutdown command";
const String MainConfigFile::restartCommandKey = "restart command";
const String MainConfigFile::sleepCommandKey = "sleep command";
const String MainConfigFile::termLaunchCommandKey = "terminal launch command";
//boolean
const String MainConfigFile::showCursorKey = "cursor";

std::vector<ConfigFile::DataKey> MainConfigFile::getDataKeys() const
{
    return {
        {backgroundKey, stringType},
        {menuTypeKey, stringType},
        {shutdownCommandKey, stringType},
        {restartCommandKey, stringType},
        {termLaunchCommandKey, stringType},
        {showCursorKey, boolType}};
}

