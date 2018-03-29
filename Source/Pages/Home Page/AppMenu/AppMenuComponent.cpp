#include <set>
#include "MainConfigFile.h"
#include "NewConfigAppEditor.h"
#include "NewDesktopAppEditor.h"
#include "NewFolderEditor.h"
#include "AppMenuComponent.h"


//TODO: load these from config, set on input page
const String AppMenuComponent::openPopupMenuBinding = "CTRL + e";
const String AppMenuComponent::reloadMenuBinding = "TAB";
DesktopEntries AppMenuComponent::desktopEntries;

AppMenuComponent::AppMenuComponent(
        String componentKey,
        OverlaySpinner& loadingSpinner) :
loadingState(false),
ConfigurableComponent(componentKey),
loadingSpinner(loadingSpinner),
menuItemFactory(desktopEntries)
{
#    if JUCE_DEBUG
    setName("AppMenuComponent");
#    endif
    MainConfigFile mainConfig;
    mainConfig.addListener(this,{
        MainConfigFile::maxRowsKey,
        MainConfigFile::maxColumnsKey
    });
    maxRows = mainConfig.getConfigValue<int>(MainConfigFile::maxRowsKey);
    maxColumns = mainConfig.getConfigValue<int>(MainConfigFile::maxColumnsKey);
    setWantsKeyboardFocus(false);
    appLauncher.setLaunchFailureCallback([this]()
    {
        setLoadingState(false);
    });
    loadBaseFolder();
}

AppMenuComponent::~AppMenuComponent()
{
    desktopEntries.clearCallbacks();
}

/**
 * @return true if the AppMenuComponent is currently in the loading state
 */
bool AppMenuComponent::isLoading() const
{
    return loadingState;
}

/**
 * Exit the loading state, hiding the spinner and enabling
 * user input
 */
void AppMenuComponent::exitLoadingState()
{
    setLoadingState(false);
}

/**
 * Open the pop-up menu used for editing menu items.
 */
void AppMenuComponent::openPopupMenu(AppMenuButton::Ptr selectedButton)
{
    if (ignoringInput())
    {
        return;
    }
    PopupMenu editMenu;

    AppMenuItem::Ptr selectedMenuItem = selectedButton == nullptr ?
            nullptr : selectedButton->getMenuItem();

    /**
     * The pop-up menu really shouldn't be appearing when the button editor
     * is visible, but if it still is, close and remove the editor.
     */
    if (buttonEditor != nullptr)
    {
        removeChildComponent(buttonEditor);
        buttonEditor = nullptr;
    }

    /* Menu Options:
     * 1. Edit button
     * 2. Delete button (and button source)
     * 3. Create new pinned favorites item
     * 4. Create new desktop entry
     * 5. Create new folder
     * 6. Add application to favorites
     * 7. Move button up
     * 8. Move button down
     */
    if (selectedButton != nullptr)
    {
        editMenu.addItem(1, "Edit");
        editMenu.addItem(2, "Delete");
        if (selectedMenuItem->isFolder())
        {
            editMenu.addItem(4, "New application link");
        }
        else if (getActiveFolderIndex() > 0)
        {
            editMenu.addItem(6, "Pin to favorites");
        }
        if (selectedMenuItem->canChangeIndex(-1))
        {
            editMenu.addItem(7, "Move back");
        }
        if (selectedMenuItem->canChangeIndex(1))
        {
            editMenu.addItem(8, "Move forward");
        }
    }
    else
    {
        editMenu.addItem(3, "New favorite application link");
        editMenu.addItem(4, "New application link");
        editMenu.addItem(5, "New folder");
    }

    AppMenuFolder* activeFolder = openFolders[getActiveFolderIndex()];
    const int selection = editMenu.show();
    std::function<void() > confirmNew = [this]()
    {
        loadBaseFolder();
    };
    switch (selection)
    {
        case 1://User selects "Edit"
            showMenuButtonEditor(selectedButton);
            break;
        case 2://User selects "Delete"
            selectedButton->confirmRemoveButtonSource([this, activeFolder]()
            {
                activeFolder->removeButton(activeFolder->getSelectedIndex());
                layoutFolders();
            });
            break;
        case 3://User selects "New favorite application"
            showPopupEditor(new NewConfigAppEditor(confirmNew));
            break;
        case 4://User selects "New application link"
        {
            AppMenuPopupEditor* newAppEditor
                    = new NewDesktopAppEditor(confirmNew);
            if (selectedButton != nullptr)
            {
                newAppEditor->setCategories
                        (selectedButton->getMenuItem()->getCategories());
            }
            showPopupEditor(newAppEditor);
            break;
        }
        case 5://User selects "New folder"
        {
            AppMenuPopupEditor* newFolderEditor
                    = new NewFolderEditor(confirmNew);
            showPopupEditor(newFolderEditor);
            break;
        }
        case 6://User selects "Pin to favorites"
        {
            AppConfigFile appConfig;
            AppConfigFile::AppItem newFavorite;
            newFavorite.name = selectedMenuItem->getAppName();
            newFavorite.icon = selectedMenuItem->getIconName();
            newFavorite.shell = selectedMenuItem->getCommand();
            newFavorite.launchInTerminal = false;
            appConfig.addFavoriteApp(newFavorite,
                    appConfig.getFavorites().size());
            confirmNew();
            break;
        }
        case 7://User selects "Move back"
            activeFolder->swapButtons(activeFolder->getSelectedIndex(),
                    activeFolder->getSelectedIndex() - 1);
            break;
        case 8://User selects "Move forward"
            activeFolder->swapButtons(activeFolder->getSelectedIndex(),
                    activeFolder->getSelectedIndex() + 1);
    }
}

