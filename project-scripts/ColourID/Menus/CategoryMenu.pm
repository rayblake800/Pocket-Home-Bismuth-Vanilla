##### CategoryMenu.pm ##########################################################
# Constructs the color category management menu.                               #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the category menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: The cache object used to save category updates.
#==============================================================================#

use strict;
use warnings;

package CategoryMenu;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

# Displays the category menu, repeatedly accepting input and running the menu 
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "ColourId category sorting:",
            "Return to main menu:",
            undef,
            $cache);
    $menu->addOption("Assign categories to uncategorized Elements",
            sub 
            { 
                my @uncategorized;
                my @elements = $cache->getElements();
                foreach my $element(@elements)
                {
                    if($element->getCategory()->getValue() == Category::NONE)
                    {
                        push(@uncategorized, $element);
                    }
                }
                assignCategories($cache, @uncategorized);
            });
    $menu->addOption("Reassign all Element categories",
            sub 
            { 
                my @elements = $cache->getElements();
                assignCategories($cache, @elements);
            });
    $menu->openMenu();
}

# Assigns a category to a single Element object.
sub assignCategory
{
    my $cache = shift;
    my $element = shift;
    my $defaultOption = shift;
    print("TODO: Finish assignment function.\n");
}

# Assigns categories to a list of cached Element objects.
sub assignCategories
{
    my $cache = shift;
    my @elements = @_;
    print("TODO: Finish assignment function.\n");
}
1;
