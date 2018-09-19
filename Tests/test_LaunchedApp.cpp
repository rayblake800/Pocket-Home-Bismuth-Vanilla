#include "JuceHeader.h"
#include "LaunchedProcess.h"
#include "AppLauncher.h"
#include "WindowFocus.h"
#include "XWindowInterface.h"

class LaunchedProcessTest : public juce::UnitTest
{
public:
    LaunchedProcessTest() : juce::UnitTest("LaunchedProcess testing") {}
    
    void runTest() override
    {
        using namespace juce;
        String output;

        if(AppLauncher::testCommand("echo"))
        {
            beginTest("echo test");
            LaunchedProcess echo("echo (test)");
            echo.waitForProcessToFinish(1000);
            expect(!echo.isRunning(), 
                "Echo process still running, but should be finished.");
            String expected("(test)");
            String output = echo.getProcessOutput().trim();
            expectEquals(output, expected,
                    "Echo process output was incorrect.");
            expect(!echo.kill(),
                    "Killing finished process didn't fail");
        }
        else
        {
            logMessage("echo is not a valid command, skipping test");
        }

        if(AppLauncher::testCommand("top"))
        {
            beginTest("top test");
            LaunchedProcess top("top");
            system("sleep 1");
            expect(top.isRunning(), "\"top\" process is not running.");
            output = top.getProcessOutput();
            expect(output.isEmpty(), String("Unexpected process output ") 
                    + output);
            expect(top.kill(), "Failed to kill process.");
        }
        else
        {
            logMessage("top is not a valid command, skipping test");
        }
        
        String badCommand("DefinitelyNotAValidLaunchCommand");
        expect(!AppLauncher::testCommand(badCommand),
                "DefinitelyNotAValidLaunchCommand should have been invalid.");
        beginTest("bad command handling");
        LaunchedProcess bad("DefinitelyNotAValidLaunchCommand");
        expect(!bad.isRunning(),
		"Process running despite bad launch command.");
        output = bad.getProcessOutput();
        expectEquals(String(), output, "Bad process should have had no output.");
        expectEquals(String(bad.getExitCode()), String("0"),
			"Bad process error code should have been 0.");
        
        if(AppLauncher::testCommand("xclock"))
        {
            beginTest("window activation");
            LaunchedProcess winApp("xclock");
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
        else
        {
            logMessage("xclock is not a valid command, skipping test");
        }
    }
};

static LaunchedProcessTest test;
