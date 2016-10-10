/*

	About: Weapon shop
	Author: ziggi
	Date: 14.01.2014

*/


#if defined _weaponshop_included
	#endinput
#endif

#define _weaponshop_included

#define MAX_WSHOP_ACTORS 11

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
	{ENTEREXIT_TYPE_AMMUNATION_1, 296.1441, -37.7606, 1001.5156,   179, 296.1232, -40.2155, 1001.5156, 359.9678},
	{ENTEREXIT_TYPE_AMMUNATION_2, 295.3583, -80.1250, 1001.5156,   179, 295.3518, -82.5274, 1001.5156, 356.2078},
	{ENTEREXIT_TYPE_AMMUNATION_3, 290.0941, -109.2533, 1001.5156,  179, 290.0896, -111.5135, 1001.5156, 356.8344},
	{ENTEREXIT_TYPE_AMMUNATION_4, 313.9999, -133.8288, 999.6016,   179, 316.1227, -133.7110, 999.6016, 97.4153},
	{ENTEREXIT_TYPE_AMMUNATION_5, 312.5029, -165.4625, 999.6010,   179, 312.5927, -167.7639, 999.5938, 3.1246}
};

static wshop_actors[MAX_WSHOP_ACTORS];


wshop_OnGameModeInit()
{
	for (new id = 0; id < sizeof(wshop_place); id++) {
		wshop_place[id][wshop_checkpoint] = CreateDynamicCP(wshop_place[id][wshop_x], wshop_place[id][wshop_y], wshop_place[id][wshop_z], 1.5, .streamdistance = 20.0);
	}
	Log_Init("services", "Weapon shop module init.");
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
				Log(systemlog, DEBUG, "weaponshop.inc: Free slot not found. Increase MAX_WSHOP_ACTORS value.");
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
	static
		Lang:lang,
		string[ (MAX_WEAPONS + 1) * (MAX_WEAPON_NAME + 12 + 4) ];

	lang = Lang_GetPlayerLang(playerid);
	Lang_GetText(lang, "WEAPON_DIALOG_LIST_HEADER", string);

	for (new weaponid = 1; weaponid < MAX_WEAPONS; weaponid++) {
		if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
			continue;
		}
		Lang_GetText(lang, "WEAPON_DIALOG_LIST_ITEM", string, _, string, ret_GetPlayerWeaponName(playerid, weaponid), GetWeaponCost(weaponid));
	}

	Dialog_Open(playerid, Dialog:ServiceWeapon, DIALOG_STYLE_TABLIST_HEADERS,
	            "WEAPON_DIALOG_HEADER",
	            string,
	            "WEAPON_DIALOG_BUTTON_0", "WEAPON_DIALOG_BUTTON_1",
	            MDIALOG_NOTVAR_INFO);
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
	new weaponid = GetPVarInt(playerid, "Buy_Weapon_ID");
	new max_ammo = GetWeaponMaxAmmo(weaponid);
	if (max_ammo == 0) {
		return;
	}

	if (max_ammo == 1) {
		Dialog_Open(playerid, Dialog:ServiceWeaponBuy, DIALOG_STYLE_MSGBOX,
		            "WEAPON_DIALOG_WEAPON_BUY_HEADER",
		            "WEAPON_DIALOG_WEAPON_ONE",
		            "WEAPON_DIALOG_WEAPON_BUY_BUTTON_0", "WEAPON_DIALOG_WEAPON_BUY_BUTTON_1",
		            MDIALOG_NOTVAR_NONE,
		            ret_GetPlayerWeaponName(playerid, weaponid),
		            GetWeaponCost(weaponid));
	} else {
		new
			max_bullets = max_ammo - GetPlayerWeaponAmmo(playerid, weaponid),
			max_bullets_money = GetPlayerMoney(playerid) / GetWeaponCost(weaponid);

		if (max_bullets_money < max_bullets) {
			max_bullets = max_bullets_money;
		}

		Dialog_Open(playerid, Dialog:ServiceWeaponBuy, DIALOG_STYLE_INPUT,
		            "WEAPON_DIALOG_WEAPON_BUY_HEADER",
		            "WEAPON_DIALOG_WEAPON",
		            "WEAPON_DIALOG_WEAPON_BUY_BUTTON_0", "WEAPON_DIALOG_WEAPON_BUY_BUTTON_1",
		            MDIALOG_NOTVAR_NONE,
		            ret_GetPlayerWeaponName(playerid, weaponid),
		            GetWeaponCost(weaponid),
		            max_bullets,
		            Declension_ReturnAmmo(playerid, max_bullets));
	}
}

