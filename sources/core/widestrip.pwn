/*

	About: wide strip system
	Author: ziggi

*/

#if defined _widestrip_included
	#endinput
#endif

#define _widestrip_included

/*
	Vars
*/

static
	Text:gTopStrip,
	Text:gBottomStrip;

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	gTopStrip = TextDrawCreate(640.125000, 1.500000, "strip");
	TextDrawLetterSize(gTopStrip, 0.000000, 4.868515);
	TextDrawTextSize(gTopStrip, -1.375000, 0.000000);
	TextDrawUseBox(gTopStrip, true);
	TextDrawBoxColor(gTopStrip, 255);

	gBottomStrip = TextDrawCreate(641.375000, 401.083312, "strip");
	TextDrawLetterSize(gBottomStrip, 0.000000, 4.998147);
	TextDrawTextSize(gBottomStrip, -1.375000, 0.000000);
	TextDrawUseBox(gBottomStrip, true);
	TextDrawBoxColor(gBottomStrip, 255);

	#if defined Widestrip_OnGameModeInit
		return Widestrip_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Widestrip_OnGameModeInit
#if defined Widestrip_OnGameModeInit
	forward Widestrip_OnGameModeInit();
#endif

/*
	Functions
*/

stock Widestrip_ShowForPlayer(playerid)
{
	TextDrawShowForPlayer(playerid, gTopStrip);
	TextDrawShowForPlayer(playerid, gBottomStrip);
}

stock Widestrip_HideForPlayer(playerid)
{
	TextDrawHideForPlayer(playerid, gTopStrip);
	TextDrawHideForPlayer(playerid, gBottomStrip);
}

stock Widestrip_ShowForAll()
{
	TextDrawShowForAll(gTopStrip);
	TextDrawShowForAll(gBottomStrip);
}

stock Widestrip_HideForAll()
{
	TextDrawShowForAll(gTopStrip);
	TextDrawShowForAll(gBottomStrip);
}
