#include "SettingsCategoryItemComponent.h"
SettingsCategoryItemComponent::SettingsCategoryItemComponent(const String &name)
: icon(new DrawableButton("icon", DrawableButton::ImageFitted)),
toggle(new SwitchComponent()),
button(new SettingsCategoryButton(name))
{
    toggle->addListener(this);
    addAndMakeVisible(icon);
    addAndMakeVisible(toggle);
    addAndMakeVisible(button);
    button->setEnabled(false); // default to disabled state
}

void SettingsCategoryItemComponent::paint(Graphics &g)
{
}

void SettingsCategoryItemComponent::resized()
{
    auto b = getLocalBounds();
    auto h = b.getHeight();

    int spacing = 10;
    int togWidth = h * 1.1f;

    layout.setItemLayout(0, h, h, h);
    layout.setItemLayout(1, spacing, spacing, spacing);
    layout.setItemLayout(2, togWidth, togWidth, togWidth);
    layout.setItemLayout(3, spacing, spacing, spacing);
    layout.setItemLayout(4, h, -1, -1);

    Component * comps[] = {icon, nullptr, toggle, nullptr, button};
    layout.layOutComponents(comps, 5, b.getX(), b.getY(), b.getWidth(), b.getHeight(), false, true);
}

void SettingsCategoryItemComponent::buttonClicked(Button *b)
{
    if (b == toggle)
    {
        enabledStateChanged(toggle->getToggleState());
    }
}

void SettingsCategoryItemComponent::enablementChanged()
{
    updateButtonText();
}