#include "JuceHeader.h"
#include "AssetFiles.h"
#include "XPMLoader.h"
#include "TestWindow.h"
#include "Widgets_DrawableImage.h"
#include <X11/xpm.h>

/* Test window bounds: */
static const constexpr int winX = 5;
static const constexpr int winY = 5;
static const constexpr int winWidth = 480;
static const constexpr int winHeight = 272;

/* Test file paths: */
static const juce::StringArray testFiles =
{
    "testing/XPM/test1.xpm",
    "testing/XPM/test2.xpm",
    "testing/XPM/test3.xpm"
};

namespace Icon { class XpmTest; }

class Icon::XpmTest : public juce::UnitTest
{
public:
    XpmTest() : juce::UnitTest("xpm Image Testing",
            "Icon") {}

    
    void runTest() override
    {
        beginTest("Initial xpm testing");

        int testIndex = 0;
        for(const juce::String& assetPath : testFiles)
        {
            juce::File assetFile(AssetFiles::findAssetFile(assetPath, false));
            expect(assetFile.existsAsFile(), assetPath + " not found!");
            juce::Image xpmImage = XPMLoader::loadXPMImage(assetFile);
            expect(xpmImage.isValid(), assetPath
                    + " not loaded as a valid image.");
            Widgets::DrawableImage* drawnImage 
                    = new Widgets::DrawableImage(xpmImage);
            TestWindow imageWindow("XPM Testing", drawnImage, winX, winY, 
                    winWidth, winHeight);
            juce::MessageManager::getInstance()->runDispatchLoopUntil
                (5000);
        }

    }
};

static Icon::XpmTest test;
