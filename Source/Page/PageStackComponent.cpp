#include "Utils.h"
#include "TempTimer.h"
#include "Layout_Transition_Animator.h"
#include "PageStackComponent.h"

PageStackComponent::PageStackComponent()
{
    setInterceptsMouseClicks(false, true);
    setWantsKeyboardFocus(false);
#    if JUCE_DEBUG
    setName("PageStackComponent");
#    endif
}

/*
 * Pushes a new PageComponent on top of the stack, optionally animating
 * the transition. 
 */
void PageStackComponent::pushPage(PageComponent* page,
        Layout::Transition::Type transition)
{
    DBG("PageStackComponent::" << __func__ << ": pushing " << page->getName());
    stack.add(page);
    signalPageAdded(page);
    transitionPage(page, transition, transitionDurationMS);
}

/*
 * Removes the top page from the stack, optionally animating the 
 * transition.  This will not remove the last page from the stack.
 */
void PageStackComponent::popPage(Layout::Transition::Type transition)
{
    if (stack.size() > 1)
    {
        PageComponent* page = stack.getLast();
        page->setEnabled(false);

        transitionPage(page, transition, transitionDurationMS,
        [this](PageComponent * page)
        {
            removeChildComponent(page);
            stack.removeObject(page);
            signalPageRevealed(stack.getLast());
        },
        false);
    }
}

/*
 * Checks if a page is the top page on the stack.
 */
bool PageStackComponent::isTopPage(PageComponent* page)
{
    int i = stack.indexOf(page);
    return page == stack.getLast();
}

/*
 * Sets the current Page bounds to match the page stack.
 */
void PageStackComponent::resized()
{
    for(PageComponent* page : stack)
    {
        page->setBounds(getBounds());
    }
}

/*
 * Animate a page as it is added to or removed from the stack.  If the page
 * is being added, this will add it to the PageStackComponent, make it 
 * visible, and set its initial bounds.  The page will be disabled while
 * animating, and re-enabled once animation finishes.
 */
void PageStackComponent::transitionPage(PageComponent* page,
        Layout::Transition::Type transition,
        int duration,
        std::function<void(PageComponent*) > postAnimation,
        bool addingPage)
{
    if (addingPage)
    {
        addAndMakeVisible(page);
    }
    page->setEnabled(false);
    if(addingPage)
    {
        Layout::Transition::Animator::transitionIn(page, transition,
                getLocalBounds(), duration);
    }
    else
    {
        Layout::Transition::Animator::transitionOut(page, transition, duration,
                true);
    }
    TempTimer::initTimer(duration * 1.2, [this, postAnimation, page]()
    {
        page->setEnabled(true);
        postAnimation(page);
    });
}
