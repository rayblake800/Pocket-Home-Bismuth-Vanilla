#include "../Utils.h"
#include "../PocketHomeApplication.h"
#include "ScalingLabel.h"

ScalingLabel::ScalingLabel(const String &componentName,
        const String &labelText, const int& fontPadding)
: Label(componentName, labelText),
fontPadding(fontPadding)
{
}

ScalingLabel::~ScalingLabel()
{
}

void ScalingLabel::resized()
{
    ComponentConfigFile& config = PocketHomeApplication::getInstance()->
            getComponentConfig();
    setFont(getFont().withHeight(config.getFontHeight
            (getLocalBounds(),getText())));
}