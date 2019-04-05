# BlockSearch.pm
# Finds different types of code blocks in c++ files.
use strict;
use warnings;

package BlockSearch;

use constant FALSE => 0;
use constant TRUE  => 1;

# Block type flags:
use constant DOUBLE_QUOTED       => 0b0000001;
use constant SINGLE_QUOTED       => 0b0000010;
use constant COMMENT_BLOCK       => 0b0000100;
use constant SQUARE_BRACKET      => 0b0001000;
use constant CURLY_BRACKET       => 0b0010000;
use constant PARENTHESES         => 0b0100000;
use constant ANGLE_BRACKET       => 0b1000000;

# Allows any block type:
use constant ALL_BLOCK_TYPES     => 0b1111111;

# Default block types used:
# Angle brackets are ignored by default to prevent conflicts with greater than/
# less than operators.
use constant DEFAULT_BLOCK_TYPES => 0b0111111;

my @blockTypes = 
(
    DOUBLE_QUOTED,
    SINGLE_QUOTED,
    COMMENT_BLOCK,
    SQUARE_BRACKET,
    CURLY_BRACKET,
    PARENTHESES,
    ANGLE_BRACKET
);

# Block edge string type flags:
#  Edge strings mark the beginning or the end of a block. Edge type indicates if
#  an edge string marks the beginning of a block, the end of a block, or both.
use constant OPEN_EDGE  => 0b001; # Used to open a block.
use constant CLOSE_EDGE => 0b010; # Used to close a block.
use constant DUAL_EDGE  => 0b100; # Used for both opening and closing blocks.
use constant ANY_EDGE   => 0b111; 


# Map block types to their properties:
my %blockNames;    # Names identifying block types
my %blockOpen;     # Strings used to open blocks
my %blockClose;    # Strings used to close blocks
my %allowsNesting; # Whether the given type of block can contain other blocks

# Map block edge strings to their block types:
my %edgeBlockTypes;
#Map block edge strings to their edge types:
my %edgeStringTypes;

# Initialize block type properties:
sub initType
{
    my($blockType, $name, $openStr, $closeStr, $allowNesting) = @_;
    $blockNames{$blockType}     = $name;
    $blockOpen{$blockType}      = $openStr;
    $blockClose{$blockType}     = $closeStr;
    $allowsNesting{$blockType}  = $allowNesting;

    $edgeBlockTypes{$openStr}  = $blockType;
    $edgeBlockTypes{$closeStr} = $blockType;
    
    if($openStr eq $closeStr)
    {
        $edgeStringTypes{$openStr} = DUAL_EDGE;
    }
    else
    {
        $edgeStringTypes{$openStr}  = OPEN_EDGE;
        $edgeStringTypes{$closeStr} = CLOSE_EDGE;
    }
}
initType(DOUBLE_QUOTED,  "Double Quoted",  "\"", "\"", FALSE);
initType(SINGLE_QUOTED,  "Single Quoted",  "'",  "'",  FALSE);
initType(COMMENT_BLOCK,  "Comment Block",  "/*", "*/", FALSE);
initType(SQUARE_BRACKET, "Square Bracket", "[",  "]",  TRUE);
initType(CURLY_BRACKET,  "Curly Bracket",  "{",  "}",  TRUE);
initType(PARENTHESES,    "Parentheses",    "(",  ")",  TRUE);

# Be careful to avoid conflict with greater than/less than operators!
initType(ANGLE_BRACKET,  "Angle Bracket",  "<",  ">",  TRUE);

