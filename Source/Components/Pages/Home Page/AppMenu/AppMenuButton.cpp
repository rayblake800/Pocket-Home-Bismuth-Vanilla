// Disabled until redesign
#if 0
#include "PokeLookAndFeel.h"
#include "Utils.h"
#include "IconLoader.h"
#include "AppMenuButton.h"
#include "MainConfigKeys.h"

/* Extra characters applied when calculating title width, defining title padding
   space relative to the font size. */
static const constexpr char* titleBuffer = "     ";

/*
 * Creates a new AppMenuButton component for a menu item.
 */
AppMenuButton::AppMenuButton(AppMenuItem menuItem) :
Button("AppMenuButton"),
menuItem(menuItem)
{
    using namespace juce;
#ifdef JUCE_DEBUG
    setName(String("AppMenuButton:") + menuItem.getTitle());
#endif
    setWantsKeyboardFocus(false);
}

/*
 * Gets this button's menu data.
 */
AppMenuItem AppMenuButton::getMenuItem() const
{
    return menuItem;
}


/*
 * Replaces the menu item used by this button.
 */
void AppMenuButton::setMenuItem(const AppMenuItem newItem)
{
    menuItem = newItem;
    reloadDataFromSource();
}


/*
 * Checks if this button is currently selected.
 */
bool AppMenuButton::isSelected() const
{
    return getToggleState();
}

/*
 * Selects or un-selects this button.
 */
void AppMenuButton::setSelected(bool select)
{
    using namespace juce;
    setToggleState(select, dontSendNotification);
    selectionStateChanged();
}

/*
 * Requests an icon from the icon thread.
 */
void AppMenuButton::loadIcon(const juce::String& icon)
{
    using namespace juce;
    IconLoader iconThread;
    iconThread.loadIcon(icon, getIconBounds().toNearestInt().getWidth(),
    [this](Image iconImg)
    {
        appIcon = iconImg;
        repaint();
    });
}

/*
 * Reloads this button's data from its menu item.
 */
void AppMenuButton::reloadDataFromSource()
{
    loadIcon(menuItem.getIconName());
}

/*
 * Calculates text size and calls menuButtonResized when the button's 
 * component bounds change.
 */
void AppMenuButton::resized()
{
    if(textWidth == 0)
    {
        textWidth = getTitleFont().getStringWidth(getMenuItem().getTitle() 
                + titleBuffer);
    }
    menuButtonResized();
}

/*
 * Custom button painting method called by juce library code.
 */
void AppMenuButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    using namespace juce;
    Rectangle<int> border = getLocalBounds();
    const Rectangle<float>& titleBounds = getTitleBounds();
    const Rectangle<float>& iconBounds = getIconBounds();
    if ((iconBounds.isEmpty() || titleBounds.isEmpty()) && 
            !border.isEmpty())
    {
        resized();
    }
    g.setColour(findColour(isSelected() ?
            selectionColourId : backgroundColourId));
    if (shouldFillBackground())
    {
        g.fillRect(border);
    }
    else
    {
        Rectangle<float> textOval = titleBounds.withSizeKeepingCentre
                (textWidth, titleBounds.getHeight());
        g.fillRoundedRectangle(textOval, textOval.getHeight() / 6);
    }
    // Draw icon:
    g.setOpacity(1);
    g.drawImageWithin(appIcon, iconBounds.getX(), iconBounds.getY(),
            iconBounds.getWidth(), iconBounds.getHeight(),
            RectanglePlacement::xMid | RectanglePlacement::yTop, false);
    // Draw title:
    g.setColour(findColour(textColourId));
    g.setFont(getTitleFont());
    g.drawText(getMenuItem().getTitle(), titleBounds, 
            getTextJustification(), true);
    if (shouldDrawBorder())
    {
        g.setColour(findColour(borderColourId));
        g.drawRect(border, 2);
    }
}

//Disabled until redesign
#endif
