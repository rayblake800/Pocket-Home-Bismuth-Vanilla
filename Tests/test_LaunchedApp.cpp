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
        expect(!echo.isRunning(), 
        	"Echo process still running, but should be finished.");
        String expected("(test)");
        String output = echo.getProcessOutput().trim();
        expectEquals(output, expected,
                "Echo process output was incorrect.");
        expect(!echo.kill(),
		"Killing process succeeded when process should have been already dead");
        
        beginTest("top test");
        LaunchedApp top("top");
        system("sleep 1");
        expect(top.isRunning(), "\"top\" process is not running.");
        output = top.getProcessOutput();
        expect(output.isEmpty(), String("Unexpected process output ") + output);
        expect(top.kill(), "Failed to kill process.");
        
        beginTest("bad command handling");
        LaunchedApp bad("DefinitelyNotAValidLaunchCommand");
        expect(!bad.isRunning(),
		"Process running despite bad launch command.");
        output = bad.getProcessOutput();
        expectEquals(String(), output, "Bad process should have had no output.");
        expectEquals(String(bad.getExitCode()), String("0"),
			"Bad process error code should have been 0.");
        
        beginTest("window activation");
        LaunchedApp winApp("xclock");
        system("sleep 1");
        expect(winApp.isRunning(),
		"Launched terminal process not running.");
        winApp.activateWindow();
	MessageManager::getInstance()->runDispatchLoopUntil(5000);
        expect(!WindowFocus::isFocused(),
		"pocket-home window should not be focused.");
        winApp.kill();
        XWindowInterface xwin;
        xwin.activateWindow(xwin.getPocketHomeWindow());
        expect(!winApp.isRunning(),
			"xclock process should be dead.");
    }
};

static LaunchedAppTest test;
