#!/usr/bin/perl
use strict;
use warnings;
use File::Slurp;

# Read in a header file, and automatically create a matching formatted cpp file 
# from it formatted according to my personal preferences.

if(!defined($ARGV[0]) || !defined($ARGV[1]))
{
    die "usage: perl cpp_from_h.pl  [input file] [output file]\n";
}

my $inFileName  = $ARGV[0];
my $outFileName = $ARGV[1];

if(-e $outFileName)
{
    my $input = "";
    while($input ne "Y\n")
    {
        print "Replace existing file $outFileName?(Y/n):";
        $input = <STDIN>;
        if($input eq "n\n")
        {
            die "Cancelled.\n";
        }
    }
}

my $header = read_file($inFileName);

#remove excess indentation
$header =~ s/\n\s+/\n/gs;
#replace include statements and other misc preprocessor commands
$header =~ s/^\n*(\#.*?\n+)*/\#include \"$inFileName\"\n/g;

#format comments:
#indent block comment asterisks by one space
$header =~ s/\n\*/\n */g;
#replace multiple asterisks with single asterisk
$header =~ s/\*\*+/*/g;
#remove javadoc fields
$header =~ s/\n\s*\*\s*@.*?\*\/\n*/\n *\//gs;  
#remove empty lines in comment blocks
$header =~ s/\n\s*\*\s*\n/\n/g;
#remove empty comment blocks
$header =~ s/\/(\s*\*\n*)+\///g;


#remove access specifiers
$header =~ s/\n+[a-z]+:\s*\n+//g;

my $altered = "";
while($altered ne $header)
{
    $altered = $header;    
    #remove any section between two semicolons that does not
    #contain parentheses or curly brackets
    $header =~ s/;[^;()\{\}]*;/;/g;
    #remove pure virtual functions
    $header =~ s/;[^;]*\s*=\s*0\s*;/;/g;
}

#remove virtual, static, and override keywords
$header =~ s/virtual |static | override//g;
#remove friend class declarations
$header =~ s/friend.*?\n/\n/g;

sub findCloseIndex
{
    my $text = shift;
    my $start = shift;
    my $endChar = shift;
    
    my $parenToClose = 0;
    my $curlyToClose = 0;
    my $squareToClose = 0;
    
    my $quoteOpen = 0;
    my $commentOpen = 0;
    my $commentBlockOpen = 0;
    my $idx = $start;
    my $cval = "";
    while((($idx == $start)
            || ($parenToClose > 0)
            || ($curlyToClose > 0)
            || ($squareToClose > 0)
            || ($quoteOpen > 0)
            || ($commentOpen > 0)
            || ($commentBlockOpen > 0)
            || (defined($endChar) && ($endChar ne $cval)))
            && ($idx < length($text)))
    {
        my $last = $cval;
        $cval = substr($text,$idx,1);
        if($cval eq "\\")
        {
            $idx++;
        }
        elsif($quoteOpen > 0)
        {
            if($cval eq "\"")
            {
                $quoteOpen = 0;
            }
        }
        elsif($commentOpen > 0)
        {
            if($cval eq "\n")
            {
                $commentOpen = 0;
            }
        }
        elsif($commentBlockOpen > 0)
        {
            if(($cval eq "/") && ($last eq "*"))
            {
                $commentBlockOpen = 0;
            }
        }
        else
        {
            if   ($cval eq "(")  { $parenToClose++; }
            elsif($cval eq ")")  { $parenToClose--; }
            elsif($cval eq "{")  { $curlyToClose++; }
            elsif($cval eq "}")  { $curlyToClose--; }
            elsif($cval eq "[")  { $squareToClose++; }
            elsif($cval eq "]")  { $squareToClose--; }
            elsif($cval eq "\"") { $quoteOpen++; }
            elsif(($cval eq "/") && ($last eq "/")) { $commentOpen++; }
            elsif(($cval eq "*") && ($last eq "/")) { $commentBlockOpen++; }
        }
        $idx++;
    }
    return $idx;
}

#remove classes, namespaces, structs, and enums, and specify function namespaces
sub blockExtract
{
    my $text = shift;
    while($text =~ 
        /
            (?:\/\* [^\/]+ \*\/\s*\n*)?     #match optional block comment
            (class|namespace|struct|enum)   #capture block type
            \s+ ([^\s;]+)                    #capture block name
            [^\{;]*(\{)                      #match block start
        /xs)                         
    {
        my $startLen = length($text);
        my $type  = $1;
        my $name  = $2;
    
        my $start = $-[0];
        my $end   = findCloseIndex($text, $-[3]);
        my $len   = $end - $start + 1;
    
        my $contentStart  = $+[3];
        my $contentLength = $end - $contentStart - 2;
        if($contentLength == $len)
        {
            die "recursion loop!\n";
        }
        if($type eq "enum")
        {
            substr($text, $start, $len) = "";
        }
        else
        {
            my $class = substr($text, $contentStart, $contentLength);
            if(length($class) > 0)
            {
                if($class eq $text)
                {
                    die "recursion loop!";
                }
                $class = blockExtract($class);
                my @functions = ($class =~ /\s[^(\[\]\s]+\s*\n?\(/g);
                foreach my $fname(@functions)
                {
                    my $fnStart = index($class, $fname);
                    substr($class, $fnStart, 1) = " $name"."::";
                }
            }
            substr($text, $start, $len) = $class;
        }
    }
    return $text;
}

$header = blockExtract($header);


my @functions = ($header =~ /[^\v\(]+\v?\(/g);
@functions = sort {index($header, $a) <=> index($header, $b)} @functions;
my $lastEnd = -1;
foreach my $func (@functions)
{
    my $start = index($header, $func);
    if($start > $lastEnd)
    {
        my $parenOpen = $start + length($func) - 1;
        my $parenClose = findCloseIndex($header, $parenOpen);
        
        #remove default parameter values
        my $paramStart = $parenOpen + 1;
        my $paramLen   = $parenClose - $parenOpen - 2;
        my $paramText = substr($header, $paramStart, $paramLen);
        while($paramText =~ /\s*=/)
        {
            my $dvBegin = $-[0];
            my $dvLen   = findCloseIndex($paramText, $dvBegin, ',') - $dvBegin;
            substr($paramText, $dvBegin, $dvLen) = "";
            substr($header, $paramStart, $paramLen) = $paramText;
            $paramLen = length($paramText);
            $parenClose = findCloseIndex($header, $parenOpen);
        }
        #remove functions implemented in the header
        my $bracketClose = findCloseIndex($header, $parenClose , '}');
        my $lineClose = findCloseIndex($header, $parenClose , ';');
        
        #print("$func : } = ".($bracketClose - $parenClose)
        #        .", ; = ".($lineClose - $parenClose)."\n");
        if($bracketClose < $lineClose)
        {
            substr($header, $start, $bracketClose - $start) = "";
            $lastEnd = $start
        }
        else
        {
            $lastEnd = $lineClose;
        }
    }
}


#place exactly one empty line above each comment block
$header =~ s/\v\v+/\n/g;
$header =~ s/\v\//\n\n\//g;

$header =~ s/;/\n{\n}/g;

write_file($outFileName, $header);

