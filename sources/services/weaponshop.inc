/*

	About: Weapon shop
	Author: ziggi
	Date: 14.01.2014

*/


#if defined _weaponshop_included
	#endinput
#endif

#define _weaponshop_included
#pragma library weaponshop

#define MAX_WSHOP_ACTORS 10

enum weapon_Shop_Info {
	wshop_type,
	Float:wshop_x,
	Float:wshop_y,
	Float:wshop_z,
	wshop_actor_model,
	Float:wshop_actor_pos_x,
	Float:wshop_actor_pos_y,
	Float:wshop_actor_pos_z,
	Float:wshop_actor_pos_a,
	wshop_checkpoint,
}

new wshop_place[][weapon_Shop_Info] = {
	{INTERIOR_TYPE_AMMUNATION_1, 296.1441, -37.7606, 1001.5156,   179, 296.1232, -40.2155, 1001.5156, 359.9678},
	{INTERIOR_TYPE_AMMUNATION_2, 295.3583, -80.1250, 1001.5156,   179, 295.3518, -82.5274, 1001.5156, 356.2078},
	{INTERIOR_TYPE_AMMUNATION_3, 290.0941, -109.2533, 1001.5156,  179, 290.0896, -111.5135, 1001.5156, 356.8344},
	{INTERIOR_TYPE_AMMUNATION_4, 313.9999, -133.8288, 999.6016,   179, 316.1227, -133.7110, 999.6016, 97.4153},
	{INTERIOR_TYPE_AMMUNATION_5, 312.5029, -165.4625, 999.6010,   179, 312.5927, -167.7639, 999.5938, 3.1246}
};

static wshop_actors[MAX_WSHOP_ACTORS];


wshop_OnGameModeInit()
{
	for (new id = 0; id < sizeof(wshop_place); id++) {
		wshop_place[id][wshop_checkpoint] = Checkpoint_Create(wshop_place[id][wshop_x], wshop_place[id][wshop_y], wshop_place[id][wshop_z], 1.5, .streamdistance = 20.0);
	}
	Log_Game(lang_texts[17][44]);
	return 1;
}

wshop_OnInteriorCreated(id, type, world)
{
	#pragma unused id
	new slot;

	for (new i = 0; i < sizeof(wshop_place); i++) {
		if (wshop_place[i][wshop_type] == type) {
			slot = wshop_GetActorFreeSlot();
			if (slot == -1) {
				Log_Debug("weaponshop.inc:%d: Free slot not found. Increase MAX_WSHOP_ACTORS value.", __line);
				break;
			}

			wshop_actors[slot] = CreateActor(wshop_place[i][wshop_actor_model],
				wshop_place[i][wshop_actor_pos_x], wshop_place[i][wshop_actor_pos_y], wshop_place[i][wshop_actor_pos_z],
				wshop_place[i][wshop_actor_pos_a]
			);
			SetActorVirtualWorld(wshop_actors[slot], world);
		}
	}
}

wshop_OnActorStreamIn(actorid, forplayerid)
{
	for (new id = 0; id < sizeof(wshop_actors); id++) {
		if (actorid == wshop_actors[id]) {
			SetPVarInt(forplayerid, "wshop_actor_id", actorid);
			ClearActorAnimations(actorid);
			return 1;
		}
	}
	return 0;
}

wshop_OnPlayerEnterCheckpoint(playerid, cp)
{
	for (new id = 0; id < sizeof(wshop_place); id++) {
		if (cp == wshop_place[id][wshop_checkpoint]) {
			Dialog_Show(playerid, Dialog:ServiceWeapon);
			ApplyActorAnimation(GetPVarInt(playerid, "wshop_actor_id"), "MISC", "Idle_Chat_02", 4.1, 0, 1, 1, 1, 1);
			return 1;
		}
	}
	return 0;
}

// select weapon from list
DialogCreate:ServiceWeapon(playerid)
{
	new string[ (sizeof(Weapons) + 1) * (MAX_NAME + 12 + 4) ];
	string = lang_texts[17][55];

	for (new weaponid = 1; weaponid < sizeof(Weapons); weaponid++) {
		if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
			continue;
		}
		format(string, sizeof(string), lang_texts[17][56], string, ReturnWeaponName(weaponid), GetWeaponCost(weaponid));
	}

	Dialog_Open(playerid, Dialog:ServiceWeapon, DIALOG_STYLE_TABLIST_HEADERS, lang_texts[17][57], string, lang_texts[17][58], lang_texts[17][59]);
}

DialogResponse:ServiceWeapon(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 0;
	}

	new weaponid = wshop_GetSelectedWeaponID(playerid, listitem);
	SetPVarInt(playerid, "Buy_Weapon_ID", weaponid);
	Dialog_Show(playerid, Dialog:ServiceWeaponBuy);
	return 1;
}

