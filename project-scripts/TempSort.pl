#!/usr/bin/perl

use strict;
use warnings;
use File::Slurp;
use List::Util qw(max);

my $inPath = "./Source/GUI/Theme/Colour/Theme_Colour_ColourIds.h";
my $outPath = "$inPath.temp";

my $idFile = read_file($inPath) or die "Couldn't read input file!\n";

# Read in all inner namespaces values in the ColourIds Namespace.
my @namespaces = 
    $idFile =~ /\h+namespace \s+\w+\s+ {
                    \s+static\ const\ Element.+?
                }/gsx;

# Extracts the first ColourID value from a namespace:
sub getFirstID
{
    my $namespace = shift;
    if($namespace =~ /(0x[0-9a-fA-F]+),/)
    {
        return hex($1);
    }
    return -1;
}

# Sort the list by ID:
@namespaces = sort {getFirstID($a) <=> getFirstID($b)} @namespaces;

print("Sorted ".(scalar @namespaces)." ID namespaces.\n");
# Output sorted list to temp file
write_file($outPath, join("\n", @namespaces));
