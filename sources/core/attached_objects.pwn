/*

	About: attached objects
	Author: ziggi

*/

#if defined _attachedobjects_included
	#endinput
#endif

#define _attachedobjects_included

/*
	Vars
*/

static
	Iterator:AttachedObjectsIterator[MAX_PLAYERS]<MAX_PLAYER_ATTACHED_OBJECTS>;

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	Iter_Init(AttachedObjectsIterator);

	#if defined AObj_OnGameModeInit
		return AObj_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit AObj_OnGameModeInit
#if defined AObj_OnGameModeInit
	forward AObj_OnGameModeInit();
#endif

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	Iter_Clear(AttachedObjectsIterator[playerid]);

	#if defined AObj_OnPlayerConnect
		return AObj_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect AObj_OnPlayerConnect
#if defined AObj_OnPlayerConnect
	forward AObj_OnPlayerConnect(playerid);
#endif

/*
	AObj_SetPlayerAttachedObject
*/

stock AObj_SetPlayerAttachedObject(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0)
{
	if (SetPlayerAttachedObject(playerid, index, modelid, bone, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, materialcolor1, materialcolor2)) {
		Iter_Add(AttachedObjectsIterator[playerid], index);
		return 1;
	}

	return 0;
}
#if defined _ALS_SetPlayerAttachedObject
	#undef SetPlayerAttachedObject
#else
	#define _ALS_SetPlayerAttachedObject
#endif

#define SetPlayerAttachedObject AObj_SetPlayerAttachedObject

/*
	AObj_RemovePlayerAttachedObject
*/

stock AObj_RemovePlayerAttachedObject(playerid, index)
{
	if (RemovePlayerAttachedObject(playerid, index)) {
		Iter_Remove(AttachedObjectsIterator[playerid], index);
		return 1;
	}

	return 0;
}
#if defined _ALS_RemovePlayerAttachedObject
	#undef RemovePlayerAttachedObject
#else
	#define _ALS_RemovePlayerAttachedObject
#endif

#define RemovePlayerAttachedObject AObj_RemovePlayerAttachedObject

/*
	AddPlayerAttachedObject
*/

stock AddPlayerAttachedObject(playerid, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0)
{
	new index = Iter_Free(AttachedObjectsIterator[playerid]);
	SetPlayerAttachedObject(playerid, index, modelid, bone, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, materialcolor1, materialcolor2);
	return index;
}

/*
	GetPlayerFreeAttachedObjectSlot
*/

stock GetPlayerFreeAttachedObjectSlot(playerid, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0)
{
	return Iter_Free(AttachedObjectsIterator[playerid]);
}
