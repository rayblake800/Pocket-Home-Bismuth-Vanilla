/**
 * @file PageStackComponent.h
 * 
 * TODO: documentation, organization
 */
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
class PageStackComponent : public Component {
public:

    PageStackComponent();
    ~PageStackComponent();

    class Page : public Component {
        friend class PageStackComponent;
    public:

        Page() {
        };

        virtual ~Page() {
        };
    protected:

        virtual void pageAddedToStack() {
        };

        virtual void pageRemovedFromStack() {
        };

        virtual void pageRevealedOnStack() {
        };

        virtual void pageCoveredOnStack() {
        };
    };
    
    enum Transition {
        kTransitionNone,
        kTransitionTranslateHorizontal,
        kTransitionTranslateHorizontalLeft
    };

    void pushPage(Page* page, Transition transition = kTransitionNone);
    void swapPage(Page* page, Transition transition = kTransitionNone);
    void popPage(Transition transition = kTransitionNone);
    void insertPage(Page* page, int idx);
    void removePage(int idx);
    void removePage(Page* page);
    void clear(Transition transition);

    int getDepth() const;

    Page *getCurrentPage();
private:
    void resized() override;
    const int transitionDurationMillis = 200;

    Array<Page *> stack;

    void transitionIn(Page *page, Transition transition, int durationMillis,
            bool reverse = false);
    void transitionOut(Page *page, Transition transition, int durationMillis,
            bool reverse = false);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageStackComponent)
};
