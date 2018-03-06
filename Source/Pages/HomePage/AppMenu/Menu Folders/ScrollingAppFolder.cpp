#include "ScrollingAppFolder.h"

ScrollingAppFolder::ScrollingAppFolder
(AppMenuItem::Ptr folderItem, MouseListener* btnListener,
        std::map<String, AppMenuButton::Ptr>& buttonNameMap,
        IconThread& iconThread) :
AppMenuFolder(folderItem, btnListener, buttonNameMap, iconThread)
{
    setMargin(0);
    setPadding(0, 0);
    reload();
}

ScrollingAppFolder::~ScrollingAppFolder() { }

/**
 * Create an AppMenuButton component for an AppMenuItem.
 * @param menuItem
 */
AppMenuButton::Ptr ScrollingAppFolder::createMenuButton
(AppMenuItem::Ptr menuItem)
{
    ComponentConfigFile config;
    Font measuringFont(config.getComponentSettings
            (ComponentConfigFile::smallTextKey).getBounds().getHeight());
    maxTextWidth = std::max<int>
            (maxTextWidth,
             measuringFont.getStringWidth(menuItem->getAppName()));
    
    return new ScrollingMenuButton(menuItem, iconThread,
            menuItem->getAppName() + String("Button"));
}

/**
 * Given a list of folder buttons, return an appropriate layout
 * for positioning them in the folder component.
 * 
 * @param buttons
 * @return a Layout containing all items in the button array.
 */
GridLayoutManager::Layout
ScrollingAppFolder::buildFolderLayout
(Array<AppMenuButton::Ptr>& buttons)
{
    GridLayoutManager::Layout layout;
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
 * @return the width in pixels of the longest menu item title in
 * the folder.
 */
int ScrollingAppFolder::getTextWidth()
{
    return maxTextWidth;
}

/**
 *
 */
ScrollingAppFolder::ScrollingMenuButton::ScrollingMenuButton
(AppMenuItem* menuItem, IconThread& iconThread, String name) :
AppMenuButton(menuItem, iconThread, name)
{
    ComponentConfigFile config;
    int textHeight = config.getComponentSettings
            (ComponentConfigFile::smallTextKey).getBounds().getHeight();
    setTitleFont(getTitleFont().withHeight(textHeight));
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
    if(textBounds.getWidth() < titleWidth)
    {
        textBounds.setLeft(bounds.getRight() - (titleWidth+padding));
        imageBounds.setRight(textBounds.getX());
    }
    imageBounds.reduce(padding, padding);
    setImageBounds(imageBounds);
    setTextBounds(textBounds);
}
