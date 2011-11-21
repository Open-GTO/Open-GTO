/*
	Author:	ZiGGi
	Date:	15.07.2010
*/

#if defined _streamer_mapicon_included
  #endinput
#endif

#define _streamer_mapicon_included
#pragma library streamer_mapicon

#define MAX_ICONS 1000
#define MAX_SHOWED_ICONS 100

#define MAP_ICON_STREAM_DISTANCE 400

enum info_MapIcon {
	icon_type,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z
};
static MapIcons[MAX_ICONS][info_MapIcon];
static bool:MapIconsShowed[MAX_PLAYERS][MAX_ICONS];
static SlotIcons[MAX_PLAYERS][MAX_SHOWED_ICONS char];

stock mapicon_OnGameModeInit()
{
	SetTimer("MapIcons_Streaming",1000,1);
	GameMSG("(streamers)Mapicon streamer init.");
	return 1;
}

stock CreateStreamMapIcon( markertype, Float:X, Float:Y, Float:Z )
{
	new icon_id = GetLastMapIconSlot();
	if(icon_id == 0)
	{
		GameMSG("Количество иконок достигло максимума!("#MAX_ICONS")");
		return 0;
	}
	MapIcons[icon_id][icon_type] = markertype;
	MapIcons[icon_id][Coord_X] = X;
	MapIcons[icon_id][Coord_Y] = Y;
	MapIcons[icon_id][Coord_Z] = Z;
	return icon_id;
}

stock DeleteStreamMapIcon(icon_id)
{
	MapIcons[icon_id][icon_type] = 0;
	MapIcons[icon_id][Coord_X] = 0.0;
	MapIcons[icon_id][Coord_Y] = 0.0;
	MapIcons[icon_id][Coord_Z] = 0.0;
	new slot;
	foreach(Player,i)
	{
		slot = GetIconSlot(i,icon_id);
		RemovePlayerMapIcon(i,slot);
		SlotIcons[i]{slot} = 0;
		MapIconsShowed[i][icon_id] = false;
	}
}

stock UpdatePlayerStreamMapIcon( playerid,icon_id )
{
	new slot = GetIconSlot(playerid,icon_id);
	RemovePlayerMapIcon(playerid,slot);
	update_House_IconType(playerid,icon_id);
	update_Business_IconType(playerid,icon_id);
	SetPlayerMapIcon(playerid,slot,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z],MapIcons[icon_id][icon_type],0,MAPICON_LOCAL);
}

stock UpdateStreamMapIcon( icon_id )
{
	new slot;
	foreach(Player,i)
	{
		slot = GetIconSlot(i,icon_id);
		RemovePlayerMapIcon(i,slot);
		update_House_IconType(i,icon_id);
		update_Business_IconType(i,icon_id);
		SetPlayerMapIcon(i,slot,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z],MapIcons[icon_id][icon_type],0,MAPICON_LOCAL);
	}
}

forward MapIcons_Streaming();
public MapIcons_Streaming()
{
	foreach(Player,playerid)
	{
		for( new icon_id=1;icon_id<MAX_ICONS;icon_id++ )
		{
			if( !IsPlayerInRangeOfPoint(playerid,MAP_ICON_STREAM_DISTANCE,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z])
				&& IsValidMapIcon(icon_id)
				&& MapIconsShowed[playerid][icon_id] == true
			) // если игрок не в зоне отображения иконки, но иконка ототбражается
			{
				new slot = GetIconSlot(playerid,icon_id);
				SlotIcons[playerid]{slot} = 0;
				
				RemovePlayerMapIcon(playerid,slot);
				MapIconsShowed[playerid][icon_id] = false;
				GivePVarInt(playerid,"IconsShowed",-1);
				continue;
			}
			
			if(	MapIconsShowed[playerid][icon_id] == true || !IsValidMapIcon(icon_id) || GetPVarInt(playerid,"IconsShowed") > MAX_SHOWED_ICONS ) continue; // если иконка показана или иконки нет
			
			if( IsPlayerInRangeOfPoint(playerid,MAP_ICON_STREAM_DISTANCE,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z]) ) // если игрок в зоне отображения иконки
			{
				update_House_IconType(playerid,icon_id);
				update_Business_IconType(playerid,icon_id);
				new slot = GetLastRealMapIconSlot(playerid);
				SlotIcons[playerid]{slot} = icon_id;
				SetPlayerMapIcon(playerid,slot,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z],MapIcons[icon_id][icon_type],0,MAPICON_LOCAL);
				MapIconsShowed[playerid][icon_id] = true;
				GivePVarInt(playerid,"IconsShowed",1);
			}
		}
	}
}

