# Adding a new AppMenu Format
The application menu is designed to support multiple display formats, each with a unique appearance and control scheme. Follow this guide to implement a new menu format:

### Planning
1. The menu format controls where menu items draw their icon and title, how menu items are arranged in their folders, how menu folders are arraged on the screen, and how user input is applied to the menu. Decide exactly how your format is going to manage these tasks.
2. Choose a single word name for the new format that describes its design. This document will use `NewFormat` as an example new menu format type.

### Initial Format Declaration
1. Within the [AppMenu::Format](../../Source/GUI/AppMenu/Formats/AppMenu_Format.h) enum, add your new format as an enum value.

### Creating New Format Classes
1. Create a directory named after your format in `[ProjectDir]/Source/GUI/AppMenu/Formats/`.
2. Add a class to the new format directory named **AppMenu::NewFormat::MenuButton**, implementing [AppMenu::MenuButton](../../Source/GUI/AppMenu/Components/AppMenu_MenuButton.h).
    * Implement `AppMenu::NewFormat::MenuButton::findTitleBounds() const` so that it returns the area where the MenuButton should draw its title relative to its own origin.
    * Implement `AppMenu::NewFormat::MenuButton::findIconBounds() const` so that it returns the area where the MenuButton should draw its icon relative to its own origin.
    * Implement `AppMenu::NewFormat::MenuButton::shouldDrawBorder() const` so that it returns true when your menu buttons should have an outline drawn around them.
    * Implement `AppMenu::NewFormat::MenuButton::shouldFillBackground() const` so that it returns true when your menu buttons should have a background drawn behind them.
    * Implement `AppMenu::NewFormat::MenuButton::getTextJustification() const` so that it returns the justification used when drawing the button's title.
    * Optionally override `AppMenu::MenuButton::findTitleFont(const juce::Rectangle<float>& titleBounds) const` to return the font used to draw the menu button title.
    * Optionally override `AppMenu::MenuButton::findTitleBGWidth(const juce::Rectangle<float>& titleBounds, const juce::Font& titleFont) const` to return the width of the background that should be drawn behind the button's title.
    * Optionally override `AppMenu::MenuButton::menuButtonResized()` to define actions the button should take when its bounds are adjusted.
3. Add a class to the new format directory named **AppMenu::NewFormat::FolderComponent**, implementing [AppMenu::FolderComponent](../../Source/GUI/AppMenu/Components/AppMenu_FolderComponent.h).
    * Implement `AppMenu::NewFormat::FolderComponent::closestIndex(const int xPos, const int yPos) const` to calculate which menu button index is closest to a coordinate in the folder component.
    * Implement `AppMenu::NewFormat::FolderComponent::updateButtonLayout()` to set the bounds of each menu button in the folder.
    * Implement `AppMenu::NewFormat::FolderComponent::createMenuButton(const MenuItem folderItem) const` to return a new AppMenu::NewFormat::MenuButton object created to represent the given folder item.
4. Add a class to the new format directory named **AppMenu::NewFormat::MenuComponent**, implementing [AppMenu::MenuComponent](../../Source/GUI/AppMenu/Components/AppMenu_MenuComponent.h).
    * Implement `AppMenu::NewFormat::MenuComponent::parentResized(const juce::Rectangle<int> parentBounds)` to return position and size that should be applied to the entire menu when the parent bounds change.
    * Implement `AppMenu::NewFormat::MenuComponent::createFolderComponent(MenuItem folderItem)` to return a new AppMenu::NewFormat::FolderComponent constructed to represent the given menu folder item.
    * Implement `AppMenu::NewFormat::MenuComponent::initialFolderBounds(const int newFolderIndex) const` to return the bounds that should be set for a new folder component before its menu item buttons are loaded.
    * Implement `AppMenu::NewFormat::MenuComponent::getFolderBounds(const int folderIndex, const bool closingFolder) const` to return the bounds where an individual open folder should be placed.
    * Implement `AppMenu::NewFormat::MenuComponent::getAnimationDuration() const` to return the duration in milliseconds to use for folder animations.
    * Optionally override `AppMenu::MenuComponent::layoutUpdateStarting(const bool closingFolder)` to add any additional actions the menu should take before it updates its folder layout.
5. Add a class to the new format directory named **AppMenu::NewFormat::InputHandler**, implementing [AppMenu::InputHandler](../../Source/GUI/AppMenu/Control/AppMenu_InputHandler.h).
    * Implement `AppMenu::NewFormat::InputHandler::keyPressed(const KeyType keyType)` to define what the menu should do when any menu control key is pressed.
    * Optionally override `AppMenu::InputHandler::menuItemClicked(const MenuButton* clickedButton, const bool rightClicked)` to change what the menu does when a menu button is clicked.
    * Optionally override `AppMenu::InputHandler::folderClicked(const FolderComponent* clickedfolder, const int closestIndex, const bool rightClicked)` to change what the menu does when a menu folder is clicked in a spot that does not contain a menu button.
5. Add a class to the new format directory named **AppMenu::NewFormat::Initializer**, implementing [AppMenu::Initializer](../../Source/GUI/AppMenu/Control/AppMenu_Initializer.h).
    * Implement `AppMenu::NewFormat::Initializer::getMenuFormat() const` to return your menu's AppMenu::Format value.
    * Implement `AppMenu::NewFormat::Initializer::createMenuComponent() const` to return a new AppMenu::NewFormat::MenuComponent.
    * Implement `AppMenu::NewFormat::Initializer::createInputHandler(MenuComponent* const menuComponent, Controller* const controller) const` to return a new AppMenu::NewFormat::InputHandler.

### Integrating the New Format
1. Add all new format classes to the [AppMenu makefile](../../makefiles/AppMenu.mk), using the other menu formats as an example. 
2. Within [AppMenu::ConfigKeys](../../Source/GUI/AppMenu/Data/JSON/AppMenu_ConfigKeys.h), edit the formatStrings map to include your new format, mapped to your format's name string, e.g. `{Format::NewFormat, "NewFormat"}`.
3. Within [AppMenu::MainComponent](../../Source/GUI/AppMenu/Components/AppMenu_MainComponent.cpp), update the switch statement in the `AppMenu::MainComponent::FormatUpdater::applySelectedFormat()` function to handle the new format:

```C++
case Format::NewFormat:
    initializer.reset(new NewFormat::Initializer());
    break;
```
4. Update [AppMenu::SettingsController](../../Source/GUI/AppMenu/AppMenu_SettingsController.cpp) to allow the user to select the new format.
    * In TextKey::formats, add an additional FormatKey object, passing it your menu format value and a format name locale key as parameters. Make sure to add and define the locale key in all files under `[ProjectDir]/assets/locale/` within the "AppMenu::SettingsController" JSON object.

### Updating Format Documentation
1. Document all new format classes in the [AppMenu module documentation](../modules/AppMenu.md).
2. Update the [menuOptions.json documentation](../configuration/menuOptions.md) to list the new menu format name as a valid AppMenu format option.
