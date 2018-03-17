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
 * @return true iff the page is currently on a page stack.
 */
bool PageStackComponent::Page::isOnPageStack()
{
    return pageStack != nullptr;
}

/**
 * If this page is currently on a page stack, this will remove it from
 * the stack.
 */
void PageStackComponent::Page::removeFromStack(Transition transition)
{
    if (isOnPageStack())
    {
        if (pageStack->getCurrentPage() == this)
        {
            pageStack->popPage(transition);
        }
        else
        {
            pageStack->stack.removeAllInstancesOf(this);
        }
    }
}

/**
 * If this page is on a page stack, this will push a new page
 * on top of the stack.
 */
void PageStackComponent::Page::pushPageToStack(Page* newPage, Transition transition)
{

    if (isOnPageStack() && !newPage->isOnPageStack())
    {
        pageStack->pushPage(newPage, transition);
    }
}

/**
 * Add a page to the page stack.
 */
void PageStackComponent::pushPage(Page *page, Transition transition)
{
    auto bounds = getLocalBounds();
    if (!stack.isEmpty())
    {
        transitionOut(stack.getLast(), transition, transitionDurationMillis);
        stack.getLast()->pageCoveredOnStack();
    }
    stack.add(page);
    page->pageStack = this;
    transitionIn(page, transition, transitionDurationMillis);
    page->pageAddedToStack();
}

/**
 * Removes the top page from the page stack.
 */
void PageStackComponent::popPage(Transition transition)
{
    const ScopedLock lock(stackLock);
    if (!stack.isEmpty())
    {
        transitionOut(stack.getLast(), transition, transitionDurationMillis,
                true);
        stack.getLast()->pageRemovedFromStack();
        stack.getLast()->pageStack = nullptr;
        stack.removeLast();
        if (!stack.isEmpty())
        {
            transitionIn(stack.getLast(), transition, transitionDurationMillis,
                    true);
            stack.getLast()->pageRevealedOnStack();
        }
    }
}

/**
 * @return a pointer to the top page on the stack, or nullptr if the stack
 * is empty.
 */
PageStackComponent::Page *PageStackComponent::getCurrentPage()
{
    const ScopedLock lock(stackLock);
    return stack.getLast();
}

/**
 * Sets the current Page bounds to match the page stack.
 */
void PageStackComponent::resized()
{
    const ScopedLock lock(stackLock);
    if (!stack.isEmpty())
    {
        stack.getLast()->setBounds(getBounds());
    }
}

/**
 * Animate a page as it is added to the stack.
 */
void PageStackComponent::transitionIn(Page *page, Transition transition,
        int durationMillis, bool reverse)
{
    addAndMakeVisible(page);
    int dir = reverse ? -1 : 1;
    const Rectangle<int>& bounds = getLocalBounds();
    switch (transition)
    {
        case kTransitionTranslateHorizontalLeft:
            dir *= -1;
        case kTransitionTranslateHorizontal:

            page->setBounds(bounds.translated(bounds.getWidth() * dir, 0));
            animateTranslation(page, 0, 0, durationMillis);
            break;

        default:
            page->setBounds(bounds);
            page->setVisible(true);
    }
    page->setEnabled(true);
}

void PageStackComponent::transitionOut(Page *page, Transition transition,
        int durationMillis, bool reverse)
{
    int dir = reverse ? 1 : -1;
    switch (transition)
    {
        case kTransitionTranslateHorizontalLeft:
            dir *= -1;
        case kTransitionTranslateHorizontal:
            animateTranslation(page, getWidth() * dir, 0,
                    durationMillis);
    }
    page->setEnabled(false);
    TempTimer::initTimer(durationMillis, [this, page]()
    {
        removeChildComponent(page);
    });
}

/**
 * Animate any page translation
 */
void PageStackComponent::animateTranslation(Page* page, int startX,
        int startY, int durationMillis)
{
    const Rectangle<int>& bounds = page->getBounds();
    auto destBounds = bounds.translated(startX - bounds.getX(),
            startY - bounds.getY());
    Desktop::getInstance().getAnimator().animateComponent(page, destBounds,
            1, durationMillis, true, 0, 0);
}
