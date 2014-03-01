/*

	About: player quest system
	Author:	ziggi

*/

#if defined _pl_quest_included
	#endinput
#endif

#define _pl_quest_included
#pragma library pl_quest


#define INVALID_QUEST_ID	-1

static
	QuestsCount = INVALID_QUEST_ID;


stock quest_Register()
{
	if (QuestsCount >= MAX_QUESTS) {
		return INVALID_QUEST_ID;
	}

	return ++QuestsCount;
}

stock player_GetQuestID(playerid) {
	return GetPVarInt(playerid, "player_QuestID");
}

stock player_SetQuestID(playerid, questid) {
	SetPVarInt(playerid, "player_QuestID", questid);
}

stock player_ResetQuest(playerid)
{
	oDisablePlayerCheckpoint(playerid);
	player_SetQuestID(playerid, INVALID_QUEST_ID);
}

stock player_IsAtQuest(playerid)
{
	return player_GetQuestID(playerid) == INVALID_QUEST_ID ? 0 : 1;
}