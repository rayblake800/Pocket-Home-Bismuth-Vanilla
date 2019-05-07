#!/bin/bash
### DebugInfo.sh ###############################################################
# Generates and saves debugging info so I can solve problems.
################################################################################

mkdir bismuthDebug

echo "Creating debug information file."

# Include a copy of all configuration files
mkdir bismuthDebug/config
cp -r ~/.config/pocket-home bismuthDebug/config
# Include a copy of the override command file, if present
overrideFile="/usr/share/pocket-home/overrideCommands.json"
if [ -f "$overrideFile" ]; then
    cp "$overrideFile" bismuthDebug/config/overrideCommands.json
fi
# Check the pocket-home installation
dpkg -s pocket-home > bismuthDebug/homeVersion.txt
# Check the network-manager installation
dpkg -s network-manager > bismuthDebug/libNMVersion.txt
# Check available network interfaces
ip link show > bismuthDebug/interfaces.txt

# Compress and remove debug information directory
tar -zcf bismuthDebug.tar.gz bismuthDebug
rm -r bismuthDebug

if [ -f bismuthDebug.tar.gz ]; then
    echo "Debug information saved to bismuthDebug.tar.gz."
    echo "Include this file with a description of your problem at https://github.com/centuryglass/Pocket-Home-Bismuth/issues, or email it to me directly at anthony0857@gmail.com."
else
    echo "Failed to create debug info! Do you have write privileges in the current directory?"
fi
