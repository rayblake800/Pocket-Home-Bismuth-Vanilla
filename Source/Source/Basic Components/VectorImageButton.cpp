/*
  ==============================================================================

    VectorImageButton.cpp
    Created: 28 Dec 2017 9:40:22pm
    Author:  anthony

  ==============================================================================
 */

#include "../Utils.h"
#include "VectorImageButton.h"

const std::vector<Colour> VectorImageButton::defaultColours =
        VectorImageButton::loadDefaultColours();

VectorImageButton::VectorImageButton(ConfigFile::ComponentSettings settings,
        String title) : TextButton(title), buttonSettings(settings) {
    buttonSettings.applyBounds(this);
    DBG(String("Created button ") + title + String(" at ") + getBounds().toString());
    setImage(0);
}

VectorImageButton::~VectorImageButton() {
}

int VectorImageButton::getImageCount() {
    return buttonSettings.getAssetFiles().size();
}

void VectorImageButton::setImage(int imageIndex) {
    if (this->imageIndex == imageIndex) {
        return;
    }
    if (imageIndex >= 0 && imageIndex < getImageCount()) {
        if (images[imageIndex] != nullptr) {
            this->imageIndex = imageIndex;
        } else {
            String filename = buttonSettings.getAssetFiles()[imageIndex];
            File svgFile = assetFile(filename);
            if (!svgFile.existsAsFile()) {
                DBG(String("VectorImageButton:") + filename + " not found!");
                return;
            }
            images.insert(imageIndex, createSVGDrawable(svgFile));
            if (images[imageIndex] == nullptr) {
                DBG(String("VectorImageButton:") + filename + " couldn't convert to drawable!");
                return;
            }
            std::vector<Colour> colours = buttonSettings.getColours();
            for (int i = 0; i < colours.size(); i++) {
                images[imageIndex]->replaceColour(defaultColours[i], colours[i]);
            }

        }
        removeChildComponent(images[this->imageIndex]);
        this->imageIndex = imageIndex;
        addAndMakeVisible(images[imageIndex]);
        images[imageIndex]->setWantsKeyboardFocus(false);
        resizeImage();
    }
}

void VectorImageButton::resizeImage() {
    if (images[imageIndex] != nullptr) {
        Rectangle<int> imageBounds=buttonSettings.getBounds()-getPosition();
        images[imageIndex]->setTransformToFit(imageBounds.toFloat(), 
                RectanglePlacement::centred);
        repaint();
    }
}

void VectorImageButton::resized() {
    resizeImage();
}

void VectorImageButton::paint(Graphics& g) {
}

