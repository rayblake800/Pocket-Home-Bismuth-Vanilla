#include "ComponentConfigFile.h"
#include "ColourConfigFile.h"
#include "ColourConfigKeys.h"
#include "MainConfigKeys.h"
#include "SwitchComponent.h"
#include "DrawableImageComponent.h"
#include "ListEditor.h"
#include "FileSelectTextEditor.h"
#include "OverlaySpinner.h"
#include "AppMenuButton.h"
#include "ScalingTextButton.h"
#include "PokeLookAndFeel.h"

PokeLookAndFeel::PokeLookAndFeel() :
seguibl(juce::Typeface::createSystemTypefaceFor(BinaryData::LatoRegular_ttf,
BinaryData::LatoRegular_ttfSize)),
cursor(juce::MouseCursor::NoCursor),
mainListener(*this)
{
    using namespace juce;
    const Array<int>& colourIds = ColourConfigKeys::getColourIds();
    for(const int& id : colourIds)
    {
        addTrackedColourId(id);
    }
    const Array<Identifier>& categoryKeys = ColourConfigKeys::getCategoryKeys();
    for(const Identifier& key : categoryKeys)
    {
        addTrackedKey(key);
    }
    DBG("PokeLookAndFeel::PokeLookAndFeel: Tracking " << colourIds.size()
            << " color id values, " << categoryKeys.size()
            << " category keys.");
    loadAllConfigProperties();
    mainListener.loadSettings();
}

/*
 * Get the appropriate typeface for the given font.
 */
juce::Typeface::Ptr PokeLookAndFeel::getTypefaceForFont(const juce::Font &font)
{
    return seguibl;
}

/*
 * Draws the thumb portion of a linear Slider.
 */
void PokeLookAndFeel::drawLinearSliderThumb(
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider &slider)
{
    using namespace juce;
    const float radius = getSliderThumbRadius(slider);
    g.setColour(slider.findColour(Slider::thumbColourId));
    float kx, ky;
    if (style == Slider::LinearVertical)
    {
        kx = x + width * 0.5f;
        ky = sliderPos;
    }
    else
    {
        kx = sliderPos;
        ky = y + height * 0.5f;
    }

    Path circle;
    circle.addCentredArc(kx, ky, radius, radius, 0.0f, 0.0f, M_PI * 2.0f);
    circle.closeSubPath();
    g.fillPath(circle);
}

/*
 * Draws the background of a linear slider.
 */
void PokeLookAndFeel::drawLinearSliderBackground(
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider &slider)
{
    using namespace juce;
    const float radius = std::floor(getSliderThumbRadius(slider) * 0.333f);
    g.setColour(slider.findColour(Slider::backgroundColourId));
    Path indent;
    if (slider.isHorizontal())
    {
        const float iy = y + height * 0.5f - radius;
        indent.addRoundedRectangle(
                x - radius,
                iy,
                width + radius * 2.0f,
                radius * 2.0f, 1);
    }
    else
    {
        const float ix = x + width * 0.5f - radius;
        indent.addRoundedRectangle(
                ix,
                y - radius,
                radius * 2.0f,
                height + radius * 2.0f,
                1);
    }
    g.fillPath(indent);
}

/*
 * Draws the entire linear slider component.
 */
void PokeLookAndFeel::drawLinearSlider(
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider &slider)
{
    using namespace juce;
    drawLinearSliderBackground(g, x, y, width, height,
            sliderPos, minSliderPos, maxSliderPos, style,
            slider);
    drawLinearSliderThumb(g, x, y, width, height, sliderPos,
            minSliderPos, maxSliderPos, style,
            slider);
}

/*
 * Defines the radius in pixels of the slider thumb.
 */
int PokeLookAndFeel::getSliderThumbRadius(juce::Slider &slider)
{
    return juce::jmin(14, slider.getHeight() / 2, slider.getWidth() / 2);
}

/*
 * Draws the text onto a TextButton.
 */
