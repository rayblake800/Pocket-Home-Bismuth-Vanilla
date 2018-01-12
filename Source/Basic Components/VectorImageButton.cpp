/*
  ==============================================================================

    VectorImageButton.cpp
    Created: 28 Dec 2017 9:40:22pm
    Author:  anthony

  ==============================================================================
 */

#include "../Utils.h"
#include "../PocketHomeApplication.h"
#include "VectorImageButton.h"

const std::vector<Colour> VectorImageButton::defaultColours =
        VectorImageButton::loadDefaultColours();

VectorImageButton::VectorImageButton(String componentKey, String title) :
Configurable(static_cast<ConfigFile*>
(&PocketHomeApplication::getInstance()->getComponentConfig()),{
    componentKey
}),
TextButton(title)
{
    loadAllConfigProperties();
    setWantsKeyboardFocus(false);
    DBG(String("Created button ") + title + String(" at ")
            + getBounds().toString());
}

VectorImageButton::~VectorImageButton()
{
    images.clear(true);
}

int VectorImageButton::getImageCount()
{
    return buttonSettings.getAssetFiles().size();
}

void VectorImageButton::setImage(int newImageIndex)
{
    if (newImageIndex >= 0
            && newImageIndex < images.size()
            && images[newImageIndex] != nullptr
            && !images[newImageIndex]->isVisible())
    {
        if (images[imageIndex] != nullptr)
        {
            images[imageIndex]->setVisible(false);
            removeChildComponent(images[imageIndex]);
        }
        addAndMakeVisible(images[newImageIndex]);
        images[newImageIndex]->setWantsKeyboardFocus(false);
        imageIndex = newImageIndex;
        resizeImage();
    }
}

/**
 * Receives notification whenever configuration values change
 */
void VectorImageButton::loadConfigProperties(ConfigFile * config, String key)
{
    ComponentConfigFile * compConfig =
            static_cast<ComponentConfigFile*> (config);
    ComponentConfigFile::ComponentSettings newSettings =
            compConfig->getComponentSettings(key);
    if (newSettings == buttonSettings)
    {
        return; //no change, so do nothing
    }

    std::vector<String> assets = newSettings.getAssetFiles();
    std::vector<Colour> colours = newSettings.getColours();

    if (assets != buttonSettings.getAssetFiles()
            || colours != buttonSettings.getColours())
    {
        images.clear(true);
        for (int i = 0; i < assets.size(); i++)
        {
            String filename = assets[i];
            File svgFile = assetFile(filename);
            if (!svgFile.existsAsFile())
            {
                DBG(String("VectorImageButton:") + filename + " not found!");
                images.add(nullptr);
                continue;
            }
            Drawable * svgDrawable = createSVGDrawable(svgFile);
            if (svgDrawable == nullptr)
            {
                DBG(String("VectorImageButton:") + filename
                        + " couldn't convert to drawable!");
                images.add(nullptr);
                continue;
            }
            images.add(svgDrawable);
            for (int ci = 0; ci < colours.size(); ci++)
            {
                images[i]->replaceColour
                        (defaultColours[ci], colours[ci]);
            }
        }
    }
    Rectangle<int>bounds = newSettings.getBounds();
    bounds -= getScreenPosition();
    setBounds(bounds);

    buttonSettings = newSettings;
    setImage(imageIndex);
}

void VectorImageButton::resizeImage()
{
    if (images.size() > 0 && images[imageIndex] != nullptr)
    {
        Rectangle<int> imageBounds = buttonSettings.getBounds() - getPosition();
        images[imageIndex]->setTransformToFit(imageBounds.toFloat(),
                RectanglePlacement::centred);
        repaint();
    }
}

void VectorImageButton::resized()
{
    resizeImage();
}

void VectorImageButton::paint(Graphics& g)
{
}

