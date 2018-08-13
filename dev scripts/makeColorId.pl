#!/usr/bin/perl
# Make Color Id
#
# Creates new juce ColourID values, generating all necessary output and placing
# it in the appropriate files.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;

my $projectRoot = "..";
my $colorIdPath     = "$projectRoot/Source/Components/ColourIds.h";
my $configClassPath = "$projectRoot/Source/Config Files/ColourConfigFile.cpp";
my $configPath      = "$projectRoot/assets/configuration/colours.json";

my $uiGroups = 
[
    'windowBackground',
	'widgetBackground',
	'widgetOff',
	'widgetOn',
	'menuBackground',
	'outline',
	'focusedOutline',
	'textField',
	'text',
	'highlightedTextField',
	'highlightedText',
	'none'
];

my $classname;
my $idNum;
my $idName;
my $idKey;
my $idValue;
my $uiGroup;

my $classHeader;

print("Enter the class name: ");
chomp($classname = <STDIN>);

sub findHeader
{
    if(defined $classHeader)
    {
        return;
    }
	if($_ =~ /^$classname\.h$/)
	{
        $classHeader = $File::Find::name;
	}
}
find(\&findHeader,"$projectRoot/Source");

if(!defined $classHeader)
{
    die "Failed to find class header file $classname.h!\n";
}
else
{
    print("Using header file at $classHeader\n");
}

my $classHeaderText = read_file($classHeader) or die "Failed to read $classHeader!\n";
my $colorIdText     = read_file($colorIdPath) or die "Failed to read $colorIdPath!\n";

#Find largest Id value in use:
my $largest = 0;
my @idNums = ($colorIdText =~ /(0x[0-9A-F]+),/gi);
foreach my $id(@idNums)
{
    if(hex $id > $largest)
    {
        $largest = hex $id;
    }
}

if($classHeaderText =~ /(ColourIds.*?{.*?})/s)
{
    my @idNums = ($1 =~ /(0x[0-9A-F]+),/gi);
    my $numIds = @idNums;
    if($numIds > 0)
    {
        $idNum = hex $idNums[$numIds - 1];
        $idNum++;
        printf( "using next ID value 0x%x\n", $idNum);
    }
}
if(!defined $idNum)
{
    while(!defined $idNum)
    {
        my $nextNum = $largest + 0x100;
        printf("Enter an ID number value(default 0x%x): ", $nextNum);
        chomp($idNum = <STDIN>);
        if(length $idNum == 0)
        {
            $idNum = $nextNum;
        }
        elsif(!($idNum =~ /^0x[0-9A-F]+$/i))
        {
            print("$idNum is not a valid hex number!\n");
            $idNum = undef;
        }
        elsif($colorIdText =~ /namespace\s+(\w+)
                                \s*{\s*
                                    namespace\s+(\w+)
                                        \s*{\s*enum\s*\{
                                        [^}]*
                                            \s(\w+)\s*=\s* $idNum,
                                        [^}]*\}
                                /sx)
        {
            print("$idNum already used by $1::$2::$3\n");
            $idNum = undef;
        }
    }
    printf("Using id value 0x%x.\n", $idNum);
    
    print("Enter color enum value name: ");
    chomp($idName = <STDIN>);
    print("Enter a key string describing the color: ");
    chomp($idKey = <STDIN>);
    print("Enter a default color value(as an eight digit hex string): ");
    chomp($idValue = <STDIN>);
    
    print("Selected values: \n"
          ."New enum value: $classname\::ColourIds::$idName = $idNum\n"
          ."New config key: \"$idKey\" : \"$idValue\"\n");
    my $confirmOrCancel;
    while(!defined $confirmOrCancel)
    {
        print("Use these values?(y/n): ");
        chomp($confirmOrCancel = <STDIN>);
        if($confirmOrCancel =~ /n/i)
        {
            die "Changes canceled, exiting.\n";
        }
        elsif(! $confirmOrCancel =~ /y/i)
        {
            $confirmOrCancel = undef;
        }
    }
    
    print("Updating color ids:");
}
