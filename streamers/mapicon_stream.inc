//Copyright by Rafelder
//edit by heufix 

#include "player"

#define MAX_ICONS 350
#define MAX_SHOWED_MAPICONS 30

new Float:XM[MAX_ICONS],Float:YM[MAX_ICONS],Float:ZM[MAX_ICONS];
new MarkerM[MAX_ICONS],IsValid[MAX_ICONS],Show[MAX_PLAYERS][MAX_ICONS],ShowedForPlayer[MAX_PLAYERS];
new IconidM=-1,Count=0;

stock CreateStreamMapIcon(iconid, Float:X, Float:Y, Float:Z)
{
	if(IconidM >= MAX_ICONS) return 0;
	IconidM++;
	IsValid[IconidM]=1;
	XM[IconidM]=X;
	YM[IconidM]=Y;
	ZM[IconidM]=Z;
	MarkerM[IconidM]=iconid;
	for(new i=0; i<MAX_PLAYERS; i++) Show[i][IconidM]=1;
	return IconidM;
}

stock IsValidMapIcon(iconid)
{
	if (IsValid[iconid] == 1) return 1;
	return 0;
}
stock GetMapIconPos(iconid, &Float:X, &Float:Y, &Float:Z)
{
	if (!IsValidMapIcon(iconid)) return 0;
	X=XM[iconid];
	Y=YM[iconid];
	Z=ZM[iconid];
	return 1;
}

stock MapIconsTimer()
{
	for(new i=0; i<MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i))
		{
			ShowedForPlayer[i]=0;
			for(new oldm=0; oldm<32; oldm++) RemovePlayerMapIcon(i, oldm);
			Count=0;
			for(new m=0; m<MAX_ICONS; m++)
			{
				if(IsValidMapIcon(m) && ShowedForPlayer[i] < MAX_SHOWED_MAPICONS && Show[i][m] == 1)
				{
					if(GetDistanceToMapIcon(i, m) <= 500)
					{
						SetPlayerMapIcon(i, Count, XM[m], YM[m], ZM[m], MarkerM[m],COLOUR_WHITE);
						ShowedForPlayer[i]++;
						Count++;
					}
				}
			}
		}
	}
	return 1;
}

stock GetDistanceToMapIcon(playerid, iconid)
{
	new Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2,Float:output;
	GetPlayerPos(playerid, x1, y1, z1);
	GetMapIconPos(iconid, x2, y2, z2);
	output = floatsqroot(floatpower(floatabs(floatsub(x2, x1)), 2)+floatpower(floatabs(floatsub(y2, y1)), 2));
	return floatround(output);
}
