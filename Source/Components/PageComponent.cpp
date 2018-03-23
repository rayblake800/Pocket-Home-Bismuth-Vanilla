#include "ComponentConfigFile.h"
#include "PageComponent.h"

PageComponent::PageComponent(ComponentConfigFile& config,
        const String& name,
        RelativeLayoutManager::Layout layout,
        PageFactoryInterface* pageFactory,
        bool showBackButton,
        bool backButtonOnRight) :
Component(name),
pageFactory(pageFactory),
backButtonOnRight(backButtonOnRight)
{
    layoutManager.setLayout(layout);
    if (showBackButton)
    {
        backButton = new ConfigurableImageButton(
                backButtonOnRight ?
                ComponentConfigFile::pageRightKey
                : ComponentConfigFile::pageLeftKey,
                config);
        backButton->addListener(this);
        addAndMakeVisible(backButton);
    }
}

/**
 * Sets a background image to draw behind all page components.
 */
void PageComponent::setBackgroundImage(Image bgImage)
{
    backgroundImage = bgImage;
}

/**
 * Adds all components in the layout to the page and makes them visible.
 */
void PageComponent::addAndShowLayoutComponents()
{
    layoutManager.addComponentsToParent(this);
}

/**
 * Sets the initial page of the page stack, which will remain on the
 * stack until the stack is destroyed.
 */
void PageComponent::PageStackInterface::setRootPage(PageComponent* page)
{
    page->pageStack = this;
    pushPage(page, Animation::none);
}

/**
 * The PageStack should call this to notify a PageComponent after 
 * pushing it on top of the page stack.
 */
void PageComponent::PageStackInterface::signalPageAdded(PageComponent* page)
{
    page->pageAddedToStack();
}

/**
 * When the top page is popped from the stack, the PageStack should
 * call this to notify the next page down that it's now the top page.
 */
void PageComponent::PageStackInterface::signalPageRevealed
(PageComponent* page)
{
    page->pageRevealedOnStack();
}

/**
 * Replaces the page layout.  All components in the old layout will be
 * removed from the page before setting the new layout.  Components in
 * the new layout will be added to the page and made visible.
 */
void PageComponent::updateLayout(RelativeLayoutManager::Layout layout)
{
    layoutManager.clearLayout(true);
    layoutManager.setLayout(layout, this);
    if (!getBounds().isEmpty())
    {
        resized();
    }
}

/**
 * Sets the amount of space to leave between page components and the edges
 * of the page.
 */
void PageComponent::setMarginFraction(float marginFraction)
{

    horizontalMargin = marginFraction;
    verticalMargin = marginFraction;
}

/**
 * Sets the amount of space to leave between components in the page layout.
 */
void PageComponent::setPadding
(float verticalFraction, float horizontalFraction)
{

    verticalPadding = verticalFraction;
    horizontalPadding = horizontalFraction;
}

/**
 * Repositions all page components using the layout manager along with
 * the margin and padding values.
 */
void PageComponent::layoutComponents()
{
    Rectangle<int> bounds = getLocalBounds();
    if (!bounds.isEmpty())
    {
        int xMargin = (int) (bounds.getHeight() * horizontalMargin);
        bounds.reduce(xMargin, (int) bounds.getHeight() * verticalMargin);
        if (backButton != nullptr)
        {

            int overlap = std::min<int>
                    (backButton->getRight() - bounds.getX(),
                     bounds.getRight() - backButton->getX());
            if (overlap > 0)
            {
                bounds.reduce(overlap, 0);
            }
        }
        layoutManager.layoutComponents(bounds,
                (int) bounds.getWidth() * horizontalPadding,
                (int) bounds.getHeight() * verticalPadding);
    }

}

/**
 * @return true iff the page is currently on the top of a page stack.
 */
bool PageComponent::isStackTop()
{
    return pageStack != nullptr && pageStack->isTopPage(this);
}

/**
 * If this page is currently on top of a page stack, this will remove it 
 * from the stack and destroy it.
 */
void PageComponent::removeFromStack(PageComponent::Animation animation)
{
    if (isStackTop())
    {
        pageStack->popPage(animation);
        pageStack = nullptr;
    }
}

/**
 * Creates and pushes a new page on top of the stack.
 */
void PageComponent::pushPageToStack(PageComponent::PageType pageType,
        PageComponent::Animation animation)
{
    if (isStackTop() && pageFactory != nullptr)
    {
        DBG(getName() << " pushing new page ");
        PageComponent* newPage = pageFactory->createPage(pageType);
        newPage->pageStack = pageStack;
        pageStack->pushPage(newPage, animation);
    }
}
/**
 * Inheriting classes can override this method to change the behavior of the
 * back button. It will be called every time the back button is clicked, and
 * if it returns true, the back button will not remove the page.
 */
bool PageComponent::overrideBackButton()
{
    return false;
}

/**
 * Recalculate component layout and back button bounds when the page is
 * resized.
 */
void PageComponent::resized()
{
    if (backButton != nullptr)
    {
        backButton->applyConfigBounds();
    }
    layoutComponents();
    pageResized();
}

/**
 * Closes the page when the back button is clicked, and passes all other
 * button clicks to the pageButtonClicked method.
 */
void PageComponent::buttonClicked(Button* button)
{
    if (button == backButton && !overrideBackButton())
    {
        removeFromStack(backButtonOnRight ?
                slideInFromLeft : slideInFromRight);
    }
    else
    {
        pageButtonClicked(button);
    }
}

/**
 * Fills the page background with an image or color.
 */
void PageComponent::paint(Graphics& g)
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
