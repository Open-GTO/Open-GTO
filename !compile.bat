set name=Open-GTO
compiler\pawncc.exe -;+ -(+ sources\%name%.pwn
if exist %name%.amx ^
move %name%.amx gamemodes\
pause
