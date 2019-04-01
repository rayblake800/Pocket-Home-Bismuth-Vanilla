##### MainMenu.pm ##############################################################
# Constructs the main ColourId management menu.                                #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the main menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: an IDCache object to use for menu options.
#==============================================================================#

use strict;
use warnings;

package MainMenu;
use lib './project-scripts/ColourID/Menus';
use InputMenu;
use EnumMenu;

# Displays the main menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "ColourId management tool options:",
            "Exit program:",
            undef,
            $cache);
    $menu->addOption("Scan project for new ColourId enums",
            \&enumScan);
    $menu->addOption("Edit Element namespaces",
            \&namespaceMenu);
    $menu->addOption("Edit configuration keys",
            \&keyMenu);
    $menu->addOption("Assign UI categories",
            \&categoryMenu);
    $menu->addOption("Update Colour files",
            \&updateFiles);
    $menu->openMenu();
}

sub todo
{
    print("TODO: ".shift."\n");
    print("press enter to continue:");
    <STDIN>
}

sub enumScan
{
    my $cache = shift;
    EnumMenu::openMenu($cache);
}

sub namespaceMenu
{
    my $cache = shift;
    #IOUtils::printNamespaces($cache);
    todo("Implement namespace menu\n");
}

sub keyMenu
{
    todo("Implement key menu\n");
}

sub categoryMenu
{
    todo("Implement key menu\n");
}

sub updateFiles
{
    todo("Implement file update\n");
}
1;