DialogResponse:ServiceWeaponBuy(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:ServiceWeapon);
		return 0;
	}

	new
		weaponid = GetPVarInt(playerid, "Buy_Weapon_ID"),
		bullets;

	if (IsWeaponHandToHand(weaponid)) {
		bullets = 1;
	} else {
		bullets = strval(inputtext);
	}

	wshop_Buy(playerid, weaponid, bullets);
	return 1;
}

stock wshop_Message(playerid, info[], bool:is_buy_menu = true)
{
	if (is_buy_menu) {
		Dialog_MessageEx(playerid, Dialog:WShopReturnBuyMenu,
		                 "WEAPON_DIALOG_WEAPON_BUY_HEADER",
		                 info,
		                 "WEAPON_DIALOG_WEAPON_BUY_BUTTON_1", "WEAPON_DIALOG_BUTTON_1",
		                 MDIALOG_NOTVAR_INFO);
	} else {
		Dialog_MessageEx(playerid, Dialog:WShopReturnMainMenu,
		                 "WEAPON_DIALOG_WEAPON_BUY_HEADER",
		                 info,
		                 "WEAPON_DIALOG_WEAPON_BUY_BUTTON_1", "WEAPON_DIALOG_BUTTON_1",
		                 MDIALOG_NOTVAR_INFO);
	}
	return 1;
}

DialogResponse:WShopReturnBuyMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:ServiceWeaponBuy);
	}
	return 1;
}

DialogResponse:WShopReturnMainMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:ServiceWeapon);
	}
	return 1;
}

stock wshop_Buy(playerid, weaponid, bullets)
{
	new string[MAX_LANG_VALUE_STRING];

	if (!IsPlayerAtWeaponShop(playerid)) {
		Lang_GetPlayerText(playerid, "WEAPON_NOT_IN_SHOP", string);
		wshop_Message(playerid, string);
		return 0;
	}

	if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
		Lang_GetPlayerText(playerid, "WEAPON_BAD_WEAPON", string, _, ret_GetPlayerWeaponName(playerid, weaponid));
		wshop_Message(playerid, string, false);
		return 0;
	}

	if (bullets < 1) {
		Lang_GetPlayerText(playerid, "WEAPON_BAD_AMMO_COUNT", string);
		wshop_Message(playerid, string);
		return 0;
	}

	new
		max_ammo = GetWeaponMaxAmmo(weaponid),
		current_bullets = GetPlayerWeaponAmmo(playerid, weaponid);

	if (current_bullets >= max_ammo) {
		Lang_GetPlayerText(playerid, "WEAPON_MAX_AMMO_COUNT", string);
		wshop_Message(playerid, string);
		return 0;
	}

	if (current_bullets + bullets > max_ammo) {
		Lang_GetPlayerText(playerid, "WEAPON_BAD_AMMO_COUNT", string);
		wshop_Message(playerid, string);
		return 0;
	}

	new purchasecost = GetWeaponCost(weaponid) * bullets;

	if (GetPlayerMoney(playerid) < purchasecost) {
		Lang_GetPlayerText(playerid, "WEAPON_NOT_ENOUGH_MONEY", string, _, purchasecost);
		wshop_Message(playerid, string, !IsWeaponHandToHand(weaponid));
		return 0;
	}

	GivePlayerMoney(playerid, -purchasecost);

	switch (weaponid) {
		case WEAPON_ARMOUR: {
			SetPlayerArmour(playerid, 100.0);
		}
		case WEAPON_FLAMETHROWER: {
			// fix for flamethrower
			GivePlayerWeapon(playerid, weaponid, bullets * 10, true);
		}
		default: {
			GivePlayerWeapon(playerid, weaponid, bullets, true);
		}
	}

	if (!IsWeaponHandToHand(weaponid)) {
		Lang_GetPlayerText(playerid, "WEAPON_BUYED", string, _, bullets, ret_GetPlayerWeaponName(playerid, weaponid), purchasecost);
	} else {
		Lang_GetPlayerText(playerid, "WEAPON_BUYED_ONE", string, _, ret_GetPlayerWeaponName(playerid, weaponid), purchasecost);
	}

	wshop_Message(playerid, string, false);
	return 1;
}

// узнаёт ид оружия по нажатию в диалоге
stock wshop_GetSelectedWeaponID(playerid, listitem)
{
	new k = 0;
	for (new weaponid = 1; weaponid < MAX_WEAPONS; weaponid++) {
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
