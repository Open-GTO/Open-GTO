set name=Open-GTO
compiler\pawncc.exe -;+ -(+ -icompiler/includes sources\%name%.pwn
if exist %name%.amx ^
move %name%.amx gamemodes\
pause
