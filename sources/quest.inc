/*

	About: quest system
	Author:	ziggi

*/

#if defined _quest_included
	#endinput
#endif

#define _quest_included
#pragma library quest

static
	QuestsCount = INVALID_QUEST_ID;


stock quest_Register()
{
	if (QuestsCount >= MAX_QUESTS) {
		return INVALID_QUEST_ID;
	}

	return ++QuestsCount;
}
