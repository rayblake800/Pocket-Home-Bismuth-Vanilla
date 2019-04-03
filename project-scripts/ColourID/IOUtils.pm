##### IOUtils.pm ###############################################################
# Reads, prints, and writes ColourId data.
################################################################################ 

##### Functions: #####

#==============================================================================#
#--- cacheCodeFiles: ---
# Populates an IDCache with data from project code files.
#--- Parameters: ---
# $cache: The IDCache object to update.
#==============================================================================#

#==============================================================================#
#--- getDeclarationText: ---
# Gets all cached Element C++ declarations as a single string.
#--- Parameters: ---
# $cache: The IDCache object to read.
#--- Returns: ---
# C++ declaration text for all cached Element objects.
#==============================================================================#

#==============================================================================#
#--- getDefinitionText: ---
# Gets all cached Element C++ definitions as a single string.
#--- Parameters: ---
# $cache: The IDCache object to read.
#--- Returns: ---
# C++ definition text for all cached Element objects.
#==============================================================================#

#==============================================================================#
#--- getKeyMapDeclaration: ---
# Gets a C++ map declaration and definition mapping all Element JSON key values
# to their element names.
#--- Parameters: ---
# $cache: The IDCache object to read.
#--- Returns: ---
# C++ text declarint the full static const colourIds map.
#==============================================================================#

use strict;
use warnings;

package IOUtils;
use lib './project-scripts/ColourID';
use lib './project-scripts/ColourID/DataObjects';
use CacheBuilder;
use Element;
use Category;
use IDCache;
use File::Slurp;
use Scalar::Util qw(blessed);

# Populates an IDCache with data from project code files.
# cache:  The IDCache to update.
sub cacheCodeFiles
{
    my $cache = shift;
    my $headerFile = './Source/GUI/Theme/Colour/Theme_Colour_ColourIds.h';
    my $sourceFile = './Source/GUI/Theme/Colour/Theme_Colour_ColourIds.cpp';
    my $keyFile    = './Source/GUI/Theme/Colour/Theme_Colour_JSONKeys.cpp';

    if(-f $headerFile)
    {
        CacheBuilder::processFile($cache, $headerFile);
    }
    if(-f $sourceFile)
    {
        CacheBuilder::processFile($cache, $sourceFile);
    }
    if(-f $keyFile)
    {
        CacheBuilder::processFile($cache, $keyFile);
    }
}

# Gets all cached Element C++ declarations as a single string.
sub getDeclarationText
{
    my $cache = shift;
    my @namespaces = $cache->getNamespaceNames();
    my $declarations = "";
    foreach my $name(@namespaces)
    {
        my $namespace = $cache->getNamespace($name);
        $declarations = $declarations.$namespace->getDeclarationText();
    }
    return $declarations;
}

# Gets all cached Element C++ definitions as a single string.
sub getDefinitionText
{
    my $cache = shift;
    my @namespaces = $cache->getNamespaceNames();
    my $definitions = "";
    foreach my $name(@namespaces)
    {
        my $namespace = $cache->getNamespace($name);
        $definitions = $definitions.$namespace->getDefinitionText();
    }
    return $definitions;
}

# Gets a C++ map declaration and definition mapping all Element JSON key values
# to their element names.
sub getKeyMapDeclaration
{
    my $cache = shift;
    my $indent = " " x 4;
    my $mapText = "static const std::map<juce::Identifier, Element> colourIds"
            ."\n{\n";
    my @keys = $cache->getElementKeys();
    foreach my $key(@keys)
    {
        my $element = $cache->findElement($key);
        my $fullName = $element->getFullName();
        if($key ne $keys[0])
        {
            $mapText = $mapText.",\n";
        }
        $mapText = $mapText."$indent\{\"$key\",\n"
            .($indent x 2)."$fullName}";
    }
    return $mapText."\n};";
}
1;