void PokeLookAndFeel::drawButtonText(
        juce::Graphics &g, 
        juce::TextButton &button,
        bool isMouseOverButton,
        bool isButtonDown)
{
    using namespace juce;

    int yIndent = jmin(4, button.proportionOfHeight(0.3f));
    int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

    int leftIndent = 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2);
    int rightIndent = 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2);
    Font font(getTextButtonFont(button, button.getHeight()));
    font.setExtraKerningFactor(0.06f);
    Rectangle<int> textBounds = button.getLocalBounds();
    textBounds.setWidth(textBounds.getWidth() - leftIndent - rightIndent);
    textBounds.setHeight(textBounds.getHeight() - 2 * yIndent);
    ComponentConfigFile componentConfig;
    int fontHeight = componentConfig.getFontHeight
            (textBounds, button.getButtonText());
    //Check if the TextButton is actually a ScalingTextButton with a maximum
    //height scale set.
    ScalingTextButton* scalingBtn = dynamic_cast<ScalingTextButton*>(&button);
    if(scalingBtn != nullptr 
       && scalingBtn->getMaxTextScale() != ComponentConfigFile::largeText)
    {
        fontHeight = std::min(fontHeight,
                componentConfig.getFontHeight(scalingBtn->getMaxTextScale()));
    }
    font.setHeight(fontHeight);
    g.setFont(font);
    
    Colour buttonColour = button.findColour(button.getToggleState() ?
            TextButton::textColourOnId : TextButton::textColourOffId);
    if (!button.isEnabled())
    {
        buttonColour = buttonColour.withMultipliedAlpha(0.5f);
    }
    if (isButtonDown && isMouseOverButton)
    {
        buttonColour = buttonColour.darker();
    }
    g.setColour(buttonColour);

    g.drawFittedText(button.getButtonText(), leftIndent, yIndent,
            button.getWidth() - leftIndent - rightIndent,
            button.getHeight() - yIndent * 2,
            Justification::centred, 2);
}

/*
 * Draws the background of a Button component.
 */
void PokeLookAndFeel::drawButtonBackground(
        juce::Graphics &g, 
        juce::Button &button,
        const juce::Colour &backgroundColour, 
        bool isMouseOverButton,
        bool isButtonDown)
{
    using namespace juce;
    const int width = button.getWidth();
    const int height = button.getHeight();

    auto path = Path();
    path.addRoundedRectangle(0, 0, width, height, 1);
    Colour buttonColour = (button.getToggleState() ?
                           findColour(TextButton::ColourIds::buttonOnColourId) :
                           findColour(TextButton::ColourIds::buttonColourId));
    if (isButtonDown && isMouseOverButton)
    {
        buttonColour = buttonColour.darker();
    }
    g.setColour(buttonColour);
    g.fillPath(path);
}

/*
 * Get the cursor to display over a given component.
 */
juce::MouseCursor PokeLookAndFeel::getMouseCursorFor(juce::Component &component)
{
    return cursor;
}

/**
 * Gets the default font to use for popup menu text.
 */
juce::Font PokeLookAndFeel::getPopupMenuFont()
{
    using namespace juce;
    ComponentConfigFile config;
    return Font(config.getFontHeight(ComponentConfigFile::mediumText));
}

/**
 * Gets the default font to use for combo box text.
 */  
juce::Font PokeLookAndFeel::getComboBoxFont(juce::ComboBox& comboBox)
{
    using namespace juce;
    ComponentConfigFile config;
    int height = std::min(config.getFontHeight(ComponentConfigFile::mediumText),
            config.getFontHeight(comboBox.getLocalBounds(), comboBox.getText()));
    return Font(height);
}

/**
 * Gets the default font to use for Label components.
 */ 
juce::Font PokeLookAndFeel::getLabelFont(juce::Label& label)
{
    using namespace juce;
    ComponentConfigFile config;
    int height = std::min(config.getFontHeight(ComponentConfigFile::smallText),
            config.getFontHeight(label.getLocalBounds(), label.getText()));
    return Font(height);
}

/**
 * Gets the default font to use for alert window title text.
 */ 
juce::Font PokeLookAndFeel::getAlertWindowTitleFont()
{
    using namespace juce;
    ComponentConfigFile config;
    return Font(config.getFontHeight(ComponentConfigFile::largeText));
}
   
/**
 * Gets the default font to use for alert window message text.
 */ 
juce::Font PokeLookAndFeel::getAlertWindowMessageFont()
{
    using namespace juce;
    ComponentConfigFile config;
    return Font(config.getFontHeight(ComponentConfigFile::mediumText));
}

PokeLookAndFeel::MainListener::MainListener(PokeLookAndFeel& owner) :
owner(owner)
{
    addTrackedKey(MainConfigKeys::showCursorKey);
}

/*
 * Updates the cursor visibility when the associated config key is changed. 
 */
void PokeLookAndFeel::MainListener::configValueChanged
(const juce::Identifier& key)
{
    using namespace juce;
    if (key == MainConfigKeys::showCursorKey)
    {
        MainConfigFile config;
        owner.cursor = (config.getConfigValue<bool>(key) ?
                  MouseCursor::ParentCursor : MouseCursor::NoCursor);
    }
}
 
/**
 * Updates Component colours when they're changed in the ColourConfigFile.
 */
void PokeLookAndFeel::colourChanged(const int colourId,
        const juce::Identifier& colourKey, const juce::Colour newColour)
{
    DBG("Color init: ColorID=" << juce::String::toHexString(colourId)
            << ", key=" << colourKey.toString() 
            << ", color=" <<newColour.toDisplayString(true));
    setColour(colourId, newColour);
}
