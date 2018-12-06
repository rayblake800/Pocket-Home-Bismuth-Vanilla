#define APPMENU_IMPLEMENTATION
#include "AppMenu/Control/Controller.h"
#include "AppMenu/Components/Editors/ExistingItemEditor.h"
#include "AppMenu/Components/Editors/NewConfigItemEditor.h"
#include "AppMenu/Components/Editors/NewDesktopAppEditor.h"
#include "AppMenu/Data/JSON/MenuFile.h"
#include "Config/MainFile.h"
#include "Config/MainKeys.h"
#include "Utils.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey        = "AppMenu::Controller";

/* Localixed text value keys: */
static const juce::Identifier editTextKey           = "edit";
static const juce::Identifier deleteTextKey         = "delete";
static const juce::Identifier pinItemTextKey        = "pinItem";
static const juce::Identifier moveBackTextKey       = "moveBack";
static const juce::Identifier moveForwardTextKey    = "moveForward";
static const juce::Identifier newShortcutTextKey    = "newShortcut";
static const juce::Identifier newEntryTextKey       = "newEntry";
static const juce::Identifier newFolderTextKey      = "newFolder";
static const juce::Identifier launchingAPPTextKey   = "launchingAPP";

/*
 * Creates a new menu controller.
 */
AppMenu::Controller::Controller
(MenuComponent* menuComponent, OverlaySpinner& loadingSpinner) : 
    menuComponent(menuComponent), 
    loadingSpinner(loadingSpinner),
    Locale::TextUser(localeClassKey) { } 

/*
 * Displays a context menu with options for editing an open menu folder.
 */
void AppMenu::Controller::showContextMenu
(const MenuItem folderItem, const int insertIndex)
{
    // Ignore request if a menu editor is visible or input is disabled.
    if(ignoringInput())
    {
        return;
    }
    juce::PopupMenu contextMenu;
    jassert(!folderItem.isNull());
    contextMenu.addItem(int(OptionCode::NewShortcut), 
            localeText(newShortcutTextKey));
    contextMenu.addItem(int(OptionCode::NewFolder), 
            localeText(newFolderTextKey));
    contextMenu.addItem(int(OptionCode::NewDesktopEntry), 
            localeText(newEntryTextKey));
    handleContextMenuAction(OptionCode(contextMenu.show()), folderItem,
            insertIndex);
}

/*
 * Displays a context menu with options for editing a single menu item.
 */
void AppMenu::Controller::showContextMenu(const MenuItem menuItem)
{  
    // Ignore request if a menu editor is visible or input is disabled.
    if(ignoringInput())
    {
        return;
    }
    juce::PopupMenu contextMenu;
    jassert(!menuItem.isNull());

    contextMenu.addItem(int(OptionCode::Edit), localeText(editTextKey));
    contextMenu.addItem(int(OptionCode::Delete), localeText(deleteTextKey));
    
    MenuFile appConfig;
    const MenuItem rootFolder = appConfig.getRootFolderItem();
    const MenuItem parent = menuItem.getParentFolder();
    if(parent != rootFolder)
    {
        contextMenu.addItem(int(OptionCode::PinToRoot), 
                localeText(pinItemTextKey));
    }

    if(menuItem.isFolder())
    {
        contextMenu.addItem(int(OptionCode::NewShortcut), 
                localeText(newShortcutTextKey));
    }

    // The menu item parameter should always have a valid parent and index.
    jassert(menuItem.getIndex() >= 0);
    jassert(!parent.isNull());
    int lastMovableIndex = parent.getMovableChildCount() - 1;
    if(menuItem.getIndex() <= lastMovableIndex)
    {
        if(menuItem.getIndex() != 0)
        {
            contextMenu.addItem(int(OptionCode::MoveBack), 
                    localeText(moveBackTextKey));
        }
        if(menuItem.getIndex() != lastMovableIndex)
        {
            contextMenu.addItem(int(OptionCode::MoveForward), 
                    localeText(moveForwardTextKey));
        }
    }
    handleContextMenuAction(OptionCode(contextMenu.show()), menuItem,
            rootFolder.getMovableChildCount());
}

/*
 * Activates an item in the menu, launching application shortcut menu items, and
 * opening folder menu items.
 */
void AppMenu::Controller::activateMenuItem(const MenuItem clickedItem)
{
    // Ignore clicks if a menu editor is visible or input is disabled.
    if(ignoringInput())
    {
        return;
    }
    // If the menu item is not in the active folder, close folders until it is.
    const MenuItem parentFolder = clickedItem.getParentFolder();
    if(parentFolder != menuComponent->getActiveFolder())
    {
        while(parentFolder != menuComponent->getActiveFolder()
            && menuComponent->openFolderCount() > 1)
        {
            menuComponent->closeActiveFolder();
        }
        jassert(parentFolder == menuComponent->getActiveFolder());
    }
    // Open folder items and launch/focus shortcut items.
    if(clickedItem.isFolder())
    {
        openFolder(clickedItem);
    }
    else
    {
        const juce::String loadingText = localeText(launchingAPPTextKey) + " "
            + clickedItem.getTitle();
        setLoadingState(true, loadingText);
        launchOrFocusApplication(clickedItem);
    }
}
    
/*
 * Sets if the Controller should show the loading spinner and ignore input.
 */
void AppMenu::Controller::setLoadingState
(const bool isLoading, const juce::String loadingText)
{
    loadingSpinner.setLoadingText(loadingText);
    loadingSpinner.setVisible(isLoading);
}

/*
 * Checks if the controller is currently ignoring user input.
 */
bool AppMenu::Controller::ignoringInput() const
{
    return loadingSpinner.isVisible() || menuComponent->showingEditor();
}


