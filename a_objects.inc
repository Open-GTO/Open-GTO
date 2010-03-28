/*  SA-MP Object Functions
 *
 *  (c) Copyright 2005-2009, SA-MP Team
 *
 */

#if defined _objects_included
	#endinput
#endif
#define _objects_included
#pragma library objects

// Objects

native CreateObject(modelid, Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ);
native SetObjectPos(objectid, Float:X, Float:Y, Float:Z);
native GetObjectPos(objectid, &Float:X, &Float:Y, &Float:Z);
native SetObjectRot(objectid, Float:RotX, Float:RotY, Float:RotZ);
native GetObjectRot(objectid, &Float:RotX, &Float:RotY, &Float:RotZ);
native IsValidObject(objectid);
native DestroyObject(objectid);
native MoveObject(objectid, Float:X, Float:Y, Float:Z, Float:Speed);
native StopObject(objectid);
native CreatePlayerObject(playerid, modelid, Float:X, Float:Y, Float:Z, Float:rX, Float:rY, Float:rZ);
native SetPlayerObjectPos(playerid, objectid, Float:X, Float:Y, Float:Z);
native GetPlayerObjectPos(playerid, objectid, &Float:X, &Float:Y, &Float:Z);
native SetPlayerObjectRot(playerid, objectid, Float:RotX, Float:RotY, Float:RotZ);
native GetPlayerObjectRot(playerid, objectid, &Float:RotX, &Float:RotY, &Float:RotZ);
native IsValidPlayerObject(playerid, objectid);
native DestroyPlayerObject(playerid, objectid);
native MovePlayerObject(playerid, objectid, Float:X, Float:Y, Float:Z, Float:Speed);
native StopPlayerObject(playerid, objectid);
native AttachObjectToPlayer(objectid, playerid, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ);
native AttachPlayerObjectToPlayer(objectplayer, objectid, attachplayer, Float:OffsetX, Float:OffsetY, Float:OffsetZ, Float:rX, Float:rY, Float:rZ);
