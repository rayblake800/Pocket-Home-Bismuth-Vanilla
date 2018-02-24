/**
 * @file PageComponent.h
 * 
 * PageComponent is a container component that organizes and manages one screen
 * of the program UI.
 * 
 * @see PageStackComponent.h
 */
#pragma once

#include "PageStackComponent.h"
#include "GridLayoutManager.h"
#include "Configuration/Configurables/ConfigurableImageButton.h"
#include "../JuceLibraryCode/JuceHeader.h"

class PageComponent : public PageStackComponent::Page, public Button::Listener {
public:

    enum ColourIds {
        backgroundColourId = 0x1900500
    };

    /**
     * @param name internal component name
     * 
     * @param layout defines the layout of all page components.  This layout
     * will be saved, but components will not be added to the page until
     * addAndShowLayoutComponents is called.
     * 
     * @param showBackButton If true, add a back button on this page.
     * 
     * @param backButtonOnRight  If true, the back button will be on the right
     * side of the page, otherwise it will be on the left.
     */
    PageComponent(const String& name = String(),
            GridLayoutManager::Layout layout = {},
            bool showBackButton = false,
            bool backButtonOnRight = false);

    virtual ~PageComponent();
    
    /**
     * Sets a background image to draw behind all page components.
     * 
     * @param bgImage the image to draw, or Image::null.  If the null
     * image is used, the background will be filled with the background color
     * instead of an image.
     */
    void setBackgroundImage(Image bgImage);
    
    /**
     * Adds all components in the layout to the page and makes them visible.
     */
    void addAndShowLayoutComponents();

protected:
    /**
     * Handles any buttons besides the back button.  Inheriting classes
     * should override this instead of buttonClicked(Button*) to handle their 
     * own button actions.
     * @param button
     */
    virtual void pageButtonClicked(Button* button){};
    
    /**
     * Handles any actions necessary whenever the page is resized, besides
     * updating the layout manager and back button.  Inheriting classes should
     * override this instead of resized() to handle page resizing.
     */
    virtual void pageResized(){};
    
    /**
     * Replaces the page layout.  All components in the old layout will be
     * removed from the page before setting the new layout.  Components in
     * the new layout will be added to the page and made visible.
     * @param layout
     */
    void updateLayout(GridLayoutManager::Layout layout);
    
    /**
     * Sets the amount of space to leave between page components and the edges
     * of the page.
     * 
     * @param marginFraction this value is multiplied by the page height to
     * get the margin size in pixels.
     */
    void setMarginFraction(float marginFraction);
    
    /**
     * Sets the amount of space to leave between components in the page layout.
     *
     * @param verticalFraction this value is multiplied by the page height to
     * find the amount of vertical empty space (in pixels) to leave between 
     * components.
     
     * @param horizontalFraction  this value is multiplied by the page width to
     * find the amount of horizontal empty space (in pixels) to leave between 
     * components.
     */
    void setPadding(float verticalFraction,float horizontalFraction);
private:
    /**
     * Inheriting classes can override this method to change the behavior of the
     * back button. It will be called every time the back button is clicked, and
     * if it returns true, the back button will not remove the page.
     * 
     * @return true if the back button's action was replaced, false to allow
     * the back button to remove the page as usual.
     */
    virtual bool overrideBackButton(); 
    
    /**
     * Recalculate component layout and back button bounds when the page is
     * resized.
     */
    void resized() final override;
    
    /**
     * Closes the page when the back button is clicked, and passes all other
     * button clicks to the pageButtonClicked method.
     * @param button
     */
    void buttonClicked(Button* button) final override;
    
    /**
     * Fills the page background with an image or color.
     */
    virtual void paint(Graphics& g) override;
    
    bool backButtonOnRight;
    GridLayoutManager layoutManager;
    float marginFraction = 0.05;
    float verticalPadding = 0.05;
    float horizontalPadding = 0.05;
    ScopedPointer<ConfigurableImageButton> backButton = nullptr;
    Image backgroundImage;
};