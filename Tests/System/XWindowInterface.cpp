#include "JuceHeader.h"
#include "WindowFocus.h"
#include "Process_Launcher.h"
#include "XWindowInterface.h"
#include "DelayUtils.h"
#include "Process_Data.h"
#include <unistd.h>

class XWindowInterfaceTest : public juce::UnitTest
{
public:
    XWindowInterfaceTest() : juce::UnitTest("XWindowInterface testing",
            "System") {}
    
    void runTest() override
    {
        using namespace Process;
        using namespace juce;
        beginTest("Home Window Tests");
        XWindowInterface xwin;
        Window homeWin = xwin.getPocketHomeWindow();
        expect(homeWin != 0, "pocket-home window couldn't be found."); 
        xwin.activateWindow(homeWin);

        expect(DelayUtils::idleUntil([&xwin, &homeWin]() 
                { 
                    return xwin.isActiveWindow(homeWin);
                }, 500, 5000),
		        "pocket-home window should have been active.");

        expectEquals(xwin.getDesktopIndex(), xwin.getWindowDesktop(homeWin),
		"pocket-home window should have been on the current desktop");
        
        StringArray testApps = { "xeyes", "xclock",  "xterm" };
        String testApp;
        for(const String& app : testApps)
        {
            if(Launcher::testCommand(app))
            {
                logMessage(String("Test application:") + app);
                testApp = app;
                break;
            }
        }
        if(testApp.isEmpty())
        {
            logMessage("No valid test application found, skipping last test.");
            return;
        }

        
        beginTest("Other window test");

        ChildProcess c;
        c.start(testApp);
        int windowProcess = -1;
        std::function<bool()> findProcess = [&windowProcess, testApp]()->bool
        {
            using namespace Process;
            Data runningProcess(getpid());
            Array<Data> childProcs = runningProcess.getChildProcesses();
            for(const Data& process : childProcs)
            {
                if(process.getExecutableName().containsIgnoreCase(testApp))
                {
                    windowProcess = (int) process.getProcessId();
                    return true;
                }
            }
            return false;
        };
        expect(DelayUtils::idleUntil(findProcess, 500, 15000)
                && windowProcess != -1,
                String("New child process for ") + testApp + " not found.");
        DBG("Process ID = " << windowProcess);
        
        std::function<bool(const Window)> validWindow =
            [&xwin, windowProcess](const Window win)
            {
                return ((xwin.getWindowName(win).isNotEmpty()
                    || xwin.getWindowClass(win).isNotEmpty()
                    || xwin.getWindowClassName(win).isNotEmpty())
                        && xwin.getWindowPID(win) == windowProcess);
            };
        
        Window testWin;
        std::function<bool()> findWindow 
            = [this, &xwin, &validWindow, &testWin]()
        {
            Array<Window> testWindows = xwin.getMatchingWindows(validWindow);
            if(testWindows.size() == 1)
            {
                testWin = testWindows[0];
                return true;
            }
            if(testWindows.size() > 1)
            {
               DBG("Found too many matching windows, " <<testWindows.size()
                       << " windows found.");
            }
            return false;
        };

        expect(DelayUtils::idleUntil(findWindow, 500, 8000),
                "Failed to find test window!");
        xwin.activateWindow(testWin);
        expect(DelayUtils::idleUntil([&xwin, &testWin]()->bool
                    {
                        return xwin.isActiveWindow(testWin); 
                    }, 500, 8000),
                "test window should have been active.");
        expect(!xwin.isActiveWindow(homeWin), 
                "pocket-home window should not have been active.");
        
        logMessage("Activating home window:");
        xwin.activateWindow(homeWin);
        expect(DelayUtils::idleUntil([&xwin, &testWin]()->bool
                    {
                        return !xwin.isActiveWindow(testWin); 
                    }, 500, 8000),
                "test window should not have been active.");
        expect(xwin.isActiveWindow(homeWin),
        "pocket-home window should have been active.");
        c.kill();
     }
};

static XWindowInterfaceTest test;
