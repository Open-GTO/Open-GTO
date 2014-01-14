/*

	Описание: Покупка скинов
	Автор: ziggi
	Дата: 13.06.2011

*/


#if defined _skinshop_included
	#endinput
#endif

#define _skinshop_included
#pragma library skinshop


new SkinShops[][CoordInfo] = {
	{207.6596, -100.7878, 1005.2578},
	{161.5244, -83.5924, 1001.8047},
	{206.8538, -129.5790, 1003.5078},
	{203.9252, -43.7523, 1001.8047}
};

stock sshop_OnGameModeInit()
{
	for (new id = 0; id < sizeof(SkinShops); id++) {
	    CreatePickup(1275, 23, SkinShops[id][Coord_X], SkinShops[id][Coord_Y], SkinShops[id][Coord_Z]);
		CreatePickupText("Одежда", SkinShops[id][Coord_X], SkinShops[id][Coord_Y], SkinShops[id][Coord_Z]);
	}
	
	clickText[sshop_left] = TextDrawCreate(288.000000, 372.000000, "<<");
	TextDrawAlignment(clickText[sshop_left], 2);
	TextDrawBackgroundColor(clickText[sshop_left], 255);
	TextDrawFont(clickText[sshop_left], 1);
	TextDrawLetterSize(clickText[sshop_left], 0.500000, 1.299999);
	TextDrawColor(clickText[sshop_left], 0xDDDDDDAA);
	TextDrawSetOutline(clickText[sshop_left], 0);
	TextDrawSetProportional(clickText[sshop_left], 0);
	TextDrawSetShadow(clickText[sshop_left], 1);
	TextDrawUseBox(clickText[sshop_left], 1);
	TextDrawBoxColor(clickText[sshop_left], 0x00000099);
	TextDrawTextSize(clickText[sshop_left], 12.000000, 50.000000);
	TextDrawSetSelectable(clickText[sshop_left], 1);

	clickText[sshop_right] = TextDrawCreate(343.000000, 372.000000, ">>");
	TextDrawAlignment(clickText[sshop_right], 2);
	TextDrawBackgroundColor(clickText[sshop_right], 255);
	TextDrawFont(clickText[sshop_right], 1);
	TextDrawLetterSize(clickText[sshop_right], 0.500000, 1.299999);
	TextDrawColor(clickText[sshop_right], 0xDDDDDDAA);
	TextDrawSetOutline(clickText[sshop_right], 0);
	TextDrawSetProportional(clickText[sshop_right], 0);
	TextDrawSetShadow(clickText[sshop_right], 1);
	TextDrawUseBox(clickText[sshop_right], 1);
	TextDrawBoxColor(clickText[sshop_right], 0x00000099);
	TextDrawTextSize(clickText[sshop_right], 12.000000, 50.000000);
	TextDrawSetSelectable(clickText[sshop_right], 1);

	clickText[sshop_buy] = TextDrawCreate(315.500000, 390.000000, "BUY");
	TextDrawAlignment(clickText[sshop_buy], 2);
	TextDrawBackgroundColor(clickText[sshop_buy], 255);
	TextDrawFont(clickText[sshop_buy], 1);
	TextDrawLetterSize(clickText[sshop_buy], 0.500000, 1.500000);
	TextDrawColor(clickText[sshop_buy], 0xDD0000AA);
	TextDrawSetOutline(clickText[sshop_buy], 0);
	TextDrawSetProportional(clickText[sshop_buy], 1);
	TextDrawSetShadow(clickText[sshop_buy], 1);
	TextDrawUseBox(clickText[sshop_buy], 1);
	TextDrawBoxColor(clickText[sshop_buy], 0x00000099);
	TextDrawTextSize(clickText[sshop_buy], 14.000000, 105.000000);
	TextDrawSetSelectable(clickText[sshop_buy], 1);

	/*clickText[sshop_list] = TextDrawCreate(288.000000, 409.000000, "List");
	TextDrawAlignment(clickText[sshop_list], 2);
	TextDrawBackgroundColor(clickText[sshop_list], 255);
	TextDrawFont(clickText[sshop_list], 1);
	TextDrawLetterSize(clickText[sshop_list], 0.420000, 0.899999);
	TextDrawColor(clickText[sshop_list], 0xDDDDDDAA);
	TextDrawSetOutline(clickText[sshop_list], 0);
	TextDrawSetProportional(clickText[sshop_list], 1);
	TextDrawSetShadow(clickText[sshop_list], 1);
	TextDrawUseBox(clickText[sshop_list], 1);
	TextDrawBoxColor(clickText[sshop_list], 0x00000099);
	TextDrawTextSize(clickText[sshop_list], 15.000000, 50.000000);
	TextDrawSetSelectable(clickText[sshop_list], 1);*/

	clickText[sshop_exit] = TextDrawCreate(343.000000, 409.000000, "Exit");
	TextDrawAlignment(clickText[sshop_exit], 2);
	TextDrawBackgroundColor(clickText[sshop_exit], 255);
	TextDrawFont(clickText[sshop_exit], 1);
	TextDrawLetterSize(clickText[sshop_exit], 0.420000, 0.899999);
	TextDrawColor(clickText[sshop_exit], 0xDDDDDDAA);
	TextDrawSetOutline(clickText[sshop_exit], 0);
	TextDrawSetProportional(clickText[sshop_exit], 1);
	TextDrawSetShadow(clickText[sshop_exit], 1);
	TextDrawUseBox(clickText[sshop_exit], 1);
	TextDrawBoxColor(clickText[sshop_exit], 0x00000099);
	TextDrawTextSize(clickText[sshop_exit], 10.000000, 50.000000);
	TextDrawSetSelectable(clickText[sshop_exit], 1);

	GameMSG(lang_texts[17][22]);
	return 1;
}

