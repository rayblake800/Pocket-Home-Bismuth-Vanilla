#include <set>
#include "../Popup Editor Components/NewConfigAppEditor.h"
#include "../Popup Editor Components/NewDesktopAppEditor.h"
#include "../Popup Editor Components/NewFolderEditor.h"
#include "../AppMenuButton/AppMenuItemFactory.h"
#include "../HomePage.h"
#include "AppMenuComponent.h"

DesktopEntries AppMenuComponent::desktopEntries;

AppMenuComponent::AppMenuComponent
(String componentKey, OverlaySpinner& loadingSpinner) :
ConfigurableComponent(componentKey),
loadingSpinner(loadingSpinner)
{
    applyConfigBounds();
    setWantsKeyboardFocus(false);
    loadBaseFolder();
}

AppMenuComponent::~AppMenuComponent() { }

/**
 * @return true if the AppMenuComponent is currently in the loading state
 */
bool AppMenuComponent::isLoading()
{
    const ScopedLock menuLock(loadingLock);
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
 * Open the pop-up menu used for editing this object.
 * @param selectionMenu if true, the pop-up menu contains only options for 
 * editing the selected menu button.
 */
void AppMenuComponent::openPopupMenu(bool selectionMenu)
{
    if (isLoading())
    {
        return;
    }
    PopupMenu appMenu;
    AppMenuButton::Ptr selectedButton = selectionMenu ?
            openFolders.getLast()->getSelectedButton() : nullptr;

    AppMenuItem::Ptr selectedMenuItem = selectedButton == nullptr ?
            nullptr : selectedButton->getMenuItem();

    /**
     * The popup menu really shouldn't be appearing when the button editor
     * is visible, if it still is, close and remove it.
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
        appMenu.addItem(1, "Edit");
        appMenu.addItem(2, "Delete");
        if (selectedMenuItem->isFolder())
        {
            appMenu.addItem(4, "New application link");
        }
        else if (selectedButton->getIndexOfFolder() != 0)
        {
            appMenu.addItem(6, "Pin to favorites");
        }
        if (selectedMenuItem->canChangeIndex(-1))
        {
            appMenu.addItem(7, "Move back");
        }
        if (selectedMenuItem->canChangeIndex(1))
        {
            appMenu.addItem(8, "Move forward");
        }
    }
    else
    {
        appMenu.addItem(3, "New favorite application link");
        appMenu.addItem(4, "New application link");
        appMenu.addItem(5, "New folder");
    }

    AppFolder* lastFolder = openFolders.getLast();
    const int selection = appMenu.show();
    std::function<void() > confirmNew = [lastFolder]()
    {
        loadBaseFolder();
    };
    AppConfigFile appConfig;
    switch (selection)
    {
        case 1://User selects "Edit"
            showMenuButtonEditor(lastFolder->getSelectedButton());
            break;
        case 2://User selects "Delete"
            selectedButton->confirmRemoveButtonSource([this]()
            {
                lastFolder->removeButton(lastFolder->getSelectedIndex());
                layoutFolders(false);
            });
            break;
        case 3://User selects "New favorite application"
            showPopupEditor(new NewConfigAppEditor(appConfig, iconThread,
                    confirmNew));
            break;
        case 4://User selects "New application link"
        {
            AppMenuPopupEditor* newAppEditor = new NewDesktopAppEditor
                    (iconThread, confirmNew);
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
            AppMenuPopupEditor* newFolderEditor = new NewFolderEditor
                    (appConfig, iconThread, confirmNew);
            showPopupEditor(newFolderEditor);
            break;
        }
        case 6://User selects "Pin to favorites"
        {
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
            lastFolder->swapButtons(lastFolder->getSelectedIndex(),
                    lastFolder->getSelectedIndex() - 1);
            break;
        case 8://User selects "Move forward"
            lastFolder->swapButtons(lastFolder->getSelectedIndex(),
                    lastFolder->getSelectedIndex() + 1);
    }
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
        int savedIndex = 0;
        if (!openFolders.isEmpty())
        {
            savedIndex = openFolders->getFirst()->getSelectedIndex();
        }
        while (openFolders.size() > 0)
        {
            closeFolder();
        }
        setLoadingState(true);
        desktopEntries.loadEntries([this](String loadingMsg)
        {
            loadingSpinner.setLoadingText(loadingMsg);
        },
        [this, savedIndex]()
        {
            setLoadingState(false);
            openFolder(AppMenuItemFactory::createBaseFolderItem(desktopEntries));
            openFolders->getFirst()->selectIndex(savedIndex);
            layoutFolders(false);
        });
    }
}

/**
 * close the last opened folder, removing all contained buttons from
 * view
 */
void AppMenuComponent::closeFolder()
{
    if (openFolders.size() > 0)
    {
        removeChildComponent(openFolders.getLast());
        openFolders.removeLast();
    }
}

/**
 * Trigger a click for the selected button in the last opened AppFolder
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
 * Open a PopupEditorComponent for editing the selected button in the last
 * opened AppFolder.
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
 * Offset the selected button index in the last opened AppFolder.
 * @param offset will be added to the index if index + offset is a valid
 * index.
 * @return true if the selected button index changed, false if index +
 * offset was an invalid index and the selection did not change. 
 */
bool AppMenuComponent::changeSelection(int offset)
{
    AppFolder * lastFolder = openFolders.getLast();
    if (lastFolder == nullptr ||
        !lastFolder->selectIndex(lastFolder->getSelectedIndex() + offset))
    {
        return false;
    }
    layoutFolders(true);
}

/**
 * @return the selected button in the last opened folder, or nullptr if
 * the last folder is empty or no folders are open.
 */
AppMenuButton::Ptr AppMenuComponent::getSelectedButton()
{
    if (!openFolders.isEmpty() && openFolders.getLast()->size() > 0)
    {
        return openFolders.getLast()->getSelectedButton();
    }
    return nullptr;
}

/**
 * Open an application category folder, creating or adding 
 * AppMenuButtons for all associated desktop applications.
 */
void AppMenuComponent::openFolder(AppMenuItem::Ptr folderItem)
{
    AppFolder* newFolder = createFolderObject(folderItem);
    openFolders.add(newFolder);
    addAndMakeVisible(newFolder);
}

/**
 * Reposition child components, and update folder layout without animating.
 */
void AppMenuComponent::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    loadingSpinner.setBounds(bounds);
    if (buttonEditor != nullptr)
    {
        buttonEditor->applyConfigBounds();
        buttonEditor->setCentrePosition(bounds.getCentreX(),
                bounds.getCentreY());
    }
}

/**
 * Adds and shows a new popup editor component, safely removing any previous
 * editor.
 */
void AppMenuComponent::showPopupEditor(PopupEditorComponent* editor)
{
    if (buttonEditor != nullptr)
    {
        removeChildComponent(buttonEditor);
    }
    buttonEditor = editor;
    addAndMakeVisible(buttonEditor);
    resized();
}

/**
 * Create and show a popup editor component for a button in this menu.
 * This will only work if the button is in the last opened folder and the menu 
 * is not in the loading state. 
 */
void AppMenuComponent::showMenuButtonEditor(AppMenuButton::Ptr button)
{
    if (!isLoading()
        && openFolders.getLast()->getButtonIndex(button) != -1)
    {

        showPopupEditor(button->getEditor([this](AppMenuPopupEditor * editor)
        {
            openFolders.getLast()->reload();
        }));
    }
}

/**
 * Sets what should happen when a button is left clicked.
 * This opens selected buttons, and selects unselected buttons
 */
void AppMenuComponent::onButtonClick(AppMenuButton::Ptr button)
{
    if (isLoading())
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
        while (i < openFolders.size() - 1)
        {
            closeFolder();
            if (i == openFolders.size() - 1)
            {
                openFolders[i]->selectIndex(buttonIndex);
                return;
            }
        }
        //If the button is the selected button in the last opened folder,
        //open its folder or launch its application.
        if (buttonIndex == openFolders[i]->getSelectedIndex())
        {
            AppMenuItem::Ptr buttonItem = button->getMenuItem();
            if (buttonItem->isFolder())
            {
                openFolder(buttonItem);
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
        else
        {
            openFolders[i]->selectIndex(buttonIndex);
        }
        return;
    }
}

/**
 * Click AppMenuButtons on left click, open the popup menu
 * on right click or control click.
 */
void AppMenuComponent::mouseDown(const MouseEvent &event)
{
    //ignore clicks when loading or when the editor is open
    if (!isLoading() &&
        (buttonEditor == nullptr || !buttonEditor->isVisible()))
    {

        AppMenuButton* appClicked = dynamic_cast<AppMenuButton*>
                (event.originalComponent);
        if (appClicked == nullptr)
        {
            return;
        }
        //handle right clicks/ctrl-clicks
        if (event.mods.isPopupMenu() || event.mods.isCtrlDown())
        {
            if (appClicked == nullptr ||
                appClicked == getSelectedButton())
            {
                openPopupMenu(appClicked == getSelectedButton());
            }
            else
            {
                onButtonClick(appClicked);
            }

        }//handle regular clicks
        else if (event.mods.isLeftButtonDown() && appClicked != nullptr)
        {
            onButtonClick(appClicked);
        }
    }
}

/**
 * Enter or exit the loading state, where the component shows the
 * loading spinner and disables user input.
 */
void AppMenuComponent::setLoadingState(bool loading)
{
    if (loading != isLoading())
    {
        const ScopedLock menuLock(loadingLock);
        loadingState = loading;
        loadingSpinner.setVisible(loading);
        if (!loading)
        {
            loadingSpinner.setLoadingText("");
        }
    }
}


//############################# AppFolder ######################################
//#################### Menu Folder Component Class #############################

AppMenuComponent::AppFolder::AppFolder
(AppMenuItem::Ptr folderItem, MouseListener* btnListener) :
sourceFolderItem(folderItem),
btnListener(btnListener)
{
    reload();
}

AppMenuComponent::AppFolder::~AppFolder() { }

/**
 * Reload all folder menu buttons from their source menu item. 
 */
void AppMenuComponent::AppFolder::reload()
{
    removeAllChildren();
    folderButtons = createMenuButtons(sourceFolderItem->getMenuItems());
    if (btnListener != nullptr && !folderButtons.isEmpty())
    {
        for (const AppMenuButton::Ptr& button : folderButtons)
        {
            button->addListener(btnListener);
        }
        folderButtons[0]->setToggleState
                (true, NotificationType::sendNotification);
    }
    layoutButtons();
}

/**
 * @return number of menu buttons in the folder.
 */
int AppMenuComponent::AppFolder::size()
{
    return folderButtons.size();
}

/**
 * Find the index of a menu button in this folder
 */
bool AppMenuComponent::AppFolder::getButtonIndex
(AppMenuButton::Ptr menuButton)
{
    return folderButtons.indexOf(menuButton);
}

/**
 * @return The selected menu button in this folder.
 * This will only return nullptr if there are no buttons in 
 * this folder.
 */
AppMenuButton::Ptr AppMenuComponent::AppFolder::getSelectedButton()
{
    if (!validBtnIndex(selectedIndex))
    {
        return nullptr;
    }
    return folderButtons[selectedIndex];
}

/**
 * Set this folder's selected menu button
 */
bool AppMenuComponent::AppFolder::selectIndex(int index)
{
    if (validBtnIndex(index))
    {
        if (validBtnIndex(selectedIndex))
        {
            folderButtons[selectedIndex]->setToggleState
                    (false, NotificationType::sendNotification);
        }
        selectedIndex = index;
        folderButtons[selectedIndex]->setToggleState
                (true, NotificationType::sendNotification);
        return true;
    }
    return false;
}

/**
 * @return the index of the selected menu button.
 */
int AppMenuComponent::AppFolder::getSelectedIndex()
{
    return selectedIndex;
}

/**
 * Insert a new button to the folder at a specific index,
 * shifting forward any buttons at indices equal or greater
 * than the index. 
 * 
 * @param index should be between 0 and appFolder.size(),
 * inclusive.  Values outside of this range will be rounded to
 * the nearest valid value.
 */
void AppMenuComponent::AppFolder::insertButton
(AppMenuButton::Ptr newButton, int index)
{
    if (index < 0)
    {
        index = 0;
    }
    if (index > size())
    {
        folderButtons.add(newButton);
    }
    else
    {
        folderButtons.insert(index, newButton);
        if (btnListener != nullptr)
        {
            newButton->addListener(btnListener);
        }
    }
    if (selectedIndex >= index)
    {
        selectedIndex++;
    }
    layoutButtons();
}

/**
 * Remove the button at a given index, shifting back any buttons
 * at greater indices to fill the gap
 */
void AppMenuComponent::AppFolder::removeButton(int index)
{
    if (validBtnIndex(index))
    {
        folderButtons.remove(index);
        layoutButtons();
    }
}

/**
 * Swap the indices and positions of two buttons in the folder.
 * Both indices must be valid, or nothing will happen.
 */
void AppMenuComponent::AppFolder::swapButtons
(int btnIndex1, int btnIndex2)
{
    if (validBtnIndex(btnIndex1) && validBtnIndex(btnIndex2))
    {
        AppMenuButton::Ptr btn1 = folderButtons[btnIndex1];
        folderButtons[btnIndex1] = folderButtons[btnIndex2];
        folderButtons[btnIndex2] = btn1;
        if (selectedIndex == btnIndex1)
        {
            selectedIndex = btnIndex2;
        }
        else if (selectedIndex == btnIndex2)
        {
            selectedIndex = btnIndex1;
        }
        layoutButtons();
    }
}

/**
 * Set the relative spacing of the folder component layout.
 */
void AppMenuComponent::AppFolder::setSpacing(float margin,
        float xPadding, float yPadding)
{
    this->margin = margin;
    this->xPadding = xPadding;
    this->yPadding = yPadding;
    resized();
}

/**
 * Reposition folder buttons when folder bounds change.
 */
void AppMenuComponent::AppFolder::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(margin * getWidth(), margin * getWidth());
    folderLayout.layoutComponents(bounds, getWidth() * xPadding,
            getWidth() * yPadding);
}

/**
 * Clear folderLayout,remove all child components, reload the
 * button layout, and re-add the layout buttons as child
 * components.
 */
void AppMenuComponent::AppFolder::layoutButtons()
{
    while (!validBtnIndex(selectedIndex) && selectedIndex != 0)
    {
        selectedIndex--;
    }
    folderLayout.clearLayout(true);
    folderLayout.setLayout(buildFolderLayout(folderButtons), this);
}