/**
 * Pass on key events to the AppMenu from its containing page.
 * Opens the pop-up menu or reloads the AppMenu if the correct shortcuts
 * are detected, otherwise let the AppMenuComponent's subclass determine
 * how to handle the key event.
 */
bool AppMenuComponent::keyPressed(const KeyPress& key)
{
    if (ignoringInput())
    {
        return true;
    }
    if (key == KeyPress::createFromDescription(openPopupMenuBinding))
    {
        openPopupMenu(getSelectedButton());
        return true;
    }
    if (key == KeyPress::createFromDescription(reloadMenuBinding))
    {
        loadBaseFolder();
        return true;
    }
    return folderKeyPressed(key, openFolders.isEmpty()
            ? nullptr : openFolders[getActiveFolderIndex()]);
}

/**
 * Load and display the base menu folder that contains favorite 
 * application shortcuts and all other folders. Any existing open folders
 * will first be closed.  If the base folder was open, its selected 
 * index will be saved and re-selected.
 */
void AppMenuComponent::loadBaseFolder()
{
    if (!isLoading())
    {
        DBG("AppMenuComponent::" << __func__ << ":started loading AppMenu");
        int savedIndex = -1;
        if (!openFolders.isEmpty())
        {
            savedIndex = openFolders.getFirst()->getSelectedIndex();
            DBG("AppMenuComponent::" << __func__
                    << ":folder exists, saving index" << savedIndex);
        }
        while (openFolders.size() > 0)
        {
            closeFolder();
        }
        setLoadingState(true);
        desktopEntries.loadEntries([this](String loadingMsg)
        {
            if (!isLoading())
            {
                setLoadingState(true);
            }
            loadingSpinner.setLoadingText(loadingMsg);
        },
        [this, savedIndex]()
        {
            DBG("AppMenuComponent::" << __func__
                    << ": Loading desktop entries complete,"
                    << " creating base folder");
            openFolder(menuItemFactory.createBaseFolderItem());
            openFolders.getFirst()->selectIndex(savedIndex);
            loadingSpinner.setLoadingText("Building folder layout:");
            MessageManager::callAsync([this]()
            {
                layoutFolders();
                setLoadingState(false);
            });
        });
    }
}

