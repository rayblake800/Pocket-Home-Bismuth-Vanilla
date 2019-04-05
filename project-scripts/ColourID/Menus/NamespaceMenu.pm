##### NamespaceMenu.pm #########################################################
# Creates the menu used for editing a single ColourId namespace.
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the namespace menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $namespace:  The name of the Namespace object this menu should edit.
#
# $cache:      An IDCache object used to store Namespace updates.
#==============================================================================#

#==============================================================================#
#--- renameNamespace: ---
# Prompts the user to rename the edited namespace, then applies the new name and
# closes the menu if the name is valid and the user chooses to confirm.
#--- Parameters: ---
# $menu:          The NamespaceMenu object.
#
# $cache:         The IDCache where the Namespace will be updated.
#
# $namespaceName: The name of the Namespace to update.
#==============================================================================#

#==============================================================================#
#--- deleteNamespace: ---
# If the user confirms the action, deletes the edited namespace and closes the
# namespace menu.
#--- Parameters: ---
# $menu:          The NamespaceMenu object.
#
# $cache:         The IDCache where the Namespace will be deleted.
#
# $namespaceName: The name of the Namespace to remove.
#==============================================================================#

use strict;
use warnings;

package NamespaceMenu;
use lib './project-scripts/ColourID';
use UserInput;
use lib './project-scripts/ColourID/Menus';
use InputMenu;
use ElementMenu;

# Displays the namespace menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $namespaceName = shift;
    my $cache = shift;
    my $menu = new InputMenu(
            "Edit Namespace $namespaceName:",
            "Return to namespace list menu");
    my $refreshAction = sub
    {
        my $menu = shift;
        $menu->clearOptions();
        $menu->addOption("Rename namespace", sub
        {
            renameNamespace($menu, $cache, $namespaceName);
        });
        $menu->addOption("Delete namespace", sub
        {
            deleteNamespace($menu, $cache, $namespaceName);
        });
        my $namespace = $cache->findNamespace($namespaceName);
        if(defined($namespace))
        {
            my @elements = $namespace->getElements();
            foreach my $element(@elements)
            {
                $menu->addOption("Edit Element ".$element->getID()
                        ." (".$element->getName().")",
                        sub { openElementMenu($element, $cache); });
            }
        }
    };
    $menu->setRefreshAction($refreshAction);
    $menu->openMenu();
}

sub renameNamespace
{
    my $menu = shift;
    my $cache = shift;
    my $namespaceName = shift;
    my $newName = UserInput::inputText(
            '[a-zA-Z_][a-zA-Z_0-9]*',
            "Enter the new name for Namespace $namespaceName:",
            undef,
            "Save new name for $namespaceName?");
    if(!$newName || $newName eq $namespaceName)
    {
        return;
    }
    my $namespace = $cache->findNamespace($namespaceName);
    my @elements = $namespace->getElements();
    foreach my $element(@elements)
    {
        my $replacement = new Element($newName,
                $element->getName(),
                $element->getID(),
                $element->getCategory(),
                $element->getKey(),
                $element->getDefaultColour());
        $cache->removeElement($element);
        $cache->addElement($replacement);
    }
    $menu->closeMenu();
}

sub deleteNamespace
{
    my $menu = shift;
    my $cache = shift;
    my $namespaceName = shift;
    print("Delete namespace $namespaceName?");
    if(!UserInput::confirm())
    {
        return;
    }
    my $namespace = $cache->findNamespace($namespaceName);
    my @elements = $namespace->getElements();
    foreach my $element(@elements)
    {
        $cache->removeElement($element);
    }
    $menu->closeMenu();
}

# Displays a menu for editing a single namespace Element.
sub openElementMenu
{
    my $element = shift;
    my $cache = shift;
    ElementMenu::openMenu($element, $cache);
}
1;
