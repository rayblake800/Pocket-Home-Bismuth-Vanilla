#include "Utils.h"
#include "TempTimer.h"
#include "PageStackComponent.h"

PageStackComponent::PageStackComponent()
{
    setInterceptsMouseClicks(false, true);
    setWantsKeyboardFocus(false);
#    if JUCE_DEBUG
    setName("PageStackComponent");
#    endif
}

/**
 * Pushes a new PageComponent on top of the stack, optionally animating
 * the transition. 
 */
void PageStackComponent::pushPage(PageComponent* page,
        PageComponent::Animation animation)
{
    DBG("PageStackComponent::" << __func__ << ": pushing " << page->getName());
    stack.add(page);
    transitionPage(page, animation, transitionDurationMS);
}

/**
 * Removes the top page from the stack, optionally animating the 
 * transition.  This will not remove the last page from the stack.
 */
void PageStackComponent::popPage(PageComponent::Animation animation)
{
    if (stack.size() > 1)
    {
        PageComponent* page = stack.getLast();
        page->setEnabled(false);

        transitionPage(page, animation, transitionDurationMS,
                [this](PageComponent * page)
                {
                    removeChildComponent(page);
                    stack.removeObject(page);
                },
        false);
    }
}

/**
 * Checks if a page is the top page on the stack.
 */
bool PageStackComponent::isTopPage(PageComponent* page)
{
    int i = stack.indexOf(page);
    return page == stack.getLast();
}

/**
 * Sets the current Page bounds to match the page stack.
 */
void PageStackComponent::resized()
{
    if (!stack.isEmpty())
    {
        stack.getLast()->setBounds(getBounds());
    }
}

/**
 * Animate a page as it is added to or removed from the stack.  If the page
 * is being added, this will add it to the PageStackComponent, make it 
 * visible, and set its initial bounds.  The page will be disabled while
 * animating, and re-enabled once animation finishes.
 */
void PageStackComponent::transitionPage(PageComponent* page,
        PageComponent::Animation animation,
        int duration,
        std::function<void(PageComponent*) > postAnimation,
        bool addingPage)
{
    if (addingPage)
    {
        addAndMakeVisible(page);
    }
    Rectangle<int> translatedBounds;
    page->setEnabled(false);
    int dir = -1;
    switch (animation)
    {
        case PageComponent::Animation::none:
            if (addingPage)
            {
                page->setBounds(getLocalBounds());
            }
            page->setEnabled(true);
            postAnimation(page);
            return;
        case PageComponent::Animation::slideInFromRight:
            dir *= -1;
        case PageComponent::Animation::slideInFromLeft:
            translatedBounds = getLocalBounds().translated(getWidth() * dir, 0);
    }
    if (addingPage)
    {
        page->setBounds(translatedBounds);
    }
    Desktop::getInstance().getAnimator().animateComponent(page,
            addingPage ? getLocalBounds() : translatedBounds,
            1, duration, true, 0.2, 1);
    TempTimer::initTimer(duration * 1.2, [this, postAnimation, page]()
    {
        page->setEnabled(true);
        postAnimation(page);
    });
}
