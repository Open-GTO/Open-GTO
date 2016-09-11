/*
	Created: 04.06.11
	Aurthor: ZiGGi
*/

#if defined _bar_included
	#endinput
#endif

#define _bar_included

#define MAX_BAR_ACTORS 12

enum Bar_Info {
	bar_type,
	Float:bar_x,
	Float:bar_y,
	Float:bar_z,
	bar_actor_model,
	Float:bar_actor_pos_x,
	Float:bar_actor_pos_y,
	Float:bar_actor_pos_z,
	Float:bar_actor_pos_a,
	bar_checkpoint,
}

static bar_place[][Bar_Info] = {
	{ENTEREXIT_TYPE_BAR, 495.3609, -76.0381, 998.7578,             176, 495.3513, -77.8193, 998.7651, 7.2703}, // green bar
	{ENTEREXIT_TYPE_JIZZY, -2654.0112, 1413.2083, 906.2771,        176, -2655.8608, 1413.0214, 906.2734, 269.8462}, // Jizzy's
	{ENTEREXIT_TYPE_FOURDRAGONS, 1955.3743, 1017.9493, 992.4688,   176, 1953.7958, 1017.9473, 992.4688, 276.7396}, // 4 Dragons Casino
	{ENTEREXIT_TYPE_REDSANS, 1139.7212, -4.2430, 1000.6719,        177, 1141.8073, -4.4088, 1000.6719, 85.2910}, // Redsands Casino
	{ENTEREXIT_TYPE_LILPROBEINN, -224.7835, 1407.4834, 27.7734,    176, -223.3067, 1407.3264, 27.7734, 77.1677}, // Lil' Probe Inn
	{ENTEREXIT_TYPE_ALHAMBRA, 499.4490, -16.8206, 1000.6719,       177, 501.7473, -17.2167, 1000.6719, 81.5544}, // Alhambra Club
	{ENTEREXIT_TYPE_RESTAURANT, -785.9133, 500.1498, 1371.7422,    177, -785.9677, 498.1729, 1371.7422, 4.4737}, // Liberty City Restoran
	{ENTEREXIT_TYPE_PIGPEN, 1215.3257, -13.0094, 1000.9219,        177, 1215.1998, -15.2597, 1000.9219, 2.2803}, // The Pig Pen
	{ENTEREXIT_TYPE_NUDESTRIPPERS, 1207.3661, -28.5663, 1000.9531, 177, 1206.0661, -28.5663, 1000.9531, -90.8725} // Nude Strippers Daily
};

static bar_actors[MAX_BAR_ACTORS];

enum DrinkInfo {
	Name[MAX_NAME],
	Cost,
	Float:Hp,
	Float:Alcohol,
	Action,
}

static drinks_data[][DrinkInfo] = {
	{"Водка", 100, 30.0, 40.0, SPECIAL_ACTION_DRINK_SPRUNK},
	{"Пиво", 30, 10.0, 4.5, SPECIAL_ACTION_DRINK_BEER},
	{"Вино", 30, 10.0, 10.0, SPECIAL_ACTION_DRINK_WINE},
	{"Чай", 15, 10.0, -20.0, SPECIAL_ACTION_DRINK_SPRUNK},
	{"Сигара", 15, 5.0, 2.0, SPECIAL_ACTION_SMOKE_CIGGY}
};

bar_OnGameModeInit()
{
	for (new id = 0; id < sizeof(bar_place); id++) {
		bar_place[id][bar_checkpoint] = CreateDynamicCP(bar_place[id][bar_x], bar_place[id][bar_y], bar_place[id][bar_z], 1.5, .streamdistance = 20.0);
	}
	Log_Init("services", "Bar module init.");
	return 1;
}

bar_OnInteriorCreated(id, type, world)
{
	#pragma unused id
	new slot;

	for (new i = 0; i < sizeof(bar_place); i++) {
		if (bar_place[i][bar_type] == type) {
			slot = bar_GetActorFreeSlot();
			if (slot == -1) {
				Log(systemlog, DEBUG, "bar.inc: Free slot not found. Increase MAX_BAR_ACTORS value.");
				break;
			}

			bar_actors[slot] = CreateActor(bar_place[i][bar_actor_model],
				bar_place[i][bar_actor_pos_x], bar_place[i][bar_actor_pos_y], bar_place[i][bar_actor_pos_z],
				bar_place[i][bar_actor_pos_a]
			);
			SetActorVirtualWorld(bar_actors[slot], world);
		}
	}
}

bar_OnActorStreamIn(actorid, forplayerid)
{
	for (new id = 0; id < sizeof(bar_actors); id++) {
		if (actorid == bar_actors[id]) {
			SetPVarInt(forplayerid, "bar_actor_id", actorid);
			ClearActorAnimations(actorid);
			return 1;
		}
	}
	return 0;
}

