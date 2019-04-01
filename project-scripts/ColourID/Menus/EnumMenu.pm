##### EnumMenu.pm ##############################################################
# Constructs the ColourId enum loading menu, parsing ColourId enums and
# selectively using them to populate or update an IDCache object.
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the enum menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: an IDCache object to use for menu options.
#==============================================================================#

use strict;
use warnings;

package EnumMenu;
use lib './project-scripts/ColourID';
use EnumSearch;
use IOUtils;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

my $projectDir = $ENV{'HOME'}.'/Workspace/C++/PocketCHIP-pocket-home';

# Displays the main menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "ColourId enum search options:",
            "Return to main menu",
            \&loadEnums,
            $cache);
    $menu->addOption("Scan project Source directory",
            "$projectDir/Source");
    $menu->addOption("Scan JUCE library files",
            "$projectDir/deps/JUCE");
    $menu->openMenu();
}

sub loadEnums
{
    my $dir = shift;
    my $cache = shift;
    my @enums = EnumSearch::findEnums($dir);
    foreach my $enum(@enums)
    {
        my $enumName = $enum->getNamespaceName();
        my @colourNames = $enum->getNames();
        my @colourIds = $enum->getIDs();
        
        # Search for missing namespaces:
        my $namespace = $cache->findNamespace($enumName);
        if(!defined($namespace))
        {
            print("Add uncached namespace $enumName?");
            if(!IOUtils::confirm())
            {
                next;
            }
        }
        # Check the cache for name/ID conflicts:
        for(my $i = 0; $i < (scalar @colourIds); $i++)
        {
            my $name = $colourNames[$i];
            my $id = $colourIds[$i];
            my $newValText = "$enumName\::$name = $id";
            my $elementMatch = $cache->findElement($id);
            if($elementMatch) 
            {
                if(($elementMatch->getNamespace() ne $enumName)
                     || ($elementMatch->getName() ne $name))
                {
                    print("Possible conflict detected:\n"
                            ."Element: ".$elementMatch->getFullName()
                            ." = ".$elementMatch->getID()."\n"
                            ."Enum value: $newValText\n"
                            ."Replace cached Element?");
                    if(IOUtils::confirm())
                    {
                        my $replacement = new Element($enumName, $name, $id,
                                $elementMatch->getCategory());
                        $cache->addElement($replacement);
                    }
                }
            }
            else
            {
                print("Adding new Element $newValText.\n");
                my $newVal = new Element($enumName, $name, $id);
                $cache->addElement($newVal);
            }
        }
    }
}

1;
