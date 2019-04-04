##### ExportMenu.pm ############################################################
# Opens the menu providing ColourId export options.                            #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the export menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: The IDCache source of data to export.
#==============================================================================#

use strict;
use warnings;

package ExportMenu;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

# Displays the export menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "Colour data export options:",
            "Return to main menu:",
            undef,
            $cache);
    $menu->addOption("Export Element object declarations.",
            sub { });
    $menu->addOption("Export Element object definitions.",
            sub { });
    $menu->addOption("Export <JSONKey, Element> map definition.",
            sub { });
    $menu->addOption("Export default colours.json file.",
            sub { });
    $menu->openMenu();
}
1;
