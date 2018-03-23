/**
 * @file PageStackComponent.h
 * 
 * PageStackComponent implements PageComponent::PageStackInterface to manage
 * the stack of open pages.  It takes ownership of new pages, and allows them
 * to add new pages above themselves or remove and delete themselves from the
 * stack.
 * 
 * @see PageComponent.h
 */
#pragma once
#include "PageComponent.h"
#include "JuceHeader.h"

class PageStackComponent : public PageComponent::PageStackInterface,
public Component
{
public:
    PageStackComponent();

    virtual ~PageStackComponent() { }

    enum Transition
    {
        kTransitionNone,
        kTransitionTranslateHorizontal,
        kTransitionTranslateHorizontalLeft
    };

private:

    /**
     * Pushes a new PageComponent on top of the stack, optionally animating
     * the transition. 
     * 
     * @param page
     * 
     * @param animation
     */
    void pushPage(PageComponent* page,
            PageComponent::Animation animation
            = PageComponent::slideInFromLeft) override;

    /**
     * Removes the top page from the stack, optionally animating the 
     * transition.  This will not remove the last page from the stack.
     * 
     * @param animation
     */
    void popPage(PageComponent::Animation animation
            = PageComponent::slideInFromLeft) override;


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
     * Sets the current Page bounds to match the page stack.
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
     * @param animation      Type of animation to show.
     * 
     * @param duration       Animation duration in milliseconds.
     * 
     * @param postAnimation  Callback function to run when animation finishes.
     * 
     * @param addingPage     True if the page is being added, false if it's 
     *                        being removed.  Animation direction is reversed 
     *                        when pages are removed.
     */
    void transitionPage(PageComponent* page,
            PageComponent::Animation animation,
            int duration,
            std::function<void(PageComponent*) > postAnimation = []
            (PageComponent* page)
            {
            },
    bool addingPage = true);

    //page transition animation duration in milliseconds
    const int transitionDurationMS = 500;

    //holds all pages in order
    OwnedArray<PageComponent> stack;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageStackComponent)
};
