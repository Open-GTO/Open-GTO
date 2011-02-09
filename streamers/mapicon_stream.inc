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

stock CreateStreamMapIcon( markertype, Float:X, Float:Y, Float:Z )
{
	new icon_id = GetLastMapIconSlot();
	if(icon_id >= MAX_ICONS)
	{
		print("Количество иконок достигло максимума!("#MAX_ICONS")");
		return -1;
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
	for(new i=GetMaxPlayers()-1;i>=0;--i)
	{
		RemovePlayerMapIcon(i,icon_id);
		MapIconsShowed[i][icon_id] = false;
	}
}

stock UpdatePlayerStreamMapIcon( playerid,icon_id )
{
	RemovePlayerMapIcon(playerid,icon_id);
	MapIconsShowed[playerid][icon_id] = false;
}

stock UpdateStreamMapIcon( icon_id )
{
	for(new i=GetMaxPlayers()-1;i>=0;--i)
	{
		RemovePlayerMapIcon(i,icon_id);
		MapIconsShowed[i][icon_id] = false;
	}
}

stock MapIcons_Streaming()
{
	for( new playerid=GetMaxPlayers()-1;playerid>=0;--playerid )
	{
		if( !IsPlayerConnected(playerid) || IsPlayerNPC(playerid) ) continue;
		for( new icon_id=0;icon_id<MAX_ICONS;icon_id++ )
		{
			if( !IsPlayerInRangeOfPoint(playerid,MAP_ICON_STREAM_DISTANCE,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z])
				&& IsValidMapIcon(icon_id)
				&& MapIconsShowed[playerid][icon_id] == true
			) // если игрок не в зоне отображения иконки
			{
				RemovePlayerMapIcon(playerid,icon_id);
				MapIconsShowed[playerid][icon_id] = false;
				SetPVarInt(playerid,"IconsShowed",GetPVarInt(playerid,"IconsShowed") - 1);
				continue;
			}
			
			if(	MapIconsShowed[playerid][icon_id] == true || !IsValidMapIcon(icon_id) || GetPVarInt(playerid,"IconsShowed") >= MAX_SHOWED_ICONS ) continue; // если иконка показана или иконки нет
			
			if( IsPlayerInRangeOfPoint(playerid,MAP_ICON_STREAM_DISTANCE,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z]) ) // если игрок в зоне отображения иконки
			{
				// for houses
				if(icon_id >= house_icon_first && icon_id <= house_icon_first+MAX_HOUSES-1)
				{
					for(new house_id=0;house_id<MAX_HOUSES;house_id++)
					{
						if(!strcmp(Houses[house_id][Houses_Owner],oGetPlayerName(playerid))
						|| !strcmp(Houses[house_id][Houses_RentName],oGetPlayerName(playerid))
						)
							MapIcons[icon_id][icon_type] = 35;
						else if(strcmp(Houses[house_id][Houses_Gang],"Unknown"))
							MapIcons[icon_id][icon_type] = 32;
						else
							MapIcons[icon_id][icon_type] = 31;
					}
				}
				// //
				SetPlayerMapIcon(playerid,icon_id,MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z],MapIcons[icon_id][icon_type],0,MAPICON_LOCAL);
				MapIconsShowed[playerid][icon_id] = true;
				SetPVarInt(playerid,"IconsShowed",GetPVarInt(playerid,"IconsShowed") + 1);
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
	for(new icon_id=0;icon_id<MAX_ICONS;icon_id++)
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
	for(new icon=0;icon<MAX_ICONS;icon++)
	{
		if(	MapIcons[icon][icon_type] == 0
			&& MapIcons[icon][Coord_X] == 0.0
			&& MapIcons[icon][Coord_Y] == 0.0
			&& MapIcons[icon][Coord_Z] == 0.0
		) return icon;
	}
	return -1;
}