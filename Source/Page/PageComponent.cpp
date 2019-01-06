#include "PageComponent.h"

PageComponent::PageComponent(const juce::String& name) :
Component(name) { }
    
/*
 * Sets the layout of the page component.  If the page layout was set
 * previously, the old layout will be cleared, and its components will be
 * removed from the page.
 */
void PageComponent::setLayout(Layout::Group::RelativeLayout layout)
{
    layoutManager.clearLayout(true);
    //make sure layout margins fit the back button
    if(backButton != nullptr) 
    {
        float minMargin = backButton->xMarginFractionNeeded();
        layout.setXMarginFraction(std::max(layout.getXMarginFraction(),
                minMargin));
    }   
    layoutManager.setLayout(layout, this);
    if (!getBounds().isEmpty())
    {
        resized();
    }
}
  
/*
 * Sets the type of back button(if any) that should be shown on the page.
 * If necessary, page layout margins will be resized to make room for the
 * back button.
 */
void PageComponent::setBackButton(PageComponent::BackButtonType buttonType)
{
    NavButton::WindowEdge newEdge;
    switch(buttonType)
    {
        case noBackButton:
            //set to ensure that newEdge != backButton->getEdge()
            newEdge = NavButton::up;
            break;
        case leftBackButton:
            newEdge = NavButton::left;
            break;
        case rightBackButton:
            newEdge = NavButton::right;
    }
    if(backButton != nullptr)
    {
        if(newEdge == backButton->getEdge())
        {
            return;
        }
        removeChildComponent(backButton);
        backButton = nullptr;
    }
    if(buttonType == noBackButton)
    {
        return;
    }
    backButton = new NavButton(newEdge);
    addAndMakeVisible(backButton);
    backButton->addListener(this);
    
    Layout::Group::RelativeLayout layout = layoutManager.getLayout();
    if(!layout.isEmpty()
       && layout.getXMarginFraction() < backButton->xMarginFractionNeeded())
    {
        layout.setXMarginFraction(backButton->xMarginFractionNeeded());
        layoutManager.setLayout(layout);
    }
}

/*
 * Sets a background image to draw behind all page components.
 */
void PageComponent::setBackgroundImage(juce::Image bgImage)
{
    backgroundImage = bgImage;
}

/*
 * Adds all components in the layout to the page and makes them visible.
 */
void PageComponent::addAndShowLayoutComponents()
{
    layoutManager.addComponentsToParent(this);
}

/*
 * Sets the initial page of the page stack, which will remain on the
 * stack until the stack is destroyed.
 */
void PageComponent::PageStackInterface::setRootPage(PageComponent* page)
{
    page->pageStack = this;
    pushPage(page, Layout::Transition::Type::none);
}

/*
 * The PageStack should call this to notify a PageComponent after 
 * pushing it on top of the page stack.
 */
void PageComponent::PageStackInterface::signalPageAdded(PageComponent* page)
{
    page->pageAddedToStack();
}

/*
 * When the top page is popped from the stack, the PageStack should
 * call this to notify the next page down that it's now the top page.
 */
void PageComponent::PageStackInterface::signalPageRevealed
(PageComponent* page)
{
    page->pageRevealedOnStack();
}

/*
 * Assigns this PageFactory to a PageComponent.  
 */
PageComponent*
PageComponent::PageFactoryInterface::setPageFactory(PageComponent* page)
{
    page->pageFactory = this;
    return page;
}



/*
 * @return true iff the page is currently on the top of a page stack.
 */
bool PageComponent::isStackTop()
{
    return pageStack != nullptr && pageStack->isTopPage(this);
}

/*
 * If this page is currently on top of a page stack, this will remove it 
 * from the stack and destroy it.
 */
void PageComponent::removeFromStack(Layout::Transition::Type transition)
{
    if (isStackTop())
    {
        pageStack->popPage(transition);
        pageStack = nullptr;
    }
}

/*
 * Creates and pushes a new page on top of the stack.
 */
void PageComponent::pushPageToStack(PageComponent::PageType pageType,
        Layout::Transition::Type transition)
{
    if (isStackTop() && pageFactory != nullptr)
    {
        DBG(getName() << " pushing new page ");
        PageComponent* newPage = pageFactory->createPage(pageType);
        newPage->pageStack = pageStack;
        pageStack->pushPage(newPage, transition);
    }
}

/*
 * Inheriting classes can override this method to change the behavior of the
 * back button. It will be called every time the back button is clicked, and
 * if it returns true, the back button will not remove the page.
 */
bool PageComponent::overrideBackButton()
{
    return false;
}

/*
 * Recalculate component layout and back button bounds when the page is
 * resized.
 */
void PageComponent::resized()
{
    if (backButton != nullptr)
    {
        backButton->applyConfigBounds();
    }
    layoutManager.layoutComponents(getLocalBounds());
    pageResized();
}

/*
 * Closes the page when the back button is clicked, and passes all other
 * button clicks to the pageButtonClicked method.
 */
void PageComponent::buttonClicked(juce::Button* button)
{
    if (button == backButton && !overrideBackButton())
    {
        removeFromStack(backButton->getEdge() == NavButton::right ?
                Layout::Transition::Type::moveLeft 
                : Layout::Transition::Type::moveRight);
    }
    else
    {
        pageButtonClicked(button);
    }
}

/*
 * Fills the page background with an image or color.
 */
void PageComponent::paint(juce::Graphics& g)
{
    if (backgroundImage.isValid())
    {
        g.drawImage(backgroundImage, getLocalBounds().toFloat());
    }
    else
    {
        g.fillAll(findColour(backgroundColourId));
    }
}
