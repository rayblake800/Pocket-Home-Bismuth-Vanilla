#!/usr/bin/perl
#
# Make .cpp test files for each header file in the project within a test
# directory.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;

my $projectDir = "/home/anthony/Workspace/C++/PocketCHIP-pocket-home";
my $sourceDir = "$projectDir/Source";
my $testDir = "$projectDir/project-scripts/cppTest";

my @commands;

sub findModules
{
    my $header = substr($File::Find::name, length($projectDir) + 1);
    if($header =~ /^.+\.h$/)
	{
        my $cppFile = substr($File::Find::name, length($sourceDir));
        $cppFile =~ s/\..*/.cpp/;
        $cppFile =~ s/\//-/g;
        $cppFile = $testDir.'/'.$cppFile;
        my $command = "./project-scripts/MakeCpp.pl $header $cppFile";
        push(@commands, $command);
	}
}
find(\&findModules,$sourceDir);

foreach my $command(@commands)
{
    system($command);
}

