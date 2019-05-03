#!/bin/bash
# Checks if a battery is available
#
# Prints the battery voltage file or device if a battery is found.

# Check for PocketCHIP battery voltage file:
voltagePath="/usr/lib/pocketchip-batt/voltage"
if [ -f "$voltagePath" ]; then
    echo "$voltagePath"
    exit 0
fi

# Check for battery devices through upower:
if [ ! -z `command -v upower` ]; then
    batteryDevice=`upower -e | grep -i -m 1 'BAT'`
    if [ ! -z "$batteryDevice" ]; then
        echo "$batteryDevice"
        exit 0
    fi
fi

# Check for battery devices through acpi:
if [ ! -z `command -v acpi` ]; then
    batteryDevice=`acpi -b`
    if [ ! -z "$batteryDevice" ]; then
        echo "$batteryDevice"
        exit 0
    fi
fi

# Check for battery directories in /sys/class/power_supply:
if [ -d /sys/class/power_supply ]; then
    batteryDir=`ls -t /sys/class/power_supply | head -n 1`
    if [ ! -z "$batteryDir" ]; then
        echo "/sys/class/power_supply/$batteryDir"
        exit 0
    fi
fi

# Check for battery directories in /proc/acpi/battery (deprecated):
if [ -d /proc/apci/battery ]; then
    batteryDir=`ls -t /proc/acpi/battery | head -n 1`
    if [ ! -z "$batteryDir" ]; then
        echo "/proc/acpi/battery/$batteryDir"
        exit 0
    fi
fi
