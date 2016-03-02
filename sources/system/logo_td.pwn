/*

	About: textdraw logo system
	Author: ziggi

*/

#if defined _logo_td_included
	#endinput
#endif

#define _logo_td_included

static
	Text:TD_Logo;

public OnGameModeInit()
{
	TD_Logo = TextDrawCreate(546.5, 4.5, MODE_NAME);
	TextDrawLetterSize(TD_Logo, 0.36, 1.8);
	TextDrawAlignment(TD_Logo, 1);
	TextDrawColor(TD_Logo, -932506625);
	TextDrawSetShadow(TD_Logo, 1);
	TextDrawSetOutline(TD_Logo, 0);
	TextDrawBackgroundColor(TD_Logo, 91);
	TextDrawFont(TD_Logo, 1);
	TextDrawSetProportional(TD_Logo, 1);
	TextDrawSetShadow(TD_Logo, 1);

	#if defined Logo_OnGameModeInit
		return Logo_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
 
#define OnGameModeInit Logo_OnGameModeInit
#if defined Logo_OnGameModeInit
	forward Logo_OnGameModeInit();
#endif

public OnPlayerConnect(playerid)
{
	TextDrawShowForPlayer(playerid, TD_Logo);

	#if defined Logo_OnPlayerConnect
		return Logo_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
 
#define OnPlayerConnect Logo_OnPlayerConnect
#if defined Logo_OnPlayerConnect
	forward Logo_OnPlayerConnect(playerid);
#endif