// buy and input bullets count
DialogCreate:ServiceWeaponBuy(playerid)
{
	new string[MAX_STRING * 2],
		dialog_style = DIALOG_STYLE_INPUT,
		weaponid = GetPVarInt(playerid, "Buy_Weapon_ID");

	if (Weapons[ GetWeaponLookupID(weaponid) ][Weapon_IsFirearm] == 0) {
		format(string, sizeof(string), lang_texts[17][60], ReturnWeaponName(weaponid), GetWeaponCost(weaponid));
		dialog_style = DIALOG_STYLE_MSGBOX;
		SetPVarInt(playerid, "weapons_Buy_One_Bullet", 1);
	} else if (Weapons[ GetWeaponLookupID(weaponid) ][Weapon_IsFirearm] == 1) {
		format(string, sizeof(string),
			lang_texts[17][61],
			ReturnWeaponName(weaponid), GetWeaponCost(weaponid), GetPlayerMoney(playerid) / GetWeaponCost(weaponid)
		);
		strcat(string, lang_texts[17][66]);
	}

	Dialog_Open(playerid, Dialog:ServiceWeaponBuy, dialog_style, lang_texts[17][62], string, lang_texts[17][63], lang_texts[17][64]);
}

DialogResponse:ServiceWeaponBuy(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:ServiceWeapon);
		return 0;
	}

	new bullets = strval(inputtext);
	if (GetPVarInt(playerid, "weapons_Buy_One_Bullet") == 1) {
		bullets = 1;
		DeletePVar(playerid, "weapons_Buy_One_Bullet");
	}

	wshop_Buy(playerid, GetPVarInt(playerid, "Buy_Weapon_ID"), bullets);
	Dialog_Show(playerid, Dialog:ServiceWeapon);
	return 1;
}

stock wshop_Buy(playerid, weaponid, bullets)
{
	new string[MAX_STRING];

	if (weaponid < 0 || weaponid > 47) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[17][46]);
		return 0;
	}

	if (bullets < 1) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[17][47]);
		return 0;
	}

	if (weaponid > 0 && weaponid < 47) {
		new current_bullets = GetPlayerWeaponSlotBullet(playerid, GetWeaponSlot(weaponid));
		if (current_bullets >= MAX_BULLETS) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[17][65]);
			return 0;
		}

		if (current_bullets + bullets > MAX_BULLETS) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[17][65]);

			bullets = MAX_BULLETS - GetPlayerWeaponSlotBullet(playerid, GetWeaponSlot(weaponid));

			if (bullets < 1) {
				return 0;
			}
		}
	}

	if (!IsPlayerAtWeaponShop(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[17][48]);
		return 0;
	}

	if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
		format(string, sizeof(string), lang_texts[17][49], ReturnWeaponName(weaponid));
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	new purchasecost = GetWeaponCost(weaponid) * bullets;
	if (purchasecost < 1) {
		format(string, sizeof(string), lang_texts[17][50], ReturnWeaponName(weaponid));
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	if (GetPlayerMoney(playerid) < purchasecost) {
		format(string, sizeof(string), lang_texts[17][51], purchasecost);
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	GivePlayerMoney(playerid, -purchasecost, 1);

	switch (weaponid) {
		case 47: SetPlayerArmour(playerid, 100.0);
		case 37: GivePlayerWeapon(playerid, weaponid, bullets * 10, true); // fix for flamethrower
		default: GivePlayerWeapon(playerid, weaponid, bullets, true);
	}

	if (bullets > 1) {
		format(string, sizeof(string), lang_texts[17][52], bullets, ReturnWeaponName(weaponid), purchasecost);
	} else {
		format(string, sizeof(string), lang_texts[17][53], ReturnWeaponName(weaponid), purchasecost);
	}

	SendClientMessage(playerid, COLOUR_RED, string);
	SendClientMessage(playerid, COLOUR_RED, lang_texts[17][54]);
	return 1;
}

// узнаёт ид оружия по нажатию в диалоге
stock wshop_GetSelectedWeaponID(playerid, listitem)
{
	new k = 0;
	for (new weaponid = 1; weaponid < sizeof(Weapons); weaponid++) {
		if (IsPlayerAllowedWeapon(playerid, weaponid)) {
			k++;

			if (listitem+1 == k) {
				return weaponid;
			}
		}
	}
	return 0;
}

stock IsPlayerAtWeaponShop(playerid)
{
	for (new id = 0; id < sizeof(wshop_place); id++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, wshop_place[id][wshop_x], wshop_place[id][wshop_y], wshop_place[id][wshop_z])) {
			return 1;
		}
	}
	return 0;
}

stock wshop_GetActorFreeSlot()
{
	static slot;

	if (slot >= sizeof(wshop_actors)) {
		return -1;
	}
	
	return slot++;
}