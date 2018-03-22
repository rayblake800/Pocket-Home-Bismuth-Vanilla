/**
 * @file PageComponent.h
 * 
 * PageComponent is a container component that organizes and manages one screen
 * of the program UI.  PageComponents fill the entire program window, so that
 * only one can be visible at a time.
 *
 * Active pages are held in a stack, with the topmost page visible. The top page
 * on the stack may add another page to the stack above it. It may also remove
 * itself to reveal the page beneath it, as long as it's not the only page on
 * the stack.  The stack takes ownership of all pages it holds, deleting
 * them when they're removed from the stack.  The page stack is managed by
 * an object implementing PageComponent::PageStackInterface, that is linked to
 * the PageComponent after the page is added to the stack.
 * 
 * All pages are aware of all other page types that they can create through the 
 * PageComponent::PageType enum.  The page on the top of the page stack can
 * create any other page type to add to the stack, without needing to know
 * anything about the created page.  This is managed by an object implementing
 * PageFactoryInterface, which must be passed in on page creation.
 */
#pragma once

#include "RelativeLayoutManager.h"
#include "ConfigurableImageButton.h"
#include "JuceHeader.h"

class PageComponent : public Component, public Button::Listener
{
public:
    class PageFactoryInterface;

    enum ColourIds
    {
        backgroundColourId = 0x1900500
    };

    /**
     * @param pageFactory        The factory object needed by this page to 
     *                            create new pages.
     * 
     * @param config             A reference to the ComponentConfigFile, which
     *                            defines the page navigation button properties.
     * 
     * @param name               The internal component name
     * 
     * @param layout             Defines the layout of all page components.  
     *                            This layout will be saved, but components will
     *                            not be added to the page until 
     *                            addAndShowLayoutComponents is called.
     * 
     * @param showBackButton     If true, add a back button on this page.
     * 
     * @param backButtonOnRight  If true, the back button will be on the right
     *                            side of the page. Otherwise, it will be on the
     *                            left.
     */
    PageComponent(PageFactoryInterface& pageFactory,
            ComponentConfigFile& config,
            const String& name = String(),
            RelativeLayoutManager::Layout layout = {},
    bool showBackButton = false,
            bool backButtonOnRight = false);

    virtual ~PageComponent() { }

    /**
     * Sets a background image to draw behind all page components.
     * 
     * @param bgImage   The image to draw, or a null Image().  If a null
     *                  image is used, the background will be filled with the 
     *                  background color instead of an image.
     */
    void setBackgroundImage(Image bgImage);

    /**
     * Adds all components in the layout to the page and makes them visible.
     */
    void addAndShowLayoutComponents();

    /**
     * Defines all pages that other pages can create to add to the page stack.
     */
    enum PageType
    {
        InputSettings,
        Keybinding,
        SetPassword,
        RemovePassword,
        Power,
        Fel,
        Settings,
        BluetoothSettings,
        WifiSettings,
        UI,
        Colour,
        AdvancedSettings,
        DateTime,
        HomeSettings,
    };

    /**
     * Defines all possible page transition animations.
     */
    enum Animation
    {
        slideInFromLeft,
        slideInFromRight,
        none
    };

    /**
     * Defines the interface between individual PageComponents and the page
     * stack.  
     */
    class PageStackInterface
    {
        friend class PageComponent;
    public:

        virtual ~PageStackInterface() { }

    protected:

        PageStackInterface() { }

        /**
         * The PageStack should call this to notify a PageComponent after 
         * pushing it on top of the page stack.
         *
         * @param page  The new top page on the stack.
         */
        void signalPageAdded(PageComponent* page);

        /**
         * When the top page is popped from the stack, the PageStack should
         * call this to notify the next page down that it's now the top page.
         *
         * @param page The new top page on the stack.
         */
        void signalPageRevealed(PageComponent* page);

    private:
        /**
         * Pushes a new PageComponent on top of the stack, optionally animating
         * the transition. 
         * 
         * @param page
         * 
         * @param animation
         */
        virtual void pushPage
        (PageComponent* page, Animation animation = slideInFromLeft) = 0;

        /**
         * Removes the top page from the stack, optionally animating the 
         * transition.
         * 
         * @param animation
         */
        virtual void popPage(Animation animation = slideInFromLeft) = 0;

        /**
         * Checks if a page is the top page on the stack.
         * 
         * @param  page
         * 
         * @return true iff page is displayed on top of the page stack.
         */
        virtual bool isTopPage(PageComponent* page) = 0;
    };

