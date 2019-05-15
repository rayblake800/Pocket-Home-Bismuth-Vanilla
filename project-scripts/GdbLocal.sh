#!/bin/bash
# Launch pocket-home with gdb, using local libraries compiled with debug flags.
env LD_LIBRARY_PATH=/home/$USER/.local/lib:$LD_LIBRARY_PATH gdb pocket-home
