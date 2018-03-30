/*----------------------------------------------------------------------------
					===========================
					 foreach efficient looping
					===========================
Description:
	Provides efficient looping through sparse data sets, such as connected
	players. Significantly improved from the original version to be a generic
	loop system, rather then purely a player loop system. When used for
	players this has constant time O(n) for number of connected players (n),
	unlike standard player loops which are O(MAX_PLAYERS), regardless of the
	actual number of connected players. Even when n is MAX_PLAYERS this is
	still faster.
Legal:
	Version: MPL 1.1

	The contents of this file are subject to the Mozilla Public License Version
	1.1 (the "License"); you may not use this file except in compliance with
	the License. You may obtain a copy of the License at
	http://www.mozilla.org/MPL/

	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.

	The Original Code is the YSI foreach include.

	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright (C) 2011
	the Initial Developer. All Rights Reserved.

	Contributors:
		ZeeX, koolk, JoeBullet/Google63, g_aSlice/Slice, Kar

	Thanks:
		JoeBullet/Google63 - Handy arbitrary ASM jump code using SCTRL.
		ZeeX - Very productive conversations.
		koolk - IsPlayerinAreaEx code.
		TheAlpha - Danish translation.
		breadfish - German translation.
		Fireburn - Dutch translation.
		yom - French translation.
		50p - Polish translation.
		Zamaroht - Spanish translation.
		Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes
			for me to strive to better.
		Pixels^ - Running XScripters where the idea was born.
		Matite - Pestering me to release it and using it.

	Very special thanks to:
		Thiadmer - PAWN, whose limits continue to amaze me!
		Kye/Kalcor - SA:MP.
		SA:MP Team past, present and future - SA:MP.

Version:
	19 (0.4.2)
Changelog:
	14/05/15:
		Cleaned up the entire include.
	16/07/12:
		Fixed a bug with "loadfs" blocking callbacks.
		Slightly reduced the memory consumption of constant strings.
	25/05/12:
		Added a delay timer to remove players for "Kick" crashes.
	05/01/12:
		Fixed multi-dimensional iterators.
		Fixed "FOREACH_NO_BOTS".
		Made "Iterator:" support multi-dimensional arrays.
	07/12/11:
		Underlying implementation tweak for more consistent code.
		Added Iter_Contains.
	06/12/11:
		Minor fixes.
	31/10/11:
		Changed the underlying loop code to be slightly faster.
		Added support for Iter_SafeRemove, prompting refactoring.
	17/09/11:
		Fixed arrays under the new syntax.
	28/04/11:
		Moved iterator identifiers to end of variables.
		Rewrote "foreach" to accept two syntaxes for "foreach (new i : Iter)".
	16/08/10:
		Removed all the "2" versions of the functions.
	14/08/10:
		Added Iter_Clear to reset an array.
	06/08/10:
		Added special array declaration format.
	18/12/09:
		Added Iter_Func2 functions for multi-dimensional iterators.
		Renamed foreact et al as keywords in the documentation.
		Changed licensing from GPL to MPL.
	02/09/09:
		Fixed (again) for 0.3.
		Added free slot finding.
	21/08/09:
		Updated to include random functions.
		Made entirely stand alone.
		Ported to 0.3 (separate version).
		Added automatic callback hook code.
		Removed debug information from stand alone version.
	06/01/08:
		Added debug information.
	09/10/07:
		Moved to system.
	16/09/07:
		Added list sorting.
		Made this part of Y SeRver Includes, not Y Sever Includes.
		Made list sorting optional.
		Fixed version number.
	08/09/07:
		First version.
Functions:
	Public:
		OnPlayerDisconnect - Called when a player leaves to remove them.
		OnPlayerConnect - Called when a player connects to add them.
	Core:
		-
	Stock:
		Iter_AddInternal - Add a value to an iterator.
		Iter_RemoveInternal - Remove a value from an iterator.
		Iter_RandomInternal - Get a random item from an iterator.
		Iter_FreeInternal - Gets the first free slot in the iterator.
		Iter_InitInternal - Initialises a multi-dimensional iterator.
		Iter_ContainsInternal - Checks if a value is in an iterator.
	Static:
		-
	Inline:
		Iter_Create - Create a new iterator value set.
		Iter_Add - Wraps Iter_AddInternal.
		Iter_SafeRemove - Wraps Iter_SafeRemoveInternal.
		Iter_Remove - Wraps Iter_RemoveInternal.
		Iter_Random - Wraps Iter_RandomInternal.
		Iter_Count - Gets the number of items in an iterator.
		Iter_Free - Wraps around Iter_FreeInternal.
		Iter_Contains - Wraps around Iter_ContainsInternal.
	API:
		-
Callbacks:
	-
Hooks:
	Iter_OnPlayerConnect - Hook for the OnPlayerConnect callback.
	Iter_OnPlayerDisconnect - Hook for the OnPlayerDisconnect callback.
	Iter_OnGameModeInit - Only exists to make the code compile correctly...
Definitions:
	-
Enums:
	-
Macros:
	-
Keywords:
	foreach - Function to loop an iterator.
Tags:
	Iterator - Declare an iterator.
Variables:
	Global:
		-
	Static:
		-
Commands:
	-
Operators:
	-
Iterators:
	Player - List of all players connected.
	Bot - List of all bots (npcs) connected.
	NPC - Alias of Bot.
	Character - All players and bots.
--------------------------------------------------------------------------------
*/

#define _FOREACH_LOCAL_VERSION 19

// Foreach is testing us.
#if defined _FOREACH_INC_TEST
	#endinput
#endif
#define _FOREACH_INC_TEST

#if !defined _samp_included
	#error "Please include a_samp or a_npc before foreach"
#endif

#if defined FOREACH_PLAYERSSTREAM_ITERATOR
	#error FOREACH_PLAYERSSTREAM_ITERATOR is deprecated, use FOREACH_I_PlayerPlayersStream
