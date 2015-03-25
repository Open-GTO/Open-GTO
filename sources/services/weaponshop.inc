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


enum weapon_Shop_Info {
	Float:wshop_x,
	Float:wshop_y,
	Float:wshop_z,
	wshop_checkpoint,
}

new weapon_Shops[][weapon_Shop_Info] = {
	{295.3583, -80.1250, 1001.5156}, // LVammunation1
	{290.0941, -109.2533, 1001.5156}, // LSAmmunation2
	{296.1441, -37.7606, 1001.5156}, // LSAmmunation3
	{312.5029, -165.4625, 999.6010} // north
};


stock wshop_OnGameModeInit()
{
	for (new id = 0; id < sizeof(weapon_Shops); id++) {
		weapon_Shops[id][wshop_checkpoint] = Checkpoint_Create(weapon_Shops[id][wshop_x], weapon_Shops[id][wshop_y], weapon_Shops[id][wshop_z], 1.5, .streamdistance = 20.0);
	}
	Log_Game(lang_texts[17][44]);
	return 1;
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
		format(string, sizeof(string), lang_texts[17][56], string, oGetWeaponName(weaponid), GetWeaponCost(weaponid));
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
	new string[MAX_STRING],
		dialog_style = DIALOG_STYLE_INPUT,
		weaponid = GetPVarInt(playerid, "Buy_Weapon_ID");

	if (Weapons[ GetWeaponLookupID(weaponid) ][Weapon_IsFirearm] == 0) {
		format(string, sizeof(string), lang_texts[17][60], oGetWeaponName(weaponid), GetWeaponCost(weaponid));
		dialog_style = DIALOG_STYLE_MSGBOX;
		SetPVarInt(playerid, "weapons_Buy_One_Bullet", 1);
	} else if (Weapons[ GetWeaponLookupID(weaponid) ][Weapon_IsFirearm] == 1) {
		format(string, sizeof(string),
			lang_texts[17][61],
			oGetWeaponName(weaponid), GetWeaponCost(weaponid), oGetPlayerMoney(playerid) / GetWeaponCost(weaponid)
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

stock wshop_OnPlayerEnterCheckpoint(playerid, cp)
{
	for (new id = 0; id < sizeof(weapon_Shops); id++) {
		if (cp == weapon_Shops[id][wshop_checkpoint]) {
			Dialog_Show(playerid, Dialog:ServiceWeapon);
			return 1;
		}
	}
	return 0;
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
		format(string, sizeof(string), lang_texts[17][49], oGetWeaponName(weaponid));
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	new purchasecost = GetWeaponCost(weaponid) * bullets;
	if (purchasecost < 1) {
		format(string, sizeof(string), lang_texts[17][50], oGetWeaponName(weaponid));
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	if (oGetPlayerMoney(playerid) < purchasecost) {
		format(string, sizeof(string), lang_texts[17][51], purchasecost);
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	oGivePlayerMoney(playerid, -purchasecost, 1);

	switch (weaponid) {
		case 47: oSetPlayerArmour(playerid, 100.0);
		case 37: GivePlayerWeaponCheck(playerid, weaponid, bullets * 10); // fix for flamethrower
		default: GivePlayerWeaponCheck(playerid, weaponid, bullets);
	}

	if (bullets > 1) {
		format(string, sizeof(string), lang_texts[17][52], bullets, oGetWeaponName(weaponid), purchasecost);
	} else {
		format(string, sizeof(string), lang_texts[17][53], oGetWeaponName(weaponid), purchasecost);
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
	for (new id = 0; id < sizeof(weapon_Shops); id++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, weapon_Shops[id][wshop_x], weapon_Shops[id][wshop_y], weapon_Shops[id][wshop_z])) {
			return 1;
		}
	}
	return 0;
}