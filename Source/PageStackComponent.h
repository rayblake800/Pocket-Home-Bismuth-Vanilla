/**
 * @file PageStackComponent.h
 * 
 * The UI of this program is organized into distinct 'pages', each managing some
 * subset of program behavior.  At any one time, only one page is visible to the
 * user.  Pages are arranged in a stack, so that whenever a new page is added
 * the previous page is hidden,  and whenever that page is removed the previous
 * page becomes visible again.
 * 
 * PageStackComponent manages this stack of pages, providing methods to add
 * and remove pages.  It notifies pages whenever their state on the stack
 * changes, and whenever window focus is gained or lost. It also allows any page
 * on the stack to remove itself or add a new page to the stack top.  
 */
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class PageStackComponent : public Component {
public:
    friend class PocketHomeWindow;
    PageStackComponent();
    ~PageStackComponent();

    enum Transition {
        kTransitionNone,
        kTransitionTranslateHorizontal,
        kTransitionTranslateHorizontalLeft
    };

    class Page : public Component {
    public:
        friend class PageStackComponent;

        Page() {
        };

        virtual ~Page() {
        };
        /**
         * @return true iff the page is currently on a page stack.
         */
        bool isOnPageStack();
    protected:

        /**
         * Whenever this page is added to a page stack, the PageStackComponent
         * will call this method. 
         */
        virtual void pageAddedToStack() {
        };

        /**
         * Whenever this page is removed a page stack, the PageStackComponent
         * will call this method. 
         */
        virtual void pageRemovedFromStack() {
        };

        /**
         * Whenever this page becomes the top page on the page stack, the 
         * PageStackComponent will call this method. 
         */
        virtual void pageRevealedOnStack() {
        };

        /**
         * When this page is on the top of the stack and a new page is added
         * above it, PageStackComponent will call this method.
         */
        virtual void pageCoveredOnStack() {
        };

        /**
         * When the application window gains or loses focus, this will notify
         * all pages on the stack.
         * 
         * @param windowFocused true if the window just gained focus, false if
         * it just lost focus.
         */
        virtual void windowFocusChanged(bool windowFocused) {

        };


        /**
         * If this page is currently on a page stack, this will remove it from
         * the stack
         * @param transition this animation will run if the page was on top
         * of the stack and was removed successfully.
         */
        void removeFromStack(Transition transition = kTransitionNone);

        /**
         * If this page is on a page stack, this will push a new page
         * on top of the stack.
         * @param newPage will be added to the top of the stack if it isn't
         * already on a page stack.
         * @param transition this animation will run if the page is successfully
         * added to the stack.
         */
        void pushPageToStack(Page* newPage, Transition transition = kTransitionNone);
    private:
        PageStackComponent* pageStack = nullptr;
    };

    /**
     * Add a page to the page stack.
     * @param page will be added to the top of the stack, as long as it's not
     * already on a page stack.
     * @param transition this animation will run if the page is successfully
     * added to the stack.
     */
    void pushPage(Page* page, Transition transition = kTransitionNone);

    /**
     * Removes the top page from the page stack.
     * @param transition this animation will run if a page is successfully
     * removed from the stack.
     */
    void popPage(Transition transition = kTransitionNone);

    /**
     * @return a pointer to the top page on the stack, or nullptr if the stack
     * is empty.
     */
    Page *getCurrentPage();

private:
    /**
     * The PocketHomeWindow that owns this PageStackComponent uses this method
     * to notify the PageStackComponent when window focus changes, so that
     * it can pass the notification on to all pages in the page stack.
     *  
     * @param windowFocused true if the window just gained focus, false if
     * it just lost focus.
     */
    void windowFocusChanged(bool windowFocused);

    void resized() override;
    const int transitionDurationMillis = 200;

    Array<Page *> stack;

    void transitionIn(Page *page, Transition transition, int durationMillis,
            bool reverse = false);
    void transitionOut(Page *page, Transition transition, int durationMillis,
            bool reverse = false);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageStackComponent)
};
