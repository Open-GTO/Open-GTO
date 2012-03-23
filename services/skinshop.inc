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
			SkinShop_SetPlayerFittingRoom(playerid);
			SetPVarInt(playerid,"SkinShop_AtFittingRoom",1);
			new string[MAX_STRING];
			format(string,sizeof(string),lang_texts[17][24],KEY_NAME);
			SendClientMessage(playerid,0,string);
			SkinShop_SelectSkin(playerid,1,1);
		}
		return 0;
	}
	// если игрок в примерочной
	if( PRESSED ( KEY_HANDBRAKE ) ) // право
	{
		return SkinShop_SelectSkin(playerid,GetPVarInt(playerid,"SkinShop_CurrentSkin"),GetPVarInt(playerid,"SkinShop_CurrentSkin") + 1);
	}
    if( PRESSED ( KEY_FIRE ) ) // лево
	{
		return SkinShop_SelectSkin(playerid,GetPVarInt(playerid,"SkinShop_CurrentSkin"),GetPVarInt(playerid,"SkinShop_CurrentSkin") - 1);
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
	#pragma unused inputtext,listitem
	if (dialogid == SkinShop_Buy_DialogID)
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
	return 1;
}

stock SkinShop_SelectSkin(playerid,oldskin,newskin) // включить выбор скинов
{
	switch (newskin)
	{
		case SKINS_IGNORE:
		{
			if (newskin > oldskin)
			{
				newskin++;
				if (newskin > SKINS_MAXID)
				{
					newskin = SKINS_MINID;
				}
			}
			else if (newskin < oldskin)
			{
				newskin--;
				if (newskin < SKINS_MINID)
				{
					newskin = SKINS_MAXID;
				}
			}
		}
	}
	
	TogglePlayerControllable(playerid,0);
	SetPlayerSkin(playerid,newskin);
	SetPVarInt(playerid,"SkinShop_CurrentSkin",newskin);
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
	DeletePVar(playerid,"SkinShop_CurrentSkin");
	DeletePVar(playerid,"SkinShop_AtFittingRoom");
	DeletePVar(playerid,"SkinShop_PlayerAtSkinShop");
	DeletePVar(playerid,"SkinShop_Interior");
	DeletePVar(playerid,"SkinShop_Coords_X");
	DeletePVar(playerid,"SkinShop_Coords_Y");
	DeletePVar(playerid,"SkinShop_Coords_Z");
	DeletePVar(playerid,"SkinShop_Coords_A");
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
		lang_texts[17][27],
		lang_texts[17][28],lang_texts[17][29]
	);
}
