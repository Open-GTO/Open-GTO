#/bin/bash

export LD_LIBRARY_PATH=$(pwd)"/compiler/:$LD_LIBRARY_PATH"

NAME="Open-GTO"

./compiler/pawncc "-;+ -(+" sources/$NAME.pwn

if [ $(stat -c%s "$NAME.amx") -gt 0 ];
then
	mv $NAME.amx gamemodes/
else
	rm $NAME.amx
fi