stock GetMapIconPos( icon_id,&Float:icon_x,&Float:icon_y,&Float:icon_z )
{
	if( !IsValidMapIcon(icon_id) ) return 0;
	icon_x = MapIcons[icon_id][Coord_X];
	icon_y = MapIcons[icon_id][Coord_Y];
	icon_z = MapIcons[icon_id][Coord_Z];
	return 1;
}

stock FindStreamMapIcon(Float:X,Float:Y,Float:Z)
{
	for(new icon_id=1;icon_id<MAX_ICONS;icon_id++)
	{
		if(!IsValidMapIcon(icon_id)) continue;
		if(loccmp(X,Y,Z,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z]))
			return icon_id;
	}
	return 0;
}

stock IsValidMapIcon(icon_id)
{
	if(	MapIcons[icon_id][icon_type] == 0
		&& MapIcons[icon_id][Coord_X] == 0.0
		&& MapIcons[icon_id][Coord_Y] == 0.0
		&& MapIcons[icon_id][Coord_Z] == 0.0
	) return false;
	return true;
}

stock GetLastMapIconSlot()
{
	for(new icon=1;icon<MAX_ICONS;icon++)
	{
		if(	MapIcons[icon][icon_type] == 0
			&& MapIcons[icon][Coord_X] == 0.0
			&& MapIcons[icon][Coord_Y] == 0.0
			&& MapIcons[icon][Coord_Z] == 0.0
		) return icon;
	}
	return -1;
}

stock GetLastRealMapIconSlot(playerid)
{
	for(new slot=1;slot<=MAX_SHOWED_ICONS;slot++)
	{
		if(SlotIcons[playerid]{slot} == 0) return slot;
	}
	return 0;
}

stock GetIconSlot(playerid,icon_id)
{
	for(new slot=1;slot<=MAX_SHOWED_ICONS;slot++)
	{
		if(SlotIcons[playerid]{slot} == icon_id) return slot;
	}
	return 0;
}

stock update_House_IconType(playerid,icon_id)
{
	new playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	for(new house_id=0;house_id<sizeof(Houses);house_id++)
	{
		if(house_icon[house_id] != icon_id) continue;
		if(!strcmp(Houses[house_id][Houses_Owner],playername)
			|| !strcmp(Houses[house_id][Houses_RentName],playername)
		)
			MapIcons[icon_id][icon_type] = 35;
		else if(strcmp(Houses[house_id][Houses_Owner],"Unknown"))
			MapIcons[icon_id][icon_type] = 32;
		else
			MapIcons[icon_id][icon_type] = 31;
	}
}

stock update_Business_IconType(playerid,icon_id)
{
	new playername[MAX_PLAYER_NAME];
	GetPlayerName(playerid,playername,sizeof(playername));
	for(new bis_id=0;bis_id<sizeof(Businesses);bis_id++)
	{
		if(business_icon[bis_id] != icon_id) continue;
		if(!strcmp(Businesses[bis_id][Business_Owner],playername))
			MapIcons[icon_id][icon_type] = 38;
		else if(strcmp(Businesses[bis_id][Business_Owner],"Unknown"))
			MapIcons[icon_id][icon_type] = 36;
		else
			MapIcons[icon_id][icon_type] = 52;
	}
}