#endif

#if defined FOREACH_VEHICLESSTREAM_ITERATOR
	#error FOREACH_VEHICLESSTREAM_ITERATOR is deprecated, use FOREACH_I_PlayerVehiclesStream
#endif

#if defined FOREACH_ACTORSSTREAM_ITERATOR
	#error FOREACH_ACTORSSTREAM_ITERATOR is deprecated, use FOREACH_I_PlayerActorsStream
#endif

#if defined PlayersStream@YSII_Cg
	#error PlayersStream is deprecated, use PlayerPlayersStream
#endif

#if defined VehiclesStream@YSII_Cg
	#error VehiclesStream is deprecated, use PlayerVehiclesStream
#endif

#if defined ActorsStream@YSII_Cg
	#error ActorsStream is deprecated, use PlayerActorsStream
#endif

/*
--------------------------------------------------------------------------------
	Base defines
--------------------------------------------------------------------------------
*/

#define ITER_NONE -1

#define _Y_ITER_ARRAY: _:_Y_ITER_C0:
#define _Y_ITER_ARRAY_SIZE(%1) _:_Y_ITER_C1:_Y_ITER_C2:sizeof %1@YSII_Ag-1

#define _Y_ITER_C0:%0[%1]@YSII_%4g%3) %0@YSII_%4g[%1]%3)
#define _Y_ITER_C1:_Y_ITER_C2:%0[%1]@YSII_Ag%3) %0@YSII_Ag[]%3)
#define _Y_ITER_C2:sizeof%0(%1)@YSII_Ag-1;_:(%2=_Y_ITER_ARRAY:%3(%4)@YSII_Ag[%5])!=_Y_ITER_ARRAY_SIZE(%6);) -1;_:(%2=%3@YSII_Ag(%4,%5))!=-1;)
#define _Y_ITER_C3:%0[%1]@YSII_Cg,%2[%3]@YSII_Ag[%4]={%5} _Y_ITER_C3:%0@YSII_Cg[%1],%0@YSII_Ag[%1][%4]

forward Iter_AddInternal(&count, array[], value, size);
forward Iter_RemoveInternal(&count, array[], value, size);
forward Iter_ClearInternal(&count, array[], size);
forward Iter_OPDCInternal(playerid);

#if !defined FOREACH_I_Player
	#define FOREACH_I_Player 1
#endif

#if !defined FOREACH_I_Bot
	#define FOREACH_I_Bot 1
#endif

#if !defined FOREACH_I_Character
	#define FOREACH_I_Character 1
#endif

#if !defined FOREACH_I_Vehicle
	#define FOREACH_I_Vehicle 1
#endif

#if !defined FOREACH_I_Actor
	#define FOREACH_I_Actor 1
#endif

#if !defined FOREACH_I_PlayerPlayersStream
	#define FOREACH_I_PlayerPlayersStream 0
#else
	#undef FOREACH_I_PlayerPlayersStream
	#define FOREACH_I_PlayerPlayersStream 1
#endif

#if !defined FOREACH_I_PlayerVehiclesStream
	#define FOREACH_I_PlayerVehiclesStream 0
#else
	#undef FOREACH_I_PlayerVehiclesStream
	#define FOREACH_I_PlayerVehiclesStream 1
#endif

#if !defined FOREACH_I_VehiclePlayersStream
	#define FOREACH_I_VehiclePlayersStream 0
#else
	#undef FOREACH_I_VehiclePlayersStream
	#define FOREACH_I_VehiclePlayersStream 1
#endif

#if !defined FOREACH_I_PlayerActorsStream
	#define FOREACH_I_PlayerActorsStream 0
#else
	#undef FOREACH_I_PlayerActorsStream
	#define FOREACH_I_PlayerActorsStream 1
#endif

#if !defined FOREACH_I_ActorPlayersStream
	#define FOREACH_I_ActorPlayersStream 0
#else
	#undef FOREACH_I_ActorPlayersStream
	#define FOREACH_I_ActorPlayersStream 1
#endif

#if defined FOREACH_MULTISCRIPT
	#if FOREACH_I_Vehicle
		forward Iter_AddVehicle(vehicleid);
		forward Iter_RemoveVehicle(vehicleid);
	#endif

	#if FOREACH_I_Actor
		forward Iter_AddActor(actorid);
		forward Iter_RemoveActor(actorid);
	#endif
#endif

/*
--------------------------------------------------------------------------------
Array:
	IteratorArray
Notes:
	Creates a new iterator array start/array pair.
--------------------------------------------------------------------------------
*/

#define IteratorArray:%1[%2]<%3> %1@YSII_Cg[%2],%1@YSII_Ag[%2][%3+1]

/*
--------------------------------------------------------------------------------
Array:
	Iterator
Notes:
	Creates a new iterator start/array pair.
--------------------------------------------------------------------------------
*/

#define Iterator:%1<%2> _Y_ITER_C3:%1@YSII_Cg,%1@YSII_Ag[(%2)+1]={(%2)*2,(%2)*2-1,...}
#define iterator%0<%1> new Iterator:%0<%1>

/*
--------------------------------------------------------------------------------
Function:
	Iter_Init
Params:
	iter - Name of the iterator array to initialise.
Return:
	-
Notes:
	Wrapper for Iter_InitInternal.

native Iter_Init(IteratorArray:Name[]<>);
--------------------------------------------------------------------------------
*/

#define Iter_Init(%1) \
	Iter_InitInternal(%1@YSII_Ag,sizeof %1@YSII_Ag,sizeof %1@YSII_Ag[]-1)

/*
--------------------------------------------------------------------------------
Function:
	Iter_Add
Params:
	iter - Name of the iterator to add the data to.
	value - Value to add to the iterator.
Return:
	-
Notes:
	Wrapper for Iter_AddInternal.

native Iter_Add(Iterator:Name<>, value);
--------------------------------------------------------------------------------
*/

