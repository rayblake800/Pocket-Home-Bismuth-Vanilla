#pragma once
#include "PageComponent.h"
#include "Layout_Transition_Type.h"
#include "JuceHeader.h"

/**
 * @file PageStackComponent.h
 * 
 * PageStackComponent implements PageComponent::PageStackInterface to manage
 * the stack of open pages.  It takes ownership of new pages, and allows them
 * to add new pages above themselves or remove and delete themselves from the
 * top of the stack.
 * 
 * @see PageComponent.h
 */

class PageStackComponent : public PageComponent::PageStackInterface,
public juce::Component
{
public:
    PageStackComponent();

    virtual ~PageStackComponent() { }

private:

    /**
     * Pushes a new PageComponent on top of the stack, optionally animating
     * the transition. 
     * 
     * @param page        The new page to display.
     * 
     * @param transition  The transition animation type to apply to the added
     *                    page.
     */
    void pushPage(PageComponent* page, Layout::Transition::Type transition 
                = Layout::Transition::Type::moveLeft) override;

    /**
     * Removes the top page from the stack, optionally animating the 
     * transition.  This will not remove the last page from the stack.
     * 
     * @param transition  The transition animation type to apply to the removed
     *                    page.
     */
    void popPage(Layout::Transition::Type transition 
                = Layout::Transition::Type::moveRight) override;


    /**
     * Checks if a page is the top page on the stack.
     * 
     * @param  page
     * 
     * @return true iff page is displayed on top of the page stack.
     */
    bool isTopPage(PageComponent* page) override;

private:

    /**
     * Sets the current Page bounds to match the page stack component.
     */
    void resized() override;

    /**
     * Animate a page as it is added to or removed from the stack.  If the page
     * is being added, this will add it to the PageStackComponent, make it 
     * visible, and set its initial bounds.  The page will be disabled while
     * animating, and re-enabled once animation finishes.
     * 
     * @param page           The pageComponent being added or removed.
     * 
     * @param transition     Type of transition animation to show.
     * 
     * @param duration       Animation duration in milliseconds.
     * 
     * @param postAnimation  Callback function to run when animation finishes.
     * 
     * @param addingPage     True if the page is being added, false if it's 
     *                        being removed.
     */
    void transitionPage(PageComponent* page,
            Layout::Transition::Type transition,
            int duration,
            std::function<void(PageComponent*) > postAnimation = []
            (PageComponent* page)
            {
            },
    bool addingPage = true);

    //page transition animation duration in milliseconds
    const int transitionDurationMS = 300;

    //holds all pages in order
    juce::OwnedArray<PageComponent> stack;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageStackComponent)
};
