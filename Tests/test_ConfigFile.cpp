#include "JuceHeader.h"
#include "SharedResource.h"
#include "MainConfigFile.h"
#include "ConfigFile.h"
#include "ConfigJSON.h"

class TestListener : public MainConfigFile::Listener
{
    public:
    TestListener() { }
    ~TestListener() { }
    virtual void configValueChanged(const juce::Identifier& key) override
    {
    }
};

class ConfigTest : public juce::UnitTest
{

public:
    ConfigTest() : juce::UnitTest("ConfigFile testing") {}
    
    void runTest() override
    {
        using namespace juce;

        TestListener * tl = new TestListener();
            
        SharedResource::Handler* h = dynamic_cast<SharedResource::Handler*>(tl);
        expect(h != nullptr);

        /*
        TestListener testListener;

        beginTest("File backup/ConfigFile creation");
        File existingConfig("~/.pocket-home/config.json");
        //Backup existing config file
        File backup = File::createTempFile(".json");
        expect(existingConfig.copyFileTo(backup),
			"Failed to backup existing config file!");
        existingConfig.deleteFile();
        existingConfig.create();

        int64 lastSize = 0;
        ScopedPointer<TestConfigFile> config
                = new TestConfigFile("config.json");
        expectGreaterThan(existingConfig.getSize(), lastSize,
		"Failed to restore and save default config values!");
        lastSize = existingConfig.getSize();
    	int64 defaultSize = lastSize;
        
        beginTest("Reading default values");
        const String strKey = backgroundKey;
        const String intKey = maxRowsKey;
        const String boolKey = showCursorKey;

        const String defaultString = config->getConfigValue<String>
                (strKey);
        const int defaultInt =  config->getConfigValue<int>
                (intKey);
        const bool defaultBool = config->getConfigValue<bool>
                (boolKey);

        expect(defaultString.isNotEmpty(), "Default string value is empty!");
        expect(defaultInt > 0, "Default int value is 0!");

        beginTest("Writing new values");
        const String newStr = "defaultString" + defaultString;
        const int newInt = defaultInt * 100;
        const bool newBool = !defaultBool;

        config->setConfigValue<String>(strKey, newStr);
        config->setConfigValue<int>(intKey, newInt);
        config->setConfigValue<bool>(boolKey, newBool);
       
        expectGreaterThan(
                existingConfig.getSize(),
                lastSize,
		"Changes were not written to file!");
        lastSize = existingConfig.getSize();
        
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
        
        expectEquals(strKey, lastValueChanged,
			"Listener registered wrong updated key!");
        lastValueChanged = ""; 
       
        expectLessThan(
                existingConfig.getSize(),
                lastSize,
		"Changes were not written to file!");
	    lastSize = existingConfig.getSize();

        expectEquals(config->getConfigValue<String>(strKey),
                String(),
		"Test string value change failed!"); 
        expectEquals(config->getConfigValue<int>(intKey),
                -1, "Test int value change failed!"); 
        expect(config->getConfigValue<bool>(boolKey)
                != newBool, "Test bool value change failed!"); 
        config->addListener(testListener, {boolKey});
   
        config->setConfigValue<String>(strKey, defaultString);
        config->setConfigValue<int>(intKey, defaultInt);
        expectEquals(lastValueChanged, strKey,
			"Listener registered wrong updated key!");

        config->setConfigValue<bool>(boolKey, newBool);  
        expectEquals(lastValueChanged, boolKey,
			"Listener registered wrong updated key!");
        existingConfig.deleteFile();
        
    	beginTest("Restoring defaults");
	    String largeString = defaultString;
	    for(int i = 0; i < 5; i++)
	    {
	        largeString += largeString;
	    }
	    config->setConfigValue<String>(strKey, largeString);
	    config->setConfigValue<int>(intKey, newInt);
	    lastSize = existingConfig.getSize();
	    expectGreaterThan(lastSize, defaultSize,
		        "Test expects the current size to exceed the default.");
	    config->restoreDefaultValue(strKey);
	    expectEquals(config->getConfigValue<String>(strKey),
		        defaultString, "Default string should have been restored!");
	    expectGreaterThan(lastSize, existingConfig.getSize(),
		        "Config should still be greater than the default size.");
	    config->restoreDefaultValues();
	    lastSize = existingConfig.getSize();
	    expectEquals(defaultSize, lastSize,
		        "File size should have matched the default size.");

        beginTest("Resource sharing");
        existingConfig.deleteFile();
        ScopedPointer<TestConfigFile> config2
                = new TestConfigFile("config.json");
        expectEquals(existingConfig.getSize(), int64(0),
	       	    "Config file should not have been updated!");
        config = nullptr;
	    config2 = nullptr;
        config2 = new TestConfigFile("config.json");
        expectGreaterThan(existingConfig.getSize(), int64(0),
		        "Config file should have been restored from default again!");
        config2 = nullptr;
        //Restore existing config file
        expect(backup.copyFileTo(existingConfig),
		       	"Failed to restore backup!");
        */
    }
};

static ConfigTest test;