#define Iter_Add(%1,%2) Iter_AddInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*
--------------------------------------------------------------------------------
Function:
	Iter_Free
Params:
	iter - Name of the iterator to get the first free slot in.
Return:
	-
Notes:
	Wrapper for Iter_FreeInternal.

native Iter_Free(Iterator:Name<>);
--------------------------------------------------------------------------------
*/

#define Iter_Free(%1) Iter_FreeInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*
--------------------------------------------------------------------------------
Function:
	Iter_Remove
Params:
	iter - Name of the iterator to remove data from.
	value - Data to remove.
Return:
	-
Notes:
	Wrapper for Iter_RemoveInternal.

native Iter_Remove(Iterator:Name<>, value);
--------------------------------------------------------------------------------
*/

#define Iter_Remove(%1,%2) Iter_RemoveInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*
--------------------------------------------------------------------------------
Function:
	Iter_Contains
Params:
	iter - Name of the iterator to check membership of.
	value - Value to check.
Return:
	-
Notes:
	Checks if the given value is in the given iterator.

native Iter_Remove(Iterator:Name<>, value);
--------------------------------------------------------------------------------
*/

#define Iter_Contains(%1,%2) Iter_ContainsInternal(_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*
--------------------------------------------------------------------------------
Function:
	Iter_SafeRemove
Params:
	iter - Name of the iterator to remove data from.
	value - Data to remove.
	next - Container for the pointer to the next element.
Return:
	-
Notes:
	Wrapper for Iter_SafeRemoveInternal. Common use:

	Iter_SafeRemove(iter, i, i);

native Iter_SafeRemove(Iterator:Name<>, value, &next);
--------------------------------------------------------------------------------
*/

#define Iter_SafeRemove(%1,%2,%3) Iter_SafeRemoveInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,%3,_Y_ITER_ARRAY_SIZE(%1))

/*
--------------------------------------------------------------------------------
Function:
	Iter_Random
Params:
	iter - Name of the iterator to get a random slot from.
Return:
	-
Notes:
	Wrapper for Iter_RandomInternal.

native Iter_Random(Iterator:Name<>);
--------------------------------------------------------------------------------
*/

#define Iter_Random(%1) Iter_RandomInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*
--------------------------------------------------------------------------------
Function:
	Iter_Count
Params:
	iter - Name of the iterator to get the number of items from.
Return:
	-
Notes:
	Returns the number of items in this iterator.

native Iter_Count(Iterator:Name<>);
--------------------------------------------------------------------------------
*/

#define Iter_Count(%1) (_Y_ITER_ARRAY:%1@YSII_Cg)

/*
--------------------------------------------------------------------------------
Function:
	Iter_Clear
Params:
	iter - Name of the iterator empty.
Return:
	-
Notes:
	Wrapper for Iter_ClearInternal.

native Iter_Clear(IteratorArray:Name[]<>);
--------------------------------------------------------------------------------
*/

#define Iter_Clear(%1) Iter_ClearInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*
--------------------------------------------------------------------------------
	Create the internal iterators.
--------------------------------------------------------------------------------
*/

#if FOREACH_I_Player
	new Iterator:Player<MAX_PLAYERS>;
#endif

#if FOREACH_I_Bot
	new Iterator:Bot<MAX_PLAYERS>;

	#define NPC@YSII_Cg Bot@YSII_Cg
	#define NPC@YSII_Ag Bot@YSII_Ag
#endif

#if FOREACH_I_Character
	new Iterator:Character<MAX_PLAYERS>;
#endif

#if FOREACH_I_Vehicle
	new Iterator:Vehicle<MAX_VEHICLES>;
#endif

#if FOREACH_I_Actor
	new Iterator:Actor<MAX_ACTORS>;
#endif

#if FOREACH_I_PlayerPlayersStream
	new Iterator:PlayerPlayersStream[MAX_PLAYERS]<MAX_PLAYERS>;
#endif

#if FOREACH_I_PlayerVehiclesStream
	new Iterator:PlayerVehiclesStream[MAX_PLAYERS]<MAX_VEHICLES>;
#endif

#if FOREACH_I_VehiclePlayersStream
	new Iterator:VehiclePlayersStream[MAX_VEHICLES]<MAX_PLAYERS>;
#endif

#if FOREACH_I_PlayerActorsStream
	new Iterator:PlayerActorsStream[MAX_PLAYERS]<MAX_ACTORS>;
#endif

#if FOREACH_I_ActorPlayersStream
	new Iterator:ActorPlayersStream[MAX_ACTORS]<MAX_PLAYERS>;
#endif

/*
--------------------------------------------------------------------------------
Function:
	foreach
Params:
	data - Data to iterate through.
	as - Variable to set value to.
Return:
	-
Notes:
	Not exactly the same as PHP foreach, just iterates through a list and
	returns the value of the current slot but uses that slot as the next index
	too. Variables must be in the form @YSII_<gname>S for the start index and
	@YSII_<gname>A for the data array where <name> is what's entered in data.
--------------------------------------------------------------------------------
*/

#define foreach%1(%0) for(new Y_FOREACH_SECOND|||Y_FOREACH_THIRD|||%0|||)
// This allows us to use "new" multiple times - stripping off ONLY whole words.
#define new%0|||%9|||%1:%2||| %9|||%0|||%1|||%2|||

// This one is called if the new syntax is required, but the state of "new" is
// as-yet unknown. This attempts to call "%1" as a macro, if it starts with
// "new" as a whole word then it will (and will also helpfully strip off the
// "new" keyword for us).
#define Y_FOREACH_THIRD|||%0|||%1|||%2||| %1=Y_FOREACH_FIFTH|||Y_FOREACH_FOURTH|||%1:%2|||

