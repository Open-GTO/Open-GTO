/*
	Created: 01.02.11
	Aurthor: ZiGGi
*/

#if defined _fastfood_included
	#endinput
#endif

#define _fastfood_included
#pragma library fastfood


new FastFoods[][CoordInfo] = {
	{373.4858, -119.4608, 1001.4922}, // LS pizza
	{377.4426, -67.5379, 1001.5078}, // LS burger
	{369.3667, -6.3469, 1001.8516}, // LS chicken
	{-784.5101,505.0186,1371.7422}, // Liberty City Restoran
	{379.4050,-190.5775,1000.6328} // donuts
};

enum FoodInfo {
	food_name[MAX_NAME],
	food_cost,
	Float:food_hp
}
new Food[][FoodInfo] = {
	{"√нилой корень", 10, 10.0},
	{"ѕеченьки с молоком", 15, 25.0},
	{"ѕетрушка с хреном", 25, 50.0},
	{"ƒва €йца всм€тку", 35, 80.0},
	{"јмлет с колбасой", 45, 100.0}
};

stock fastfood_OnGameModeInit()
{
	for (new ff_id = 0; ff_id < sizeof(FastFoods); ff_id++) {
	    CreatePickup(2703 , 23, FastFoods[ff_id][Coord_X], FastFoods[ff_id][Coord_Y], FastFoods[ff_id][Coord_Z]);
		CreatePickupText("‘астфуд", FastFoods[ff_id][Coord_X], FastFoods[ff_id][Coord_Y], FastFoods[ff_id][Coord_Z]);
	}
	GameMSG(lang_texts[17][1]);
	return 1;
}

stock fastfood_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	Dialog_Show(playerid, Dialog:ServiceFastfood);
	return 1;
}

DialogCreate:ServiceFastfood(playerid)
{
	new string[MAX_STRING * sizeof(Food)];

	for (new i = 0; i < sizeof(Food); i++) {
		format(string, sizeof(string),
			lang_texts[17][2],
			string, Food[i][food_name], Food[i][food_cost], Food[i][food_hp]
		);
	}

	Dialog_Open(playerid, Dialog:ServiceFastfood, DIALOG_STYLE_LIST, lang_texts[17][3], string, lang_texts[17][4], lang_texts[17][5]);
}

DialogResponse:ServiceFastfood(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	if (oGetPlayerMoney(playerid) < Food[listitem][food_cost]) {
		Dialog_Message(playerid, lang_texts[17][6], lang_texts[17][7], lang_texts[17][8]);
		return 1;
	}

	oGivePlayerMoney(playerid,-Food[listitem][food_cost], 1);
	ApplyAnimation(playerid, "FOOD", "EAT_Burger", 4.1, 0, 1, 1, 1, 1, 1);

	new Float:max_health;
	GetMaxHealth(playerid, max_health);

	new Float:health;
	oGetPlayerHealth(playerid, health);

	if (Food[listitem][food_hp] + health > max_health) {
		oSetPlayerHealth(playerid, max_health);
	} else {
		oSetPlayerHealth(playerid, health + Food[listitem][food_hp]);
	}

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[17][9], Food[listitem][food_name], Food[listitem][food_cost], Food[listitem][food_hp]);
	Dialog_Message(playerid, lang_texts[17][6], string, lang_texts[17][8]);
	return 1;
}

stock IsPlayerAtFastFood(playerid)
{
	for (new ff_id = 0; ff_id < sizeof(FastFoods); ff_id++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, FastFoods[ff_id][Coord_X], FastFoods[ff_id][Coord_Y], FastFoods[ff_id][Coord_Z])) {
			return 1;
		}
	}
	return 0;
}
