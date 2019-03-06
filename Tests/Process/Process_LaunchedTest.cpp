#include "Process_Launched.h"
#include "Process_Launcher.h"
#include "WindowFocus.h"
#include "XWindowInterface.h"
#include "DelayUtils.h"
#include "SystemCommands.h"
#include "JuceHeader.h"

namespace Process { class LaunchedTest; }

class Process::LaunchedTest : public juce::UnitTest
{
public:
    LaunchedTest() : juce::UnitTest("Process::Launched testing", "Process") {}
    
    void runTest() override
    {
        using namespace juce;
        String output;
        SystemCommands commandReader;

        if(Launcher::testCommand("echo"))
        {
            beginTest("echo test");
            Launched echo("echo (test)");
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

        if(Launcher::testCommand("top"))
        {
            beginTest("top test");
            Launched top("top");
            expect(DelayUtils::idleUntil([&top](){ return top.isRunning(); },
                    500, 5000), "\"top\" process is not running.");
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
        expect(!Launcher::testCommand(badCommand),
                "DefinitelyNotAValidLaunchCommand should have been invalid.");
        beginTest("bad command handling");
        Launched bad("DefinitelyNotAValidLaunchCommand");
        expect(!DelayUtils::idleUntil([&bad]() { return bad.isRunning(); },
                200, 2000), "Process running despite bad launch command.");
        output = bad.getProcessOutput();
        expectEquals(String(), output,
                "Bad process should have had no output.");
        expectEquals(String(bad.getExitCode()), String("0"),
			"Bad process error code should have been 0.");
        
        if(Launcher::testCommand("xclock"))
        {
            beginTest("window activation");
            Launched winApp("xclock");
            expect(DelayUtils::idleUntil([&winApp]()
                    { return winApp.isRunning(); }, 100, 1000), 
                    "Launched terminal process not running.");
            winApp.activateWindow();
            expect(DelayUtils::idleUntil(
                    []() { return !WindowFocus::isFocused(); }, 500, 8000),
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

static Process::LaunchedTest test;
