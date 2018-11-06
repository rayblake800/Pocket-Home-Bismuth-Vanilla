#!/usr/bin/perl
#
# Finds all .cpp and .h project files, and lists them alphabetically by name,
# removing file extensions and duplicates.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;

my $projectDir = "/home/anthony/Workspace/C++/PocketCHIP-pocket-home";
my $sourceDir = "$projectDir/Source";
my $testDir = "$projectDir/Tests";
my %names;

sub findModules
{
    my $name = $File::Find::name;
    if($name =~ /^.+\.(h|cpp)$/)
	{
        $name =~ s/\..*//;
        $name =~ s/.*PocketCHIP-pocket-home//g;

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
