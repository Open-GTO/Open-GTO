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
	Functions
*/

stock IsPlayerHavePremium(playerid)
{
	new time = Account_GetPremiumTime(playerid);

	if (IsDateExpired(time)) {
		return 0;
	}

	return 1;
}

stock ReturnPlayerPremiumDateString(playerid)
{
	new
		time,
		year,
		month,
		day;

	time = Account_GetPremiumTime(playerid);
	gmtime(time, year, month, day);

	new string[11];
	format(string, sizeof(string), "%02d.%02d.%04d", day, month, year);
	return string;
}