// This is called if the "new" macro is called for a second time.
#define Y_FOREACH_FOURTH|||%0=Y_FOREACH_FIFTH|||%1|||%2||| new Y_FOREACH_SIXTH;%0|||Y_FOREACH_SEVENTH|||%2|||

// This is called when there are tags on the "new" declaration.
#define Y_FOREACH_SEVENTH|||%9Y_FOREACH_SIXTH;%0|||%1|||%2||| new %0:%1=%0:(_Y_ITER_ARRAY_SIZE(%2));_:(%1=%0:_Y_ITER_ARRAY:%2@YSII_Ag[_:%1])!=_Y_ITER_ARRAY_SIZE(%2);

// This is called when there aren't.
#define Y_FOREACH_SIXTH;%0|||Y_FOREACH_SEVENTH|||%2||| %0=_Y_ITER_ARRAY_SIZE(%2);_:(%0=_Y_ITER_ARRAY:%2@YSII_Ag[%0])!=_Y_ITER_ARRAY_SIZE(%2);

// This is called if "%1" didn't have "new" at the start.
#define Y_FOREACH_FIFTH|||Y_FOREACH_FOURTH|||%1:%2||| _Y_ITER_ARRAY_SIZE(%2);_:(%1=_Y_ITER_ARRAY:%2@YSII_Ag[%1])!=_Y_ITER_ARRAY_SIZE(%2);

// This is the old version, but DON'T add "new" because that already exists from
// the failed "new" macro call above.
#define Y_FOREACH_SECOND|||Y_FOREACH_THIRD|||%1,%2||| %2=_Y_ITER_ARRAY_SIZE(%1);_:(%2=_Y_ITER_ARRAY:%1@YSII_Ag[%2])!=_Y_ITER_ARRAY_SIZE(%1);

/*
--------------------------------------------------------------------------------
Function:
	Iter_OnFilterScriptInit
Params:
	-
Return:
	-
Notes:
	Fixes a bug where callbacks are not detected when "loadfs" is used after the
	GM has already started. If this is a GM this is just never used called.
--------------------------------------------------------------------------------
*/

