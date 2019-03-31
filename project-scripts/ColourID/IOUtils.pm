# IOUtils.pm
# Reads and prints ColourId data
use strict;
use warnings;

package IOUtils;
use lib './project-scripts/ColourID';
use Element;
use Category;
use IDCache;
use CacheBuilder;
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

# Prints the list of cached namespaces
sub printNamespaces
{
    my $cache = shift;
    my @namespaces = $cache->getNamespaceNames();
    my $count = 1;
    foreach my $namespace(@namespaces)
    {
        print("$count. $namespace\n");
        $count++;
    }
}

#prints a single element
sub printElement
{
    my $element = shift;
    if(defined($element))
    {
        print($element->getNameKey().": ID:".$element->getID().", category "
                .$element->getCategory()->getTypeName()."\n");
    }
}

#prints all elements in a namespace
sub printNamespaceElements
{
    my $cache = shift;
    my $namespace = shift;
    my @elements = $namespace->getElements();
    my $count = 1;
    foreach my $element(@elements)
    {
        print("$count. ");
        printElement($element);
        $count++;
    }
}
1;
