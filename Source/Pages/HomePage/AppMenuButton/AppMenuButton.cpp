#include "../../../PokeLookAndFeel.h"
#include "../../../Utils.h"
#include "AppMenuButton.h"

AppMenuButton::AppMenuButton(AppMenuItem::Ptr menuItem, IconThread& iconThread,
        int columnIndex, int rowIndex, String name) :
Button(name),
ConfigurableComponent(ComponentConfigFile::appMenuButtonKey),
menuItem(menuItem),
iconThread(iconThread),
columnIndex(columnIndex),
rowIndex(rowIndex)
{

    loadAllConfigProperties();
    setName(name);
    setWantsKeyboardFocus(false);
    loadIcon(menuItem->getIconName());
}

AppMenuButton::~AppMenuButton()
{
}


AppMenuItem::Ptr AppMenuButton::getMenuItem()
{
    return menuItem;
}

/**
 * Gets a PopupEditorComponent configured to edit this button's data
 */
AppMenuPopupEditor* AppMenuButton::getEditor(std::function<void(AppMenuPopupEditor*) > onConfirm)
{
    AppMenuPopupEditor* editor = new AppMenuPopupEditor
            (menuItem->getEditorTitle(),
            iconThread,
            [this, onConfirm](AppMenuPopupEditor * editor)
            {
                onConfirm(editor);
                menuItem->getEditorCallback()(editor);
                reloadDataFromSource();
            },
    menuItem->hasEditableCategories(),
            menuItem->hasEditableCommand());

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
void AppMenuButton::confirmRemoveButtonSource(std::function<void() > onRemove)
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
 * Requests an icon from the icon thread.
 */
void AppMenuButton::loadIcon(String icon)
{
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
 * Custom button painting method.
 */
void AppMenuButton::paintButton
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    Rectangle<int> border = getBounds().withPosition(0, 0);
    if ((imageBox.isEmpty() || textBox.isEmpty()) && !border.isEmpty())
    {
        resized();
    }
    if (fillInBackground)
    {
        g.setColour(findColour(getToggleState() ? 
            selectionColourId : backgroundColourId));
        g.setOpacity(getToggleState() ? .8 : .2);
        g.fillRect(border);
        g.setOpacity(1);
    }
    //app icon
    g.drawImageWithin(appIcon, imageBox.getX(), imageBox.getY(),
            imageBox.getWidth(), imageBox.getHeight(),
            RectanglePlacement::centred, false);
    //app title
    g.setColour(findColour(textColourId));
    g.setFont(titleFont);
    g.drawText(getMenuItem()->getAppName(), textBox,
            Justification::centredLeft, true);
    if (drawBorder)
    {
        g.setColour(findColour(borderColourId));
        g.setOpacity(getToggleState() ? 1.0 : 0.8);
        g.drawRect(border, 2);
    }
}
