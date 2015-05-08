/*

	About: premium account system
	Author: ziggi

*/

#if defined _pl_premium_included
	#endinput
#endif

#define _pl_premium_included
#pragma library pl_premium


stock Premium_UpdatePlayerStatus(playerid, date[])
{
	Premium_SetPlayerDate(playerid, date);
	
	new status = Premium_GetDateStatus(date);
	Premium_SetPlayerStatus(playerid, status);
	return 0;
}

stock Premium_SetPlayerDate(playerid, date[])
{
	SetPVarString(playerid, "Premium_Date", date);
}

stock Premium_GetPlayerDate(playerid)
{
	new string[11];
	GetPVarString(playerid, "Premium_Date", string, sizeof(string));
	return string;
}

stock Premium_SetPlayerStatus(playerid, status)
{
	SetPVarInt(playerid, "Premium_Status", status == 1 ? 1 : 0);
}

stock Premium_GetPlayerStatus(playerid)
{
	return GetPVarInt(playerid, "Premium_Status");
}

stock Premium_GetDateStatus(date[])
{
	new
		idx,
		input_day = strval(strcharsplit(date, idx, '.')),
		input_month = strval(strcharsplit(date, idx, '.')),
		input_year = strval(strcharsplit(date, idx, '.'));

	if (getdate() <= date_to_timestamp(input_year, input_month, input_day)) {
		return 1;
	}
	
	return 0;
}