##########
# Finds the next symbol in a string that marks the beginning or end of a block. 
# Parameters:
#   $text:             The string to search.
#
#   $startIndex:       Optionally used to ignore all matches before this index.
#
#   $validBlockTypes:  Block type flags indicating which block types should be
#                      used and which should be ignored. By default, this 
#                      includes all block types except angle brackets.
#
#   $validEdgeTypes:   Edge type flags indicating which edge types will be used
#                      and which will be ignored. By default, this includes all 
#                      edge types.
#
# Returns(If a match is found):
#   -The index of the located symbol
#   -The block type encountered
#   -The edge string type encountered
#
# Returns(If no match is found):
#   -undef
##########
sub nextBlockSymbol
{
    my ($text, $startIndex, $validBlockTypes, $validEdgeTypes) = @_;
    if(!defined($startIndex))
    {
        $startIndex = 0;
    }
    if(!defined($text) || length($text) == 0 
            || $startIndex >= length($text))
    {
        return undef;
    }
    if(!defined($validBlockTypes))
    {
        # Only search for <> blocks when specifically requested.
        $validBlockTypes = DEFAULT_BLOCK_TYPES;
    }
    if(!defined($validEdgeTypes))
    {
        $validEdgeTypes = ANY_EDGE;
    }
    my $searchedText = substr($text, $startIndex);
    if($searchedText =~ /
            (|.*?[^\\])?    # Capture all text before the match, and ensure the 
                            # match isn't escaped.
            (
              ["'[\]{}()<>] # Match single-character edge strings
              |  \/\*       # Match comment block openings
              |  \*\/       # Match comment block closings
            )/gsx)
    {

        my $preMatchStr = $1;
        my $edgeString = $2;
        if(!defined($preMatchStr))
        {
            $preMatchStr = "";
        }

        my $symbolIndex = $startIndex + length($preMatchStr);
        my $edgeType  = $edgeStringTypes{$edgeString};
        my $blockType = $edgeBlockTypes{$edgeString};;

        if(!defined($edgeType) || !defined($blockType))
        {
            die("Block symbol \"$edgeString\" matched but isn't found!\n");
        }
        elsif((($edgeType & $validEdgeTypes) == 0)
                || (($blockType & $validBlockTypes) == 0))
        {
            my $newStartIdx = $symbolIndex + length($edgeString);
            return nextBlockSymbol($text, $newStartIdx, $validBlockTypes,
                    $validEdgeTypes);
        }
        return $symbolIndex, $blockType, $edgeType;
    }
    return undef;
}

##########
# Finds the next code block within a string.
#
# Parameters:
#   $text:        The string to search.
#
#   $startIndex:  Optionally used to ignore all matches before this index.
#
#   $validTypes:  Optionally search for certain types of block, using any
#                 combination of block flags.
#
# Returns(If a match is found):
#   -The block type encountered
#   -The index of the block's first character
#   -The length of the block
#
# Returns(If no match is found):
#   -undef
##########
sub findBlock
{
    my ($text, $startIndex, $validTypes) = @_;
    if(!defined($startIndex))
    {
        $startIndex = 0;
    }
    if(!defined($validTypes))
    {
        $validTypes = DEFAULT_BLOCK_TYPES;
    }

    my %openBlockCounts; # Maps block type to open block count
    my $outerBlockIndex; # Start index of the first encountered block
    my $outerBlockType;  # Block type of the first encountered block

    # Sets which block types are considered valid.
    # When within a non-nesting block, set this to ignore all other block edge 
    # characters until the block is closed.
    my $currentValidBlockTypes = $validTypes;
    
    # Sets which edge types are considered valid.
    # Use this to ignore closing edges before the first block is found, and to
    # ignore opening edges when within a non-nesting block.
    my $currentValidEdgeTypes = OPEN_EDGE | DUAL_EDGE;

    foreach my $type(@blockTypes)
    {
        $openBlockCounts{$type} = 0;
    }

    # Check if all encountered blocks have been closed.
    my $allBlocksClosed = sub
    {
        if(!defined($outerBlockIndex) || !defined($outerBlockType))
        {
            return FALSE;
        }
        foreach my $type(@blockTypes)
        {
           if($openBlockCounts{$type} != 0)
           {
               return FALSE;
           }
        }
        return TRUE;
    };
    

    # Checks if an encountered type is valid.
    my $isValidType = sub
    {
        my $foundType = shift;
        my $flags = shift;
        return ($foundType & $flags) != 0;
    };

    my ($edgeStrIdx, $blockType, $edgeType) = nextBlockSymbol
        ($text, $startIndex, $currentValidBlockTypes, $currentValidEdgeTypes);
    while(defined($edgeStrIdx))
    {
        my $edgeString = ($edgeType == OPEN_EDGE) ?
            $blockOpen{$blockType} : $blockClose{$blockType};
        my $edgeLength = length($edgeString);

        if(!defined($outerBlockIndex))
        {
            $outerBlockIndex = $edgeStrIdx;
            $outerBlockType  = $blockType;
            $currentValidEdgeTypes = ANY_EDGE;
        }
        if($edgeType == OPEN_EDGE || $edgeType == DUAL_EDGE)
        {
            $openBlockCounts{$blockType}++;
            if(!$allowsNesting{$blockType})
            {
                $currentValidBlockTypes = $blockType;
                $currentValidEdgeTypes = DUAL_EDGE | CLOSE_EDGE;
            }
        }
        else
        {
            if($openBlockCounts{$blockType} <= 0)
            {
                #Unbalanced block!
                return undef;
            }
            $openBlockCounts{$blockType}--;
            $currentValidBlockTypes = $validTypes;
            $currentValidEdgeTypes = ANY_EDGE;
        }
        if($allBlocksClosed->())
        {
            my $length = $edgeStrIdx + $edgeLength - $outerBlockIndex;
            return $outerBlockType, $outerBlockIndex, $length;
        }
        $edgeStrIdx += $edgeLength;
        ($edgeStrIdx, $blockType, $edgeType) = nextBlockSymbol($text,
                $edgeStrIdx, $currentValidBlockTypes, $currentValidEdgeTypes);
    }
    if(defined($outerBlockType) && !$allBlocksClosed->())
    {
        print "Unclosed blocks after $startIndex:";
        foreach my $type(@blockTypes)
        {
            print "[$blockNames{$type}, $openBlockCounts{$type}]\n";
        }
    }
    return undef;
}
1;
