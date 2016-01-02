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
	Log_Game(_(WEAPON_INIT));
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
				Log_Debug("weaponshop.inc: Free slot not found. Increase MAX_WSHOP_ACTORS value.");
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
	string = _(WEAPON_DIALOG_LIST_HEADER);

	for (new weaponid = 1; weaponid < sizeof(Weapons); weaponid++) {
		if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
			continue;
		}
		format(string, sizeof(string), _(WEAPON_DIALOG_LIST_ITEM), string, ReturnWeaponName(weaponid), GetWeaponCost(weaponid));
	}

	Dialog_Open(playerid, Dialog:ServiceWeapon, DIALOG_STYLE_TABLIST_HEADERS, _(WEAPON_DIALOG_HEADER), string, _(WEAPON_DIALOG_BUTTON_0), _(WEAPON_DIALOG_BUTTON_1));
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
	new
		string[MAX_STRING * 2],
		dialog_style = DIALOG_STYLE_INPUT,
		weaponid = GetPVarInt(playerid, "Buy_Weapon_ID");

	new max_ammo = GetWeaponMaxAmmo(weaponid);
	if (max_ammo == 0) {
		return;
	}

	if (max_ammo == 1) {
		format(string, sizeof(string), _(WEAPON_DIALOG_WEAPON_ONE), ReturnWeaponName(weaponid), GetWeaponCost(weaponid));
		dialog_style = DIALOG_STYLE_MSGBOX;
	} else {
		new
			max_bullets = max_ammo - GetPlayerWeaponAmmo(playerid, weaponid),
			max_bullets_money = GetPlayerMoney(playerid) / GetWeaponCost(weaponid);

		if (max_bullets_money < max_bullets) {
			max_bullets = max_bullets_money;
		}

		format(string, sizeof(string),
			_(WEAPON_DIALOG_WEAPON),
			ReturnWeaponName(weaponid), GetWeaponCost(weaponid), max_bullets, Declension_GetAmmo(max_bullets)
		);
		strcat(string, _(WEAPON_DIALOG_INFORMATION_TEXT_AMMO));
	}

	Dialog_Open(playerid, Dialog:ServiceWeaponBuy, dialog_style, _(WEAPON_DIALOG_WEAPON_BUY_HEADER), string, _(WEAPON_DIALOG_WEAPON_BUY_BUTTON_0), _(WEAPON_DIALOG_WEAPON_BUY_BUTTON_1));
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
		Dialog_MessageEx(playerid, Dialog:WShopReturnBuyMenu, _(WEAPON_DIALOG_WEAPON_BUY_HEADER), info, _(WEAPON_DIALOG_WEAPON_BUY_BUTTON_1), _(WEAPON_DIALOG_BUTTON_1));
	} else {
		Dialog_MessageEx(playerid, Dialog:WShopReturnMainMenu, _(WEAPON_DIALOG_WEAPON_BUY_HEADER), info, _(WEAPON_DIALOG_WEAPON_BUY_BUTTON_1), _(WEAPON_DIALOG_BUTTON_1));
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
	if (!IsPlayerAtWeaponShop(playerid)) {
		wshop_Message(playerid, _(WEAPON_NOT_IN_SHOP));
		return 0;
	}

	new string[MAX_STRING];

	if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
		format(string, sizeof(string), _(WEAPON_BAD_WEAPON), ReturnWeaponName(weaponid));
		wshop_Message(playerid, string, false);
		return 0;
	}

	if (bullets < 1) {
		wshop_Message(playerid, _(WEAPON_BAD_AMMO_COUNT));
		return 0;
	}

	new
		max_ammo = GetWeaponMaxAmmo(weaponid),
		current_bullets = GetPlayerWeaponAmmo(playerid, weaponid);
	
	if (current_bullets >= max_ammo) {
		wshop_Message(playerid, _(WEAPON_MAX_AMMO_COUNT));
		return 0;
	}

	if (current_bullets + bullets > max_ammo) {
		wshop_Message(playerid, _(WEAPON_BAD_AMMO_COUNT));
		return 0;
	}

	new purchasecost = GetWeaponCost(weaponid) * bullets;

	if (GetPlayerMoney(playerid) < purchasecost) {
		format(string, sizeof(string), _(WEAPON_NOT_ENOUGH_MONEY), purchasecost);
		wshop_Message(playerid, string, !IsWeaponHandToHand(weaponid));
		return 0;
	}

	GivePlayerMoney(playerid, -purchasecost);

	switch (weaponid) {
		case 47: SetPlayerArmour(playerid, 100.0);
		case 37: GivePlayerWeapon(playerid, weaponid, bullets * 10, true); // fix for flamethrower
		default: GivePlayerWeapon(playerid, weaponid, bullets, true);
	}

	if (!IsWeaponHandToHand(weaponid)) {
		format(string, sizeof(string), _(WEAPON_BUYED), bullets, ReturnWeaponName(weaponid), purchasecost);
	} else {
		format(string, sizeof(string), _(WEAPON_BUYED_ONE), ReturnWeaponName(weaponid), purchasecost);
	}

	wshop_Message(playerid, string, false);
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
