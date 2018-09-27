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
AppMenuButton::AppMenuButton(AppMenuItem::Ptr menuItem) :
Button("AppMenuButton"),
menuItem(menuItem)
{
    using namespace juce;
#ifdef JUCE_DEBUG
    setName(String("AppMenuButton:") + menuItem->getAppName());
#endif
    setWantsKeyboardFocus(false);
    textWidth = titleFont.getStringWidth(getMenuItem()->getAppName() 
            + titleBuffer);
}

/*
 * Gets this button's menu data.
 */
AppMenuItem::Ptr AppMenuButton::getMenuItem()
{
    return menuItem;
}

/*
 * Gets a PopupEditorComponent configured to edit this button's data
 */
AppMenuPopupEditor* AppMenuButton::getEditor
(const std::function<void(AppMenuPopupEditor*) >& onConfirm)
{
    using namespace juce;
    MainConfigFile mainConfig;
    
    AppMenuPopupEditor* editor = new AppMenuPopupEditor
            (menuItem->getEditorTitle(),
            [this, onConfirm](AppMenuPopupEditor * editor)
            {
                onConfirm(editor);
                menuItem->getEditorCallback()(editor);
                reloadDataFromSource();
            },
    menuItem->hasEditableCategories(),
            menuItem->hasEditableCommand());
    File file;
    editor->setNameField(menuItem->getAppName());
    editor->setIconField(menuItem->getIconName());
    editor->setCategories(menuItem->getCategories());
    String command = menuItem->getCommand();
    if(menuItem->isTerminalApp())
    {
        command = command.substring(mainConfig.getConfigValue<String>
                (MainConfigKeys::termLaunchCommandKey).length() + 1);
    }
    editor->setCommandField(command);
    editor->setTerminalCheckbox(menuItem->isTerminalApp());
    return editor;
};

/*
 * Displays a confirmation window to the user requesting permission to delete 
 * this button, and runs a deletion callback function if they confirm.
 */
void AppMenuButton::confirmRemoveButtonSource
(const std::function<void() >& onRemove)
{
    confirmAction(menuItem->getConfirmDeleteTitle(),
            menuItem->getConfirmDeleteMessage(),
            [this, onRemove]()
            {
                menuItem->removeMenuItemSource();
                onRemove();
            });
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
    iconThread.loadIcon(icon, iconBounds.toNearestInt().getWidth(),
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
    loadIcon(menuItem->getIconName());
}

/*
 * Gets the button's title bounds.
 */
const juce::Rectangle<float>& AppMenuButton::getTitleBounds() const
{
    return titleBounds;
}

/*
 * Gets the button's image bounds.
 */
const juce::Rectangle<float>& AppMenuButton::getIconBounds() const
{
    return iconBounds;
}

/*
 * Gets the button's title font.
 */
const juce::Font& AppMenuButton::getTitleFont() const
{
    return titleFont;
}

/*
 * Sets the bounds of the button's title.
 */
void AppMenuButton::setTitleBounds(const juce::Rectangle<float>& bounds)
{
    titleBounds = bounds;
    textWidth = std::min((int) bounds.getWidth(),
            titleFont.getStringWidth(getMenuItem()->getAppName() + "    "));
}

/*
 * Sets the bounds of the button's icon.
 */
void AppMenuButton::setIconBounds(const juce::Rectangle<float>& bounds)
{
    iconBounds = bounds;
    if(appIcon.isNull())
    {
        loadIcon(menuItem->getIconName());
    }
}

/*
 * Sets if this button will draw an outline around its border.
 */
void AppMenuButton::setDrawBorder(const bool shouldDraw)
{
    drawBorder = shouldDraw;
}

/*
 * Sets if this button will fill in its background with its background color.
 */
void AppMenuButton::setFillBackground(const bool shouldFill)
{
    fillBackground = shouldFill;
}

/*
 * Sets the button's title font.
 */
void AppMenuButton::setTitleFont(const juce::Font& font)
{
    titleFont = font;
    textWidth = std::min((int) titleBounds.getWidth(),
            font.getStringWidth(getMenuItem()->getAppName() + titleBuffer));
}

/*
 * Sets the text justification of the button title.
 */
void AppMenuButton::setTextJustification
(const juce::Justification justification)
{
    textJustification = justification;
}

/*
 * Custom button painting method called by juce library code.
 */
void AppMenuButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    using namespace juce;
    Rectangle<int> border = getLocalBounds();
    if ((iconBounds.isEmpty() || titleBounds.isEmpty()) && !border.isEmpty())
    {
        resized();
    }
    g.setColour(findColour(isSelected() ?
            selectionColourId : backgroundColourId));
    if (fillBackground)
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
    g.setFont(titleFont);
    g.drawText(getMenuItem()->getAppName(), titleBounds, textJustification,
            true);
    if (drawBorder)
    {
        g.setColour(findColour(borderColourId));
        g.drawRect(border, 2);
    }
}
