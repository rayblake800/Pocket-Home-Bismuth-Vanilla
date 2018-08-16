#include "JuceHeader.h"
#include "ConfigFile.h"

static juce::ScopedPointer<ResourceManager::SharedResource>
        testResource = nullptr;

static juce::ReadWriteLock configLock;


//#### Integer value keys #######
const juce::String maxRowsKey = "max menu row count";
const juce::String maxColumnsKey = "max menu column count";
//string
const juce::String menuTypeKey = "app menu type";
const juce::String shutdownCommandKey = "shutdown command";
const juce::String restartCommandKey = "restart command";
const juce::String sleepCommandKey = "sleep command";
const juce::String termLaunchCommandKey = "terminal launch command";
const juce::String backgroundKey = "background";
const juce::String wifiInterfaceKey = "Wifi interface";
//boolean
const juce::String showCursorKey = "cursor";
const juce::String showClockKey = "show clock";
const juce::String use24HrModeKey = "use 24h mode";

class TestJson : public ConfigFile
{
public:
    TestJson(juce::String configPath) : ConfigFile(configPath) 
    {
        loadJSONData();
    }
    
    const std::vector<ConfigFile::DataKey>& getDataKeys() const
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
};


static juce::String lastValueChanged;
class TestListener : public ConfigFile::Listener
{
public:
    virtual void configValueChanged(juce::String key) override
    {
        lastValueChanged = key;
    }
};


class TestConfigFile : public ResourceManager
{
public:
    TestConfigFile(juce::String filename) : 
            ResourceManager(testResource,configLock,
    [this, filename]()->ResourceManager::SharedResource*
    {
        return new TestJson(filename);
    }) { }

    /**
     * Gets one of the values stored in the json configuration file.
     * 
     * @see ConfigFile.h
     */
    template<typename T > T getConfigValue(juce::String key)
    {
        const juce::ScopedReadLock readLock(configLock);
        TestJson* config = static_cast<TestJson*> (testResource.get());
        return config->getConfigValue<T>(key);
    }

    /**
     * Sets one of this ConfigFile's values, writing it to the config 
     * file if the value has changed.  
     * 
     * @see ConfigFile.h
     */
    template<typename T>
    void setConfigValue(juce::String key, T newValue)
    {
        const juce::ScopedWriteLock writeLock(configLock);
        TestJson* config = static_cast<TestJson*> (testResource.get());
        config->setConfigValue<T>(key, newValue);
    }

 
    /*
     * Add a listener to track component setting changes.
     */
    void addListener(TestListener& listener,
        juce::StringArray trackedKeys)
    {
        using namespace juce;
        const ScopedWriteLock writeLock(configLock);
        TestJson* config = static_cast<TestJson*>(testResource.get());
        config->addListener(
                static_cast<ConfigFile::Listener*>(&listener), trackedKeys);
    }
};

class ConfigTest : public juce::UnitTest
{

public:
    ConfigTest() : juce::UnitTest("ConfigFile testing") {}
    
    void runTest() override
    {
        using namespace juce;
        TestListener testListener;

        beginTest("File backup/ConfigFile creation");
        File existingConfig("~/.pocket-home/config.json");
        //Backup existing config file
        File backup = File::createTempFile(".json");
        expect(existingConfig.copyFileTo(backup),
			"Failed to backup existing config file!");
        existingConfig.deleteFile();
        existingConfig.create();

        Time writeTime = existingConfig.getLastModificationTime();
        ScopedPointer<TestConfigFile> config
                = new TestConfigFile("config.json");
        expectGreaterThan(
                existingConfig.getLastModificationTime().toMilliseconds(),
                writeTime.toMilliseconds(),
		"Failed to restore and save default config values!");
        writeTime = existingConfig.getLastModificationTime();
        
        beginTest("Reading default values");
        const String strKey = backgroundKey;
        const String intKey = maxRowsKey;
        const String boolKey = showCursorKey;

        const String testString = config->getConfigValue<String>
                (strKey);
        const int testInt =  config->getConfigValue<int>
                (intKey);
        const bool testBool = config->getConfigValue<bool>
                (boolKey);

        expect(testString.isNotEmpty(), "Test string value is empty!");
        expect(testInt > 0, "Test int value is 0!");

        beginTest("Writing new values");
        const String newStr = "testString";
        const int newInt = testInt + 100;
        const bool newBool = !testBool;

        config->setConfigValue<String>(strKey, newStr);
        config->setConfigValue<int>(intKey, newInt);
        config->setConfigValue<bool>(boolKey, newBool);
       
        expectGreaterThan(
                existingConfig.getLastModificationTime().toMilliseconds(),
                writeTime.toMilliseconds(),
		"Changes were not written to file!");
        writeTime = existingConfig.getLastModificationTime();
        
        expectEquals(config->getConfigValue<String>(strKey),
                newStr,
		"Test string value change failed!"); 
        expectEquals(config->getConfigValue<int>(intKey),
                newInt,
		"Test int value change failed!"); 
        expect(config->getConfigValue<bool>(boolKey)
               == newBool,
	       "Test bool value change failed!"); 

        beginTest("Listener testing");
        expect(lastValueChanged.isEmpty(),
			"Listener called without being added!"); 
        config->addListener(testListener, {strKey});
  
        config->setConfigValue<String>(strKey, "");
        config->setConfigValue<int>(intKey, -1);
        config->setConfigValue<bool>(boolKey, !newBool);  
        
        expectEquals(lastValueChanged, strKey,
			"Listener registered wrong updated key!");
        lastValueChanged = ""; 
       
        expectGreaterThan(
                existingConfig.getLastModificationTime().toMilliseconds(),
                writeTime.toMilliseconds(),
		"Changes were not written to file!");
        writeTime = existingConfig.getLastModificationTime();

        expectEquals(config->getConfigValue<String>(strKey),
                String(),
		"Listener registered wrong updated key"); 
        expectEquals(config->getConfigValue<int>(intKey),
                -1, "Test int value change failed!"); 
        expect(config->getConfigValue<bool>(boolKey)
                != newBool, "Test bool value change failed!"); 
        config->addListener(testListener, {boolKey});
   
        config->setConfigValue<String>(strKey, testString);
        config->setConfigValue<int>(intKey, testInt);
        expectEquals(lastValueChanged, strKey,
			"Listener registered wrong updated key!");

        config->setConfigValue<bool>(boolKey, !newBool);  
        expectEquals(lastValueChanged, boolKey,
			"Listener registered wrong updated key!");
        existingConfig.deleteFile();
        
        beginTest("Resource sharing");
        ScopedPointer<TestConfigFile> config2
                = new TestConfigFile("config.json");
        expectEquals(
                existingConfig.getLastModificationTime().toMilliseconds(),
                writeTime.toMilliseconds(),
	       	"Config file should not have been updated!");
        config = nullptr;
        config2 = new TestConfigFile("config.json");
        expectGreaterThan(
                existingConfig.getLastModificationTime().toMilliseconds(),
                writeTime.toMilliseconds(),
		"Config file should have been restored from default again!");
        config2 = nullptr;
        //save altered config file for examination
        File alteredCopy("~/altered.json");
        alteredCopy.create();
        existingConfig.copyFileTo(alteredCopy);
        //Restore existing config file
        expect(existingConfig.copyFileTo(backup),
		       	"Failed to restore backup!");
    }
};

static ConfigTest test;
