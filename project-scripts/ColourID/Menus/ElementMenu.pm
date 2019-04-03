##### ElementMenu.pm ###########################################################
# Creates the menu used for editing a single ColourId element
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the element menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $element:  The element object to edit.
#
# $cache:    The IDCache object where changes will be saved.
#==============================================================================#

use strict;
use warnings;

package ElementMenu;
use lib './project-scripts/ColourID';
use UserInput;
use lib './project-scripts/ColourID/DataObjects';
use IDCache;
use Category;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

# Displays the element menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $element = shift;
    my $cache = shift;
    my $menu = new InputMenu(
            "Element ".$element->getFullName()
            ."\nID: ".$element->getID()
            ."\nCategory: ".$element->getCategory()->getTypeName()
            ."\nKey: ".$element->getKey()
            ."\nEdit Element:",
            "Return to main menu:");
    $menu->addOption("Rename Element", sub
            { renameElement($menu, $element, $cache); });
    $menu->addOption("Set category", sub
            { setCategory($menu, $element, $cache); });
    $menu->addOption("Set key", sub
            { setKey($menu, $element, $cache); });
    $menu->openMenu();
}

sub renameElement
{
    my $menu = shift;
    my $element = shift;
    my $cache = shift;
    my $newName = UserInput::inputName();
    if($newName)
    {
        my $replacement = new Element($newName,
                $element->getName(),
                $element->getID(),
                $element->getCategory());
        $replacement->setKey($element->getKey());
        $cache->removeElement($element);
        $cache->addElement($replacement);
        $menu->closeMenu();
    }
}

sub setCategory
{
    my $menu = shift;
    my $element = shift;
    my $cache = shift;
    my @validOptions = ('q');
    print("Select a category:\n");
    for(my $i = 0; $i < Category::NUM_TYPES; $i++)
    {
        my $category = new Category($i);
        print("$i: ".$category->getTypeName()."\n");
        push(@validOptions, $i);
    }
    print("q: Cancel choice:");
    my $selection = UserInput::checkInput(@validOptions);
    if($selection ne 'q')
    {
        my $selectedCategory = new Category($selection);
        my $replacement = new Element($element->getNamespace(),
                $element->getName(),
                $element->getID(),
                $selectedCategory);
        $replacement->setKey($element->getKey());
        $cache->removeElement($element);
        $cache->addElement($replacement);
        $menu->closeMenu();
    }
}

sub setKey
{
    my $menu = shift;
    my $element = shift;
    my $cache = shift;
    my $newKey = UserInput::inputText('.*', "Enter the new element key:");
    if($newKey)
    {
        $cache->assignKey($element->getID(), $newKey);
        $menu->closeMenu();
    }
}

1;
