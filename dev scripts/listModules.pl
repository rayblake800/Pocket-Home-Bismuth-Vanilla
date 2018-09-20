#!/usr/bin/perl
#
# Finds all .cpp and .h project files, and lists them alphabetically by name,
# removing file extensions and duplicates.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;

my $sourceDir = "../Source";
my $testDir = "../Tests";
my %names;

sub findModules
{
	if($_ =~ /^.+\.(h|cpp)$/)
	{
        my $name = $_;
        $name =~ s/\..*//;
        $names{$name} = 1;;
	}
}
find(\&findModules,$sourceDir);
find(\&findModules,$testDir);

my @names = sort( keys (%names));        

foreach my $name(@names)
{
    print("$name\n");
}
