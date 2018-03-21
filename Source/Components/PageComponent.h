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

#include "PageStackComponent.h"
#include "RelativeLayoutManager.h"
#include "ConfigurableImageButton.h"
#include "JuceHeader.h"

class PageComponent : public PageStackComponent::Page, public Button::Listener 
{
public:

    enum ColourIds 
    {
        backgroundColourId = 0x1900500
    };

    /**
     * @param name               The internal component name
     * 
     * @param layout             Defines the layout of all page components.  
     *                           This layout will be saved, but components will
     *                           not be added to the page until 
     *                           addAndShowLayoutComponents is called.
     * 
     * @param showBackButton     If true, add a back button on this page.
     * 
     * @param backButtonOnRight  If true, the back button will be on the right
     *                           side of the page. Otherwise, it will be on the
     *                           left.
     */
    PageComponent(const String& name = String(),
            RelativeLayoutManager::Layout layout = {},
            bool showBackButton = false,
            bool backButtonOnRight = false);

    virtual ~PageComponent();
    
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
     * Defines the interface between individual PageComponents and the page
     * stack.  
     */
    class PageStackInterface
    {
    friend class PageComponent;
    public:	
    	
    	virtual ~PageStackInterface() { }
	
	/**
	 * Defines all possible page transition animations.
	 */
	enum Animation
	{
	    slideInLeft,
	    slideInRight,
	    none
	};
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
	virtual void pushPage
		(PageComponent* page, Animation animation = none) = 0;

        virtual void popPage(Animation animation = none) = 0;

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
	 * Create a new page to
	 */
	virtual PageComponent* createPage(PageType type) = 0;
    };

protected:
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
    void setPadding(float verticalFraction,float horizontalFraction);  
    
    /**
     * Repositions all page components using the layout manager along with
     * the margin and padding values.
     */
    void layoutComponents();

    /**
     * @return true iff the page is currently on a page stack.
     */
     bool isOnPageStack();

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
     * When this page is on the top of the stack and a new page is added
     * above it, PageStackComponent will call this method.
     */
     virtual void pageCoveredOnStack() { }

    /**
     * If this page is currently on a page stack, this will remove it from
     * the stack.
     *
     * @param transition   This animation will run if the page was on top
     *                     of the stack and was removed successfully.
     */
     void removeFromStack(Transition transition = kTransitionNone);

    /**
     * If this page is on a page stack, this will push a new page
     * on top of the stack.
     *
     * @param newPage     This page will be added to the top of the stack if it 
     *                    isn't already on a page stack.
    
     * @param transition  This animation will run if the page is successfully
     *                    added to the stack.
     */
     void pushPageToStack(Page* newPage, 
             Transition transition = kTransitionNone);
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
    
    bool backButtonOnRight;
    RelativeLayoutManager layoutManager;
    float horizontalMargin = 0.05;
    float verticalMargin = 0.05;
    float verticalPadding = 0.05;
    float horizontalPadding = 0.05;
    ScopedPointer<ConfigurableImageButton> backButton = nullptr;
    Image backgroundImage;
    PageFactoryInterface& pageFactory;
    PageStackInterface* pageStack;
};
