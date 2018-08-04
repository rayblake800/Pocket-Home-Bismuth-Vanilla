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
        /* Test command summary:
        * beginTest("testName");
        * expect(condition);
        * expectEquals(item1, item2);
        * logMessage(text);
        * see deps/JUCE/modules/juce_core/unit_tests
        */
        beginTest("Home Window Tests");
        XWindowInterface xwin;
        Window homeWin = xwin.getPocketHomeWindow();
        expect(homeWin != 0); 
        xwin.activateWindow(homeWin);
        system("sleep 1");
        expect(xwin.isActiveWindow(homeWin));
        expectEquals(xwin.getDesktopIndex(), xwin.getWindowDesktop(homeWin));
        expectEquals(xwin.getHeightIndex(homeWin), 0);
        
        beginTest("Other window test");
        
        String termName(std::getenv("TERMINAL"));
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
        expect(termProcess != -1);
        
        
        Array<Window> termWindows = xwin.getMatchingWindows(
                [&xwin, termProcess, termName](const Window win)->bool
                {
                    return xwin.getWindowName(win).isNotEmpty()
                            && xwin.getWindowClass(win).equalsIgnoreCase
                            (termName)
                            && xwin.getWindowPID(win) == termProcess;
                });
        expectEquals(termWindows.size(), 1);
        Window termWin = termWindows[0];
        expect(xwin.isActiveWindow(termWin));
        expect(!xwin.isActiveWindow(homeWin));
        
        //find shared parent
        Array<Window> homeLine = xwin.getWindowAncestry(homeWin);
        Array<Window> termLine = xwin.getWindowAncestry(termWin);
        Window sharedParent = BadWindow;
        for(int i = termLine.size() - 1; i >= 0; i--)
        {
            if(homeLine.contains(termLine[i]))
            {
                expectEquals(i, homeLine.indexOf(termLine[i]));
                sharedParent = termLine[i];
                logMessage(String(
                        "pocket-home and $TERMINAL share a window on level ") 
                        + String(i));
                break;
            }
        }
        expect(sharedParent != BadWindow);
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
        expect(homeParent != BadWindow);
        expect(termParent != BadWindow);
        
        int activeIndex = xwin.getHeightIndex(termParent);
        int inactiveIndex = xwin.getHeightIndex(homeParent);
        logMessage( String("pocket home window is on level ") 
                + String(inactiveIndex) 
                + String(", terminal window is on level ")
                + String(activeIndex));
        logMessage("Activating home window:");
        expectLessThan(activeIndex, inactiveIndex);
        
        xwin.activateWindow(homeWin);
        system("sleep 1");
        expect(!xwin.isActiveWindow(termWin));
        expect(xwin.isActiveWindow(homeWin));
        logMessage( String("pocket home window is on level ") 
                + String(xwin.getHeightIndex(homeParent)) 
                + String(", terminal window is on level ")
                + String(xwin.getHeightIndex(termParent)));
        logMessage("Activating home window:");
        expectEquals(xwin.getHeightIndex(termParent), inactiveIndex);
        expectEquals(xwin.getHeightIndex(homeParent), activeIndex);
        c.kill();
     }
};

static XWindowInterfaceTest test;
