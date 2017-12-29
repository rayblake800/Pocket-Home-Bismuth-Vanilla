/*
  ==============================================================================

    VectorImageButton.cpp
    Created: 28 Dec 2017 9:40:22pm
    Author:  anthony

  ==============================================================================
 */

#include "VectorImageButton.h"
#include "../../Utils.h"

const std::vector<Colour> VectorImageButton::defaultColours =
        VectorImageButton::loadDefaultColours();

VectorImageButton::VectorImageButton(ConfigFile::ComponentSettings settings,
        String title) : TextButton(title), buttonSettings(settings) {
    buttonSettings.applyBounds(this);
    setImage(0);
}

VectorImageButton::~VectorImageButton() {
    image=nullptr;
}

int VectorImageButton::getImageCount() {
    return buttonSettings.getAssetFiles().size();
}

void VectorImageButton::setImage(int imageIndex) {
    if (imageIndex >= 0 && imageIndex < getImageCount()) {
        loadSVG(buttonSettings.getAssetFiles()[imageIndex]);
    }
    repaint();
}

void VectorImageButton::resized() {
    if(image!=nullptr){
        image->setTransformToFit(getBounds().withPosition(0,0).toFloat(),RectanglePlacement::centred);
    }
}

void VectorImageButton::paint(Graphics& g) {
}

void VectorImageButton::loadSVG(String svgFilename) {
    File svgFile = assetFile(svgFilename);
    if (!svgFile.exists()) {
        return;
    }
    ScopedPointer<XmlElement> svgElement = XmlDocument::parse(svgFile);
    image = Drawable::createFromSVG(*svgElement);
    std::vector<Colour> colours=buttonSettings.getColours();
    for(int i=0;i<colours.size();i++){
        image->replaceColour(defaultColours[i],colours[i]);
    }
    addAndMakeVisible(image);
    image->setTransformToFit(getBounds().withPosition(0,0).toFloat(),RectanglePlacement::centred);
    image->setWantsKeyboardFocus(false);
}
