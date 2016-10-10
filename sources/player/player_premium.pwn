/*

	About: premium account system
	Author: ziggi

*/

#if defined _pl_premium_included
	#endinput
#endif

#define _pl_premium_included

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

stock GetPremiumDateString(time, string[], const size = sizeof(string))
{
	new
		year,
		month,
		day;

	gmtime(time, year, month, day);

	format(string, size, "%02d.%02d.%04d", day, month, year);
}

stock GetPlayerPremiumDateString(playerid, string[], const size = sizeof(string))
{
	new
		time;

	time = Account_GetPremiumTime(playerid);

	GetPremiumDateString(time, string, size);
}

stock ret_GetPlayerPremiumDateString(playerid)
{
	new string[11];
	GetPlayerPremiumDateString(playerid, string);
	return string;
}
