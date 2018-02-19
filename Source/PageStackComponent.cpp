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

int PageStackComponent::getDepth() const
{
    return stack.size();
}

void PageStackComponent::pushPage(Page *page, Transition transition)
{
    auto bounds = getLocalBounds();
    if (!stack.isEmpty())
    {
        transitionOut(stack.getLast(), transition, transitionDurationMillis);
    }
    stack.add(page);
    transitionIn(page, transition, transitionDurationMillis);
}

void PageStackComponent::swapPage(Page *page, Transition transition)
{
    if (!stack.isEmpty())
    {
        transitionOut(stack.getLast(), transition, transitionDurationMillis);
        stack.removeLast();
    }
    stack.add(page);
    transitionIn(page, transition, transitionDurationMillis);
}

void PageStackComponent::popPage(Transition transition)
{
    if (!stack.isEmpty())
    {
        transitionOut(stack.getLast(), transition, transitionDurationMillis, true);
        stack.removeLast();
        if (!stack.isEmpty())
        {
            transitionIn(stack.getLast(), transition, transitionDurationMillis, true);
        }
    }
}

void PageStackComponent::insertPage(Page *page, int idx)
{
    stack.insert(idx, page);
}

void PageStackComponent::removePage(int idx)
{
    if (idx == stack.size() - 1)
    {
        popPage();
    }
    else
    {
        stack.remove(idx);
    }
}


void PageStackComponent::removePage(Page* page)
{
    int index = stack.indexOf(page);
    if(index >= 0)
    {
        removePage(index);
    }
}

void PageStackComponent::clear(Transition transition)
{
    if (!stack.isEmpty())
    {
        transitionOut(stack.getLast(), transition, transitionDurationMillis, true);
    }
    stack.clear();
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
