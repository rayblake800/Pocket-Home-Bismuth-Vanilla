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

my $colourIds = "namespace ColourIds\n{\n";

sub headers
{
	my $file = shift;
	if($_ =~ /.*\.h$/)
	{
		my $text = read_file($_) or die "Couldn't read $_!\n";
		my $class = $_;
		$class =~ s/juce_//;
		$class =~ s/\.h//;
		$class = lc(substr($class,0,1)).substr($class,1);
		if($text =~ /enum\s+ColourIds\s*\{(.*?)\}/gs)
		{
			$colourIds=$colourIds."    namespace $class\n    {\n";
		        $colourIds=$colourIds."        enum\n        {\n";
			my @names;
			my @ids;
			my @comments;
			my $longest = 0;
			while($text =~ /^(.*?\s* (\w+)          \s*=\s*
				            (0x[\da-f]{7})  \s*
				            (\/\*[^\/]*\/)?  )/sx)
			{ 
				my $blockLen = length($1);
				my $name = $2;
				my $id = $3;
				my $comment = $4;
				if(!defined($id))
				{
					die "Missing id!\n";
				}
				$name =~ s/Colour//;
				$name =~ s/Id//;
				if(length($name) > $longest)
				{
					$longest = length($name);
				}
				push(@names,$name);
				push(@ids,$id);
				if(defined($comment))
				{
					push(@comments,"            $comment\n");
				}
				else
				{
					push(@comments,"");
				}
				$text = substr($text,$blockLen);
			}
			my $nColors = @names;
			for(my $i = 0; $i < $nColors; $i++)
			{
				my $line = $comments[$i]."            ";
				while(length($names[$i]) < $longest)
				{
					$names[$i]=$names[$i]." ";
				}
				$line=$line.$names[$i]." = ".$ids[$i];
				if($i < (scalar @names - 1))
				{
					$line=$line.",";
				}
				$colourIds=$colourIds. "$line\n";
			}
			$colourIds=$colourIds."        };\n    };\n\n";
		}
	}
}

find(\&headers,$ARGV[0]);
$colourIds=$colourIds."};";

open(IDLIST,">",$ARGV[1]) or die "Couldn't open output file!\n";
print(IDLIST $colourIds);
close IDLIST;
