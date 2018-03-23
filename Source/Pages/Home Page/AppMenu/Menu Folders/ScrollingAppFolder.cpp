#include "ScrollingAppFolder.h"

ScrollingAppFolder::ScrollingAppFolder(
        AppMenuItem::Ptr folderItem,
        MouseListener* btnListener,
        std::map<String, AppMenuButton::Ptr>& buttonNameMap,
        IconThread& iconThread,
        ComponentConfigFile& config) :
AppMenuFolder(folderItem, btnListener, buttonNameMap, iconThread),
config(config)
{
    setMargin(0);
    setPadding(0, 0);
    reload();
}

/**
 * Create an AppMenuButton component for an AppMenuItem.
 * @param menuItem
 */
AppMenuButton::Ptr ScrollingAppFolder::createMenuButton
(AppMenuItem::Ptr menuItem, IconThread& iconThread)
{
    return new ScrollingMenuButton(
            menuItem,
            iconThread,
            menuItem->getAppName() + String("Button"),
            config);
}

/**
 * Given a list of folder buttons, return an appropriate layout
 * for positioning them in the folder component.
 * 
 * @param buttons
 * @return a Layout containing all items in the button array.
 */
RelativeLayoutManager::Layout ScrollingAppFolder::buildFolderLayout
(Array<AppMenuButton::Ptr>& buttons)
{
    RelativeLayoutManager::Layout layout;
    for (AppMenuButton::Ptr button : buttons)
    {
        layout.push_back({1,
            {
                {button, 1}
            }});
    }
    return layout;
}

/**
 * @return the minimum width, in pixels, needed by this folder to
 * display its contents properly. 
 */
int ScrollingAppFolder::getMinimumWidth()
{
    int iconSize = getParentHeight() / getMaxRows();
    int maxTextWidth = 0;
    Font measureFont = getButtonFont(config);
    for (int i = 0; i < getButtonCount(); i++)
    {
        maxTextWidth = std::max<int> (maxTextWidth,
                                      measureFont.getStringWidth(getMenuButtonName(i)));
    }
    return maxTextWidth + iconSize;
}

/**
 * Get the font used by all buttons in this menu type.
 */
Font ScrollingAppFolder::getButtonFont(ComponentConfigFile& config)
{
    return Font(config.getComponentSettings
            (ComponentConfigFile::smallTextKey).getBounds().getHeight());
}

/**
 *
 */
ScrollingAppFolder::ScrollingMenuButton::ScrollingMenuButton(
        AppMenuItem* menuItem,
        IconThread& iconThread,
        String name,
        ComponentConfigFile& config) :
AppMenuButton(menuItem, iconThread, name),
config(config)
{
    setTitleFont(ScrollingAppFolder::getButtonFont(config));
    setTextJustification(Justification::centredLeft);
}

ScrollingAppFolder::ScrollingMenuButton::~ScrollingMenuButton() { }

/**
 * @return the width in pixels of this button's title
 */
int ScrollingAppFolder::ScrollingMenuButton::getTitleWidth()
{
    String title = getMenuItem()->getAppName();
    const Font& titleFont = getTitleFont();
    return getTitleFont().getStringWidth(title);
}

/**
 * Re-calculates draw values whenever the button is resized
 */
void ScrollingAppFolder::ScrollingMenuButton::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    Rectangle<float> imageBounds = bounds.toFloat();
    Rectangle<float> textBounds = imageBounds;

    imageBounds.setWidth(imageBounds.getHeight());
    textBounds.setLeft(imageBounds.getRight());
    float padding = imageBounds.getWidth() / 20;
    int titleWidth = getTitleWidth();
    textBounds.reduce(padding, padding);
    if (textBounds.getWidth() < titleWidth)
    {
        textBounds.setLeft(bounds.getRight() - (titleWidth + padding));
        imageBounds.setRight(textBounds.getX());
    }
    imageBounds.reduce(padding, padding);
    setImageBounds(imageBounds);
    setTextBounds(textBounds);
}
