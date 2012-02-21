/*

	Описание: Покупка скинов, с разделением их по полу
	Автор: ZiGGi
	Дата: 13.06.2011

*/


#if defined _skinshop_included
	#endinput
#endif

#define _skinshop_included
#pragma library skinshop


new SkinShops[][CoordInfo] = {
	{207.6596,-100.7878,1005.2578},
	{161.5244,-83.5924,1001.8047},
	{206.8538,-129.5790,1003.5078},
	{203.9252,-43.7523,1001.8047}
};

new MenSkins[] = {
	2,47,48,50,58,60,68,72,73,80,81,82,83,95,100,101,102,103,104,108,
	109,110,121,122,123,135,142,143,144,146,153,154,155,156,158,159,
	160,161,170,179,180,189,202,203,204,258,259,260,293,295,296,297
};

new WomanSkins[] = {
	55,56,63,69,76,85,91,93,131,141,148,150,151,152,157,169,172,190,
	192,193,194,195,198,201,214,216,219,225,233,237,251,263,298
};

stock ss_OnGameModeInit()
{
	for(new id=0;id<sizeof(SkinShops);id++)
	{
	    CreatePickup(1275,23,SkinShops[id][Coord_X],SkinShops[id][Coord_Y],SkinShops[id][Coord_Z]);
		CreatePickupText("Одежда",SkinShops[id][Coord_X],SkinShops[id][Coord_Y],SkinShops[id][Coord_Z]);
	}
	GameMSG(lang_texts[17][22]);
	return 1;
}

stock ss_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys
	// если игрок не в примерочной
	if(GetPVarInt(playerid,"SkinShop_AtFittingRoom") != 1)
	{
		if( PRESSED ( KEY_USING ) )
		{
			SkinShop_ShowDialog(playerid);
		}
		return 0;
	}
	// если игрок в примерочной
	if( PRESSED ( KEY_HANDBRAKE ) ) // право
	{
		return SkinShop_SelectSkin(playerid,GetPVarInt(playerid,"SkinShop_SkinType"),GetPVarInt(playerid,"SkinShop_CurrentSkinID") + 1);
	}
    if( PRESSED ( KEY_FIRE ) ) // лево
	{
		return SkinShop_SelectSkin(playerid,GetPVarInt(playerid,"SkinShop_SkinType"),GetPVarInt(playerid,"SkinShop_CurrentSkinID") - 1);
	}
	if( PRESSED ( KEY_USING ) )
	{
		if(GetPlayerMoney(playerid) < SKINS_COST)
		{
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[17][23],SKINS_COST);
			return SendClientMessage(playerid,0,string);
		}
		return SkinShop_ShowBuyDialog(playerid);
	}
	return 1;
}

stock ss_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	#pragma unused inputtext
	switch(dialogid)
	{
		case SkinShop_DialogID:
		{
			SetPVarInt(playerid,"SkinShop_Old",GetPlayerSkin(playerid));
			if(!response)
			{
				SkinShop_Exit(playerid);
				SetPlayerSkin(playerid,GetPVarInt(playerid,"SkinShop_Old"));
				SkinShop_DeletePVars(playerid);
				return 0;
			}
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[17][24],KEY_NAME);
			SendClientMessage(playerid,0,string);
			SkinShop_SelectSkin(playerid,listitem,0);
		}
		case SkinShop_Buy_DialogID:
		{
			if(!response)
			{
				SkinShop_Exit(playerid);
				SkinShop_DeletePVars(playerid);
				return 0;
			}
			// покупка скина
			SkinShop_Exit(playerid);
			SetPlayerSkin(playerid,GetPVarInt(playerid,"SkinShop_CurrentSkin"));
			SetPlayerSkinModel(playerid,GetPVarInt(playerid,"SkinShop_CurrentSkin"));
			SkinShop_DeletePVars(playerid);
			oGivePlayerMoney(playerid,-SKINS_COST);
			SendClientMessage(playerid,0,lang_texts[17][25]);
		}
	}
	return 1;
}

