#/bin/bash

PAWNCC=LD_LIBRARY_PATH=compiler/:$(LD_LIBRARY_PATH) ./compiler/pawncc

NAME=Open-GTO
PARAMS=-d2

all:
	$(PAWNCC) "-;+" "-(+" "-icompiler/includes" "-ogamemodes/$(NAME).amx" $(PARAMS) "sources/$(NAME).pwn"

clean:
	rm Open-GTO.lst Open-GTO.asm gamemodes/Open-GTO.amx
