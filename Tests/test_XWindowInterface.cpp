#include "JuceHeader.h"
#include "WindowFocus.h"
#include "ProcessUtils.h"
#include "XWindowInterface.h"

class XWindowInterfaceTest : public juce::UnitTest
{
public:
    XWindowInterfaceTest() : juce::UnitTest("XWindowInterface testing") {}
    
    void runTest() override
    {
        using namespace juce;
        beginTest("Home Window Tests");
        XWindowInterface xwin;
        Window homeWin = xwin.getPocketHomeWindow();
        expect(homeWin != 0, "pocket-home window couldn't be found."); 
        xwin.activateWindow(homeWin);
	system("sleep 2");
        expect(xwin.isActiveWindow(homeWin),
		"pocket-home window should have been active.");
        expectEquals(xwin.getDesktopIndex(), xwin.getWindowDesktop(homeWin),
		"pocket-home window should have been on the current desktop");
        
        beginTest("Other window test");
        ChildProcess c;
        c.start("xclock");
	system("sleep 2");
        Array<ProcessUtils::ProcessData> childProcs 
                = ProcessUtils::getChildProcesses
                (ProcessUtils::getProcessId());
        int windowProcess = -1;
        for(const ProcessUtils::ProcessData& process : childProcs)
        {
            if(process.executableName.containsIgnoreCase("xclock"))
            {
                windowProcess = process.processId;
                break;
            }
        }
        expect(windowProcess != -1, "New child process for xclock not found.");
        
        
        Array<Window> xclockWindows = xwin.getMatchingWindows(
                [&xwin, windowProcess](const Window win)->bool
                {
                    return (xwin.getWindowName(win).isNotEmpty()
			    || xwin.getWindowClass(win).isNotEmpty()
			    || xwin.getWindowClassName(win).isNotEmpty())
                            && xwin.getWindowPID(win) == windowProcess;
                });
        expectEquals(xclockWindows.size(), 1, 
		"Exactly one xclock window should have been found.");
        Window xclockWin = xclockWindows[0];
	xwin.activateWindow(xclockWin);
	system("sleep 2");
        expect(xwin.isActiveWindow(xclockWin), "xclock window should have been active.");
        expect(!xwin.isActiveWindow(homeWin), "pocket-home window should not have been active.");
        
	logMessage("Activating home window:");
        xwin.activateWindow(homeWin);
	system("sleep 2");
        expect(!xwin.isActiveWindow(xclockWin), 
		"xclock window should not have been active.");
        expect(xwin.isActiveWindow(homeWin),
		"pocket-home window should have been active.");
        c.kill();
     }
};

static XWindowInterfaceTest test;