#if defined FILTERSCRIPT

	public OnFilterScriptInit()
	{
		OnGameModeInit();

		#if defined Iter_OnFilterScriptInit
			return Iter_OnFilterScriptInit();
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif

	#define OnFilterScriptInit Iter_OnFilterScriptInit
	#if defined Iter_OnFilterScriptInit
		forward Iter_OnFilterScriptInit();
	#endif

#endif

/*
--------------------------------------------------------------------------------
Function:
	Iter_OnGameModeInit
Params:
	-
Return:
	-
Notes:
	There are WIERD bugs in this script, seemingly caused by the compiler, so
	this hopefully fixes them. The OnGameModeInit code is written to be
	very fast by utilising the internal array structure instead of the regular
	Add functions.
--------------------------------------------------------------------------------
*/

public OnGameModeInit()
{
	#if defined GetPlayerPoolSize
		new
			LAST_PLAYER_ID = GetPlayerPoolSize() + 1,
			LAST_VEHICLE_ID = GetVehiclePoolSize() + 1,
			LAST_ACTOR_ID = GetActorPoolSize() + 1;
	#else
		#define LAST_PLAYER_ID  MAX_PLAYERS
		#define LAST_VEHICLE_ID MAX_VEHICLES
		#define LAST_ACTOR_ID   MAX_ACTORS
	#endif

	#if FOREACH_I_Player || FOREACH_I_Bot || FOREACH_I_Character
		#if FOREACH_I_Player
			Iter_Clear(Player);
		#endif

		#if FOREACH_I_Bot
			Iter_Clear(Bot);
		#endif

		#if FOREACH_I_Character
			Iter_Clear(Character);
		#endif

		for (new playerid = 0; playerid != LAST_PLAYER_ID; ++playerid) {
			if (!IsPlayerConnected(playerid)) {
				continue;
			}

			if (!IsPlayerNPC(playerid)) {
				#if FOREACH_I_Player
					Iter_Add(Player, playerid);
				#endif
			} else {
				#if FOREACH_I_Bot
					Iter_Add(Bot, playerid);
				#endif
			}

			#if FOREACH_I_Character
				Iter_Add(Character, playerid);
			#endif
		}
	#endif

	#if FOREACH_I_Vehicle
		Iter_Clear(Vehicle);

		for (new vehicleid = 1; vehicleid != LAST_VEHICLE_ID; ++vehicleid) {
			if (!GetVehicleModel(vehicleid)) {
				continue;
			}

			Iter_Add(Vehicle, vehicleid);
		}
	#endif

	#if FOREACH_I_Actor
		Iter_Clear(Actor);

		for (new actorid = 0; actorid != LAST_ACTOR_ID; ++actorid) {
			if (!IsValidActor(actorid)) {
				continue;
			}

			Iter_Add(Actor, actorid);
		}
	#endif

	#if FOREACH_I_PlayerPlayersStream
		Iter_Init(PlayerPlayersStream);

		for (new playerid = 0; playerid != LAST_PLAYER_ID; ++playerid) {
			if (!IsPlayerConnected(playerid)) {
				continue;
			}

			for (new targetid = 0; targetid != LAST_PLAYER_ID; ++targetid) {
				if (!IsPlayerStreamedIn(playerid, targetid)) {
					continue;
				}

				Iter_Add(PlayerPlayersStream[playerid], targetid);
			}
		}
	#endif

	#if FOREACH_I_PlayerVehiclesStream || FOREACH_I_VehiclePlayersStream
		#if FOREACH_I_PlayerVehiclesStream
			Iter_Init(PlayerVehiclesStream);
		#endif

		#if FOREACH_I_VehiclePlayersStream
			Iter_Init(VehiclePlayersStream);
		#endif

		for (new playerid = 0; playerid != LAST_PLAYER_ID; ++playerid) {
			if (!IsPlayerConnected(playerid)) {
				continue;
			}

			for (new vehicleid = 1; vehicleid != LAST_VEHICLE_ID; ++vehicleid) {
				if (!IsVehicleStreamedIn(vehicleid, playerid)) {
					continue;
				}

				#if FOREACH_I_PlayerVehiclesStream
					Iter_Add(PlayerVehiclesStream[playerid], vehicleid);
				#endif

				#if FOREACH_I_VehiclePlayersStream
					Iter_Add(VehiclePlayersStream[vehicleid], playerid);
				#endif
			}
		}
	#endif

	#if FOREACH_I_PlayerActorsStream || FOREACH_I_ActorPlayersStream
		#if FOREACH_I_PlayerActorsStream
			Iter_Init(PlayerActorsStream);
		#endif

		#if FOREACH_I_ActorPlayersStream
			Iter_Init(ActorPlayersStream);
		#endif

		for (new playerid = 0; playerid != LAST_PLAYER_ID; ++playerid) {
			if (!IsPlayerConnected(playerid)) {
				continue;
			}

			for (new actorid = 0; actorid != LAST_ACTOR_ID; ++actorid) {
				if (!IsActorStreamedIn(actorid, playerid)) {
					continue;
				}

				#if FOREACH_I_PlayerActorsStream
					Iter_Add(PlayerActorsStream[playerid], actorid);
				#endif

				#if FOREACH_I_ActorPlayersStream
					Iter_Add(ActorPlayersStream[actorid], playerid);
				#endif
			}
		}
	#endif

	#pragma unused LAST_PLAYER_ID, LAST_VEHICLE_ID, LAST_ACTOR_ID

	#if defined Iter_OnGameModeInit
		return Iter_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Iter_OnGameModeInit
#if defined Iter_OnGameModeInit
	forward Iter_OnGameModeInit();
#endif

/*
	CHARACTERS
*/

#if FOREACH_I_Player || FOREACH_I_Bot || FOREACH_I_Character

	/*
	--------------------------------------------------------------------------------
	Function:
		Iter_OnPlayerConnect
	Params:
		playerid - Player who joined.
	Return:
		-
	Notes:
		Adds a player to the loop data. Now sorts the list too. Note that I found
		the most bizzare bug ever (I *think* it may be a compiler but, but it
		requires further investigation), basically it seems that multiple variables
		were being treated as the same variable (namely @YSII_EgotS and
		@YSII_CgharacterS were the same and @YSII_EgotC and @YSII_CgharacterC were the
		same). Adding print statements which reference these variables seem to fix
		the problem, and I have tried to make sure that the values will never actually
		get printed.
	--------------------------------------------------------------------------------
	*/

	public OnPlayerConnect(playerid)
	{
		if (!IsPlayerNPC(playerid)) {
			#if FOREACH_I_Player
				Iter_Add(Player, playerid);
			#endif
		} else {
			#if FOREACH_I_Bot
				Iter_Add(Bot, playerid);
			#endif
		}

		#if FOREACH_I_Character
			Iter_Add(Character, playerid);
		#endif

		#if defined Iter_OnPlayerConnect
			return Iter_OnPlayerConnect(playerid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif

	#define OnPlayerConnect Iter_OnPlayerConnect
	#if defined Iter_OnPlayerConnect
		forward Iter_OnPlayerConnect(playerid);
	#endif

	/*
	--------------------------------------------------------------------------------
	Function:
		Iter_OnPlayerDisconnect
	Params:
		playerid - Player who left.
	Return:
		-
	Notes:
		Removes a player from the loop data. No longer uses "hook" to ENSURE that
		this is always last. Previously I think that the order of evaluation in
		y_hooks meant that this got called before the user "OnPlayerDisconnect".
	--------------------------------------------------------------------------------
	*/

	public OnPlayerDisconnect(playerid, reason)
	{
	#if defined Iter_OnPlayerDisconnect
		Iter_OnPlayerDisconnect(playerid, reason);
	#endif

		SetTimerEx("Iter_OPDCInternal", 0, false, "i", playerid);
		return 1;
	}
	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif

	#define OnPlayerDisconnect Iter_OnPlayerDisconnect
	#if defined Iter_OnPlayerDisconnect
		forward Iter_OnPlayerDisconnect(playerid, reason);
	#endif

	/*
	--------------------------------------------------------------------------------
	Function:
		Iter_OPDCInternal
	Params:
		playerid - Player who left.
	Return:
		-
	Notes:
		Called AFTER "OnPlayerDisconnect" so that using "Kick" inside a "foreach"
		loop does not crash the server due to an OOB error.
	--------------------------------------------------------------------------------
	*/

	public Iter_OPDCInternal(playerid)
	{
		if (IsPlayerConnected(playerid)) {
			return;
		}

		if (!IsPlayerNPC(playerid)) {
			#if FOREACH_I_Player
				Iter_Remove(Player, playerid);
			#endif
		} else {
			#if FOREACH_I_Bot
				Iter_Remove(Bot, playerid);
			#endif
		}

		#if FOREACH_I_Character
			Iter_Remove(Character, playerid);
		#endif

		#if FOREACH_I_PlayerPlayersStream
			Iter_Clear(PlayerPlayersStream[playerid]);
		#endif

		#if FOREACH_I_PlayerVehiclesStream
			Iter_Clear(PlayerVehiclesStream[playerid]);
		#endif

		#if FOREACH_I_PlayerActorsStream
			Iter_Clear(PlayerActorsStream[playerid]);
		#endif
	}

#endif

/*
	CHARACTERS ENDS
*/

/*
	VEHICLES
*/

#if FOREACH_I_Vehicle

	/*
		Iter_CreateVehicle
	*/

	stock Iter_CreateVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2, respawn_delay, addsiren = 0)
	{
		new
			ret = CreateVehicle(modelid, x, y, z, angle, color1, color2, respawn_delay, addsiren);

		if (ret != INVALID_VEHICLE_ID) {
			#if defined FOREACH_MULTISCRIPT
				CallRemoteFunction("Iter_AddVehicle", "i", ret);
			#else
				Iter_Add(Vehicle, ret);
			#endif
		}

		return ret;
	}
	#if defined _ALS_CreateVehicle
		#undef CreateVehicle
	#else
		#define _ALS_CreateVehicle
	#endif

	#define CreateVehicle Iter_CreateVehicle

	/*
		Iter_AddStaticVehicle
	*/

	stock Iter_AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2)
	{
		new
			ret = AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, angle, color1, color2);

		if (ret != INVALID_VEHICLE_ID) {
			#if defined FOREACH_MULTISCRIPT
				CallRemoteFunction("Iter_AddVehicle", "i", ret);
			#else
				Iter_Add(Vehicle, ret);
			#endif
		}

		return ret;
	}
	#if defined _ALS_AddStaticVehicle
		#undef AddStaticVehicle
	#else
		#define _ALS_AddStaticVehicle
	#endif

	#define AddStaticVehicle Iter_AddStaticVehicle

	/*
		Iter_AddStaticVehicleEx
	*/

	stock Iter_AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2, respawn_delay, addsiren = 0)
	{
		new
			ret = AddStaticVehicleEx(modelid, spawn_x, spawn_y, spawn_z, angle, color1, color2, respawn_delay, addsiren);

		if (ret != INVALID_VEHICLE_ID) {
			#if defined FOREACH_MULTISCRIPT
				CallRemoteFunction("Iter_AddVehicle", "i", ret);
			#else
				Iter_Add(Vehicle, ret);
			#endif
		}

		return ret;
	}
	#if defined _ALS_AddStaticVehicleEx
		#undef AddStaticVehicleEx
	#else
		#define _ALS_AddStaticVehicleEx
	#endif

	#define AddStaticVehicleEx Iter_AddStaticVehicleEx

	/*
		DestroyVehicleSafe
	*/

	stock DestroyVehicleSafe(&vehicleid)
	{
		new success = DestroyVehicle(vehicleid);
		#if defined FOREACH_MULTISCRIPT
			vehicleid = CallRemoteFunction("Iter_RemoveVehicle", "i", vehicleid);
		#else
			Iter_SafeRemove(Vehicle, vehicleid, vehicleid);
		#endif
		return success;
	}

	/*
		Iter_DestroyVehicle
	*/

	stock Iter_DestroyVehicle(vehicleid)
	{
		#if defined FOREACH_MULTISCRIPT
			CallRemoteFunction("Iter_RemoveVehicle", "i", vehicleid);
		#else
			Iter_Remove(Vehicle, vehicleid);
		#endif
		return DestroyVehicle(vehicleid);
	}
	#if defined _ALS_DestroyVehicle
		#undef DestroyVehicle
	#else
		#define _ALS_DestroyVehicle
	#endif

	#define DestroyVehicle Iter_DestroyVehicle

	/*
		Iter_AddVehicle
	*/

	#if defined FOREACH_MULTISCRIPT

		public Iter_AddVehicle(vehicleid)
		{
			return Iter_Add(Vehicle, vehicleid);
		}

	#endif

	/*
		Iter_RemoveVehicle
	*/

	#if defined FOREACH_MULTISCRIPT

		public Iter_RemoveVehicle(vehicleid)
		{
			Iter_SafeRemove(Vehicle, vehicleid, vehicleid);
			return vehicleid;
		}

	#endif

