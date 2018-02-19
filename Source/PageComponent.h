/**
 * @file PageComponent.h
 * 
 * TODO: everything
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

    PageComponent(const String& name = String(),
            std::vector<GridLayoutManager::RowLayoutParams> layout = {},
            bool showBackButton = false,
            bool backButtonOnRight = false);

    virtual ~PageComponent();
    
    void setBackgroundImage(Image bgImage);
    
    void addAndShowLayoutComponents();

protected:
    virtual void pageButtonClicked(Button* button){};
    virtual void pageResized(){};
    void updateLayout(std::vector<GridLayoutManager::RowLayoutParams> layout);
    void setMarginFraction(float marginFraction);
    void setPadding(float verticalFraction,float horizontalFraction);
private:
    bool backButtonOnRight;
    void resized() final override;
    void buttonClicked(Button* button) final override;
    virtual void paint(Graphics& g) override;
    GridLayoutManager layoutManager;
    float marginFraction = 0.05;
    float verticalPadding = 0.05;
    float horizontalPadding = 0.05;
    ScopedPointer<ConfigurableImageButton> backButton = nullptr;
    ScopedPointer<Image> backgroundImage = nullptr;
};