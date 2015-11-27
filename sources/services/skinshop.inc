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

enum SkinShop_Info {
	Float:ss_x,
	Float:ss_y,
	Float:ss_z,
	Float:ss_select_x,
	Float:ss_select_y,
	Float:ss_select_z,
	Float:ss_select_a,
	ss_checkpoint
}

static SkinShops[][SkinShop_Info] = {
	{207.6596, -100.7878, 1005.2578, 217.5216, -98.4044,  1005.2578, 113.2594},
	{161.5244, -83.5924, 1001.8047,  181.1685, -87.4101,  1002.0234, 110.1505},
	{206.8538, -129.5790, 1003.5078, 199.9325, -127.3408, 1003.5152, 155.2709},
	{203.9252, -43.7523, 1001.8047,  214.4611, -41.2278,  1002.0234, 107.1854}
};

static
	skin_array[SKINS_MAXID + 1],
	skin_count = 0;

stock sshop_OnGameModeInit()
{
	for (new skinid = 0; skinid <= SKINS_MAXID; skinid++) {
		switch (skinid) {
			case SKINS_IGNORE: {

			}
			default: {
				skin_array[skin_count] = skinid;
				skin_count++;
			}
		}
	}

	for (new id = 0; id < sizeof(SkinShops); id++) {
		SkinShops[id][ss_checkpoint] = Checkpoint_Create(SkinShops[id][ss_x], SkinShops[id][ss_y], SkinShops[id][ss_z], 1.5, .streamdistance = 20.0);
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

	clickText[sshop_list] = TextDrawCreate(288.000000, 409.000000, "List");
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
	TextDrawSetSelectable(clickText[sshop_list], 1);

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

	Log_Game(_(SKINSHOP_INIT));
	return 1;
}

stock ss_OnPlayerEnterCheckpoint(playerid, cp)
{
	for (new id = 0; id < sizeof(SkinShops); id++) {
		if (cp == SkinShops[id][ss_checkpoint]) {
			SkinShop_SetPlayerFittingRoom(playerid, id);

			SetPVarInt(playerid, "SkinShop_ShopID", id);
			SetPVarInt(playerid, "SkinShop_PlayerAtSkinShop", 1);
			SetPVarInt(playerid, "SkinShop_Old", GetPlayerSkin(playerid));
			
			SkinShop_SelectSkin(playerid, SKINS_MINID, SKINS_MINID);
			sshop_ShowTextDraws(playerid);
			return 1;
		}
	}
	return 0;
}

DialogCreate:ServiceSkin(playerid)
{
	Dialog_Open(playerid, Dialog:ServiceSkin, DIALOG_STYLE_MSGBOX, _(SKINSHOP_DIALOG_HEADER), _(SKINSHOP_DIALOG_INFO_BUY), _(SKINSHOP_DIALOG_BUTTON_0), _(SKINSHOP_DIALOG_BUTTON_1));
}

DialogResponse:ServiceSkin(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 0;
	}

	SkinShop_Exit(playerid);
	
	player_SetSkin(playerid, GetPVarInt(playerid, "SkinShop_CurrentSkin"));
	GivePlayerMoney(playerid, -SKINS_COST);
	
	SkinShop_DeletePVars(playerid);

	SendClientMessage(playerid, 0, _(SKINSHOP_DIALOG_INFO_THANKS));
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
			format(string, sizeof(string), _(SKINSHOP_NOT_ENOUGH_MONEY), SKINS_COST);
			SendClientMessage(playerid, 0, string);
			return 1;
		}
		Dialog_Show(playerid, Dialog:ServiceSkin);
		return 1;
	} else if (clickedid == clickText[sshop_list]) {
		SetPVarInt(playerid, "SkinShop_IsInList", 1);
		sshop_HideTextDraws(playerid, false);
		ShowModelSelectionMenuEx(playerid, skin_array, skin_count, "Select skin", ms_skinshop, 16.0, 0.0, -55.0);
		return 1;
	} else if ((clickedid == clickText[sshop_exit] || clickedid == Text:INVALID_TEXT_DRAW) && GetPVarInt(playerid, "SkinShop_IsInList") == 0) {
		SkinShop_Exit(playerid);
		SkinShop_DeletePVars(playerid);
		return 1;
	}
	return 0;
}

