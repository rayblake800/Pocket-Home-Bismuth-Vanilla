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
        system("sleep 1");
        expect(xwin.isActiveWindow(homeWin),
		"pocket-home window should have been active.");
        expectEquals(xwin.getDesktopIndex(), xwin.getWindowDesktop(homeWin),
		"pocket-home window should have been on the current desktop");
        
        beginTest("Other window test");
        
        String termName(std::getenv("TERMINAL"));
	expect(termName.isNotEmpty(),
		"Terminal environment variable is not set.");
        ChildProcess c;
        c.start(termName);
        system("sleep 2");
        
        Array<ProcessUtils::ProcessData> childProcs 
                = ProcessUtils::getChildProcesses
                (ProcessUtils::getProcessId());
        int termProcess = -1;
        for(const ProcessUtils::ProcessData& process : childProcs)
        {
            if(process.executableName.containsIgnoreCase(termName))
            {
                termProcess = process.processId;
                break;
            }
        }
        expect(termProcess != -1, "Terminal child process not found.");
        
        
        Array<Window> termWindows = xwin.getMatchingWindows(
                [&xwin, termProcess, termName](const Window win)->bool
                {
                    return xwin.getWindowName(win).isNotEmpty()
                            && xwin.getWindowPID(win) == termProcess;
                });
        expectEquals(termWindows.size(), 1, 
		"Only one terminal window should have been found.");
        Window termWin = termWindows[0];
        expect(xwin.isActiveWindow(termWin), "Terminal window should have been active.");
        expect(!xwin.isActiveWindow(homeWin), "pocket-home window should not have been active.");
        
        //find shared parent
        Array<Window> homeLine = xwin.getWindowAncestry(homeWin);
        Array<Window> termLine = xwin.getWindowAncestry(termWin);
        Window sharedParent = BadWindow;
        for(int i = termLine.size() - 1; i >= 0; i--)
        {
            if(homeLine.contains(termLine[i]))
            {
                expectEquals(i, homeLine.indexOf(termLine[i]),
			"HomeWindow and termWindow shared parents are not on the same level.");
                sharedParent = termLine[i];
                logMessage(String(
                        "pocket-home and $TERMINAL share a window on level ") 
                        + String(i));
                break;
            }
        }
        expect(sharedParent != BadWindow, "Found no valid shared parent window");
        Array<Window> sharedLevel = xwin.getWindowChildren(sharedParent);
        Window homeParent = BadWindow;
        Window termParent = BadWindow;
        for(const Window& window : sharedLevel)
        {
            if(homeLine.contains(window))
            {
                homeParent = window;
            }
            else if(termLine.contains(window))
            {
                termParent = window;
            }
        }
        expect(homeParent != BadWindow, "pocket-home parent window not found.");
        expect(termParent != BadWindow, "terminal parent window not found.");
        
        int activeIndex = xwin.getHeightIndex(termParent);
        int inactiveIndex = xwin.getHeightIndex(homeParent);
        logMessage( String("pocket home window is on level ") 
                + String(inactiveIndex) 
                + String(", terminal window is on level ")
                + String(activeIndex));
        logMessage("Activating home window:");
        
        xwin.activateWindow(homeWin);
        system("sleep 1");
        expect(!xwin.isActiveWindow(termWin), 
		"terminal window should not have been active.");
        expect(xwin.isActiveWindow(homeWin),
		"pocket-home window should have been active.");
        logMessage( String("pocket home window is on level ") 
                + String(xwin.getHeightIndex(homeParent)) 
                + String(", terminal window is on level ")
                + String(xwin.getHeightIndex(termParent)));
        logMessage("Activating home window:");
        c.kill();
     }
};

static XWindowInterfaceTest test;
