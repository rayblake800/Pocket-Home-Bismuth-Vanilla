##### Paths.pm: ################################################################
# Defines project colour file paths
################################################################################ 
use strict;
use warnings;

package Paths;

# Main project directory:
use constant PROJECT_DIR 
        => $ENV{'HOME'}."/Workspace/C++/PocketCHIP-pocket-home";

# Theme::Colour directory:
use constant COLOUR_DIR  
        => PROJECT_DIR."/Source/GUI/Theme/Colour";

# ColourId header file:
use constant COLOUR_ID_HEADER 
        => COLOUR_DIR."/Theme_Colour_ColourIds.h";

# ColourId source file:
use constant COLOUR_ID_SOURCE 
        => COLOUR_DIR."/Theme_Colour_ColourIds.cpp";

# JSON colour key source file:
use constant COLOUR_KEY_SOURCE 
        => COLOUR_DIR."/Theme_Colour_JSONKeys.cpp";

# Default colour configuration file:
use constant DEFAULT_COLOUR_CONFIG 
        => PROJECT_DIR."/assets/configuration/colours.json";
1;
