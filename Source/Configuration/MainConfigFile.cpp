#include "AssetFiles.h"
#include "MainConfigFile.h"
#include "Utils.h"

MainConfigFile::MainConfigFile() : ConfigFile(filenameConst)
{
    ASSERT_SINGULAR;
    const ScopedLock readLock(getConfigLock());
    var jsonConfig = AssetFiles::loadJSONAsset
            (String(configPath) + filenameConst, true);
    var defaultConfig = var();
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
//boolean
const String MainConfigFile::showCursorKey = "cursor";

std::vector<ConfigFile::DataKey> MainConfigFile::getDataKeys() const
{
    return {
        {maxRowsKey, intType},
        {maxColumnsKey, intType},
        {backgroundKey, stringType},
        {menuTypeKey, stringType},
        {shutdownCommandKey, stringType},
        {restartCommandKey, stringType},
        {termLaunchCommandKey, stringType},
        {showCursorKey, boolType}};
}

