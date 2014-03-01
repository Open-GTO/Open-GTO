/*
	Author:	ZiGGi
	Date:	15.07.2010
*/

#if defined _pickup_fix_included
  #endinput
#endif

#define _pickup_fix_included
#pragma library pickup_fix


new pickup_model[MAX_PICKUPS];

stock oCreatePickup(model, type, Float:coord_X, Float:coord_Y, Float:coord_Z, Virtualworld = 0)
{
	new id = CreatePickup(model, type, coord_X, coord_Y, coord_Z, Virtualworld);
	pickup_model[id] = model;
	return id;
}

#define GetPickupModel(%0) pickup_model[%0]
#define CreatePickup oCreatePickup
