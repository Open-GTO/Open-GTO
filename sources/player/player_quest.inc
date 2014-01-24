/*

	About: player quest system
	Author:	ziggi

*/

#if defined _pl_quest_included
	#endinput
#endif

#define _pl_quest_included
#pragma library pl_quest


stock pl_quest_GetQuestID(playerid) {
	return GetPVarInt(playerid, "pl_quest_QuestID");
}

stock pl_quest_SetQuestID(playerid, questid) {
	SetPVarInt(playerid, "pl_quest_QuestID", questid);
}

stock pl_quest_Reset(playerid)
{
	oDisablePlayerCheckpoint(playerid);
	pl_quest_SetQuestID(playerid, INVALID_QUEST_ID);
}

stock pl_quest_IsAtQuest(playerid)
{
	return pl_quest_GetQuestID(playerid) == INVALID_QUEST_ID ? 0 : 1;
}