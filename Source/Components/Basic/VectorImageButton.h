/*
  ==============================================================================

    VectorImageButton.h
    Created: 28 Dec 2017 9:40:22pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../ConfigFile.h"

class VectorImageButton : public TextButton {
public:
    VectorImageButton(ConfigFile::ComponentSettings settings, String title);
    virtual ~VectorImageButton();
    
    int getImageCount();
    void setImage(int imageIndex);
private:
    void resized() override;
    void paint (Graphics& g) override;
    void loadSVG(String svgFilename);
    ConfigFile::ComponentSettings buttonSettings;
    ScopedPointer<Drawable>image;
    static const std::vector<Colour> defaultColours;

    static const std::vector<Colour> loadDefaultColours() {
        std::vector<Colour> defaults;
        defaults.push_back(Colours::white);
        defaults.push_back(Colours::black);
        defaults.push_back(Colours::red);
        defaults.push_back(Colours::lime);
        return defaults;
    };
};