stock sshop_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused newkeys, oldkeys
	// если игрок не в примерочной
	if (GetPVarInt(playerid, "SkinShop_AtFittingRoom") != 1) {
		SkinShop_SetPlayerFittingRoom(playerid);
		SetPVarInt(playerid, "SkinShop_AtFittingRoom", 1);
		SetPVarInt(playerid, "SkinShop_Old", GetPlayerSkin(playerid));
		
		SkinShop_SelectSkin(playerid, SKINS_MINID, SKINS_MINID);
		sshop_ShowTextDraws(playerid);
		return 0;
	}
	return 1;
}

stock sshop_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext, listitem
	if (dialogid == SkinShop_Buy_DialogID) {
		SkinShop_Exit(playerid);

		if (!response) {
			SkinShop_DeletePVars(playerid);
			return 0;
		}
		
		SetPlayerSkin(playerid, GetPVarInt(playerid, "SkinShop_CurrentSkin"));
		SetPlayerSkinModel(playerid, GetPVarInt(playerid, "SkinShop_CurrentSkin"));
		SkinShop_DeletePVars(playerid);
		oGivePlayerMoney(playerid, -SKINS_COST);
		SendClientMessage(playerid, 0, lang_texts[17][25]);
	}
	return 1;
}

stock sshop_OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	new currskin = GetPVarInt(playerid, "SkinShop_CurrentSkin");
	if (clickedid == clickText[sshop_left]) {
		SkinShop_SelectSkin(playerid, currskin, currskin - 1);
		return 1;
	} else if (clickedid == clickText[sshop_right]) {
		SkinShop_SelectSkin(playerid, currskin, currskin + 1);
		return 1;
	} else if (clickedid == clickText[sshop_buy]) {
		if (GetPlayerMoney(playerid) < SKINS_COST) {
			new string[MAX_STRING];
			format(string, sizeof(string), lang_texts[17][23], SKINS_COST);
			SendClientMessage(playerid, 0, string);
			return 1;
		}
		SkinShop_ShowBuyDialog(playerid);
		return 1;
	} else if (clickedid == clickText[sshop_list]) {
		return 1;
	} else if (clickedid == clickText[sshop_exit] || clickedid == Text:INVALID_TEXT_DRAW) {
		SkinShop_Exit(playerid);
		SkinShop_DeletePVars(playerid);
		return 1;
	}
	return 0;
}

stock SkinShop_SelectSkin(playerid, oldskin, newskin) // включить выбор скинов
{
	if (newskin > SKINS_MAXID) {
		newskin = SKINS_MINID;
	} else if (newskin < SKINS_MINID) {
		newskin = SKINS_MAXID;
	}

	switch (newskin) {
		case SKINS_IGNORE: {
			newskin += (newskin > oldskin) ? 1 : -1;
		}
	}

	TogglePlayerControllable(playerid, 0);
	SetPlayerSkin(playerid, newskin);
	SetPVarInt(playerid, "SkinShop_CurrentSkin", newskin);
	return 1;
}

