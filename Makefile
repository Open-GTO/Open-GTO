PAWNCC=LD_LIBRARY_PATH=compiler/:$(LD_LIBRARY_PATH) ./compiler/pawncc

NAME=Open-GTO
PARAMS=-d2

all:
	$(PAWNCC) $(PARAMS) "-;+" "-(+" "-icompiler/includes" "-ogamemodes/$(NAME).amx" "sources/$(NAME).pwn"

clean:
	rm Open-GTO.lst Open-GTO.asm gamemodes/Open-GTO.amx
