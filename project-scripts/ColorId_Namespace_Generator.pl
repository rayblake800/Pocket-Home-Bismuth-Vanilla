#!/usr/bin/perl

# ColourIds Namespace Generator
#
# usage: 
#   perl ColourIdsNamespace.pl [project directory] [output file]
#
#  This script will recursively scan directories to find header files with
# ColourId enums.  It extracts all those enums, and copies their values into
# new enums named after classes within a single ColourIds namespace. This 
# makes it easy to ensure any new ColourId values are unique, and allows
# classes that manage colors to reference ColourIds without having to copy the
# ids over or include every class.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;

if(!defined($ARGV[0]) || !defined($ARGV[1]))
{
    die "usage: ./ColorIDs_Namespace_Generator.pl [project directory] [output file]\n";
}

my $colourIds = "#pragma once\n\n#include \"Theme_Colour_Element.h\"\n"
        ."namespace Theme { namespace Colour { namespace ColourIds\n{\n";

my @allElements;

my $indent = "    ";

sub headers
{
	my $file = shift;
	if($_ =~ /.*\.h$/)
	{
		my $text = read_file($_) or die "Couldn't read $_!\n";
		my $class = $_;
		$class =~ s/juce_//;
		$class =~ s/\.h//;
		if($text =~ /enum\s+ColourIds\s*\{(.*?)\}/gs)
		{
			$colourIds=$colourIds."    namespace $class\n    {\n";
			my @names;
			my @ids;
			while($text =~ /
                    ^(.*?\s* (\w+)          
                    \s*=\s*
				    (0x[\da-f]{7})  
                    \s*)/sx)
			{ 
				my $blockLen = length($1);
				my $name = $2;
				my $id = $3;
				if(!defined($id))
				{
					die "Missing id!\n";
				}
				$name =~ s/Colour//;
				$name =~ s/Id//;
				push(@names,$name);
				push(@ids,$id);
				$text = substr($text,$blockLen);
			}
			my $nColors = @names;
			for(my $i = 0; $i < $nColors; $i++)
			{
                push(@allElements, $class."::".$names[$i]);
				my $line = ($indent x 2)."static const Element "
                        .$names[$i]."(\n"
                        .($indent x 4).$ids[$i]
                        .", UICategory::none);";
				$colourIds=$colourIds."$line\n";
			}
			$colourIds=$colourIds.$indent."}\n";
		}
	}
}

find(\&headers,$ARGV[0]);
$colourIds = $colourIds.$indent
        ."static const std::vector<Element> allElements = \n".$indent."{\n";
foreach my $element(@allElements)
{
    $colourIds = $colourIds.($indent x 2).$element.",\n"
}
$colourIds = substr($colourIds, 0, -2)."\n".$indent."};\n";
$colourIds = $colourIds."} } }";

open(IDLIST,">",$ARGV[1]) or die "Couldn't open output file!\n";
print(IDLIST $colourIds);
close IDLIST;
