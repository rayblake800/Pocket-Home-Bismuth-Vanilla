#!/bin/bash
# Launch pocket-home with gdb, using local libraries compiled with debug flags.
env LD_LIBARARY_PATH=/home/$USER/.local/lib:$LD_LIBARARY_PATH gdb pocket-home