#endif

/*
	VEHICLES ENDS
*/

/*
	ACTORS
*/

#if FOREACH_I_Actor

	/*
		Iter_CreateActor
	*/

	stock Iter_CreateActor(modelid, Float:X, Float:Y, Float:Z, Float:Rotation)
	{
		new
			ret = CreateActor(modelid, X, Y, Z, Rotation);
		if (ret != INVALID_ACTOR_ID) {
			#if defined FOREACH_MULTISCRIPT
				CallRemoteFunction("Iter_AddActor", "i", ret);
			#else
				Iter_Add(Actor, ret);
			#endif
		}
		return ret;
	}
	#if defined _ALS_CreateActor
		#undef CreateActor
	#else
		#define _ALS_CreateActor
	#endif

	#define CreateActor Iter_CreateActor

	/*
		DestroyActorSafe
	*/

	stock DestroyActorSafe(&actorid)
	{
		new success = DestroyActor(actorid);
		#if defined FOREACH_MULTISCRIPT
			actorid = CallRemoteFunction("Iter_RemoveActor", "i", actorid);
		#else
			Iter_SafeRemove(Actor, actorid, actorid);
		#endif
		return success;
	}

	/*
		Iter_DestroyActor
	*/

	stock Iter_DestroyActor(actorid)
	{
		#if defined FOREACH_MULTISCRIPT
			CallRemoteFunction("Iter_RemoveActor", "i", actorid);
		#else
			Iter_Remove(Actor, actorid);
		#endif
		return DestroyActor(actorid);
	}
	#if defined _ALS_DestroyActor
		#undef DestroyActor
	#else
		#define _ALS_DestroyActor
	#endif

	#define DestroyActor Iter_DestroyActor

	/*
		Iter_AddActor
	*/

	#if defined FOREACH_MULTISCRIPT

		public Iter_AddActor(actorid)
		{
			return Iter_Add(Actor, actorid);
		}

	#endif

	/*
		Iter_RemoveActor
	*/

	#if defined FOREACH_MULTISCRIPT

		public Iter_RemoveActor(actorid)
		{
			Iter_SafeRemove(Actor, actorid, actorid);
			return actorid;
		}

	#endif

#endif

/*
	ACTORS ENDS
*/

/*
	PLAYERS STREAM
*/