/**
 * Close the active folder, removing it and all folders after it from
 * the menu component.
 */
void AppMenuComponent::closeFolder()
{
    if (openFolders.size() > 0)
    {
        int targetFolderCount = getActiveFolderIndex();
        DBG("AppMenuComponent::" << __func__ << ": Closing folder "
                << String(openFolders.size() - 1));
        if (getActiveFolderIndex() > 0)
        {
            setActiveFolderIndex(getActiveFolderIndex() - 1);
            layoutFolders(true);
        }
        while (openFolders.size() > targetFolderCount)
        {
            openFolders.getLast()->deselect();
            removeChildComponent(openFolders.getLast());
            openFolders.removeLast();
        }
    }
}

/**
 * Closes the active folder, unless the active folder is at index 0,
 * in which case it will reload the folder.
 */
void AppMenuComponent::closeNonBaseFolder()
{
    if (openFolders.size() > 0)
    {
        if (getActiveFolderIndex() > 0)
        {
            closeFolder();
        }
        else
        {
            loadBaseFolder();
        }
    }
}

/**
 * Sets which open folder is currently focused.  This value will change to
 * the index of the last open folder whenever a folder opens or closes.
 */
void AppMenuComponent::setActiveFolderIndex(int folder)
{
    if (folder >= 0 && folder < openFolders.size())
    {
        activeFolderIndex = folder;
    }
}

/**
 * Get the index of the active folder.  AppMenuComponents should center the
 * folder layout on the active folder, and apply key controls to that
 * folder's menu items.
 */
int AppMenuComponent::getActiveFolderIndex() const
{
    return median<int>(0, activeFolderIndex, openFolders.size() - 1);
}

/**
 * Trigger a click for the selected button in the active AppFolder
 */
void AppMenuComponent::clickSelected()
{
    AppMenuButton::Ptr selected = getSelectedButton();
    if (selected != nullptr)
    {
        onButtonClick(selected);
    }
}

/**
 * Open a PopupEditorComponent for editing the selected button in the active 
 * AppFolder.
 */
void AppMenuComponent::openEditorForSelected()
{
    AppMenuButton::Ptr selected = getSelectedButton();
    if (selected != nullptr)
    {
        showMenuButtonEditor(selected);
    }
}

/**
 * @return the number of currently open folder components
 */
int AppMenuComponent::getNumFolders() const
{
    return openFolders.size();
}

/**
 * @return the number of buttons in the folder at this index, or -1
 * if there is no folder at this index.
 */
int AppMenuComponent::getFolderSize(int index)
{
    if (index < 0 || index >= openFolders.size())
    {
        return -1;
    }
    return openFolders.getUnchecked(index)->getButtonCount();
}

/**
 * @param index
 * @return the minimum width, in pixels, that the folder at this index
 * should have, or -1 if there is no folder at this index.
 */
int AppMenuComponent::getMinimumFolderWidth(int index)
{
    if (index < 0 || index >= openFolders.size())
    {
        return -1;
    }
    return openFolders.getUnchecked(index)->getMinimumWidth();
}

/**
 * @param index the index of one of this menu's AppMenuFolders
 * @return the selected menu button index for the folder, or -1 if
 * the folder at the index parameter doesn't exist or has no selected menu
 * button.
 */
int AppMenuComponent::getFolderSelectedIndex(int index) const
{
    if (index < 0 || index > openFolders.size())
    {
        return -1;
    }
    return openFolders.getUnchecked(index)->getSelectedIndex();
}

/**
 * Updates the folder component layout, optionally animating the transition.
 */
void AppMenuComponent::layoutFolders(bool animate)
{
    if (getBounds().isEmpty())
    {
        return;
    }
    DBG("AppMenuComponent::" << __func__
            << ": Updating all AppMenuFolder bounds");
    for (int i = 0; i < openFolders.size(); i++)
    {
        Rectangle<int> folderBounds = updateFolderBounds(openFolders[i], i);
        if (animate)
        {
            Desktop::getInstance().getAnimator().animateComponent(openFolders[i],
                    folderBounds, 1.0f, animationDuration, true, 0.0, 0.0);
        }
        else
        {
            openFolders[i]->setBounds(folderBounds);
        }
    }
}

