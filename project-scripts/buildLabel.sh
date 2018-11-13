#!/bin/bash
# Generates an appropriate build label to apply while compiling.
releasePath="/etc/os-release"
altReleasePath="/usr/lib/os-release"

unquote()
{
    unquoted="${1%\"}"
    unquoted="${unquoted#\"}"
    echo $unquoted
}

readReleaseFile()
{
    filePath=$1
    buildName=`grep -oP '(?<=^PRETTY_NAME=).+' $filePath`
    if [ -z "$buildName" ]; then
        buildName=`grep -oP '(?<=^NAME=).+' $filePath`
    fi
    buildName=`unquote "$buildName"`

    version=`grep -oP '(?<=^VERSION=").+' $filePath`
    if [ -n "$version" ]; then
        version=`unquote "$version"`
        buildName="$buildName $version"
    fi
    if [ -z "$buildName" ]; then
        echo "Unknown"
    else
        echo $buildName
    fi
}

if [ -f $releasePath ]; then
    readReleaseFile $releasePath
elif [ -f $altReleasePath ]; then
    readReleaseFile $altReleasePath
else
    echo "Unknown"
fi
