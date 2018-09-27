#include "ScrollingAppFolder.h"

ScrollingAppFolder::ScrollingAppFolder(
        AppMenuItem::Ptr folderItem,
        juce::MouseListener* btnListener,
        std::map<juce::String, AppMenuButton::Ptr>& buttonNameMap) :
AppMenuFolder(folderItem, btnListener, buttonNameMap)
{
    setMargin(0);
    setPadding(0, 0);
    reload();
}

/**
 * Create an AppMenuButton component for an AppMenuItem.
 */
AppMenuButton::Ptr ScrollingAppFolder::createMenuButton
(AppMenuItem::Ptr menuItem)
{
    return new ScrollingMenuButton(
            menuItem,
            menuItem->getAppName() + juce::String("Button"));
}

/**
 * Given a list of folder buttons, return an appropriate layout
 * for positioning them in the folder component.
 */
LayoutManager::Layout ScrollingAppFolder::buildFolderLayout
(juce::Array<AppMenuButton::Ptr>& buttons)
{
    LayoutManager::Layout layout;
    for (AppMenuButton::Ptr button : buttons)
    {
        layout.addRow(LayoutManager::Row(1,{
            LayoutManager::RowItem(button)
        }
        ));
    }
    return layout;
}

/**
 * @return the minimum width, in pixels, needed by this folder to
 *          display its contents properly. 
 */
int ScrollingAppFolder::getMinimumWidth()
{
    using namespace juce;
    int iconSize = getParentHeight() / getMaxRows();
    int maxTextWidth = 0;
    Font measureFont = getButtonFont();
    for (int i = 0; i < getButtonCount(); i++)
    {
        maxTextWidth = std::max<int>(maxTextWidth,
                measureFont.getStringWidth(getMenuButtonName(i)));
    }
    return maxTextWidth + iconSize;
}

/**
 * Get the font used by all buttons in this menu type.
 */
juce::Font ScrollingAppFolder::getButtonFont()
{
    ComponentConfigFile config;
    return juce::Font(config.getFontHeight(ComponentConfigFile::smallText));
}

ScrollingAppFolder::ScrollingMenuButton::ScrollingMenuButton
(AppMenuItem* menuItem, juce::String name) :
AppMenuButton(menuItem)
{
    using namespace juce;
    setTitleFont(ScrollingAppFolder::getButtonFont());
    setTextJustification(Justification::centredLeft);
}

/**
 * @return the width in pixels of this button's title
 */
int ScrollingAppFolder::ScrollingMenuButton::getTitleWidth()
{
    using namespace juce;
    String title = getMenuItem()->getAppName();
    const Font& titleFont = getTitleFont();
    return getTitleFont().getStringWidth(title);
}

/**
 * Re-calculates draw values whenever the button is resized
 */
void ScrollingAppFolder::ScrollingMenuButton::resized()
{
    using namespace juce;
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
    setIconBounds(imageBounds);
    setTitleBounds(textBounds);
}
