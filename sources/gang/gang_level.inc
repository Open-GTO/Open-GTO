/*

	Author: ziggi

*/

#if defined _gang_level_included
  #endinput
#endif

#define _gang_level_included
#pragma library gang_level


new GangLevelList[] = {
	0, // 0
	500, // 1
	1000, // 2
	2000, // 3
	5000, // 4
	7500, // 5
	11000, // 6
	16000, // 7
	21000, // 8
	31000, // 9
	46000 // 10
};

#define MAX_GANG_LEVEL	(sizeof(GangLevelList)-1)
#define MAX_GANG_XP		GangLevelList[MAX_GANG_LEVEL]

stock GetGangLevel(gangid)
{
	return Gangs[gangid][gang_level];
}

stock SetGangLevel(gangid, level)
{
	return Gangs[gangid][gang_level] = level;
}

stock GetGangXP(gangid)
{
	return Gangs[gangid][gang_xp];
}

stock GetGangColour(gangid)
{
	return Gangs[gangid][gang_colour];
}

stock GiveGangXP(gangid, xpamount)
{
	Gangs[gangid][gang_xp] += xpamount;
	CheckGangLevel(gangid);
}
stock SetGangXP(gangid, xpamount)
{
	Gangs[gangid][gang_xp] = xpamount;
	CheckGangLevel(gangid);
}

stock GetGangXPToLevel(gangid, level)
{
	return GangLevelList[level] - Gangs[gangid][gang_xp];
}

stock CalculateGangLevel(gangid)
{
	for (new i = 0; i <= MAX_GANG_LEVEL; i++)
	{
		if (GetGangXP(gangid) < GangLevelList[i]) continue;
		return i;
	}
	return -1;
}

stock CheckGangLevel(gangid)
{
	return Gangs[gangid][gang_level] = CalculateGangLevel(gangid);
}
