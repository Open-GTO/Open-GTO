/*
	Author:	ZiGGi
	Date:	15.07.2010
*/

#if defined _pickup_fix_included
  #endinput
#endif

#define _pickup_fix_included
#pragma library pickup_fix


enum pickups_Info {
	pi_model,
	pi_type,
	Float: pi_x,
	Float: pi_y,
	Float: pi_z,
	pi_world,
}
new pickups[MAX_PICKUPS][pickups_Info];

stock oCreatePickup(model, type, Float:coord_X, Float:coord_Y, Float:coord_Z, Virtualworld = 0)
{
	new id = CreatePickup(model, type, coord_X, coord_Y, coord_Z, Virtualworld);
	pickups[id][pi_model] = model;
	pickups[id][pi_type] = type;
	pickups[id][pi_x] = coord_X;
	pickups[id][pi_y] = coord_Y;
	pickups[id][pi_z] = coord_Z;
	pickups[id][pi_world] = Virtualworld;
	return id;
}

#define CreatePickup oCreatePickup