/*
 * Performs the appropriate action for a selected context menu option.
 */
void AppMenu::Controller::handleContextMenuAction(OptionCode selectedOption,
        MenuItem editedItem,
        const int insertIndex)
{
    switch(selectedOption)
    {
        case OptionCode::Cancelled:
            return;
        case OptionCode::Edit:
            jassert(!editedItem.isNull());
            createExistingItemEditor(editedItem);
            break;
        case OptionCode::Delete:
            jassert(!editedItem.isNull());
            confirmAction(editedItem.getConfirmDeleteTitle(),
                    editedItem.getConfirmDeleteMessage(),
                    [editedItem]() 
                    { 
                        MenuItem(editedItem).remove(true);
                    });
            break;
        case OptionCode::PinToRoot:
        {
            jassert(!editedItem.isNull());
            MenuFile appConfig;
            MenuItem rootFolder = appConfig.getRootFolderItem();
            copyMenuItem(editedItem, rootFolder, insertIndex);
            // Move to the new selected item:
            while(menuComponent->openFolderCount() > 1)
            {
                menuComponent->closeActiveFolder();
            }
            menuComponent->getOpenFolder(0)->setSelectedIndex(insertIndex);
            menuComponent->updateMenuLayout();
            break;
        }
        case OptionCode::MoveBack:
        case OptionCode::MoveForward:
        {
            jassert(!editedItem.isNull());
            MenuItem parent = editedItem.getParentFolder();
            int itemIndex = editedItem.getIndex();
            int swapIndex = itemIndex +
                ((selectedOption == OptionCode::MoveBack) ? -1 : 1);
            if(!parent.swapChildren(itemIndex, swapIndex))
            {
                DBG("AppMenu::Controller::" << __func__
                        << ": Couldn't swap indices " << itemIndex
                        << " and " << swapIndex);
            }
            break;
        }
        case OptionCode::NewShortcut:
            createNewShortcutEditor(editedItem, insertIndex);
            break;
        case OptionCode::NewFolder:
            createNewFolderEditor(editedItem, insertIndex);
            break;
        case OptionCode::NewDesktopEntry:
            createNewEntryEditor(editedItem);
            break;
        default:
            DBG("AppMenu::ContextMenu::" << __func__ 
                    << ": Unhandled menu option " << int(selectedOption));
            jassertfalse;
    }
}

/*
 * Launches or focuses an application from the menu.
 */
void AppMenu::Controller::launchOrFocusApplication(MenuItem toLaunch)
{
    using juce::String;
    String command = toLaunch.getCommand();
    if(toLaunch.getLaunchedInTerm())
    {
        Config::MainFile mainConfig;
        String termPrefix = mainConfig.getConfigValue<String>
            (Config::MainKeys::termLaunchCommandKey);
        command = termPrefix + " " + command;
    }
    setLoadingState(true);
    appLauncher.startOrFocusApp(command);
}

/*
 * Opens a folder menu item as a new FolderComponent.
 */
void AppMenu::Controller::openFolder(const MenuItem folderItem)
{
    menuComponent->openFolder(folderItem);
}

/*
 * Creates and shows a new PopupEditor component that can create a new 
 * application shortcut menu item.
 */
void AppMenu::Controller::createNewShortcutEditor
(const MenuItem folder, const int insertIndex)
{
    PopupEditor* newEditor = new NewConfigItemEditor(folder, false, insertIndex,
    [this]()
    {
        menuComponent->removeEditor();
    });
    menuComponent->saveAndShowEditor(newEditor);
}

/*
 * Creates and shows a new PopupEditor component that can create a new folder 
 * menu item.
 */
void AppMenu::Controller::createNewFolderEditor
(const MenuItem folder, const int insertIndex)
{
    PopupEditor* newEditor = new NewConfigItemEditor(folder, true, insertIndex,
    [this, folder, insertIndex]()
    {
        menuComponent->removeEditor();
    });
    menuComponent->saveAndShowEditor(newEditor);
}

/*
 * Creates and shows a new PopupEditor component that can create a new desktop 
 * entry.
 */
void AppMenu::Controller::createNewEntryEditor(const MenuItem categorySource)
{
    PopupEditor* newEditor = new NewDesktopAppEditor([this]()
    {
        menuComponent->removeEditor();
    });
    newEditor->setCategoryList(categorySource.getCategories());
    menuComponent->saveAndShowEditor(newEditor);
}

/*
 * Creates and shows a new PopupEditor component that edits an existing item
 * in the menu.
 */
void AppMenu::Controller::createExistingItemEditor(MenuItem toEdit)
{
    juce::StringArray oldCategories = toEdit.getCategories();
    PopupEditor* newEditor = new ExistingItemEditor(toEdit, 
    [this, oldCategories, toEdit]()
    {
        if(toEdit.isFolder() && oldCategories != toEdit.getCategories())
        {
            // Categories changed, refresh desktop entry items
            for(int i = toEdit.getFolderSize() - 1;
                    i >= toEdit.getMovableChildCount(); i--)
            {
               toEdit.getFolderItem(i).remove(false); 
            }
            entryLoader.loadFolderEntries(toEdit);
        }
        menuComponent->removeEditor();
    });
    menuComponent->saveAndShowEditor(newEditor);
}

/*
 * Creates a copy of a menu item at a new index in the menu.
 */
void AppMenu::Controller::copyMenuItem
(const MenuItem toCopy, MenuItem copyFolder, const int insertIndex)
{
    MenuFile appConfig;
    appConfig.addMenuItem(toCopy.getTitle(),
            toCopy.getIconName(),
            toCopy.getCommand(),
            toCopy.getLaunchedInTerm(),
            toCopy.getCategories(),
            copyFolder,
            insertIndex);
}
