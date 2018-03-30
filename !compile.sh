#/bin/bash

export LD_LIBRARY_PATH=$(pwd)"/compiler/:$LD_LIBRARY_PATH"

NAME="Open-GTO"
PARAMS=$1

./compiler/pawncc "-;+" "-(+" "-icompiler/includes" "-isources" $PARAMS "-ogamemodes/$NAME.amx" "sources/$NAME.pwn"
