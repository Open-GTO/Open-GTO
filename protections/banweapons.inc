/*
	Author:	SLICK
	Created: 15.06.11
*/

new BanWeapons[] = {35, 36, 37, 38, 39};

stock banweapons_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused playerid, reason
	for(new i=0; i<sizeof(BanWeapons); i++)
	{
		if(GetPlayerWeapon(killerid) == BanWeapons[i])
		{
			new str[70];
			ResetPlayerWeapons(killerid);
			GetWeaponName(BanWeapons[i], str, sizeof(str));
			format(str, sizeof(str), "* ¬ы воспользовались запрещенным оружием ( %s )!", str);
			SendClientMessage(killerid, COLOUR_RED, str);
		}
	}
	return 1;
}
