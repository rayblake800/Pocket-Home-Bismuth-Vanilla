#include "JuceHeader.h"
#include "LaunchedApp.h"
#include "WindowFocus.h"
#include "XWindowInterface.h"

class LaunchedAppTest : public juce::UnitTest
{
public:
    LaunchedAppTest() : juce::UnitTest("LaunchedApp testing") {}
    
    void runTest() override
    {
        using namespace juce;
        beginTest("echo test");
        
        LaunchedApp echo("echo (test)");
        echo.waitForProcessToFinish(1000);
        expect(!echo.isRunning());
        String expected("(test)");
        String output = echo.getProcessOutput().trim();
        expectEquals(output, expected,
                String("Actual output==") + output);
        expect(!echo.kill());
        
        beginTest("top test");
        LaunchedApp top("urxvt -e top");
        system("sleep 0.3");
        expect(top.isRunning());
        output = top.getProcessOutput();
        expect(output.isEmpty());
        expect(top.kill());
        
        beginTest("bad command handling");
        LaunchedApp bad("DefinitelyNotAValidLaunchCommand");
        expect(!bad.isRunning());
        output = bad.getProcessOutput();
        expectEquals(String(), output);
        expectEquals(String(bad.getExitCode()), String("0"));
        
        beginTest("window activation");
        LaunchedApp winApp("urxvt");
        system("sleep 0.5");
        expect(winApp.isRunning());
        winApp.activateWindow();
        system("sleep 0.5");
        expect(!WindowFocus::isFocused());
        winApp.kill();
        XWindowInterface xwin;
        xwin.activateWindow(xwin.getPocketHomeWindow());
        expect(!winApp.isRunning());
        
        
    }
};

static LaunchedAppTest test;
