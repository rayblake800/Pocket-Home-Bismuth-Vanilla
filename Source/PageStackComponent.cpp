#include "Utils.h"
#include "PageStackComponent.h"

PageStackComponent::PageStackComponent()
{
    setInterceptsMouseClicks(false, true);
    setWantsKeyboardFocus(false);
}

PageStackComponent::~PageStackComponent() { }

void PageStackComponent::resized()
{
    if (!stack.isEmpty())
    {
        stack.getLast()->setBounds(getBounds());
    }
}

bool PageStackComponent::Page::isOnPageStack()
{
    return pageStack != nullptr;
}

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

void PageStackComponent::Page::pushPageToStack(Page* newPage, Transition transition)
{

    if (isOnPageStack() && !newPage->isOnPageStack())
    {
        pageStack->pushPage(newPage, transition);
    }
}

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
    page->pageAddedToStack();
    transitionIn(page, transition, transitionDurationMillis);
}

/**
 * Removes the top page from the page stack.
 */
void PageStackComponent::popPage(Transition transition)
{
    if (!stack.isEmpty())
    {
        transitionOut(stack.getLast(), transition, transitionDurationMillis, true);
        stack.getLast()->pageRemovedFromStack();
        stack.getLast()->pageStack = nullptr;
        stack.removeLast();
        if (!stack.isEmpty())
        {
            transitionIn(stack.getLast(), transition, transitionDurationMillis, true);
            stack.getLast()->pageRevealedOnStack();
        }
    }
}

/**
 * The PocketHomeWindow that owns this PageStackComponent uses this method
 * to notify the PageStackComponent when window focus changes, so that
 * it can pass the notification on to all pages in the page stack.
 */
void PageStackComponent::windowFocusChanged(bool windowFocused) {
    for(Page* page : stack)
    {
        page->windowFocusChanged(windowFocused);
    }
}

void PageStackComponent::transitionIn(Page *page, Transition transition,
        int durationMillis, bool reverse)
{
    addAndMakeVisible(page);
    auto bounds = getLocalBounds();
    switch (transition)
    {
        case kTransitionTranslateHorizontal:
        {
            float dir = reverse ? -1.0f : 1.0f;
            page->setBounds(bounds.translated(bounds.getWidth() * dir, 0));
            animateTranslation(page, 0, 0, 1.0f, durationMillis);
            break;
        }
        case kTransitionTranslateHorizontalLeft:
        {
            float dir = reverse ? 1.0f : -1.0f;
            page->setBounds(bounds.translated(bounds.getWidth() * dir, 0));
            animateTranslation(page, 0, 0, 1.0f, durationMillis);
        }
            break;
        default:
        {
            page->setBounds(bounds);
            page->setVisible(true);
        }
    }

    page->setEnabled(true);
}

void PageStackComponent::transitionOut(Page *page, Transition transition,
        int durationMillis, bool reverse)
{
    switch (transition)
    {
        case kTransitionTranslateHorizontal:
        {
            auto bounds = getLocalBounds();
            float dir = reverse ? 1.0f : -1.0f;
            animateTranslation(page, bounds.getWidth() * dir, 0, 1.0f, durationMillis);
            break;
        }
        case kTransitionTranslateHorizontalLeft:
        {
            auto bounds = getLocalBounds();
            float dir = reverse ? -1.0f : 1.0f;
            animateTranslation(page, bounds.getWidth() * dir, 0, 1.0f, durationMillis);
            break;
        }
            break;
        default:
        {
        }
    }

    page->setEnabled(false);
    removeChildComponent(page);
}

PageStackComponent::Page *PageStackComponent::getCurrentPage()
{
    return stack.getLast();
}
