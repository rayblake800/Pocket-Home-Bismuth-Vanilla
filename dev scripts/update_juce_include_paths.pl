#!/usr/bin/perl
use strict;
use warnings;
use File::Find;
use File::Slurp;

# The Projucer is still the easiest way to make sure the makefiles generate
# correctly, but it does a poor job at handling changes to the source code 
# directory structure.  This script scans the source directories, maps all files,
# and updates the juce project file.

#convert a global path to one that begins in the Source directory
sub localPath
{
    my $path = shift;
    my $index = rindex($path, "Source");
    if($index == -1)
    {
        return undef;
    }
    return substr($path, $index);
}


my %dirPaths;
my %codeFiles;

sub dirSearch
{
    my $path = localPath($File::Find::dir);
    if($path)
    {
        $dirPaths{$path} = 1;
    }
    
	if($_ =~ /.*\.(h|cpp)$/)
	{
        $codeFiles{$_} = localPath($File::Find::name);
	}
}

chdir("../");
find(\&dirSearch, "./Source");

my $headerPath = "\"";
foreach my $dir(keys %dirPaths)
{
    if(length($headerPath) > 1)
    {
        $headerPath = $headerPath."&#10;";
    }
    $headerPath = $headerPath."../../$dir";
}
$headerPath = "headerPath=".$headerPath."\"";

my $juceFileName = "./pocket-home.jucer";
my $juceFile = read_file($juceFileName);

$juceFile =~ s/headerPath=".*?"/$headerPath/;
foreach my $codeFile(keys %codeFiles)
{
    my $fileMatch = "file=\".*?\/$codeFile\"";
    $juceFile =~ s/$fileMatch/file="$codeFiles{$codeFile}"/g;
}

write_file($juceFileName, $juceFile);