stock SkinShop_Exit(playerid)
{
	sshop_HideTextDraws(playerid);
	TogglePlayerControllable(playerid, 1);
	SetPlayerSkin(playerid, GetPVarInt(playerid, "SkinShop_Old"));
	SetPlayerInterior(playerid, GetPVarInt(playerid, "SkinShop_Interior"));
	oSetPlayerPos(playerid, GetPVarFloat(playerid, "SkinShop_Coords_X"), GetPVarFloat(playerid, "SkinShop_Coords_Y"), GetPVarFloat(playerid, "SkinShop_Coords_Z"));
	SetPlayerFacingAngle(playerid, GetPVarFloat(playerid, "SkinShop_Coords_A"));
	SetCameraBehindPlayer(playerid);
	return 1;
}

stock SkinShop_DeletePVars(playerid)
{
	DeletePVar(playerid, "SkinShop_Old");
	DeletePVar(playerid, "SkinShop_CurrentSkin");
	DeletePVar(playerid, "SkinShop_AtFittingRoom");
	DeletePVar(playerid, "SkinShop_Interior");
	DeletePVar(playerid, "SkinShop_PlayerAtSkinShop");
	DeletePVar(playerid, "SkinShop_Coords_X");
	DeletePVar(playerid, "SkinShop_Coords_Y");
	DeletePVar(playerid, "SkinShop_Coords_Z");
	DeletePVar(playerid, "SkinShop_Coords_A");
}

stock SkinShop_SetPlayerFittingRoom(playerid)
{
	new Float:pos[4];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, pos[3]);
	SetPVarInt(playerid, "SkinShop_Interior", GetPlayerInterior(playerid));
	SetPVarFloat(playerid, "SkinShop_Coords_X", pos[0]);
	SetPVarFloat(playerid, "SkinShop_Coords_Y", pos[1]);
	SetPVarFloat(playerid, "SkinShop_Coords_Z", pos[2]);
	SetPVarFloat(playerid, "SkinShop_Coords_A", pos[3]);
	
	oSetPlayerPos(playerid, 181.751159, -86.932769, 1002.023437);
	SetPlayerFacingAngle(playerid, 122.692977);
	SetPlayerInterior(playerid, 18);
	SetPlayerCameraPos(playerid, 179.138198, -88.452796, 1003.536376);
	SetPlayerCameraLookAt(playerid, 187.413238, -83.639259, 1000.646118);
}

stock IsPlayerAtSkinShop(playerid)
{
	if (GetPVarInt(playerid, "SkinShop_PlayerAtSkinShop") == 1) {
		return 1;
	}

	for (new id = 0; id < sizeof(SkinShops); id++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, SkinShops[id][Coord_X], SkinShops[id][Coord_Y], SkinShops[id][Coord_Z])) {
			SetPVarInt(playerid, "SkinShop_PlayerAtSkinShop", 1);
			return 1;
		}
	}
	return 0;
}

stock SkinShop_ShowBuyDialog(playerid)
{
	return ShowPlayerDialog(playerid, SkinShop_Buy_DialogID, DIALOG_STYLE_MSGBOX,
		lang_texts[17][26],
		lang_texts[17][27],
		lang_texts[17][28], lang_texts[17][29]
	);
}

stock sshop_ShowTextDraws(playerid)
{
	TextDrawShowForPlayer(playerid, clickText[sshop_left]);
	TextDrawShowForPlayer(playerid, clickText[sshop_right]);
	TextDrawShowForPlayer(playerid, clickText[sshop_buy]);
	TextDrawShowForPlayer(playerid, clickText[sshop_list]);
	TextDrawShowForPlayer(playerid, clickText[sshop_exit]);
	SelectTextDraw(playerid, 0xFFFFFFFF);
	return 1;
}

stock sshop_HideTextDraws(playerid)
{
	CancelSelectTextDraw(playerid);
	TextDrawHideForPlayer(playerid, clickText[sshop_left]);
	TextDrawHideForPlayer(playerid, clickText[sshop_right]);
	TextDrawHideForPlayer(playerid, clickText[sshop_buy]);
	TextDrawHideForPlayer(playerid, clickText[sshop_list]);
	TextDrawHideForPlayer(playerid, clickText[sshop_exit]);
	return 1;
}