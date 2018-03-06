#include "Basic Components/SwitchComponent.h"
#include "Basic Components/DrawableImageComponent.h"
#include "Basic Components/ListEditor.h"
#include "Basic Components/FileSelectTextEditor.h"
#include "Basic Components/OverlaySpinner.h"
#include "Pages/HomePage/AppMenu/AppMenuButton.h"
#include "PocketHomeApplication.h"
#include "PokeLookAndFeel.h"

Colour PokeLookAndFeel::lightGrey = Colour(0xffe1e1e1);
Colour PokeLookAndFeel::medGrey = Colour(0xffc0c0c0);
Colour PokeLookAndFeel::chipPink = Colour(0xffbc3662);
Colour PokeLookAndFeel::chipLightPink = Colour(0xfff799aa);
Colour PokeLookAndFeel::chipPurple = Colour(0xffd23c6d);

PokeLookAndFeel::PokeLookAndFeel() :
Configurable(new ComponentConfigFile(), componentConfig.getColourKeys()),
cursor(MouseCursor::NoCursor)
{
    loadAllConfigProperties();
    seguibl = Typeface::createSystemTypefaceFor(BinaryData::LatoRegular_ttf,
            BinaryData::LatoRegular_ttfSize);
}

PokeLookAndFeel::~PokeLookAndFeel() { };

float PokeLookAndFeel::getDrawableButtonTextHeightForBounds(const Rectangle<int> &bounds)
{
    ComponentConfigFile config;
    String largestString;
    for (int i = 0; i < maxButtonStrSize; i++)
    {
        largestString += "A";
    }
    //return jmin(23.0f, bounds.getHeight() * 0.95f);
    return config.getFontHeight(bounds, largestString);
}

float PokeLookAndFeel::getDrawableButtonImageHeightForBounds(const Rectangle<int> &bounds)
{
    static const float padding = 5.0f;
    return bounds.getHeight() - (getDrawableButtonTextHeightForBounds(bounds)
                                 + padding);
}

Typeface::Ptr PokeLookAndFeel::getTypefaceForFont(const Font &font)
{
    return seguibl;
}

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

void PokeLookAndFeel::drawLinearSliderBackground(Graphics &g, int x, int y, int width, int height,
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

void PokeLookAndFeel::drawLinearSlider(Graphics &g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider &slider)
{

    drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style,
            slider);
    drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style,
            slider);
}

int PokeLookAndFeel::getSliderThumbRadius(Slider &slider)
{
    return jmin(14, slider.getHeight() / 2, slider.getWidth() / 2);
}

void PokeLookAndFeel::drawButtonText(Graphics &g, TextButton &button, bool isMouseOverButton,
        bool isButtonDown)
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

void PokeLookAndFeel::drawButtonBackground(Graphics &g, Button &button,
        const Colour &backgroundColour, bool isMouseOverButton,
        bool isButtonDown)
{
    const int width = button.getWidth();
    const int height = button.getHeight();

    auto path = Path();
    path.addRoundedRectangle(0, 0, width, height, 1);
    if (isButtonDown && isMouseOverButton)
    {
        g.setColour(chipPink.darker());
    }
    else
    {
        g.setColour(chipPink);
    }
    g.fillPath(path);
}

void PokeLookAndFeel::drawDrawableButton(Graphics &g, DrawableButton &button,
        bool isMouseOverButton, bool isButtonDown)
{
    bool toggleState = button.getToggleState();

    g.fillAll(button.findColour(toggleState ? DrawableButton::backgroundOnColourId
            : DrawableButton::backgroundColourId));

    const int textH = (button.getStyle() == DrawableButton::ImageAboveTextLabel)
            ? getDrawableButtonTextHeightForBounds(button.getLocalBounds())

            : 0;

    Font font(18);
    font.setExtraKerningFactor(0.06f);
    if (textH > 0)
    {
        g.setFont(font);
        //g.setFont(textH);
        g.setColour(button.findColour(toggleState ? DrawableButton::textColourOnId
                : DrawableButton::textColourId)
                .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.4f));

        g.drawFittedText(button.getButtonText(), 2, button.getHeight() - textH - 1,
                button.getWidth() - 4, textH, Justification::centred, 1);
    }
}

void PokeLookAndFeel::setCursorVisible(bool visible)
{
    if (visible) cursor = MouseCursor::ParentCursor;
    else cursor = MouseCursor::NoCursor;
}

MouseCursor PokeLookAndFeel::getMouseCursorFor(Component &component)
{
    return cursor;
    //return LookAndFeel_V3::getMouseCursorFor(component);
}

/**
 * Reloads and applies object properties defined by a single key in
 * a configuration file
 */
void PokeLookAndFeel::loadConfigProperties(ConfigFile* config, String key)
{
    if (componentConfig == *config)
    {
        int colourId = componentConfig.getColourId(key);
        if (colourId != -1)
        {
            Colour confColour = Colour(componentConfig.getConfigValue<String>(key)
                    .getHexValue32());
            setColour(colourId, confColour);
        }
    }
}
