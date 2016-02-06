/*

	About: player quest system
	Author:	ziggi

*/

#if defined _pl_quest_included
	#endinput
#endif

#define _pl_quest_included

/*
	Defines
*/

#define INVALID_QUEST_ID	-1

/*
	Vars
*/

static
	gPlayerQuestID[MAX_PLAYERS],
	gQuestsCount = -1;

/*
	Quest functions
*/

stock RegisterQuest()
{
	if (gQuestsCount >= MAX_QUESTS) {
		return INVALID_QUEST_ID;
	}

	return ++gQuestsCount;
}

/*
	Player functions
*/

stock GetPlayerQuestID(playerid) {
	return gPlayerQuestID[playerid];
}

stock SetPlayerQuestID(playerid, questid) {
	gPlayerQuestID[playerid] = questid;
}

stock ResetPlayerQuest(playerid)
{
	SetPlayerQuestID(playerid, INVALID_QUEST_ID);
}

stock IsPlayerAtQuest(playerid)
{
	return GetPlayerQuestID(playerid) != INVALID_QUEST_ID;
}
