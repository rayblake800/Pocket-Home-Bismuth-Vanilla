#include "PokeLookAndFeel.h"
#include "Utils.h"
#include "IconThread.h"
#include "AppMenuButton.h"

AppMenuButton::AppMenuButton(AppMenuItem::Ptr menuItem, String name) :
Button(name),
menuItem(menuItem)
{

    setName(name);
    setWantsKeyboardFocus(false);
    loadIcon(menuItem->getIconName());
}

/**
 * Get this button's menu data.
 */
AppMenuItem::Ptr AppMenuButton::getMenuItem()
{
    return menuItem;
}

/**
 * Gets a PopupEditorComponent configured to edit this button's data
 */
AppMenuPopupEditor* AppMenuButton::getEditor(ComponentConfigFile& config,
        const std::function<void(AppMenuPopupEditor*) >& onConfirm)
{
    AppMenuPopupEditor* editor = new AppMenuPopupEditor
            (menuItem->getEditorTitle(),
            config,
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
    editor->setCommandField(menuItem->getCommand());
    editor->setTerminalCheckbox(menuItem->isTerminalApp());
    return editor;
};

/**
 * Calling this method will create a message box asking for user 
 * confirmation that this button and its source should be removed.
 * If the user clicks "OK", removeButtonSource is called.
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

/**
 * If possible, change the index of this button's data source by some
 * offset amount.
 */
bool AppMenuButton::moveDataIndex(int offset)
{
    return menuItem->moveDataIndex(offset);
}

/**
 * @return true if this button is selected, false otherwise.
 */
bool AppMenuButton::isSelected() const
{
    return getToggleState();
}

/**
 * @param select sets the button as selected if true and unselected if
 * false.
 */
void AppMenuButton::setSelected(bool select)
{
    setToggleState(select, dontSendNotification);
    selectionStateChanged();
}

/**
 * Requests an icon from the icon thread.
 */
void AppMenuButton::loadIcon(String icon)
{
    IconThread iconThread;
    iconThread.loadIcon(icon, [this](Image iconImg)
    {
        appIcon = iconImg;
        repaint();
    });
}

/**
 * Reload this button's data from its menu item
 */
void AppMenuButton::reloadDataFromSource()
{
    loadIcon(menuItem->getIconName());
}

/**
 * @return  the area relative to this button's position where
 *           it will draw its name.
 */
const Rectangle<float>& AppMenuButton::getTextBounds() const
{
    return textBounds;
}

/**
 * @return  the area relative to this button's position where
 *           it will draw its image.
 */
const Rectangle<float>& AppMenuButton::getImageBounds() const
{
    return imageBounds;
}

/**
 * @return the font used to draw this button's title.
 */
const Font& AppMenuButton::getTitleFont() const
{
    return titleFont;
}

/**
 * Set new bounds to draw the button title within.
 */
void AppMenuButton::setTextBounds(const Rectangle<float>& bounds)
{
    textBounds = bounds;
}

/**
 * Set new bounds to draw the button icon within.
 */
void AppMenuButton::setImageBounds(const Rectangle<float>& bounds)
{
    imageBounds = bounds;
}

/**
 * Sets if this button will draw an outline around its border.
 */
void AppMenuButton::setDrawBorder(bool shouldDraw)
{
    drawBorder = shouldDraw;
}

/**
 * Sets if this button will fill in its background with its background
 * color.
 */
void AppMenuButton::setFillBackground(bool shouldFill)
{
    fillBackground = shouldFill;
}

/**
 * Sets the button's title font.
 */
void AppMenuButton::setTitleFont(const Font& font)
{
    titleFont = font;
}

/**
 * Set the text justification of the button title.
 */
void AppMenuButton::setTextJustification(Justification justification)
{
    textJustification = justification;
}

/**
 * Custom button painting method.
 */
void AppMenuButton::paintButton
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    Rectangle<int> border = getBounds().withPosition(0, 0);
    if ((imageBounds.isEmpty() || textBounds.isEmpty()) && !border.isEmpty())
    {
        resized();
    }
    if (fillBackground)
    {
        g.setColour(findColour(isSelected() ?
                selectionColourId : backgroundColourId));
        g.fillRect(border);
    }
    //app icon
    g.setOpacity(1);
    g.drawImageWithin(appIcon, imageBounds.getX(), imageBounds.getY(),
            imageBounds.getWidth(), imageBounds.getHeight(),
            RectanglePlacement::xMid | RectanglePlacement::yTop, false);
    //app title
    g.setColour(findColour(textColourId));
    g.setFont(titleFont);
    g.drawText(getMenuItem()->getAppName(), textBounds, textJustification,
            true);
    if (drawBorder)
    {
        g.setColour(findColour(borderColourId));
        g.drawRect(border, 2);
    }
}
