/*

	About: wide strip system
	Author: ziggi

*/

#if defined _widestrip_included
  #endinput
#endif

#define _widestrip_included


static
	Text:top_strip,
	Text:bottom_strip;

stock widestrip_OnGameModeInit()
{
	top_strip = TextDrawCreate(640.125000, 1.500000, "strip");
	TextDrawLetterSize(top_strip, 0.000000, 4.868515);
	TextDrawTextSize(top_strip, -1.375000, 0.000000);
	TextDrawAlignment(top_strip, 3);
	TextDrawColor(top_strip, 255);
	TextDrawUseBox(top_strip, true);
	TextDrawBoxColor(top_strip, 255);
	TextDrawSetShadow(top_strip, 0);
	TextDrawSetOutline(top_strip, 0);
	TextDrawFont(top_strip, 0);

	bottom_strip = TextDrawCreate(641.375000, 401.083312, "strip");
	TextDrawLetterSize(bottom_strip, 0.000000, 4.998147);
	TextDrawTextSize(bottom_strip, -1.375000, 0.000000);
	TextDrawAlignment(bottom_strip, 3);
	TextDrawColor(bottom_strip, 0);
	TextDrawUseBox(bottom_strip, true);
	TextDrawBoxColor(bottom_strip, 255);
	TextDrawSetShadow(bottom_strip, 0);
	TextDrawSetOutline(bottom_strip, 0);
	TextDrawFont(bottom_strip, 0);
	return 1;
}

stock widestrip_Show(playerid)
{
	TextDrawShowForPlayer(playerid, top_strip);
	TextDrawShowForPlayer(playerid, bottom_strip);
	return 1;
}

stock widestrip_Hide(playerid)
{
	TextDrawHideForPlayer(playerid, top_strip);
	TextDrawHideForPlayer(playerid, bottom_strip);
	return 1;
}
