/*
	Created: 01.02.11
	Aurthor: ZiGGi
*/

#if defined _fastfood_included
	#endinput
#endif

#define _fastfood_included
#pragma library fastfood

// types (icon id's)
#define e_FASTFOOD_TYPE_PIZZA      29
#define e_FASTFOOD_TYPE_BURGER     10
#define e_FASTFOOD_TYPE_CHICKEN    14
#define e_FASTFOOD_TYPE_RESTAURANT 50
#define e_FASTFOOD_TYPE_DONUTS     17

#define MAX_FASTFOOD_ACTORS        38

enum e_Fastfood_Info {
	ff_type,
	Float:ff_x,
	Float:ff_y,
	Float:ff_z,
	ff_actor_model,
	Float:ff_actor_pos_x,
	Float:ff_actor_pos_y,
	Float:ff_actor_pos_z,
	Float:ff_actor_pos_a,
	ff_checkpoint_id,
}

static fastfood_place[][e_Fastfood_Info] = {
	{e_FASTFOOD_TYPE_PIZZA,       373.4858, -119.4608, 1001.4922,    209,  373.5132, -117.0893, 1001.4995, 178.9151},
	{e_FASTFOOD_TYPE_BURGER,      377.4426, -67.5379,  1001.5078,    205,  377.2474, -65.2258,  1001.5078, 180.7951},
	{e_FASTFOOD_TYPE_CHICKEN,     369.3667, -6.3469,   1001.8516,    167,  369.3396, -4.1569,   1001.8516, 178.9150},
	{e_FASTFOOD_TYPE_RESTAURANT, -784.5101,  505.0186, 1371.7422,    171, -782.3088,  504.5334, 1371.7422, 85.2274},
	{e_FASTFOOD_TYPE_DONUTS,      379.4050, -190.5775, 1000.6328,    155,  380.6582, -188.8537, 1000.6328, 147.8714}
};

static fastfood_actors[MAX_FASTFOOD_ACTORS];

enum e_Food_Info {
	food_name[MAX_NAME],
	food_cost,
	Float:food_hp
}

static food_data[][e_Food_Info] = {
	{"√нилой корень", 10, 10.0},
	{"ѕеченьки с молоком", 15, 25.0},
	{"ѕетрушка с хреном", 25, 50.0},
	{"ƒва €йца всм€тку", 35, 80.0},
	{"јмлет с колбасой", 45, 100.0}
};

stock fastfood_OnGameModeInit()
{
	for (new id = 0; id < sizeof(fastfood_place); id++) {
		fastfood_place[id][ff_checkpoint_id] = Checkpoint_Create(fastfood_place[id][ff_x], fastfood_place[id][ff_y], fastfood_place[id][ff_z], 1.5, .streamdistance = 20.0);
	}

	Log_Game(lang_texts[17][0]);
	return 1;
}

stock fastfood_OnInteriorCreated(id, type, world)
{
	#pragma unused id
	new slot;

	for (new i = 0; i < sizeof(fastfood_place); i++) {
		if (fastfood_place[i][ff_type] == type) {
			slot = fastfood_GetActorFreeSlot();
			if (slot == -1) {
				break;
			}

			fastfood_actors[slot] = CreateActor(fastfood_place[i][ff_actor_model],
				fastfood_place[i][ff_actor_pos_x], fastfood_place[i][ff_actor_pos_y], fastfood_place[i][ff_actor_pos_z],
				fastfood_place[i][ff_actor_pos_a]
			);
			SetActorVirtualWorld(fastfood_actors[slot], world);
		}
	}
}

stock fastfood_OnActorStreamIn(actorid, forplayerid)
{
	for (new id = 0; id < sizeof(fastfood_actors); id++) {
		if (actorid == fastfood_actors[id]) {
			SetPVarInt(forplayerid, "fastfood_current_id", actorid);
			ClearActorAnimations(actorid);
		}
	}
	return 1;
}

stock fastfood_OnPlayerEnterCP(playerid, cp)
{
	for (new id = 0; id < sizeof(fastfood_place); id++) {
		if (cp == fastfood_place[id][ff_checkpoint_id]) {
			Dialog_Show(playerid, Dialog:ServiceFastfood);
			ApplyActorAnimation(GetPVarInt(playerid, "fastfood_current_id"), "MISC", "Idle_Chat_02", 4.1, 0, 1, 1, 1, 1);
			return 1;
		}
	}
	return 0;
}

DialogCreate:ServiceFastfood(playerid)
{
	new string[MAX_STRING * (sizeof(food_data) + 1)];
	string = lang_texts[17][1];

	for (new i = 0; i < sizeof(food_data); i++) {
		format(string, sizeof(string),
			lang_texts[17][2],
			string, food_data[i][food_name], food_data[i][food_cost], food_data[i][food_hp]
		);
	}

	Dialog_Open(playerid, Dialog:ServiceFastfood, DIALOG_STYLE_TABLIST_HEADERS, lang_texts[17][3], string, lang_texts[17][4], lang_texts[17][5]);
}

DialogResponse:ServiceFastfood(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	if (GetPlayerMoney(playerid) < food_data[listitem][food_cost]) {
		Dialog_Message(playerid, lang_texts[17][6], lang_texts[17][7], lang_texts[17][8]);
		return 1;
	}

	GivePlayerMoney(playerid,-food_data[listitem][food_cost], 1);
	ApplyAnimation(playerid, "FOOD", "EAT_Burger", 4.1, 0, 1, 1, 1, 1, 1);

	new Float:max_health;
	GetMaxHealth(playerid, max_health);

	new Float:health;
	GetPlayerHealth(playerid, health);

	if (food_data[listitem][food_hp] + health > max_health) {
		SetPlayerHealth(playerid, max_health);
	} else {
		SetPlayerHealth(playerid, health + food_data[listitem][food_hp]);
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[17][9], food_data[listitem][food_name], food_data[listitem][food_cost], food_data[listitem][food_hp]);
	Dialog_Message(playerid, lang_texts[17][6], string, lang_texts[17][8]);
	return 1;
}

stock IsPlayerAtFastfood_data(playerid)
{
	for (new ff_id = 0; ff_id < sizeof(fastfood_place); ff_id++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, fastfood_place[ff_id][ff_x], fastfood_place[ff_id][ff_y], fastfood_place[ff_id][ff_z])) {
			return 1;
		}
	}
	return 0;
}

stock fastfood_GetActorFreeSlot()
{
	static slot;
	slot++;

	if (slot >= sizeof(fastfood_actors)) {
		return -1;
	}
	
	return slot;
}