/**
 * @return the maximum number of menu item columns to show on screen
 */
int AppMenuComponent::getMaxColumns() const
{
    return maxColumns;
}

/**
 * @return the maximum number of menu item rows to show on screen
 */
int AppMenuComponent::getMaxRows() const
{
    return maxRows;
}

/**
 * @param newVal if set to true, clicking unselected menu buttons 
 * only selects them. If set to false, clicking them also
 * immediately launches their application or opens their folder.
 * This is set to true by default.
 */
void AppMenuComponent::setOnlyTriggerSelected(bool newVal)
{
    onlyTriggerSelected = newVal;
}

/**
 * @return true iff the menu is loading, empty, animating, or otherwise
 * in a state where user input should be ignored.
 */
bool AppMenuComponent::ignoringInput() const
{
    return openFolders.isEmpty() || isLoading()
            || (buttonEditor != nullptr && buttonEditor->isVisible())
            || Desktop::getInstance().getAnimator().isAnimating
            (openFolders[getActiveFolderIndex()]);
}

/**
 * Exit the loading state if the window loses focus.
 */
void AppMenuComponent::windowFocusLost()
{
    setLoadingState(false);
}

/**
 * Updates the layout if row/column size changes.
 */
void AppMenuComponent::extraConfigValueChanged(String key)
{
    MainConfigFile mainConfig;

    if (key == MainConfigFile::maxColumnsKey)
    {
        maxColumns = mainConfig.getConfigValue<int>(key);
    }
    else if (key == MainConfigFile::maxRowsKey)
    {
        maxRows = mainConfig.getConfigValue<int>(key);
    }
    for (AppMenuFolder* folder : openFolders)
    {
        folder->updateGridSize(maxRows, maxColumns);
    }
    layoutFolders();
}

/**
 * Reposition child components, and update folder layout without animating.
 */
void AppMenuComponent::resized()
{
    menuResized();
    Rectangle<int> bounds = getLocalBounds();
    DBG("AppMenuComponent::" << __func__ << ": bounds set to "
            << getScreenBounds().toString());
    if (buttonEditor != nullptr)
    {
        buttonEditor->applyConfigBounds();
        buttonEditor->setCentrePosition(bounds.getCentreX(),
                bounds.getCentreY());
    }
    layoutFolders();
}

/**
 * Open an application category folder, creating or adding 
 * AppMenuButtons for all associated desktop applications.
 * If any folders after the active folder are already open, they
 * will first be closed.
 */
void AppMenuComponent::openFolder(AppMenuItem::Ptr folderItem)
{
    while (getActiveFolderIndex() < openFolders.size() - 1)
    {
        removeChildComponent(openFolders.getLast());
        openFolders.removeLast();
    }
    AppMenuFolder* newFolder = createFolderObject(folderItem, buttonNameMap);

    openFolders.add(newFolder);
    newFolder->addMouseListener(this, false);
    newFolder->updateGridSize(maxRows, maxColumns);
    addAndMakeVisible(newFolder);
    layoutFolders();
    setActiveFolderIndex(openFolders.size() - 1);
}

/**
 * @return the selected button in the active folder, or nullptr if
 * the active folder is empty or no folders are open.
 */
AppMenuButton::Ptr AppMenuComponent::getSelectedButton()
{
    if (!openFolders.isEmpty())
    {
        AppMenuFolder* folder = openFolders[getActiveFolderIndex()];
        return folder->getSelectedButton();
    }
    return nullptr;
}

/**
 * Adds and shows a new pop-up editor component, safely removing any previous
 * editor.
 */
