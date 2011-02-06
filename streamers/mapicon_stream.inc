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
	bool:Showed,
	icon_type,
	iconid,
	Float:Coord_X,
	Float:Coord_Y,
	Float:Coord_Z
};
static MapIcons[MAX_ICONS][info_MapIcon];
static IconsShowed[MAX_PLAYERS];

stock CreateStreamMapIcon( markertype, Float:X, Float:Y, Float:Z )
{
	new icon_id = GetLastMapIconSlot();
	if(icon_id > MAX_ICONS) return -1;
	MapIcons[icon_id][icon_type] = markertype;
	if( icon_id == 0 ) // фикс для 0 иконки
	{
		MapIcons[icon_id][iconid] = 0;
	}
	else
	{
		MapIcons[icon_id][iconid] = MapIcons[icon_id-1][iconid]+1;
	}
	MapIcons[icon_id][Coord_X] = X;
	MapIcons[icon_id][Coord_Y] = Y;
	MapIcons[icon_id][Coord_Z] = Z;
	return icon_id;
}

stock DeleteStreamMapIcon(icon_id)
{
	for(new i=GetMaxPlayers()-1;i>=0;--i)
		RemovePlayerStreamMapIcon(i,icon_id);
	MapIcons[icon_id][icon_type] = 0;
	MapIcons[icon_id][Showed] = false;
	MapIcons[icon_id][Coord_X] = 0.0;
	MapIcons[icon_id][Coord_Y] = 0.0;
	MapIcons[icon_id][Coord_Z] = 0.0;
}

stock RemovePlayerStreamMapIcon( playerid,icon_id )
{
	MapIcons[icon_id][icon_type] = 0;
	MapIcons[icon_id][Showed] = false;
	MapIcons[icon_id][Coord_X] = 0.0;
	MapIcons[icon_id][Coord_Y] = 0.0;
	MapIcons[icon_id][Coord_Z] = 0.0;
	RemovePlayerMapIcon(playerid,icon_id);
}

stock MapIcons_Streaming()
{
	for( new playerid=GetMaxPlayers()-1;playerid>=0;--playerid )
	{
		if( !IsPlayerConnected(playerid) || IsPlayerNPC(playerid) ) continue;
		//icon_count[playerid]=0;
		for( new icon_id=0;icon_id<MAX_ICONS;icon_id++ )
		{
			if( GetDistanceToMapIcon(playerid,icon_id) > MAP_ICON_STREAM_DISTANCE
				&& IsValidMapIcon(icon_id)
				&& MapIcons[icon_id][Showed] == true
			) // если игрок не в зоне отображения иконки
			{
				RemovePlayerMapIcon(playerid,MapIcons[icon_id][iconid]);
				MapIcons[icon_id][Showed] = false;
				IconsShowed[playerid]--;
				continue;
			}
			if(	MapIcons[icon_id][Showed] == true || !IsValidMapIcon(icon_id) ) continue; // если иконка показана или иконки нет

			if( GetDistanceToMapIcon(playerid,icon_id) <= MAP_ICON_STREAM_DISTANCE && IconsShowed[playerid] < MAX_SHOWED_ICONS) // если игрок в зоне отображения иконки
			{
				// for houses
				for(new house_id=0,icon_buf;house_id<MAX_HOUSES;house_id++)
				{
					icon_buf = house_icon[house_id];
					if(!strcmp(Houses[house_id][Houses_Owner],oGetPlayerName(playerid))
					|| !strcmp(Houses[house_id][Houses_RentName],oGetPlayerName(playerid))
					)
						MapIcons[icon_buf][icon_type] = 35;
					else if(strcmp(Houses[house_id][Houses_Gang],"Unknown"))
						MapIcons[icon_buf][icon_type] = 32;
					else
						MapIcons[icon_buf][icon_type] = 31;
				}
				// //
				SetPlayerMapIcon(playerid,MapIcons[icon_id][iconid],MapIcons[icon_id][Coord_X],MapIcons[icon_id][Coord_Y],MapIcons[icon_id][Coord_Z],MapIcons[icon_id][icon_type],0);
				MapIcons[icon_id][Showed] = true;
				IconsShowed[playerid]++;
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

stock GetDistanceToMapIcon(playerid,icon_id)
{
	new Float:icon_x,Float:icon_y,Float:icon_z;
	new Float:player_x,Float:player_y,Float:player_z;

	GetMapIconPos(icon_id,icon_x,icon_y,icon_z);
	GetPlayerPos(playerid,player_x,player_y,player_z);

	return floatround
	(
		floatsqroot
		(
			floatpower( icon_x - player_x, 2 )
			+ 
			floatpower( icon_y - player_y, 2 ) 
		)
	);
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
	if(	MapIcons[icon_id][icon_type] == 0 && MapIcons[icon_id][Showed] == false
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
		if(	MapIcons[icon][icon_type] == 0 && MapIcons[icon][Showed] == false
			&& MapIcons[icon][Coord_X] == 0.0
			&& MapIcons[icon][Coord_Y] == 0.0
			&& MapIcons[icon][Coord_Z] == 0.0
		) return icon;
	}
	return -1;
}