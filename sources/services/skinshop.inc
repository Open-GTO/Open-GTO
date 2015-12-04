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

stock sshop_OnGameModeInit()
{
	for (new id = 0; id < sizeof(SkinShops); id++) {
		SkinShops[id][ss_checkpoint] = Checkpoint_Create(SkinShops[id][ss_x], SkinShops[id][ss_y], SkinShops[id][ss_z], 1.5, .streamdistance = 20.0);
	}

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

			SkinSelect_Start(playerid, SkinSelect:SkinShop);
			return 1;
		}
	}
	return 0;
}

SkinSelectResponse:SkinShop(playerid, SS_Response:type, oldskin, newskin)
{
	if (type == SS_Response:SS_Response_Stop) {
		SkinShop_Exit(playerid);
		SkinShop_DeletePVars(playerid);
	} else if (type == SS_Response:SS_Response_Select) {
		if (GetPlayerMoney(playerid) < SKINS_COST) {
			new string[MAX_STRING];
			format(string, sizeof(string), _(SKINSHOP_NOT_ENOUGH_MONEY), SKINS_COST);
			SendClientMessage(playerid, 0, string);
		} else {
			Dialog_Show(playerid, Dialog:ServiceSkin);
		}
	}
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

	new newskin = SkinSelect_GetCurrentSkin(playerid);

	SkinSelect_Stop(playerid);
	SkinShop_Exit(playerid);
	
	player_SetSkin(playerid, newskin);
	GivePlayerMoney(playerid, -SKINS_COST);
	
	SkinShop_DeletePVars(playerid);

	SendClientMessage(playerid, 0, _(SKINSHOP_DIALOG_INFO_THANKS));
	return 1;
}

stock SkinShop_Exit(playerid)
{
	TogglePlayerControllable(playerid, 1);
	SetPlayerSkin(playerid, SkinSelect_GetOldSkin(playerid));

	new shopid = GetPVarInt(playerid, "SkinShop_ShopID");
	SetPlayerPos(playerid, SkinShops[shopid][ss_select_x], SkinShops[shopid][ss_select_y], SkinShops[shopid][ss_select_z]);
	SetPlayerFacingAngle(playerid, SkinShops[shopid][ss_select_a]);

	SetCameraBehindPlayer(playerid);
	return 1;
}

stock SkinShop_DeletePVars(playerid)
{
	DeletePVar(playerid, "SkinShop_ShopID");
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
