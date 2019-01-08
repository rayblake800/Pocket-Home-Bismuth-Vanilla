#!/usr/bin/perl
#
# Recursively finds and replaces text over the entire project directory,
# confirming each change.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;
use FindBin;
use Cwd 'abs_path';

#Text color codes:
my $red = "\33[41m\33[37m";
my $green = "\33[42m\33[37m";
my $white = "\33[47m\33[30m";
my $reset = "\33[0m";

sub printRed
{
    print("$red$_[0]$reset");
}
sub printGreen
{
    print("$green$_[0]$reset");
}

sub printWhite
{
    print("$white$_[0]$reset");
}


sub rewindLines
{
    my $numLines = shift;
    print("\r\033[K");
    for(my $y = 0; $y < $numLines; $y++)
    {
        print("\33[1A\r\033[K");
    }
}

chdir($FindBin::Bin);
my $projectDir = "../";
my $toFind = $ARGV[0];
my $replace = $ARGV[1];

#Subdirectories to exclude:
my @excluded = (
    "../build",
    "../debian",
    "../deps",
    "../docs/Doxygen",
    "../JuceLibraryCode",
    "../pack-debian",
    "../.git"
    );

if(!defined($toFind) || !defined($replace))
{
    die("Missing arguments!\n");
}

print("Replacing ");
printRed($toFind);
print(" with ");
printGreen($replace);
print(".\n");

sub readFiles
{
    if(-d) # Stop recursion from continuing under excluded directories.
    {
        my $file = $_;
        foreach my $dir(@excluded)
        {
            if($File::Find::name =~ /$dir/)
            {
                $File::Find::prune = 1;
                return;
            }
        }
    }
    if(-f)
    {
        my $file = $_;
        my $text= read_file($file);
        my @matches = ($text =~ /$toFind/g);
        my $matchCount = @matches;
        if($matchCount > 0)
        {
            print("Found $matchCount ".($matchCount > 1 ? "matches" : "match")
                    ." in ");
            printGreen($File::Find::name);
            print(":\n");
            my $changesMade = 0;
            my $outText = "";
            my $input = "";
            while($text =~ /(.*?)(^.*?)($toFind)(.*?$)(.*)/ms)
            {
                print("Found line:\n");
                print($2);
                printRed($3);
                print("$4\n");

                print($2);
                printGreen($replace);
                print("$4\n");
                
                print("Change line? (");
                printGreen("y"); print("es/");
                printRed("n");   print("o/");
                printWhite("s"); print("kip file/");
                printRed("q");   print("uit):");
                chomp($input = <STDIN>);
                if($input eq "y")
                {
                    $changesMade++;
                    $outText = "$outText$1$2$replace$4";
                    $text = $5;
                }
                elsif($input eq "n")
                {
                    $outText = "$outText$1$2$3$4";
                    $text = $5;
                }
                elsif($input eq "s")
                {
                    rewindLines(3);
                    last;
                }
                elsif($input eq "q")
                {
                    rewindLines(5);
                    exit;
                }
                rewindLines(5);

                if($input eq "s")
                {
                    last;
                }
            }
            if($changesMade)
            {
                print("Writing $changesMade changes back to $file.\n");
                my $fileText = "$outText$text";
                length($fileText) > 0 or die "Empty output!\n";
                write_file($file, $fileText);
            }
        }
    }
}
find(\&readFiles,$projectDir);
