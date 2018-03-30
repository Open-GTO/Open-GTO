PAWNCC=LD_LIBRARY_PATH=compiler/:$(LD_LIBRARY_PATH) ./compiler/pawncc

NAME=Open-GTO
PARAMS=

all:
	$(PAWNCC) $(PARAMS) "-;+" "-(+" "-icompiler/includes" "-isources" "-isources/lib/protection" "-ogamemodes/$(NAME).amx" "sources/$(NAME).pwn"

clean:
	rm gamemodes/Open-GTO.lst gamemodes/Open-GTO.asm gamemodes/Open-GTO.amx
