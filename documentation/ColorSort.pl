#
# ColourId Sorter 
#
# usage: 
#   perl ColourSort.pl [input file] [output file]
#
#  This script will read in all color ids, prompt the user to sort them
# into default color categories, and output a file containing a c++
# function that returns all associated color ids when passed in a default color
# category.
#
use strict;
use warnings;
use File::Slurp;

if(!defined($ARGV[0]) || !defined($ARGV[1]))
{
    die "usage: perl ColourSort.pl [input file] [output file]\n";
}

my @defaults = 
(
	"windowBackground",
	"widgetBackground",
	"widgetOff",
	"widgetOn",
	"menuBackground",
	"outline",
	"focusedOutline",
	"textField",
	"text",
	"highlightedTextField",
	"highlightedText",
	"none"
);
my $lastEntry = "0";
sub prompt{
	my $id = shift;
	my $num = shift;
	my $max = shift,
	my $result = "";
	while(! $result =~ /[0-9]+/)
	{
		system("clear");
		print("Pick a default category number.\n");
		my $numDefaults = @defaults;
		for(my $i = 0; $i < $numDefaults; $i++)
		{
			print("$i: ".$defaults[$i]."\n");
		}
		print("ID $num/$max: $id\n");
		print("Enter 0 - $numDefaults,(Q)uit, or nothing to"
			." repeat[$lastEntry]:");
		$result = <STDIN>;
		if($result =~ /q/i)
		{
			die "exiting...";
		}
		if($result =~ /^\s*$/)
		{
			$result = $lastEntry."\n";
		}
	}
	$lastEntry = $result;
	$lastEntry =~ s/[^0-9]//g;
	return $result;
}

my $idFile = read_file($ARGV[0]) or die "Couldn't read input file!\n";

my @colourIds;

my @enums = ($idFile =~ /enum\s+(.*?\{.*?\})/gs);

foreach my $enum(@enums)
{
	my $class = "";
	if($enum =~ /^(\w+)/)
	{
		$class = $1;
		my @colors = ($enum =~ /(\w+)\s+=/g);
		foreach my $color(@colors)
		{
			push(@colourIds,{"class"=>$class, "color"=>$color});
		}
	}
}
print("Found ".scalar @colourIds." colors in ".scalar @enums." classes\n");
@colourIds = sort { $a->{"color"} cmp $b->{"color"}} @colourIds;

my @sortedIds;
foreach my $i(0...(scalar @defaults))
{
	$sortedIds[$i]= [];
}
my $sorted = 0;
foreach my $cID(@colourIds)
{
	$sorted++;
	my $idStr = $cID->{"class"}."::".$cID->{"color"};
	my $selection = $sortedIds[prompt($idStr,$sorted,scalar @colourIds) ];
	push(@{$selection},$idStr);
}

open(IDLIST,">",$ARGV[1]) or die "Couldn't open output file!\n";

print(IDLIST "Array<int> getIdsForDefault");
print(IDLIST "(ComponentConfigFile::DefaultColors defaultColor)\n{\n");
print(IDLIST "    using namespace ColourIds;\n");
print(IDLIST "    switch(defaultColor)\n    {\n");
for(my $i = 0; $i < (scalar @defaults - 1); $i++)
{
	print(IDLIST "    case ".$defaults[$i].":\n");
	print(IDLIST "        return\n        {\n");
	my $list = "";
	foreach my $cID(@{$sortedIds[$i]})
	{
		my $line = "";
		if(length($list) > 0)
		{
			$line = ",\n";
		}
		$line=$line."            $cID";
		$list = $list.$line;
	}
	print(IDLIST "$list\n        };\n\n");
}
print(IDLIST "    default:\n        return {};");
print(IDLIST "\n    }\n}");
close IDLIST;
