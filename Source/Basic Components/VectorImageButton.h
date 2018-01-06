/*
  ==============================================================================

    VectorImageButton.h
    Created: 28 Dec 2017 9:40:22pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "../Configuration/ComponentConfigFile.h"
#include "../Configuration/Configurable.h"

class VectorImageButton : public TextButton, public Configurable {
public:
    VectorImageButton(String componentKey, String title);
    virtual ~VectorImageButton();

    int getImageCount();
    void setImage(int newImageIndex);
protected:
    /**
     * Receives notification whenever configuration values change
     * @param config the component config file object
     * @param key should be the key for this object's component settings
     */
    void loadConfigProperties(ConfigFile * config,String key);
    void resizeImage();
private:
    void resized() override;
    void paint(Graphics& g) override;
    ComponentConfigFile::ComponentSettings buttonSettings;
    OwnedArray<Drawable>images;
    int imageIndex = 0;
    static const std::vector<Colour> defaultColours;

    static const std::vector<Colour> loadDefaultColours() {
        std::vector<Colour> defaults;
        defaults.push_back(Colour(0xff, 0xff, 0xff));
        defaults.push_back(Colour(0x00, 0x00, 0x00));
        defaults.push_back(Colour(0xff, 0x00, 0x00));
        defaults.push_back(Colour(0x00, 0xff, 0x00));
        return defaults;
    };
};