/*
	Author:	ziggi
*/

#if defined _pickup_fix_included
	#endinput
#endif

#define _pickup_fix_included

/*
	Vars
*/

static
	gPickupModel[MAX_PICKUPS];

/*
	Functions
*/

stock GetPickupModel(pickupid)
{
	return gPickupModel[pickupid];
}

stock GTO_CreatePickup(model, type, Float:coord_x, Float:coord_y, Float:coord_z, virtualworld = 0)
{
	new id = CreatePickup(model, type, Float:coord_x, Float:coord_y, Float:coord_z, virtualworld);
	gPickupModel[id] = model;
	return id;

}
#if defined _ALS_CreatePickup
	#undef CreatePickup
#else
	#define _ALS_CreatePickup
#endif

#define CreatePickup GTO_CreatePickup