stock SkinShop_SelectSkin(playerid,type,pos) // включить выбор скинов
{
	SetPVarInt(playerid,"SkinShop_CurrentSkinID",pos);
	SetPVarInt(playerid,"SkinShop_SkinType",type);
	TogglePlayerControllable(playerid,0);
	switch(type)
	{
		case 0: // мужские
		{
			if(pos > sizeof(MenSkins)-1)
			{
				SetPVarInt(playerid,"SkinShop_CurrentSkinID",0);
				pos = 0;
			}
			else if(pos < 0)
			{
				SetPVarInt(playerid,"SkinShop_CurrentSkinID",sizeof(MenSkins)-1);
				pos = sizeof(MenSkins)-1;
			}
			SetPlayerSkin(playerid,MenSkins[pos]);
			SetPVarInt(playerid,"SkinShop_CurrentSkin",MenSkins[pos]);
		}
		case 1: // женские
		{
			if(pos > sizeof(WomanSkins)-1)
			{
				SetPVarInt(playerid,"SkinShop_CurrentSkinID",0);
				pos = 0;
			}
			else if(pos < 0)
			{
				SetPVarInt(playerid,"SkinShop_CurrentSkinID",sizeof(WomanSkins)-1);
				pos = sizeof(WomanSkins)-1;
			}
			SetPlayerSkin(playerid,WomanSkins[pos]);
			SetPVarInt(playerid,"SkinShop_CurrentSkin",WomanSkins[pos]);
		}
	}
	return 1;
}

stock SkinShop_Exit(playerid)
{
	TogglePlayerControllable(playerid,1);
	SetPlayerSkin(playerid,GetPVarInt(playerid,"SkinShop_Old"));
	SetPlayerInterior(playerid,GetPVarInt(playerid,"SkinShop_Interior"));
	oSetPlayerPos(playerid,GetPVarFloat(playerid,"SkinShop_Coords_X"),GetPVarFloat(playerid,"SkinShop_Coords_Y"),GetPVarFloat(playerid,"SkinShop_Coords_Z"));
	SetPlayerFacingAngle(playerid,GetPVarFloat(playerid,"SkinShop_Coords_A"));
	SetCameraBehindPlayer(playerid);
	return 1;
}

stock SkinShop_DeletePVars(playerid)
{
	DeletePVar(playerid,"SkinShop_Old");
	DeletePVar(playerid,"SkinShop_CurrentSkinID");
	DeletePVar(playerid,"SkinShop_CurrentSkin");
	DeletePVar(playerid,"SkinShop_SkinType");
	DeletePVar(playerid,"SkinShop_AtFittingRoom");
	DeletePVar(playerid,"SkinShop_PlayerAtSkinShop");
	DeletePVar(playerid,"SkinShop_Interior");
	DeletePVar(playerid,"SkinShop_Coords_X");
	DeletePVar(playerid,"SkinShop_Coords_Y");
	DeletePVar(playerid,"SkinShop_Coords_Z");
	DeletePVar(playerid,"SkinShop_Coords_A");
}

stock SkinShop_ShowDialog(playerid)
{
	SetPVarInt(playerid,"SkinShop_AtFittingRoom",1);
	SkinShop_SetPlayerFittingRoom(playerid);
	return ShowPlayerDialog(playerid,SkinShop_DialogID,DIALOG_STYLE_LIST,
		lang_texts[17][26],
		lang_texts[17][27],
		lang_texts[17][28],lang_texts[17][29]
	);
}
stock SkinShop_SetPlayerFittingRoom(playerid)
{
	new Float:pos[4];
	GetPlayerPos(playerid,pos[0],pos[1],pos[2]);
	GetPlayerFacingAngle(playerid,pos[3]);
	SetPVarInt(playerid,"SkinShop_Interior",GetPlayerInterior(playerid));
	SetPVarFloat(playerid,"SkinShop_Coords_X",pos[0]);
	SetPVarFloat(playerid,"SkinShop_Coords_Y",pos[1]);
	SetPVarFloat(playerid,"SkinShop_Coords_Z",pos[2]);
	SetPVarFloat(playerid,"SkinShop_Coords_A",pos[3]);
	
	oSetPlayerPos(playerid,181.751159,-86.932769,1002.023437);
	SetPlayerFacingAngle(playerid,122.692977);
	SetPlayerInterior(playerid,18);
	SetPlayerCameraPos(playerid,179.138198,-88.452796,1003.536376);
	SetPlayerCameraLookAt(playerid,187.413238,-83.639259,1000.646118);
}

stock IsPlayerAtSkinShop(playerid)
{
	if(GetPVarInt(playerid,"SkinShop_PlayerAtSkinShop") == 1) return 1;
	for(new id=0;id<sizeof(SkinShops);id++)
	{
		if(IsPlayerInRangeOfPoint(playerid,2,SkinShops[id][Coord_X],SkinShops[id][Coord_Y],SkinShops[id][Coord_Z]))
		{
			SetPVarInt(playerid,"SkinShop_PlayerAtSkinShop",1);
			return 1;
		}
	}
	DeletePVar(playerid,"SkinShop_PlayerAtSkinShop");
	return 0;
}

stock SkinShop_ShowBuyDialog(playerid)
{
	return ShowPlayerDialog(playerid,SkinShop_Buy_DialogID,DIALOG_STYLE_MSGBOX,
		lang_texts[17][26],
		lang_texts[17][30],
		lang_texts[17][31],lang_texts[17][32]
	);
}