    /**
     * Defines the interface between PageComponents and the PageFactory object
     * that creates new pages to add to the page stack.
     */
    class PageFactoryInterface
    {
    public:
        friend class PageComponent;

        virtual ~PageFactoryInterface() { }

    protected:

        PageFactoryInterface() { }

    private:
        /**
         * Create a new page to push on top of the page stack.
         */
        virtual PageComponent* createPage(PageType type) = 0;
    };

protected:

    /**
     * Whenever this page is added to a page stack, the PageStack
     * will call this method. 
     */
    virtual void pageAddedToStack() { }

    /**
     * Whenever this page becomes the top page on the page stack, the 
     * PageStackComponent will call this method. 
     */
    virtual void pageRevealedOnStack() { }

    /**
     * Handles any buttons besides the back button.  Inheriting classes
     * should override this instead of buttonClicked(Button*) to handle their 
     * own button actions.
     * 
     * @param button  Points to a button that was just clicked that added this
     *                PageComponent as a listener.
     */
    virtual void pageButtonClicked(Button* button) { }

    /**
     * Handles any actions necessary whenever the page is resized, besides
     * updating the layout manager and back button.  Inheriting classes should
     * override this instead of resized() to handle page resizing.
     */
    virtual void pageResized() { }

    /**
     * Replaces the page layout.  All components in the old layout will be
     * removed from the page before setting the new layout.  Components in
     * the new layout will be added to the page and made visible.
     *
     * @param layout
     */
    void updateLayout(RelativeLayoutManager::Layout layout);

    /**
     * Sets the amount of space to leave between page components and the edges
     * of the page.  Note that margins don't affect the back button, and if
     * necessary they will be expanded to make room for the back button.
     * 
     * @param marginFraction This value is multiplied by the page height to
     *                       get the margin size in pixels.
     */
    void setMarginFraction(float marginFraction);

    /**
     * Sets the amount of space to leave between components in the page layout.
     *
     * @param verticalFraction    This value is multiplied by the page height to
     *                            find the amount of vertical empty space (in 
     *                            pixels) to leave between components.
     *
     * @param horizontalFraction  This value is multiplied by the page width to
     *                            find the amount of horizontal empty space (in
     *                            pixels) to leave between components.
     */
    void setPadding(float verticalFraction, float horizontalFraction);

    /**
     * Repositions all page components using the layout manager along with
     * the margin and padding values.
     */
    void layoutComponents();

    /**
     * @return true iff the page is currently on the top of a page stack.
     */
    bool isStackTop();

    /**
     * If this page is currently on top of a page stack, this will remove it 
     * from the stack and destroy it.
     *
     * @param animation   This animation will run if the page was on top
     *                     of the stack and was removed successfully.
     */
    void removeFromStack(Animation animation = slideInFromLeft);

    /**
     * Creates and pushes a new page on top of the stack.
     *
     * @param pageType   The type of page to create and add.
    
     * @param animation  This animation will run if the page is successfully
     *                     added to the stack.
     */
    void pushPageToStack(PageType pageType, Animation animation
            = slideInFromLeft);

private:
    /**
     * Inheriting classes can override this method to change the behavior of the
     * back button. It will be called every time the back button is clicked, and
     * if it returns true, the back button will not remove the page.
     * 
     * @return true if the back button's action was replaced, false to allow
     *         the back button to remove the page as usual.
     */
    virtual bool overrideBackButton();


    /**
     * Recalculate component layout and back button bounds when the page is
     * resized.
     */
    void resized() final override;

    /**
     * Closes the page when the back button is clicked, and passes all other
     * button clicks to the pageButtonClicked method.
     *
     * @param button
     */
    void buttonClicked(Button* button) final override;

    /**
     * Fills the page background with an image or color.
     *
     * @param g
     */
    virtual void paint(Graphics& g) override;

    //A button that removes the page from the page stack may be held here.
    ScopedPointer<ConfigurableImageButton> backButton = nullptr;
    //If the page contains a back button, this sets the side it appears on.
    bool backButtonOnRight;

    //Layout manager and component margin/padding values.
    RelativeLayoutManager layoutManager;
    float horizontalMargin = 0.05;
    float verticalMargin = 0.05;
    float verticalPadding = 0.05;
    float horizontalPadding = 0.05;

    //Optional page background image.
    Image backgroundImage;

    //Used by the page to create additional pages to display.
    PageFactoryInterface& pageFactory;

    //Points to the page stack this page is on, if any.
    PageStackInterface* pageStack = nullptr;
};