stock sshop_OnPlayerModelSelectionEx(playerid, response, extraid, modelid)
{
	#pragma unused extraid
	DeletePVar(playerid, "SkinShop_IsInList");
	sshop_ShowTextDraws(playerid);

	if (response) {
		SkinShop_SelectSkin(playerid, modelid, modelid);
	}
	return 1;
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

	new shopid = GetPVarInt(playerid, "SkinShop_ShopID");
	SetPlayerPos(playerid, SkinShops[shopid][ss_select_x], SkinShops[shopid][ss_select_y], SkinShops[shopid][ss_select_z]);
	SetPlayerFacingAngle(playerid, SkinShops[shopid][ss_select_a]);

	SetCameraBehindPlayer(playerid);
	return 1;
}

stock SkinShop_DeletePVars(playerid)
{
	DeletePVar(playerid, "SkinShop_ShopID");
	DeletePVar(playerid, "SkinShop_Old");
	DeletePVar(playerid, "SkinShop_CurrentSkin");
	DeletePVar(playerid, "SkinShop_PlayerAtSkinShop");
}

stock SkinShop_SetPlayerFittingRoom(playerid, shopid)
{
	new Float:pos[4];
	pos[0] = SkinShops[shopid][ss_select_x];
	pos[1] = SkinShops[shopid][ss_select_y];
	pos[2] = SkinShops[shopid][ss_select_z];
	pos[3] = SkinShops[shopid][ss_select_a];
	SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	SetPlayerFacingAngle(playerid, pos[3]);

	new Float:camera_pos[3];
	camera_pos[2] = SkinShops[shopid][ss_select_z] + 1.0;

	GetCoordsBefore(pos[0], pos[1], pos[3], 2.0, camera_pos[0], camera_pos[1]);
	SetPlayerCameraPos(playerid, camera_pos[0], camera_pos[1], camera_pos[2]);
	SetPlayerCameraLookAt(playerid, pos[0], pos[1], pos[2] + 0.5);
}

stock IsPlayerAtSkinShop(playerid)
{
	if (GetPVarInt(playerid, "SkinShop_PlayerAtSkinShop") == 1) {
		return 1;
	}

	for (new id = 0; id < sizeof(SkinShops); id++) {
		if (IsPlayerInRangeOfPoint(playerid, 2, SkinShops[id][ss_x], SkinShops[id][ss_y], SkinShops[id][ss_z])) {
			SetPVarInt(playerid, "SkinShop_PlayerAtSkinShop", 1);
			return 1;
		}
	}
	return 0;
}

stock sshop_ShowTextDraws(playerid, bool:set_select = true)
{
	TextDrawShowForPlayer(playerid, clickText[sshop_left]);
	TextDrawShowForPlayer(playerid, clickText[sshop_right]);
	TextDrawShowForPlayer(playerid, clickText[sshop_buy]);
	TextDrawShowForPlayer(playerid, clickText[sshop_list]);
	TextDrawShowForPlayer(playerid, clickText[sshop_exit]);
	
	if (set_select) {
		SelectTextDraw(playerid, 0xFFFFFFFF);
	}
	return 1;
}

stock sshop_HideTextDraws(playerid, bool:cancel_select = true)
{
	TextDrawHideForPlayer(playerid, clickText[sshop_left]);
	TextDrawHideForPlayer(playerid, clickText[sshop_right]);
	TextDrawHideForPlayer(playerid, clickText[sshop_buy]);
	TextDrawHideForPlayer(playerid, clickText[sshop_list]);
	TextDrawHideForPlayer(playerid, clickText[sshop_exit]);

	if (cancel_select) {
		CancelSelectTextDraw(playerid);
	}
	return 1;
}

stock IsSkinValid(skinid)
{
	switch (skinid) {
		case SKINS_IGNORE: return 0;
	}
	return 1;
}
