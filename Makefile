#/bin/bash

PAWNCC=LD_LIBRARY_PATH=compiler/:$(LD_LIBRARY_PATH) ./compiler/pawncc

NAME=Open-GTO

all:
	$(PAWNCC) "-;+" "-(+" "-icompiler/includes" "-ogamemodes/$(NAME).amx" $(param) "sources/$(NAME).pwn"

clean:
	rm Open-GTO.lst Open-GTO.asm gamemodes/Open-GTO.amx
