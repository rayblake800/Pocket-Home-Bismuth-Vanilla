# CacheBuilder.pm
# Caches ColourID elements loaded from C++ definitions in a file.
use strict;
use warnings;

package CacheBuilder;
use lib './project-scripts/ColourID';
use Element;
use Category;
use IDCache;
use File::Slurp;
use Scalar::Util qw(blessed);

# Add all Element definitions in a file to a cache
# cache:  The IDCache to update.
# path:   The path to the file that should be read.
sub processFile
{
    my $cache = shift;
    my $path = shift;
    my $file = read_file($path);

    my @elements;
    # Parse definitions that are also declarations:
    my @innerNamespaces = ($file =~ /namespace \w+\s+{[^{]*?}/gs);
    foreach my $namespace(@innerNamespaces)
    {
        my $name;
        if($namespace =~ /namespace (\w+)/)
        {
            $name = $1;
        }
        else
        {
            print("CacheBuilder::ProcessFile: Invalid namespace found.\n");
            print("namespace= $namespace\n");
            next;
        }
        my @elementDefs = ($namespace =~ /
                static\ const\ Element\ [a-zA-Z_][a-zA-Z0-9_]*
                \s*\(.*?\)\s*;/gsx);
        foreach my $definition(@elementDefs)
        {
            my $element = Element::parseElement($definition, $name);
            if(defined($element))
            {
                push(@elements, $element);
            }
        }
    }

    # Parse definitions for Elements declared elsewhere:
    my @definitions = ($file =~ /Element (\w+::)+\w+\s*\(.*?\)/gs);
    foreach my $definition(@definitions)
    {
        if($definition =~ /Element (?:\w+::)*(\w+)::(.*)/gs)
        {
            my $namespace = $1;
            my $def = $2;
            my $element = Element::parseElement($def, $namespace);
            if(defined($element))
            {
                push(@elements, $element);
            }
        }
        else
        {
            print("CacheBuilder::ProcessFile: Invalid definition found.\n");
            print("definition=$definition\n");
            next;
        }
    }

    # parse config key definitions:
    if($file =~ /map\s*<\s*juce::Identifier\s*,\s*Element\s*>[^{]+
            {([^;]+)};/sx)
    {
        my $keyMap = $1;
        my @keyList = ($keyMap =~ /{[^}]+}/gs);
        print("Found probable key map with ".(scalar @keyList)
                ." key(s) defined\n");
        foreach my $keyPair(@keyList)
        {
            if($keyPair =~ /{\s*"(.*?)"\s*,\s*(.*?)\s*}/s)
            {
                my $key = $1;
                my $elementName = $2;
                $cache->assignKey($elementName, $key);
            }
            else
            {
                print("Found invalid <key,element> pair $keyPair\n");
            }
        }
    } 

    print("Found ".(scalar @elements)." elements to cache in $path.\n");
    foreach my $element(@elements)
    {
        if(defined($element) && blessed($element)
                && blessed($element) eq 'Element')
        {
            $cache->addElement($element);
        }
    }
}
1;
