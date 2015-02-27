#/bin/bash

export LD_LIBRARY_PATH=$(pwd)"/compiler/:$LD_LIBRARY_PATH"

NAME="Open-GTO"

./compiler/pawncc "-;+ -(+" sources/$NAME.pwn
mv $NAME.amx gamemodes/