#include "ComponentConfigFile.h"
#include "MainConfigFile.h"
#include "SwitchComponent.h"
#include "DrawableImageComponent.h"
#include "ListEditor.h"
#include "FileSelectTextEditor.h"
#include "OverlaySpinner.h"
#include "AppMenuButton.h"
#include "PokeLookAndFeel.h"

PokeLookAndFeel::PokeLookAndFeel(MainConfigFile& mainConfig, 
        ComponentConfigFile& componentConfig) :
seguibl(Typeface::createSystemTypefaceFor(BinaryData::LatoRegular_ttf,
BinaryData::LatoRegular_ttfSize)),
cursor(MouseCursor::NoCursor),
mainConfig(mainConfig),
componentConfig(componentConfig)
{
    componentConfig.addListener(this,componentConfig.getColourKeys());
    mainConfig.addListener(this,{MainConfigFile::showCursorKey});
    loadAllConfigProperties();
}


/**
 * Get the appropriate typeface for the given font.
 */
Typeface::Ptr PokeLookAndFeel::getTypefaceForFont(const Font &font)
{
    return seguibl;
}

/**
 * Draws the thumb portion of a linear slider
 */
void PokeLookAndFeel::drawLinearSliderThumb(Graphics &g, int x, int y,
        int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider &slider)
{
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

/**
 * Draw the background of a linear slider
 */
void PokeLookAndFeel::drawLinearSliderBackground(Graphics &g, int x, int y,
        int width, int height,
        float sliderPos, float minSliderPos,
        float maxSliderPos,
        const Slider::SliderStyle style, Slider &slider)
{
    const float radius = std::floor(getSliderThumbRadius(slider) * 0.333f);
    g.setColour(slider.findColour(Slider::backgroundColourId));
    Path indent;
    if (slider.isHorizontal())
    {
        const float iy = y + height * 0.5f - radius;
        indent.addRoundedRectangle(x - radius, iy, width + radius * 2.0f, radius * 2.0f, 1);
    }
    else
    {
        const float ix = x + width * 0.5f - radius;
        indent.addRoundedRectangle(ix, y - radius, radius * 2.0f, height + radius * 2.0f, 1);
    }
    g.fillPath(indent);
}

/**
 * Draws the entire linear slider component.
 */
void PokeLookAndFeel::drawLinearSlider(Graphics &g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider &slider)
{

    drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style,
            slider);
    drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style,
            slider);
}

/**
 * Defines the radius in pixels of the slider thumb.
 */
int PokeLookAndFeel::getSliderThumbRadius(Slider &slider)
{
    return jmin(14, slider.getHeight() / 2, slider.getWidth() / 2);
}

/**
 * Draws the text onto a TextButton.
 */
void PokeLookAndFeel::drawButtonText(Graphics &g, TextButton &button,
        bool isMouseOverButton, bool isButtonDown)
{
    Font font(getTextButtonFont(button, button.getHeight()));
    font.setExtraKerningFactor(0.06f);
    font.setHeight(componentConfig.getFontHeight
            (button.getLocalBounds(), button.getButtonText()));
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

    const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
    const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

    const int fontHeight = roundToInt(font.getHeight() * 0.6f);
    const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));

    g.drawFittedText(button.getButtonText(), leftIndent, yIndent,
            button.getWidth() - leftIndent - rightIndent, button.getHeight() - yIndent * 2,
            Justification::centred, 2);
}

/**
 * Draws the background of a Button component.
 */
void PokeLookAndFeel::drawButtonBackground(Graphics &g, Button &button,
        const Colour &backgroundColour, bool isMouseOverButton,
        bool isButtonDown)
{
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

/**
 * Get the cursor to display over a given component.
 */
MouseCursor PokeLookAndFeel::getMouseCursorFor(Component &component)
{
    return cursor;
}

/**
 * Loads and applies component colors from components.json, and updates
 * cursor visibility.
 */
void PokeLookAndFeel::configValueChanged(ConfigFile* config, String key)
{
    ComponentConfigFile* componentConfig =
            dynamic_cast<ComponentConfigFile*> (config);
    if (componentConfig != nullptr)
    {
        int colourId = componentConfig->getColourId(key);
        if (colourId != -1)
        {
            Colour confColour = Colour(componentConfig->getConfigValue<String>
                    (key).getHexValue32());
            setColour(colourId, confColour);
        }
    }
    else
    {
        if (key == MainConfigFile::showCursorKey)
        {
            cursor = (config->getConfigValue<bool>(key) ?
                      MouseCursor::ParentCursor : MouseCursor::NoCursor);
        }
    }
}
