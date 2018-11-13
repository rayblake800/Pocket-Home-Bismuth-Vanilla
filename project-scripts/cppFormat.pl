#!/usr/bin/perl
# Read in a .cpp file, and format code to fit my personal preferences.
# Ideal for quickly formatting blocks of code copied from header files.
use strict;
use warnings;
use File::Slurp;

if(!defined($ARGV[0]))
{
    die "usage: ./cppFormat.pl  [input file]\n";
}

use constant OPEN_CHAR  => 1;
use constant CLOSE_CHAR => 2;
use constant DUAL_CHAR  => 3;


my %blockTypes =  (
                        "\"" => "\"",
                        "'"  => "'",
                        "["  => "]",
                        "{"  => "}",
                        "("  => ")",
                        "/*" => "*/"
                    );
sub nextGroupChar
{
    my $text = shift;
    my $startIndex = shift;
    if(!defined($startIndex))
    {
        $startIndex = 0;
    }
    else
    {
        $text = substr($text, $startIndex);
    }
    if($startIndex >= length($text))
    {
        print "nextGroupChar: Starting past the end at $startIndex\n";
        return undef;
    }
    if($text =~ /(|.*?[^\\])?([[\]"'{}()]|\/\*|\*\/)/gs)
    {
        my $idx = $startIndex;
        if($1)
        { 
            $idx += length($1);
        }
        my $blockStr = $2;
        my $charType;
        if($blockStr =~ /['"]/)
        {
            $charType = DUAL_CHAR;
        }
        elsif($blockStr =~ /[[{(]|\/\*/)
        {
            $charType = OPEN_CHAR;
            $blockStr = $blockTypes{$blockStr};
        }
        elsif($blockStr =~ /[\]})]|\*\//)
        {
            $charType = CLOSE_CHAR;
        }
        else
        {
            die "Unhandled block character \"$blockStr\"!\n
                 Found at $idx in range $startIndex - ".length($text)."\n";
        }
        return $idx, $blockStr, $charType;
    }
    return undef;
}

sub nextBlock
{
    my $text = shift;
    my $startIndex = shift;
    if(!defined($startIndex))
    {
        $startIndex = 0;
    }
    print "Starting at $startIndex\n";
    my %blockCounts;
    foreach my $blockOpen(keys %blockTypes)
    {
       $blockCounts{$blockTypes{$blockOpen}} = 0; 
    }
    my $allBlocksClosed = sub
    {
        foreach my $block(keys %blockCounts)
        {
            if($blockCounts{$block} > 0)
            {
                return 0;
            }
        }
        return 1;
    };
    
    my $inString = sub
    {
        return $blockCounts{"\""} || $blockCounts{"'"}
                || $blockCounts{"*/"};
    };

    my $start;
    my ($idx, $blockStr, $charType) = nextGroupChar($text, $startIndex);
    while(defined($idx))
    {
        if(!defined($start))
        {
            if($charType != CLOSE_CHAR)
            {
                $start = $idx;
                $blockCounts{$blockStr} = 1;
            }
        }
        elsif($inString->())
        {
            if($blockCounts{$blockStr} > 0)
            {
                $blockCounts{$blockStr} = 0;
            }
            if($allBlocksClosed->())
            {
                return $start, ($idx + length($blockStr));
            }
        }
        else
        {
            if($charType == OPEN_CHAR || $charType == DUAL_CHAR)
            {
                $blockCounts{$blockStr}++;
            }
            else
            {
                if($blockCounts{$blockStr} <= 0)
                {
                    #Unbalanced block!
                    return undef;
                }
                $blockCounts{$blockStr}--;
            }
            if($allBlocksClosed->())
            {
                return $start, ($idx + length($blockStr));
            }
        }
        $idx += length($blockStr);
        print "\tUnclosed at $startIndex:";
        foreach my $cval(keys %blockCounts)
        {
            print "[$cval,".$blockCounts{$cval}."] ";
        }
        print "\n";
          
        ($idx, $blockStr, $charType) = nextGroupChar($text, $idx);
    }
    if(!$allBlocksClosed->())
    {
        print "Unclosed at $startIndex:";
        foreach my $cval(keys %blockCounts)
        {
            print "[$cval,".$blockCounts{$cval}."] ";
        }
        print "\n";
    }
    return undef;
}


my $inFileName  = $ARGV[0];
my $source = read_file($inFileName);

my $blockText = "";
my $blockCount = 0;

sub appendOutput
{
    $blockText = $blockText.shift;
}
my $idx = 0;
my ($blockStart, $blockEnd) = nextBlock($source);
while(defined($blockStart))
{
    if($blockStart > $idx)
    {
        appendOutput("\n".substr($source, $idx, ($blockStart - $idx))."\n");
    }
    appendOutput("\n[GROUP $blockCount]\n"
            .substr($source, $blockStart, ($blockEnd - $blockStart))
            ."\n[GROUP $blockCount END]\n");
    $idx = $blockEnd;
    ($blockStart, $blockEnd) = nextBlock($source, $idx);
    $blockCount++;
}
if($idx < (length($source) - 1))
{
    appendOutput("\n\t[FINAL UNGROUPED]\n"
            .substr($source, $idx));
}

write_file("blockTest.txt", $blockText);


