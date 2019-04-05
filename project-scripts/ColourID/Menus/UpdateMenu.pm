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
use File::Slurp;
use lib './project-scripts/ColourID/';
use IOUtils;
use Paths;
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
            undef,
            $cache);
    $menu->addOption("Update all colour files.",
            sub 
            { 
                updateIDHeader($cache);
                updateIDSource($cache);
                updateKeyFile($cache);
                updateDefaultConfig($cache);
            });
    $menu->addOption("Update ColourId header file",
            \&updateIDHeader);
    $menu->addOption("Update ColourId source file",
            \&updateIDSource);
    $menu->addOption("Update Colour JSON key file",
            \&updateKeyFile);
    $menu->addOption("Update default colours.json file",
            \&updateDefaultConfig);
    $menu->openMenu();
}


sub updateIDHeader
{
    my $cache = shift;
    if(!defined($cache))
    {
        die "UpdateMenu::updateIDHeader: Invalid cache!\n";
    }
    my $headerPath = Paths::COLOUR_ID_HEADER;
    if(! -f $headerPath)
    {
        print("Unable to find ColourIds header file!\n"
                ."Path tested:$headerPath\n");
        return;
    }
    my $fileText = read_file($headerPath);
    my $startText, my $endText;

    #find text before the start of element namespaces:
    if($fileText =~ /
            ^(.*?)\h*                   # Capture from the start of the file
            namespace\ \w+\s+           # until just before the first namespace
            {\s+static\ const\ Element  # starting with an Element declaration.
            /sx)
    {
        $startText = $1;
    }
    else
    {
        print("Unable to find start of Element declarations, cancelling.\n");
        return;
    }

    #find text after the last element declaration:
    if($fileText =~ /
            .*                      # Match beginning of file
            static\ const\ Element  # Match start of last Element declaration
            [^}]+}\v*               # Match remaining Element namespace
            (\v.*?)$                # Capture all text after last namespace
            /sx)
    {
        $endText = $1;
    }
    else
    {
        print("Unable to find end of Element declarations, cancelling.\n");
        return;
    }

    my $declarations = IOUtils::getDeclarationText($cache);
    $fileText = $startText.$declarations.$endText;
    if(write_file($headerPath, $fileText))
    {
        print("Updated header file.\n");
    }
    else
    {
        print("Failed to write to header file.\n");
    }
}

sub updateIDSource
{
    my $cache = shift;
    my $sourcePath = Paths::COLOUR_ID_SOURCE;
    if(! -f $sourcePath)
    {
        print("Unable to find ColourIds source file!\n"
                ."Path tested:$sourcePath\n");
        return;
    }
    my $fileText = read_file($sourcePath);
    my $startText, my $endText;

    # Save text before element array:
    if($fileText =~ /(.*allElements =\s+).*/s)
    {
        $startText = $1;
    }
    else
    {
        print("Unable to find start of Element array, cancelling.\n");
        return;
    }

    # Save text after element array:
    if($fileText =~ /
        allElements\ =\s+ # Match start of array definition
        {[^}]*?};         # Match end of array definition
        (.*)$             # Capture all remaining text
        /xs)
    {
        $endText = $1;
    }
    else
    {
        print("Unable to find text after full Element array, cancelling.\n");
        return;
    }

    # Build array contents:
    my $arrayText = "{\n";
    my @elements = $cache->getElements();
    for(my $i = 0; $i < (scalar @elements); $i++)
    {
        if($i > 0)
        {
            $arrayText = "$arrayText,\n";
        }
        $arrayText = $arrayText."    &".$elements[$i]->getFullName();
    }
    $arrayText = $arrayText."\n};";

    $fileText = $startText.$arrayText.$endText;
    if(write_file($sourcePath, $fileText))
    {
        print("Updated source file.\n");
    }
    else
    {
        print("Failed to write to source file.\n");
    }
}

sub updateKeyFile
{
    my $cache = shift;
    my $keyPath = Paths::COLOUR_KEY_SOURCE;
    my $fileText = read_file($keyPath);

    my $startText, my $endText;

    # Find file text before key map:
    if($fileText =~ /(.*)static const std::map<juce::Identifier, Element>/s)
    {
        $startText = $1;
    }
    else
    {
        print("Failed to find start of colour map, cancelling.\n");
        return;
    }

    # Find file text after key map:
    if($fileText =~ /
            std::map<juce::Identifier,\ Element>\ colourIds\s+
            [^;]+;(.*)$/sx)
    {
        $endText = $1;
    }
    else
    {
        print("Failed to find end of colour map, cancelling.\n");
        return;
    }

    my $map = IOUtils::getKeyMapDeclaration($cache);
    $fileText = $startText.$map.$endText;
    if(write_file($keyPath, $fileText))
    {
        print("Updated JSON key file.\n");
    }
    else
    {
        print("Failed to update JSON key file.\n");
    }
}

sub updateDefaultConfig
{
    my $cache = shift;
    my $configText = IOUtils::getDefaultColourConfig($cache);
    if(write_file(Paths::DEFAULT_COLOUR_CONFIG, $configText))
    {
        print("Updated default json file.\n");
    }
    else
    {
        print("Failed to write to default json file.\n");
    }
}
1;
