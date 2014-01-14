/*

	Описание: Покупка оружия
	Автор: ziggi
	Дата: 14.01.2014

*/


#if defined _weaponshop_included
	#endinput
#endif

#define _weaponshop_included
#pragma library weaponshop


new weapon_Shops[][CoordInfo] = {
	{295.3583, -80.1250, 1001.5156}, // LVammunation1
	{290.0941, -109.2533, 1001.5156}, // LSAmmunation2
	{296.1441, -37.7606, 1001.5156}, // LSAmmunation3
	{312.5029, -165.4625, 999.6010} // север карты
};


stock wshop_OnGameModeInit()
{
	for (new id = 0; id < sizeof(weapon_Shops); id++) {
		CreatePickup(1247, 1, weapon_Shops[id][Coord_X], weapon_Shops[id][Coord_Y], weapon_Shops[id][Coord_Z]);
		CreatePickupText("Оружие", weapon_Shops[id][Coord_X], weapon_Shops[id][Coord_Y], weapon_Shops[id][Coord_Z]);
	}
	GameMSG("SERVER: Weapon shop module init");
	return 1;
}

stock wshop_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid) {
		// Выбираем оружие из списка
		case wshop_Select_DialogID: {
			if (!response) {
				return 0;
			}

			new weaponid = wshop_GetSelectedWeaponID(playerid, listitem);
			SetPVarInt(playerid, "Buy_Weapon_ID", weaponid);
			wshop_ShowBuyDialog(playerid, weaponid);
			return 1;
		}
		// Покупаем, вводим патроны
		case wshop_Buy_DialogID: {
			if (!response) {
				wshop_ShowSelectDialog(playerid);
				return 0;
			}

			new bullets = strval(inputtext);
			if (GetPVarInt(playerid, "weapons_Buy_One_Bullet") == 1) {
				bullets = 1;
				DeletePVar(playerid, "weapons_Buy_One_Bullet");
			}

			wshop_Buy(playerid, GetPVarInt(playerid, "Buy_Weapon_ID"), bullets);
			wshop_ShowSelectDialog(playerid);
			return 1;
		}
	}
	return 1;
}

stock wshop_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	wshop_ShowSelectDialog(playerid);
	return 1;
}

stock wshop_Buy(playerid, weaponid, bullets)
{
	new string[MAX_STRING];

	if (weaponid < 0 || weaponid > 47) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[16][7]);
		return 0;
	}

	if (bullets < 1) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[16][8]);
		return 0;
	}

	if (weaponid > 0 && weaponid < 47) {
		new current_bullets = GetPlayerWeaponSlotBullet(playerid, GetWeaponSlot(weaponid));
		if (current_bullets >= MAX_BULLETS) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[16][1]);
			return 0;
		}

		if (current_bullets + bullets > MAX_BULLETS) {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[16][1]);

			bullets = MAX_BULLETS - GetPlayerWeaponSlotBullet(playerid, GetWeaponSlot(weaponid));

			if (bullets < 1) {
				return 0;
			}
		}
	}

	if (IsPlayerAtWeaponShop(playerid) == -1) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[16][9]);
		return 0;
	}

	if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
		format(string, sizeof(string), lang_texts[16][10], oGetWeaponName(weaponid));
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	new purchasecost = GetWeaponCost(weaponid) * bullets;
	if (purchasecost < 1) {
		format(string, sizeof(string), lang_texts[16][11], oGetWeaponName(weaponid));
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	if (oGetPlayerMoney(playerid) < purchasecost) {
		format(string, sizeof(string), lang_texts[16][12], purchasecost);
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}

	oGivePlayerMoney(playerid,-purchasecost, 1);

	switch (weaponid) {
		case 47: oSetPlayerArmour(playerid, 100.0);
		case 37: GivePlayerOwnedWeapon(playerid, weaponid, bullets * 10); // фикс для количества патрон огнемёта
		default: GivePlayerOwnedWeapon(playerid, weaponid, bullets);
	}

	if (bullets > 1) {
		format(string, sizeof(string), lang_texts[16][13], bullets, oGetWeaponName(weaponid), purchasecost);
	} else {
		format(string, sizeof(string), lang_texts[16][14], oGetWeaponName(weaponid), purchasecost);
	}

	SendClientMessage(playerid, COLOUR_RED, string);
	SendClientMessage(playerid, COLOUR_RED, lang_texts[16][15]);
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

stock wshop_ShowSelectDialog(playerid)
{
	new string[ sizeof(Weapons) * (MAX_NAME + 12 + 4) ];

	for (new weaponid = 1; weaponid < sizeof(Weapons); weaponid++) {
		if (!IsPlayerAllowedWeapon(playerid, weaponid)) {
			continue;
		}
		format(string, sizeof(string), "%s%s, цена $%d\n", string, oGetWeaponName(weaponid), GetWeaponCost(weaponid));
	}

	return ShowPlayerDialog(playerid, wshop_Select_DialogID, DIALOG_STYLE_LIST,
		"Меню оружия",
		string,
		"Выбрать", "Выход"
	);
}

stock wshop_ShowBuyDialog(playerid, weaponid)
{
	new string[MAX_STRING],
		dialog_style = DIALOG_STYLE_INPUT;

	if (Weapons[ GetWeaponLookupID(weaponid) ][Weapon_IsFirearm] == 0) {
		format(string, sizeof(string), "Оружие: %s\nЦена: $%d", oGetWeaponName(weaponid), GetWeaponCost(weaponid));
		dialog_style = DIALOG_STYLE_MSGBOX;
		SetPVarInt(playerid, "weapons_Buy_One_Bullet", 1);
	} else if (Weapons[ GetWeaponLookupID(weaponid) ][Weapon_IsFirearm] == 1) {
		format(string, sizeof(string),
			"Оружие: %s\nЦена за 1 патрон: $%d\nВаших денег хватит на %d патрон\n\nВведите количество патрон:",
			oGetWeaponName(weaponid), GetWeaponCost(weaponid), oGetPlayerMoney(playerid) / GetWeaponCost(weaponid)
		);
	}

	return ShowPlayerDialog(playerid, wshop_Buy_DialogID, dialog_style,
		"Покупка оружия",
		string,
		"Купить", "Назад"
	);
}

stock IsPlayerAtWeaponShop(playerid)
{
	for (new id = 0; id < sizeof(weapon_Shops); id++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, weapon_Shops[id][Coord_X], weapon_Shops[id][Coord_Y], weapon_Shops[id][Coord_Z])) {
			return id;
		}
	}
	return -1;
}