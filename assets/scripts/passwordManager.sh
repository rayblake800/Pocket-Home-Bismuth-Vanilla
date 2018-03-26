#!/bin/bash
#
#  Updates or removes the saved Pocket-Home password hash.
#
# To set or update:
# ./passwordManager.sh [User] [new password hash]
#
# To remove:
# ./passwordManager.sh [User]
#
# This needs to be run as root, to ensure the password file/folder
# are secured.  Pocket-home will attempt to use pkexec to run this script as 
# root.  
#

#return values
INVALID_USER=-5
MISSING_APP_DIR=-4
NOT_ROOT=-3
FAILED_TO_SET_PASSWORD=-2
FAILED_TO_REMOVE_PASSWORD=-1
ACTION_SUCCEEDED=0

targetUser=$1
newValue=$2
allUsers=$(users)
nameLength=${#targetUser}

# Username must have only valid user characters, be at least two
# characters but less than 32, and it must be present in the list
# of all users.

if ! [[ ${targetUser} =~ [a-z_][a-z0-9_-]*$ ]] \
|| [[ ${nameLength} -gt 32 ]] \
|| [[ ${allUsers} != *$targetUser* ]]; then
    echo "invalid user!"
    exit $INVALID_USER
fi

# The user must have a directory in /home that contains a
# .pocket-home directory.

homedir="/home/$targetUser/.pocket-home"
if [ ! -d "$homedir" ]; then 
    echo "missing pocket home directory $passdir"
    exit $MISSING_APP_DIR
fi

# This must be running as root to properly set the password directory.
if [ "$USER" = "root" ]; then
    passdir="$homedir/.passwd"
    if [ ! -d "$passdir" ]; then
        mkdir "$passdir"
        echo "created password directory $passdir"
    fi
    passfile="$passdir/passwd"
    if [[ ${newValue} =~ ^\s*$ ]]; then
        rm "$passfile"
        
        if [ -f "$passfile" ]; then
            echo "Couldn't remove password!"
            exit $FAILED_TO_REMOVE_PASSWORD
        else
            echo "Removed password file."
            exit $ACTION_SUCCEEDED
        fi
    else
        echo "$newValue" >| "$passfile"
        writtenFile=$(cat "$passfile") 
        if [ ! -f "$passfile" ]; then
            echo "Couldn't create password file!"
            exit $FAILED_TO_SET_PASSWORD
        fi
        if [ "$newValue" != "$writtenFile" ]; then
            echo "Couldn't change password!"            
            exit $FAILED_TO_SET_PASSWORD
        fi
        echo "Updated the password file correctly."
        exit $ACTION_SUCCEEDED
        
    fi
else
    echo "You need to be running this as root."
    exit $NOT_ROOT
fi
