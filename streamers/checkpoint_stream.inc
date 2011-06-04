/*


*/

#if defined _checkpoint_stream_included
	#endinput
#endif

#define _checkpoint_stream_included
#pragma library checkpoint_stream


#define MAX_ACTIVECP 255
#define CP_SHOW_DIST 65000.0

new ActiveCPCount;
enum ActiveCPInfo {	// active checkpoints are checkpoints around the world that player can see when they get near
	bool:IsActive,
	Float:CP_X,
	Float:CP_Y,
	Float:CP_Z,
	CP_Dist,		// distance from checkpoint before player can see it
	CP_Size			// size of checkpoint (bugged)
}
new ActiveCheckpoints[MAX_ACTIVECP][ActiveCPInfo];

stock SyncActiveCP()
{
	for(new playerid=0;playerid<=GetPlayerLastID();playerid++)
	{
		if(!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) continue;
		if(PlayerQuest[playerid] == 0)
		{
	    	new cpid = GetPlayerClosestActiveCP(playerid);
			if(cpid != 0)
				SetPlayerActiveCP(playerid,cpid);
			else
				oDisablePlayerCheckpoint(playerid);
		}
	}
}

stock AddActiveCheckpoint(Float:cpx,Float:cpy,Float:cpz,cpdist,cpsize) //
{
	if (ActiveCPCount == MAX_ACTIVECP) return 0;
	for (new cpid=1;cpid<=MAX_ACTIVECP;cpid++)
	{
		if (ActiveCheckpoints[cpid][IsActive] == false)
		{ // we have found an inactive cp to use
			ActiveCPCount++; // 0 will be invalid, so we ++ first
			ActiveCheckpoints[cpid][CP_X] = cpx;
			ActiveCheckpoints[cpid][CP_Y] = cpy;
			ActiveCheckpoints[cpid][CP_Z] = cpz;
			ActiveCheckpoints[cpid][CP_Dist] = cpdist;
			ActiveCheckpoints[cpid][CP_Size] = cpsize;
			ActiveCheckpoints[cpid][IsActive] = true;
			return cpid; // exit and send back our new cpid
		}
	}
	return 0;
}

stock RemoveActiveCheckpoint(cpid)
{
	if (ActiveCheckpoints[cpid][IsActive] == false) return 0;
	ActiveCheckpoints[cpid][CP_X] = 0;
	ActiveCheckpoints[cpid][CP_Y] = 0;
	ActiveCheckpoints[cpid][CP_Z] = 0;
	ActiveCheckpoints[cpid][CP_Size] = 0;
	ActiveCheckpoints[cpid][IsActive] = false;
	ActiveCPCount--;
	return 1;
}

stock FindActiveCheckpoint(Float:cpx,Float:cpy,Float:cpz)
{
	for(new cpid=1;cpid<=ActiveCPCount;cpid++)
	{
		if(ActiveCheckpoints[cpid][IsActive])
		{
			if(loccmp(cpx,cpy,cpz,ActiveCheckpoints[cpid][CP_X],ActiveCheckpoints[cpid][CP_Y],ActiveCheckpoints[cpid][CP_Z]))
			{
				return cpid;
			}
		}
	}
	return 0;
}

stock SetPlayerActiveCP(playerid,cpid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if(!ActiveCheckpoints[cpid][IsActive]) return 0;
	if(cpid == 0)
	{
		oDisablePlayerCheckpoint(playerid); // cpid 0 is invalid
		return 1;
	}
	if((PlayerCP[playerid][Coord_X] == ActiveCheckpoints[cpid][CP_X]) && (PlayerCP[playerid][Coord_Y] == ActiveCheckpoints[cpid][CP_Y]) && (PlayerCP[playerid][Coord_Z] == ActiveCheckpoints[cpid][CP_Z]))
	{ // if player cp already == this cp
		return 0;
	}
	oSetPlayerCheckpoint(playerid,ActiveCheckpoints[cpid][CP_X],ActiveCheckpoints[cpid][CP_Y],ActiveCheckpoints[cpid][CP_Z],8.0);
	return 1;
}

stock GetPlayerClosestActiveCP(playerid)
{
 	new Float:cpdist=CP_SHOW_DIST;
	new retval=0;
	for (new cpid=1;cpid<MAX_ACTIVECP;cpid++)
	{
		if (ActiveCheckpoints[cpid][IsActive])
		{
			new Float:playerdist=GetDistancePlayertoXYZ(playerid,ActiveCheckpoints[cpid][CP_X],ActiveCheckpoints[cpid][CP_Y],ActiveCheckpoints[cpid][CP_Z]);
			if (playerdist < ActiveCheckpoints[cpid][CP_Dist])
			{
				if (playerdist < cpdist)
				{
					cpdist = playerdist;
					retval = cpid;
				}
			}
			
		}
	}
	return retval;
}



stock oSetPlayerCheckpoint(playerid, Float:x, Float:y, Float:z, Float:size)
{
	DisablePlayerCheckpoint(playerid);
	SetPlayerCheckpoint(playerid, 0.0, 0.0, 0.0, size);
	DisablePlayerCheckpoint(playerid);
	PlayerCP[playerid][Coord_X] = x;
	PlayerCP[playerid][Coord_Y] = y;
	PlayerCP[playerid][Coord_Z] = z;
	PlayerCPActive[playerid] = true;
	SetPlayerCheckpoint(playerid, x, y, z, size);
}

stock oDisablePlayerCheckpoint(playerid)
{
	DisablePlayerCheckpoint(playerid);
	PlayerCPActive[playerid] = false;
	PlayerCP[playerid][Coord_X] = 0.0;
	PlayerCP[playerid][Coord_Y] = 0.0;
	PlayerCP[playerid][Coord_Z] = 0.0;
}

stock oIsPlayerInCheckpoint(playerid,Float:cpx,Float:cpy,Float:cpz,dist)
{
	if (!IsPlayerConnected(playerid)) return 0;
	if (!PlayerCPActive[playerid]) return 0;
	if (!loccmp(cpx,cpy,cpz,PlayerCP[playerid][Coord_X],PlayerCP[playerid][Coord_Y],PlayerCP[playerid][Coord_Z])) return 0;
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	if(GetDistanceXYZtoXYZ(playerx,playery,playerz,cpx,cpy,cpz) < dist) return IsPlayerInCheckpoint(playerid);
	return 0;
}