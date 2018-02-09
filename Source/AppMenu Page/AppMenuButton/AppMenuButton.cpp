#include "../../PokeLookAndFeel.h"
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "AppMenuButton.h"

/**
 * Create a new AppMenuButton
 */
AppMenuButton::AppMenuButton(AppMenuItem* menuItem, IconThread& iconThread,
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

int AppMenuButton::getColumnIndex()
{
    return columnIndex;
};

int AppMenuButton::getRowIndex()
{
    return rowIndex;
};

void AppMenuButton::setColumnIndex(int newColumn)
{
    columnIndex = newColumn;
};

void AppMenuButton::setRowIndex(int newRow)
{
    rowIndex = newRow;
};

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
 * Load button colors from configuration files.
 */
void AppMenuButton::applyConfigAssets
(Array<String> assetNames, Array<Colour> colours)
{
    while (colours.size() < 3)
    {
        colours.add(Colours::transparentBlack);
    }
    textColour = colours[0];
    fillColour = colours[1];
    selectedFillColour = colours[2];
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
        g.setColour(getToggleState() ? selectedFillColour : fillColour);
        g.setOpacity(getToggleState() ? .8 : .2);
        g.fillRect(border);
        g.setOpacity(1);
    }
    //app icon
    g.drawImageWithin(appIcon, imageBox.getX(), imageBox.getY(),
            imageBox.getWidth(), imageBox.getHeight(),
            RectanglePlacement::centred, false);
    //app title
    g.setColour(textColour);
    g.setFont(titleFont);
    g.drawText(getMenuItem()->getAppName(), textBox,
            Justification::centredLeft, true);
    if (drawBorder)
    {
        g.setColour(Colour(0x4D4D4D));
        g.setOpacity(getToggleState() ? 1.0 : 0.8);
        g.drawRect(border, 2);
    }
}
