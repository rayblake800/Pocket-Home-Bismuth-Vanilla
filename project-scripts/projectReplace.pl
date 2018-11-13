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
        my @lines = read_file($file);
        my @matches;
        foreach my $line(@lines)
        {
            push(@matches, ($line =~ /$toFind/g));
        }
        my $matchCount = @matches;
        if($matchCount > 0)
        {
            print("Found $matchCount ".($matchCount > 1 ? "matches" : "match")
                    ." in ");
            printGreen($File::Find::name);
            print(":\n");
            my $changesMade = 0;
            my $input = "";
            foreach my $line(@lines)
            {
                my $skipChars = 0;
                while(substr($line, $skipChars) =~ /(.*)($toFind)(.*)/)
                {
                    print(substr($line, 0, $skipChars));
                    print($1);
                    printRed($2);
                    print("$3\n");

                    print($1);
                    printGreen($replace);
                    print("$3\n");
                    
                    print("Change line? (");
                    printGreen("y"); print("es/");
                    printRed("n");   print("o/");
                    printWhite("s"); print("kip file/");
                    printRed("q");   print("uit):");
                    chomp($input = <STDIN>);
                    if($input eq "y")
                    {
                        $changesMade++;
                        $line = substr($line, 0, $skipChars)."$1$replace$3";
                        $skipChars += length($1) + length($replace);
                    }
                    elsif($input eq "n")
                    {
                        $skipChars += length($1) + length($2);
                    }
                    elsif($input eq "s")
                    {
                        rewindLines(3);
                        last;
                    }
                    elsif($input eq "q")
                    {
                        rewindLines(3);
                        exit;
                    }
                    rewindLines(3);
                }
                if($input eq "s")
                {
                    last;
                }
            }
            if($changesMade)
            {
                print("Writing $changesMade changes back to $file.\n");
                my $fileText = join("", @lines);
                $fileText =~ s/\r//g;
                length($fileText) > 0 or die "Empty output!\n";
                write_file($file, $fileText);
            }
        }
    }
}
find(\&readFiles,$projectDir);