#if FOREACH_I_PlayerPlayersStream

	/*
		Iter_OnPlayerStreamIn
	*/

	public OnPlayerStreamIn(playerid, forplayerid)
	{
		Iter_Add(PlayerPlayersStream[playerid], forplayerid);

		#if defined Iter_OnPlayerStreamIn
			return Iter_OnPlayerStreamIn(playerid, forplayerid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnPlayerStreamIn
		#undef OnPlayerStreamIn
	#else
		#define _ALS_OnPlayerStreamIn
	#endif

	#define OnPlayerStreamIn Iter_OnPlayerStreamIn
	#if defined Iter_OnPlayerStreamIn
		forward Iter_OnPlayerStreamIn(playerid, forplayerid);
	#endif

	/*
		Iter_OnPlayerStreamOut
	*/

	public OnPlayerStreamOut(playerid, forplayerid)
	{
		Iter_Remove(PlayerPlayersStream[playerid], forplayerid);

		#if defined Iter_OnPlayerStreamOut
			return Iter_OnPlayerStreamOut(playerid, forplayerid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnPlayerStreamOut
		#undef OnPlayerStreamOut
	#else
		#define _ALS_OnPlayerStreamOut
	#endif

	#define OnPlayerStreamOut Iter_OnPlayerStreamOut
	#if defined Iter_OnPlayerStreamOut
		forward Iter_OnPlayerStreamOut(playerid, forplayerid);
	#endif

#endif

/*
	PLAYERS STREAM ENDS
*/

/*
	VEHICLES STREAM
*/

#if FOREACH_I_PlayerVehiclesStream || FOREACH_I_VehiclePlayersStream

	/*
		Iter_OnVehicleStreamIn
	*/

	public OnVehicleStreamIn(vehicleid, forplayerid)
	{
		#if FOREACH_I_PlayerVehiclesStream
			Iter_Add(PlayerVehiclesStream[forplayerid], vehicleid);
		#endif

		#if FOREACH_I_VehiclePlayersStream
			Iter_Add(VehiclePlayersStream[vehicleid], forplayerid);
		#endif

		#if defined Iter_OnVehicleStreamIn
			return Iter_OnVehicleStreamIn(vehicleid, forplayerid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnVehicleStreamIn
		#undef OnVehicleStreamIn
	#else
		#define _ALS_OnVehicleStreamIn
	#endif

	#define OnVehicleStreamIn Iter_OnVehicleStreamIn
	#if defined Iter_OnVehicleStreamIn
		forward Iter_OnVehicleStreamIn(vehicleid, forplayerid);
	#endif

	/*
		Iter_OnVehicleStreamOut
	*/

	public OnVehicleStreamOut(vehicleid, forplayerid)
	{
		#if FOREACH_I_PlayerVehiclesStream
			Iter_Remove(PlayerVehiclesStream[forplayerid], vehicleid);
		#endif

		#if FOREACH_I_VehiclePlayersStream
			Iter_Remove(VehiclePlayersStream[vehicleid], forplayerid);
		#endif

		#if defined Iter_OnVehicleStreamOut
			return Iter_OnVehicleStreamOut(vehicleid, forplayerid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnVehicleStreamOut
		#undef OnVehicleStreamOut
	#else
		#define _ALS_OnVehicleStreamOut
	#endif

	#define OnVehicleStreamOut Iter_OnVehicleStreamOut
	#if defined Iter_OnVehicleStreamOut
		forward Iter_OnVehicleStreamOut(vehicleid, forplayerid);
	#endif

#endif

/*
	VEHICLES STREAM ENDS
*/

/*
	ACTORS STREAM
*/

#if FOREACH_I_PlayerActorsStream || FOREACH_I_ActorPlayersStream

	/*
		Iter_OnActorStreamIn
	*/

	public OnActorStreamIn(actorid, forplayerid)
	{
		#if FOREACH_I_PlayerActorsStream
			Iter_Add(PlayerActorsStream[forplayerid], actorid);
		#endif

		#if FOREACH_I_ActorPlayersStream
			Iter_Add(ActorPlayersStream[actorid], forplayerid);
		#endif

		#if defined Iter_OnActorStreamIn
			return Iter_OnActorStreamIn(actorid, forplayerid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnActorStreamIn
		#undef OnActorStreamIn
	#else
		#define _ALS_OnActorStreamIn
	#endif

	#define OnActorStreamIn Iter_OnActorStreamIn
	#if defined Iter_OnActorStreamIn
		forward Iter_OnActorStreamIn(actorid, forplayerid);
	#endif

	/*
		Iter_OnActorStreamOut
	*/

	public OnActorStreamOut(actorid, forplayerid)
	{
		#if FOREACH_I_PlayerActorsStream
			Iter_Remove(PlayerActorsStream[forplayerid], actorid);
		#endif

		#if FOREACH_I_ActorPlayersStream
			Iter_Remove(ActorPlayersStream[actorid], forplayerid);
		#endif

		#if defined Iter_OnActorStreamOut
			return Iter_OnActorStreamOut(actorid, forplayerid);
		#else
			return 1;
		#endif
	}
	#if defined _ALS_OnActorStreamOut
		#undef OnActorStreamOut
	#else
		#define _ALS_OnActorStreamOut
	#endif

	#define OnActorStreamOut Iter_OnActorStreamOut
	#if defined Iter_OnActorStreamOut
		forward Iter_OnActorStreamOut(actorid, forplayerid);
	#endif

#endif

/*
	ACTORS STREAM ENDS
*/

/*
--------------------------------------------------------------------------------
Function:
	Iter_RandomInternal
Params:
	count - Number of items in the iterator.
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Returns a random value from an iterator.
--------------------------------------------------------------------------------
*/

stock Iter_RandomInternal(count, array[], size)
{
	if (count == 0) {
		return ITER_NONE;
	}
	new
		rnd = random(count),
		cur = array[size];
	while (cur != size) {
		if (rnd-- == 0) {
			return cur;
		}
		cur = array[cur];
	}
	return ITER_NONE;
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_FreeInternal
Params:
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Finds the first free slot in the iterator. Iterators now HAVE to be
	sorted for this function to work correctly as it uses that fact to decide
	whether a slot is unused or the last one. If you want to use the slot
	straight after finding it the iterator will need to re-find it to add in
	the data.
--------------------------------------------------------------------------------
*/

stock Iter_FreeInternal(array[], size)
{
	for (new i = 0; i != size; ++i) {
		if (array[i] > size) {
			return i;
		}
	}
	return ITER_NONE;
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_AddInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	value - Item to add.
	size - Size of the iterator.
Return:
	-
Notes:
	Adds a value to a given iterator set. Now detects when you try and add the
	last item multiple times, as well as all the other items. Now simplified even
	further with the new internal representation.
--------------------------------------------------------------------------------
*/

stock Iter_AddInternal(&count, array[], value, size)
{
	if (0 <= value < size && array[value] > size) {
		new
			last = size,
			next = array[last];
		while (next < value) {
			last = next;
			next = array[last];
		}
		array[last] = value;
		array[value] = next;
		++count;
		return 1;
	}
	return 0;
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_RemoveInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	value - Item to remove.
	size - Size of the iterator.
Return:
	-
Notes:
	Removes a value from an iterator.
--------------------------------------------------------------------------------
*/

stock Iter_RemoveInternal(&count, array[], value, size)
{
	new
		last;
	return Iter_SafeRemoveInternal(count, array, value, last, size);
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_SafeRemoveInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	value - Item to remove.
	&last - Pointer in which to store the last pointer.
	size - Size of the iterator.
Return:
	-
Notes:
	Removes a value from an iterator safely.
--------------------------------------------------------------------------------
*/

stock Iter_SafeRemoveInternal(&count, array[], value, &last, size)
{
	if (0 <= value < size && array[value] <= size) {
		last = size;
		new
			next = array[last];
		while (next < size) {
			if (next == value) {
				array[last] = array[value];
				array[value] = size + 1;
				--count;
				return 1;
			}
			last = next;
			next = array[last];
		}
	}
	return 0;
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_ContainsInternal
Params:
	array[] - Iterator data.
	value - Item to check.
	size - Size of the iterator.
Return:
	-
Notes:
	Checks if this item is in the iterator.
--------------------------------------------------------------------------------
*/

stock Iter_ContainsInternal(array[], value, size)
{
	return 0 <= value < size && array[value] <= size;
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_ClearInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Resets an iterator.
--------------------------------------------------------------------------------
*/

stock Iter_ClearInternal(&count, array[], size)
{
	for (new i = 0, t = size + 1; i < size; ++i) {
		array[i] = t;
	}
	array[size] = size;
	count = 0;
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_InitInternal
Params:
	array[][] - Iterator array to initialise.
	s0 - Size of first dimension.
	s1 - Size of second dimension.
Return:
	-
Notes:
	Multi-dimensional arrays can not be initialised at compile time, so need to be
	done at run time, which is slightly annoying.
--------------------------------------------------------------------------------
*/

stock Iter_InitInternal(arr[][], s0, s1)
{
	for (new i = 0, t = s1 + 1; i < s0; ++i) {
		for (new j = 0; j < s1; ++j) {
			arr[i][j] = t;
		}
		arr[i][s1] = s1;
	}
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_PrevInternal
Params:
	array[] - Iterator data.
	size - Size of the iterator.
	slot - The current slot.
Return:
	-
Notes:
	Gets the element in an iterator that points to the current element.
--------------------------------------------------------------------------------
*/

stock Iter_PrevInternal(array[], size, slot)
{
	if (0 <= slot <= size && array[slot] <= size) {
		for (new last = slot; last--; ) {
			if (array[last] == slot) {
				return last;
			}
		}
	}
	return size;
}

/*
--------------------------------------------------------------------------------
Function:
	Iter_Begin
Params:
	iter - Name of the iterator to get the start of.
Return:
	-
Notes:
	Gets a point BEFORE the start of the iterator (the theoretical beginning).
--------------------------------------------------------------------------------
*/

#define Iter_Begin(%1) (_Y_ITER_ARRAY_SIZE(%1))
#define Iter_Start Iter_Begin

/*
--------------------------------------------------------------------------------
Function:
	Iter_End
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets a point AFTER the end of the iterator (think "MAX_PLAYERS").
--------------------------------------------------------------------------------
*/

#define Iter_End(%1) (_Y_ITER_ARRAY_SIZE(%1))
#define Iter_Finish Iter_End

/*
--------------------------------------------------------------------------------
Function:
	Iter_First
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the first element in an iterator.
--------------------------------------------------------------------------------
*/

#define Iter_First(%1) (_Y_ITER_ARRAY:%1@YSII_Ag[_Y_ITER_ARRAY_SIZE(%1)])

/*
--------------------------------------------------------------------------------
Function:
	Iter_Last
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the last element in an iterator.
--------------------------------------------------------------------------------
*/

#define Iter_Last(%1) Iter_PrevInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1),_Y_ITER_ARRAY_SIZE(%1))

/*
--------------------------------------------------------------------------------
Function:
	Iter_Next
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the element in an interator after the current one.
--------------------------------------------------------------------------------
*/

#define Iter_Next(%1,%2) (_Y_ITER_ARRAY:%1@YSII_Ag[(%2)])

/*
--------------------------------------------------------------------------------
Function:
	Iter_Prev
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the element in an iterator before the current one. Slow.
--------------------------------------------------------------------------------
*/

#define Iter_Prev(%1,%2) Iter_PrevInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1),(%2))
#define Iter_Previous Iter_Prev
