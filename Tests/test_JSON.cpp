#include "JuceHeader.h"

class VarTest : public juce::UnitTest
{
public:
    VarTest() : juce::UnitTest("LaunchedApp testing") {}
    
    void runTest() override
    {
        using namespace juce;
        const char* jsonPath = "/components.json";
        const char* key1 = "BackButton";
        const char* key2 = "assets":

        
        beginTest("direct access test");
        var direct = JSON::parse(File(jsonPath));
        String asset0 = direct[key1][key2][0];
        expect(asset0.isNotEmpty());

        var vFile;
        var vComp;
        var vAssets;
        var vImage;

        String sImage;

        DynamicObject::Ptr oFile = nullptr;
        DynamicObject::Ptr oComp = nullptr;

        Array<var>* apAssets = nullptr;
        Array<var> aAssets;

        struct Test
        {
            Identifier failureMsg;
            std::function<bool() test;
        };

        Test vFileVoid
        {
            "File var is void",
            [this]()->bool
            {
                return vFile.isVoid();
            }
        };

        Test vCompVoid
        {
            "ComponentData var is void",
            [this]()->bool
            {
                return vComp.isVoid();
            }
        };

        Test vAssetsVoid
        {
            "Assets var is void",
            [this]()->bool
            {
                return vAssets.isVoid();
            }
        };

        Test vImageVoid
        {
            "Image var is void",
            [this]()->bool
            {
                return vImage.isVoid();
            }
        };

        Test sImageEmpty
        {
            "Image path is empty",
            [this]()->bool
            {
                return sImage.isEmpty();
            }
        };

        Test oFileNull
        {
            "File object is null",
            [this]()->bool
            {
                return oFile == nullptr;
            }
        };

        Test oFileEmpty
        {
            "File object is empty",
            [this]()->bool
            {
                return oFile->getProperties->isEmpty();          }
        };
        
        Test oCompNull
        {
            "Component object is null",
            [this]()->bool
            {
                return oComp == nullptr;
            }
        };

        Test oCompEmpty
        {
            "File object is empty",
            [this]()->bool
            {
                return oComp->getProperties->isEmpty();          }
        };

        Test apAssetsNull
        {
            "Assets array pointer is null",
            [this]->bool
            {
                return apAssets == nullptr;
            }

        };

        FailCondition apAssetsEmpty
        {
            "Assets array pointer is empty",
            [this]()->bool
            {
                return apAssets->isEmpty();
            }
        };

        Test aAssetsEmpty
        {
            "Assets array is empty",
            [this]()->bool
            {
                return aAssets.isEmpty();
            }
        };



    }
};

static VarTest test;
