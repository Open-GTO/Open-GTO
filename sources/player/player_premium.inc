/*

	About: premium account system
	Author: ziggi

*/

#if defined _pl_premium_included
	#endinput
#endif

#define _pl_premium_included
#pragma library pl_premium

/*
	Vars
*/

static
	gPremiumTime[MAX_PLAYERS];

/*
	Functions
*/

stock Premium_SetPlayerTime(playerid, timestamp)
{
	gPremiumTime[playerid] = timestamp;
}

stock Premium_GetPlayerTime(playerid)
{
	return gPremiumTime[playerid];
}

stock Premium_IsPlayerHave(playerid)
{
	new time = Premium_GetPlayerTime(playerid);

	if (Premium_IsDateExpired(time)) {
		return 0;
	}

	return 1;
}

stock Premium_IsDateExpired(timestamp)
{
	if (getdate() <= timestamp) {
		return 0;
	}
	
	return 1;
}

stock Premium_GetPlayerDate(playerid)
{
	new
		time,
		year,
		month,
		day;

	time = Premium_GetPlayerTime(playerid);
	gmtime(time, year, month, day);

	new string[11];
	format(string, sizeof(string), "%02d.%02d.%04d", day, month, year);
	return string;
}