void AppMenuComponent::showPopupEditor(AppMenuPopupEditor * editor)
{
    if (buttonEditor != nullptr)
    {
        removeChildComponent(buttonEditor);
    }
    buttonEditor = editor;
    addAndMakeVisible(buttonEditor);
    buttonEditor->setAlwaysOnTop(true);
    resized();
}

/**
 * Create and show a pop-up editor component for a button in this menu.
 * This will only work if the button is in the active folder and the menu 
 * is not ignoring input.
 */
void AppMenuComponent::showMenuButtonEditor(AppMenuButton::Ptr button)
{
    if (ignoringInput() ||
        openFolders[getActiveFolderIndex()]->getButtonIndex(button) == -1)
    {
        return;
    }

    showPopupEditor(button->getEditor(
            [this, button]
            (AppMenuPopupEditor * editor)
            {
                openFolders[getActiveFolderIndex()]->repaint();
                if (button->getMenuItem()->changesDesktopEntries())
                {
                    desktopEntries.loadEntries([](String s)
                    {
                    }, []()
                    {
                    });
                }
            }));
}

/**
 * Sets what should happen when a button is left clicked.
 * This opens selected buttons, and selects unselected buttons
 */
void AppMenuComponent::onButtonClick(AppMenuButton::Ptr button)
{
    if (ignoringInput())
    {
        return;
    }
    for (int i = 0; i < openFolders.size(); i++)
    {
        int buttonIndex = openFolders[i]->getButtonIndex(button);
        //if button is not in this folder, move to the next one
        if (buttonIndex == -1)
        {
            continue;
        }
        //if the button is not in the last opened folder, close folders until
        //the button's folder is the last one.  Then, select the button.
        while (i < getActiveFolderIndex())
        {
            closeFolder();
            if (i == openFolders.size() - 1)
            {
                openFolders[i]->selectIndex(buttonIndex);
                return;
            }
        }
        if (buttonIndex != openFolders[i]->getSelectedIndex())
        {
            openFolders[i]->selectIndex(buttonIndex);
            layoutFolders();
            if (onlyTriggerSelected)
            {
                return;
            }
        }
        AppMenuItem::Ptr buttonItem = button->getMenuItem();
        if (buttonItem->isFolder())
        {
            openFolder(buttonItem);
            layoutFolders(true);
        }
        else
        {
            setLoadingState(true);
            loadingSpinner.setLoadingText(String("Launching ")
                    + buttonItem->getAppName());
            appLauncher.startOrFocusApp(buttonItem->getAppName(),
                    buttonItem->getCommand());
        }

    }
}

/**
 * Click AppMenuButtons on left click, open the pop-up menu
 * on right click or control click.
 */
void AppMenuComponent::mouseDown(const MouseEvent & event)
{
    if (ignoringInput())
    {
        return;
    }

    AppMenuButton* appClicked = dynamic_cast<AppMenuButton*>
            (event.originalComponent);
    //handle right clicks/ctrl-clicks
    if (event.mods.isPopupMenu() || event.mods.isCtrlDown())
    {
        if (appClicked == nullptr
            || appClicked == getSelectedButton())
        {
            openPopupMenu(appClicked);
        }
        else
        {
            //On right click/ctrl-click, clicking an unselected
            //button should never do anything other than change
            //the selection
            bool selectOnly = onlyTriggerSelected;
            onlyTriggerSelected = true;
            onButtonClick(appClicked);
            onlyTriggerSelected = selectOnly;
        }

    }//handle regular clicks
    else if (event.mods.isLeftButtonDown() && appClicked != nullptr)
    {
        onButtonClick(appClicked);
    }
}

/**
 * Enter or exit the loading state, where the component shows the
 * loading spinner and disables user input.
 */
void AppMenuComponent::setLoadingState(bool loading)
{
    DBG("AppMenuComponent::" << __func__
            << (loading ? ": started loading" : ": stopped loading"));
    if (loading != isLoading())
    {
        loadingState = loading;
        loadingSpinner.setVisible(loading);
        if (!loading)
        {
            loadingSpinner.setLoadingText("");
        }
    }
}

