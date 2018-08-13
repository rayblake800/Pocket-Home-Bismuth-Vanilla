#include "JuceHeader.h"

class VarTest : public juce::UnitTest
{
public:
    VarTest() : juce::UnitTest("Var debug testing") {}
    
    void runTest() override
    {
        using namespace juce;
        const char* jsonPath = "/usr/share/pocket-home/configuration/components.json";
        const char* key1 = "battery";
        const char* key2 = "asset files";

        var vFile;
        var vComp;
        var vAssets;
        var vImage;

        String sImage;

        DynamicObject::Ptr oFile = nullptr;
        DynamicObject::Ptr oComp = nullptr;

        Array<var>* apAssets = nullptr;
        Array<var> aAssets;
        
        beginTest("direct access test");
        vFile = JSON::parse(File(jsonPath));
        sImage = vFile[key1][key2][0];
        expect(sImage.isNotEmpty());
        DBG("Read:" << sImage);
        sImage = "";
        
        beginTest("indirect access test");
        vComp = vFile[key1];
        sImage = vComp[key2][0];
        expect(sImage.isNotEmpty());
        DBG("Read:" << sImage);
        sImage = "";
    }
};

static VarTest test;