bar_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if ( PRESSED ( KEY_FIRE ) ) {
		if (GetPVarInt(playerid, "bar_Drinking") == 1 && GetPVarInt(playerid, "bar_Gulping") == 0) {
			SetPVarInt(playerid, "bar_Gulps", GetPVarInt(playerid, "bar_Gulps") + 1);
			SetPVarInt(playerid, "bar_Gulping", 1);
			SetTimerEx("bar_Gulp", 2000, 0, "d", playerid);
			return 1;
		}
	} else if ( PRESSED ( KEY_SECONDARY_ATTACK ) ) {
		bar_GulpStop(playerid);
		return 1;
	}
	return 0;
}

bar_OnPlayerEnterCheckpoint(playerid, cp)
{
	for (new id = 0; id < sizeof(bar_place); id++) {
		if (cp == bar_place[id][bar_checkpoint]) {
			Dialog_Show(playerid, Dialog:ServiceBar);
			ApplyActorAnimation(GetPVarInt(playerid, "bar_actor_id"), "MISC", "Idle_Chat_02", 4.1, 0, 1, 1, 1, 1);
			return 1;
		}
	}
	return 0;
}

forward bar_Gulp(playerid);
public bar_Gulp(playerid)
{
	new id = GetPVarInt(playerid, "bar_Alcohol_Id");
	SetPlayerDrunkLevel(playerid, GetPlayerDrunkLevel(playerid) + floatround(drinks_data[id][Alcohol] * 100 / MAX_GULPS, floatround_round));

	new Float:max_health;
	GetMaxHealth(playerid, max_health);

	new Float:health;
	GetPlayerHealth(playerid, health);

	if (health + drinks_data[id][Hp] / MAX_GULPS > max_health) {
		SetPlayerHealth(playerid, max_health);
	} else {
		SetPlayerHealth(playerid, health + drinks_data[id][Hp] / MAX_GULPS);
	}

	if (GetPVarInt(playerid, "bar_Gulps") >= MAX_GULPS) {
		bar_GulpStop(playerid);
	}

	SetPVarInt(playerid, "bar_Gulping", 0);
	return 1;
}

DialogCreate:ServiceBar(playerid)
{
	static
		string[MAX_LANG_VALUE_STRING * (sizeof(drinks_data) + 1)];

	Lang_GetPlayerText(playerid, "BAR_DIALOG_LIST_HEADER", string);

	for (new i = 0; i < sizeof(drinks_data); i++) {
		Lang_GetPlayerText(playerid, "BAR_DIALOG_LIST_ITEM", string, _,
		                   string,
		                   drinks_data[i][Name],
		                   drinks_data[i][Cost],
		                   drinks_data[i][Alcohol],
		                   drinks_data[i][Hp]);
	}

	Dialog_Open(playerid, Dialog:ServiceBar, DIALOG_STYLE_TABLIST_HEADERS,
	            "BAR_DIALOG_HEADER",
	            string,
	            "BAR_DIALOG_BUTTON_0", "BAR_DIALOG_BUTTON_1",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:ServiceBar(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	if (GetPlayerMoney(playerid) < drinks_data[listitem][Cost]) {
		Dialog_Message(playerid, "BAR_DIALOG_HEADER", "BAR_NOT_ENOUGH_MONEY", "BAR_DIALOG_BUTTON_OK");
		return 1;
	}

	GivePlayerMoney(playerid,-drinks_data[listitem][Cost]);
	SetPlayerSpecialAction(playerid, drinks_data[listitem][Action]);
	SetPVarInt(playerid, "bar_Drinking", 1);
	SetPVarInt(playerid, "bar_Alcohol_Id", listitem);

	Dialog_Message(playerid,
	               "BAR_DIALOG_HEADER",
	               "BAR_DIALOG_INFORMATION_TEXT",
	               "BAR_DIALOG_BUTTON_OK",
	               MDIALOG_NOTVAR_NONE,
	               drinks_data[listitem][Name],
	               drinks_data[listitem][Cost],
	               drinks_data[listitem][Alcohol],
	               drinks_data[listitem][Hp]);
	return 1;
}

bar_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	#pragma unused vehicleid
	if (ispassenger) {
		bar_GulpStop(playerid);
	}
	return 1;
}

stock bar_GulpStop(playerid)
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	DeletePVar(playerid, "bar_Drinking");
	DeletePVar(playerid, "bar_Gulps");
	DeletePVar(playerid, "bar_Gulping");
	DeletePVar(playerid, "bar_Alcohol_Id");
	return 1;
}

stock IsPlayerAtBar(playerid)
{
	for (new b_id = 0; b_id < sizeof(bar_place); b_id++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, bar_place[b_id][bar_x], bar_place[b_id][bar_y], bar_place[b_id][bar_z])) {
			return 1;
		}
	}
	return 0;
}

stock bar_GetActorFreeSlot()
{
	static slot;

	if (slot >= sizeof(bar_actors)) {
		return -1;
	}

	return slot++;
}
