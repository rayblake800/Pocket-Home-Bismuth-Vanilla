#include "../../PokeLookAndFeel.h"
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "AppMenuButton.h"

/**
 * Create a new AppMenuButton
 */
AppMenuButton::AppMenuButton
(AppMenuItem* menuItem, IconThread& iconThread, String name) : Button(name),
menuItem(menuItem),
iconThread(iconThread)
{
    setWantsKeyboardFocus(false);
    loadIcon(menuItem->getIconName());
}

AppMenuButton::~AppMenuButton()
{
}

AppMenuItem* AppMenuButton::getMenuItem()
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
            [this,onConfirm](AppMenuPopupEditor * editor)
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
            [this,onRemove]()
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
