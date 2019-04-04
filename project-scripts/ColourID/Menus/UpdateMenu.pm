##### UpdateMenu.pm ############################################################
# Constructs the menu used for updating project files with new data.           #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the update menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: The IDCache object source of updated colour data.
#==============================================================================#

use strict;
use warnings;

package UpdateMenu;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

# Displays the update menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "Project file update options:",
            "Exit to main menu:",
            $cache);
    $menu->addOption("Update all colour files.h.",
            sub { });
    $menu->addOption("Update Theme_Colour_ColourIds.h.",
            sub { });
    $menu->addOption("Update Theme_Colour_ColourIds.cpp.",
            sub { });
    $menu->addOption("Update Theme_Colour_JSONKeys.cpp.",
            sub { });
    $menu->addOption("Update default colours.json file.",
            sub { });
    $menu->openMenu();
}
1;
