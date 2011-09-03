/*----------------------------------------------------------------------------*-
					===========================
					 foreach efficient looping
					===========================
Description:
	Provides efficient looping through sparse data sets, such as connected
	players.  Significantly improved from the original version to be a generic
	loop system, rather then purely a player loop system.  When used for
	players this has constant time O(n) for number of connected players (n),
	unlike standard player loops which are O(MAX_PLAYERS), regardless of the
	actual number of connected players.  Even when n is MAX_PLAYERS this is
	still faster.
Legal:
	Copyright (C) 2009 Alex "Y_Less" Cole
	
	The contents of this file are subject to the Mozilla Public License Version
	1.1 (the "License"); you may not use this file except in compliance with
	the License. You may obtain a copy of the License at
	[url]http://www.mozilla.org/MPL/[/url]
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the SA:MP foreach iterator code.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
Version:
	0.1.8
Changelog:
	05/03/11:
		Added tag-free code.
		Updated to YSI y_iterate compatible code.
		Added self-fixing code for internal errors.
	16/08/10:
		Removed all the "2" versions of the functions.
	14/08/10:
		Added Iter_Clear to reset an array.
	06/08/10:
		Added special array declaration format.
	18/12/09:
		Added Itter_Func2 functions for multi-dimensional iterators.
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
		Itter_ShowArray - Displays the contents of the array.
		Itter_AddInternal - Add a value to an itterator.
		Itter_RemoveInternal - Remove a value from an itterator.
		Itter_RandomInternal - Get a random item from an itterator.
		Itter_FreeInternal - Gets the first free slot in the itterator.
		Itter_InitInternal - Initialises a multi-dimensional itterator.
	Static:
		-
	Inline:
		Itter_Create - Create a new itterator value set.
		Itter_Add - Wraps Itter_AddInternal.
		Itter_Remove - Wraps Itter_RemoveInternal.
		Itter_Random - Wraps Itter_RandomInternal.
		Itter_Count - Gets the number of items in an itterator.
		Itter_Debug - Wraps around Itter_ShowArray.
		Itter_Free - Wraps around Itter_FreeInternal.
		Itter_Create2 - Create a new itterator array value set.
		Itter_Add2 - Wraps Itter_AddInternal for arrays.
		Itter_Remove2 - Wraps Itter_RemoveInternal for arrays.
		Itter_Random2 - Wraps Itter_RandomInternal for arrays.
		Itter_Count2 - Gets the number of items in an itterator array.
		Itter_Debug2 - Wraps around Itter_ShowArray for arrays.
		Itter_Free2 - Wraps around Itter_FreeInternal for arrays.
	API:
		-
Callbacks:
	-
Hooks:
	Itter_OnPlayerConnect - Hook for the OnPlayerConnect callback.
	Itter_OnPlayerDisconnect - Hook for the OnPlayerDisconnect callback.
	Itter_OnGameModeInit - Only exists to make the code compile correctly...
Definitions:
	-
Enums:
	-
Macros:
	-
Keywords:
	foreach - Command to loop an iterator.
	foreachex - Like foreach but without a new variable.
	foreach2 - Command to loop through an iterator array.
	foreachex - Like foreach2 but without a new variable.
Tags:
	Iterator - Declare an iterator.
Variables:
	Global:
		-
	Static:
		YSI_g_OPC - Records wether Itter_OnPlayerConnect exists for speed.
		YSI_g_OPDC - Records wether Itter_OnPlayerDisconnect exists for speed.
Commands:
	-
Compile options:
	YSI_ITTER_NO_SORT - Removed.
	FOREACH_NO_BOTS - Remove the bot iterators for smaller code.
	FOREACH_NO_PLAYERS - Remove all default code for player itteration.
Operators:
	-
Iterators:
	Player - List of all players connected.
	Bot - List of all bots (npcs) connected.
	NPC - Alias of Bot.
	Character - All players and bots.
-*----------------------------------------------------------------------------*/

#define _FOREACH_LOCAL_VERSION        2

// Foreach is testing us.
#if defined _FOREACH_INC_TEST
	#define _FOREACH_CUR_VERSION _FOREACH_LOCAL_VERSION
	#endinput
#endif
#if !defined _FOREACH_NO_TEST
	#define _FOREACH_INC_TEST
	#tryinclude <YSI\y_iterate>
	#undef _FOREACH_INC_TEST
	// <y_iterate> exists - test which is newer.
	#if defined _inc_y_iterate
		#if !defined _FOREACH_CUR_VERSION
			// y_iterate exists, but it's an old version - don't try use this
			// system or the variables will conflict.
			#endinput
		#endif
		#if _FOREACH_CUR_VERSION > _FOREACH_LOCAL_VERSION
			// y_iterate is newer.
			#undef _inc_y_iterate
			#define _FOREACH_NO_TEST
			#include <YSI\y_iterate>
			#endinput
		#endif
	#endif
#endif

#if !defined _samp_included
	#error "Please include a_samp or a_npc before foreach"
#endif

#if defined SendChat || defined FOREACH_NO_PLAYERS
	#define BOTSYNC_IS_BOT     (true)
#endif

#if defined IsPlayerNPC
	#define _FOREACH_BOT
#endif

#if !defined BOTSYNC_IS_BOT
	static
		bool:YSI_g_OPC = false,
		bool:YSI_g_OPDC = false;
#endif

#if defined YSI_ITTER_NO_SORT
	#error YSI_ITTER_NO_SORT is no longer supported by foreach.
#endif

/*----------------------------------------------------------------------------*-
Function:
	Itter_Create2
Params:
	name - Itterator identifier.
	size0 - Number of iterators.
	size1 - Number of items per iterator.
Return:
	-
Notes:
	Creates a new array of itterator start/array pair.
-*----------------------------------------------------------------------------*/

#define Iter_Create2 Itter_Create2
#define Itter_Create2(%1,%2,%3) \
	new \
		YSI_gS%1[%2] = {-1, ...}, \
		YSI_gC%1[%2] = {0}, \
		YSI_gA%1[%2][%3]

#define IteratorArray:%1[%2]<%3> \
	YSI_gS%1[%2] = {-1, ...}, \
	YSI_gC%1[%2] = {0}, \
	YSI_gA%1[%2][%3]

/*----------------------------------------------------------------------------*-
Function:
	Itter_Init2
Params:
	itter - Name of the itterator array to initialise.
Return:
	-
Notes:
	Wrapper for Itter_InitInternal.

native Iter_Init(IteratorArray:Name[]<>);

-*----------------------------------------------------------------------------*/

#define Iter_Init Itter_Init
#define Itter_Init(%1) \
	Itter_InitInternal(YSI_gA%1, sizeof (YSI_gA%1), sizeof (YSI_gA%1[]))

/*----------------------------------------------------------------------------*-
Function:
	Itter_Create
Params:
	name - Itterator identifier.
	size - Number of values.
Return:
	-
Notes:
	Creates a new itterator start/array pair.
-*----------------------------------------------------------------------------*/

#define Iter_Create Itter_Create
#define Itter_Create(%1,%2) \
	new \
		YSI_gS%1 = -1, \
		YSI_gC%1 = 0, \
		YSI_gA%1[%2] = {-1, ...}

/*----------------------------------------------------------------------------*-
Array:
	Iterator
Notes:
	Creates a new itterator start/array pair.
-*----------------------------------------------------------------------------*/

#define Iterator:%1<%2> \
	YSI_gS%1 = -1, \
	YSI_gC%1 = 0, \
	YSI_gA%1[%2] = {-1, ...}

/*----------------------------------------------------------------------------*-
Function:
	Itter_Add
Params:
	itter - Name of the itterator to add the data to.
	value - Value to add to the itterator.
Return:
	-
Notes:
	Wrapper for Itter_AddInternal.

native Iter_Add(Iterator:Name<>, value);

-*----------------------------------------------------------------------------*/

#define Iter_Add Itter_Add
#define Itter_Add(%1,%2) \
	Itter_AddInternal(YSI_gS%1, YSI_gC%1, YSI_gA%1, %2)

/*----------------------------------------------------------------------------*-
Function:
	Itter_Free
Params:
	itter - Name of the itterator to get the first free slot in.
Return:
	-
Notes:
	Wrapper for Itter_FreeInternal.

native Iter_Free(Iterator:Name<>);

-*----------------------------------------------------------------------------*/

#define Iter_Free Itter_Free
#define Itter_Free(%1) \
	Itter_FreeInternal(YSI_gS%1, YSI_gC%1, YSI_gA%1, sizeof (YSI_gA%1))

/*----------------------------------------------------------------------------*-
Function:
	Itter_Remove
Params:
	itter - Name of the itterator to remove data from.
	value - Data to remove.
Return:
	-
Notes:
	Wrapper for Itter_RemoveInternal.

native Iter_Remove(Iterator:Name<>, value);

-*----------------------------------------------------------------------------*/

#define Iter_Remove Itter_Remove
#define Itter_Remove(%1,%2) \
	Itter_RemoveInternal(YSI_gS%1, YSI_gC%1, YSI_gA%1, %2)

/*----------------------------------------------------------------------------*-
Function:
	Itter_Random
Params:
	itter - Name of the itterator to get a random slot from.
Return:
	-
Notes:
	Wrapper for Itter_RandomInternal.

native Iter_Random(Iterator:Name<>);

-*----------------------------------------------------------------------------*/

#define Iter_Random Itter_Random
#define Itter_Random(%1) \
	Itter_RandomInternal(YSI_gS%1, YSI_gC%1, YSI_gA%1)

/*----------------------------------------------------------------------------*-
Function:
	Itter_Debug
Params:
	itter - Name of the itterator to output debug information from.
Return:
	-
Notes:
	Wrapper for Itter_ShowArray.
-*----------------------------------------------------------------------------*/

#define Iter_Debug Itter_Debug
#define Itter_Debug(%1) \
	Itter_ShowArray(YSI_gS%1, YSI_gA%1, YSI_gC%1)

/*----------------------------------------------------------------------------*-
Function:
	Itter_Count
Params:
	itter - Name of the itterator to get a random slot from4.
Return:
	-
Notes:
	Returns the number of items in this itterator.

native Iter_Count(Iterator:Name<>);

-*----------------------------------------------------------------------------*/

#define Iter_Count Itter_Count
#define Itter_Count(%1) \
	YSI_gC%1

/*----------------------------------------------------------------------------*-
Function:
	Itter_Clear
Params:
	itter - Name of the itterator empty.
Return:
	-
Notes:
	Wrapper for Itter_ClearInternal.

native Iter_Clear(IteratorArray:Name[]<>);

-*----------------------------------------------------------------------------*/

#define Iter_Clear Itter_Clear
#define Itter_Clear(%1) \
	Itter_ClearInternal(YSI_gS%1, YSI_gC%1, YSI_gA%1)

/*----------------------------------------------------------------------------*-
Create the internal itterators.
-*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	new
		Iterator:Player<MAX_PLAYERS>;
	
	#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
		new
			Iterator:Bot<MAX_PLAYERS>,
			Iterator:Character<MAX_PLAYERS>;
		
		#define YSI_gNPCS YSI_gBotS
		#define YSI_gNPCC YSI_gBotC
		#define YSI_gNPCA YSI_gBotA
	#endif
#endif

/*----------------------------------------------------------------------------*-
Function:
	foreach
Params:
	data - Data to itterate through.
	as - Variable to set value to.
Return:
	-
Notes:
	Not exactly the same as PHP foreach, just itterates through a list and
	returns the value of the current slot but uses that slot as the next index
	too.  Variables must be in the form YSI_g<name>S for the start index and
	YSI_g<name>A for the data array where <name> is what's entered in data.
-*----------------------------------------------------------------------------*/

#define foreach(%1,%2) \
	for (new %2 = YSI_gS%1; _:%2 != -1; %2 = YSI_gA%1[%2])

/*----------------------------------------------------------------------------*-
Function:
	foreachex
Params:
	data - Data to itterate through.
	as - Variable to set value to.
Return:
	-
Notes:
	Similar to foreach but doesn't declare a new variable for the itterator.
-*----------------------------------------------------------------------------*/

#define foreachex(%1,%2) \
	for (%2 = YSI_gS%1; _:%2 != -1; %2 = YSI_gA%1[%2])

/*----------------------------------------------------------------------------*-
Function:
	Itter_OnPlayerConnect
Params:
	playerid - Player who joined.
Return:
	-
Notes:
	Adds a player to the loop data.  Now sorts the list too.  Note that I found
	the most bizzare bug ever (I *think* it may be a compiler but, but it
	requires further investigation), basically it seems that multiple variables
	were being treated as the same variable (namely YSI_gBotS and
	YSI_gCharacterS were the same and YSI_gBotC and YSI_gCharacterC were the
	same).  Adding print statements which reference these variables seem to fix
	the problem, and I've tried to make sure that the values will never actually
	get printed.
-*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	public
		OnPlayerConnect(playerid)
	{
		#if defined _FOREACH_BOT
			if (!IsPlayerNPC(playerid))
			{
				Itter_Add(Player, playerid);
			}
			#if !defined FOREACH_NO_BOTS
				else
				{
					Itter_Add(Bot, playerid);
				}
				#pragma tabsize 4
				Itter_Add(Character, playerid);
			#endif
		#else
			Itter_Add(Player, playerid);
		#endif
		if (YSI_g_OPC)
		{
			return CallLocalFunction("Itter_OnPlayerConnect", "i", playerid);
		}
		return 1;
	}
	
	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect Itter_OnPlayerConnect
	
	forward
		Itter_OnPlayerConnect(playerid);
#endif

/*----------------------------------------------------------------------------*-
Function:
	Itter_OnGameModeInit
Params:
	-
Return:
	-
Notes:
	There are WIERD bugs in this script, seemingly caused by the compiler, so
	this hopefully fixes them.  The OnFilterScriptInit code is written to be
	very fast by utilising the internal array structure instead of the regular
	Add functions.
-*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	#if defined FILTERSCRIPT
		public
			OnFilterScriptInit()
		{
			if (YSI_gCPlayer)
			{
				print("foreach error: Something went wrong again!  Please tell Y_less");
				// Try reset.
				#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
					printf("%d", YSI_gSBot);
					printf("%d", YSI_gCBot);
					printf("%d", YSI_gSCharacter);
					printf("%d", YSI_gCCharacter);
					
				#endif
				printf("%d", YSI_gSPlayer);
				printf("%d", YSI_gCPlayer);
			}
			#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
				new
					lastBot = -1,
					lastCharacter = -1;
			#endif
			new
				lastPlayer = -1;
			for (new i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
				{
					#if defined _FOREACH_BOT
						if (!IsPlayerNPC(i))
						{
							if (lastPlayer == -1)
							{
								YSI_gSPlayer = i;
							}
							else
							{
								YSI_gAPlayer[lastPlayer] = i;
							}
							++YSI_gCPlayer;
							lastPlayer = i;
						}
						#if !defined FOREACH_NO_BOTS
							else
							{
								if (lastBot == -1)
								{
									YSI_gSBot = i;
								}
								else
								{
									YSI_gABot[lastBot] = i;
								}
								++YSI_gCBot;
								lastBot = i;
							}
							#pragma tabsize 4
							if (lastCharacter == -1)
							{
								YSI_gSCharacter = i;
							}
							else
							{
								YSI_gACharacter[lastCharacter] = i;
							}
							++YSI_gCCharacter;
							lastCharacter = i;
						#endif
					#else
						if (lastPlayer == -1)
						{
							YSI_gSPlayer = i;
						}
						else
						{
							YSI_gAPlayer[lastPlayer] = i;
						}
						++YSI_gCPlayer;
						lastPlayer = i;
					#endif
				}
			}
			YSI_g_OPC = (funcidx("Itter_OnPlayerConnect") != -1);
			YSI_g_OPDC = (funcidx("Itter_OnPlayerDisconnect") != -1);
			CallLocalFunction("Itter_OnFilterScriptInit", "");
		}
		
		#if defined _ALS_OnFilterScriptInit
			#undef OnFilterScriptInit
		#else
			#define _ALS_OnFilterScriptInit
		#endif
		#define OnFilterScriptInit Itter_OnFilterScriptInit
		
		forward Itter_OnFilterScriptInit();
	#else
		public
			OnGameModeInit()
		{
			if (YSI_gCPlayer)
			{
				print("foreach error: Something went wrong again!  Is this a Filterscript and have you");
				print("foreach error: got \"#define FILTERSCRIPT\" above all your includes?  Resetting...");
				#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
					printf("%d", YSI_gSBot);
					printf("%d", YSI_gCBot);
					printf("%d", YSI_gSCharacter);
					printf("%d", YSI_gCCharacter);
					YSI_gCBot = 0;
					YSI_gCCharacter = 0;
					YSI_gSBot = -1;
					YSI_gSCharacter = -1;
				#endif
				printf("%d", YSI_gSPlayer);
				printf("%d", YSI_gCPlayer);
				YSI_gCPlayer = 0;
				YSI_gSPlayer = -1;
				for (new i = 0; i != MAX_PLAYERS; ++i)
				{
					#if defined _FOREACH_BOT && !defined FOREACH_NO_BOTS
						YSI_gABot[i] = -1;
						YSI_gACharacter[i] = -1;
					#endif
					YSI_gAPlayer[i] = -1;
				}
			}
			YSI_g_OPC = (funcidx("Itter_OnPlayerConnect") != -1);
			YSI_g_OPDC = (funcidx("Itter_OnPlayerDisconnect") != -1);
			CallLocalFunction("Itter_OnGameModeInit", "");
		}
		
		#if defined _ALS_OnGameModeInit
			#undef OnGameModeInit
		#else
			#define _ALS_OnGameModeInit
		#endif
		#define OnGameModeInit Itter_OnGameModeInit
		
		forward
			Itter_OnGameModeInit();
	#endif
#endif

/*----------------------------------------------------------------------------*-
Function:
	Itter_OnPlayerDisconnect
Params:
	playerid - Player who left.
Return:
	-
Notes:
	Removes a player from the loop data.
-*----------------------------------------------------------------------------*/

#if !defined BOTSYNC_IS_BOT
	public
		OnPlayerDisconnect(playerid, reason)
	{
		#if defined _FOREACH_BOT
			if (!IsPlayerNPC(playerid))
			{
				Itter_Remove(Player, playerid);
			}
			#if !defined FOREACH_NO_BOTS
				else
				{
					Itter_Remove(Bot, playerid);
				}
				#pragma tabsize 4
				Itter_Remove(Character, playerid);
			#endif
		#else
			Itter_Remove(Player, playerid);
		#endif
		if (YSI_g_OPDC)
		{
			return CallLocalFunction("Itter_OnPlayerDisconnect", "ii", playerid, reason);
		}
		return 1;
	}
	
	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect Itter_OnPlayerDisconnect
	
	forward
		Itter_OnPlayerDisconnect(playerid, reason);
#endif

/*----------------------------------------------------------------------------*-
Function:
	Itter_ShowArray
Params:
	start - Itterator start point.
	members[] - Itterator contents.
	size - Number of itterator values
Return:
	-
Notes:
	Pure debug function.  Has regular prints not debug prints
	as it's only called when debug is on.
-*----------------------------------------------------------------------------*/

stock
	Itter_ShowArray(start, members[], size)
{
	static
		sString[61];
	new
		i,
		j = 10;
	printf("Start: %d", start);
	printf("Size:  %d", size);
	while (i < size)
	{
		sString[0] = '\0';
		while (i < j && i < size)
		{
			format(sString, sizeof (sString), "%s, %d", sString, members[i]);
			i++;
		}
		printf("Array (%d): %s", j, sString);
		j += 10;
	}
}

/*----------------------------------------------------------------------------*-
Function:
	Itter_RandomInternal
Params:
	start - Array start index.
	count - Number of items in the itterator.
	array[] - Itterator data.
Return:
	-
Notes:
	Returns a random value from an iterator.
-*----------------------------------------------------------------------------*/

stock
	Itter_RandomInternal(start, count, array[])
{
	if (count == 0)
	{
		return -1;
	}
	new
		rnd = random(count),
		cur = start;
	while (cur != -1)
	{
		if (rnd--)
		{
			cur = array[cur];
		}
		else
		{
			return cur;
		}
	}
	return -1;
}

/*----------------------------------------------------------------------------*-
Function:
	Itter_FreeInternal
Params:
	start - Array start index.
	count - Number of items in the itterator.
	array[] - Itterator data.
	size - Size of the itterator.
Return:
	-
Notes:
	Finds the first free slot in the itterator.  Itterators now HAVE to be
	sorted for this function to work correctly as it uses that fact to decide
	wether a slot is unused or the last one.  If you want to use the slot
	straight after finding it the itterator will need to re-find it to add in
	the data.
-*----------------------------------------------------------------------------*/

stock
	Itter_FreeInternal(start, count, array[], size)
{
	if (count == size)
	{
		return -1;
	}
	else if (count == 0)
	{
		return 0;
	}
	new
		first = 0;
	while (first != -1)
	{
		if (first == start)
		{
			start = array[start];
		}
		else if (array[first] == -1)
		{
			return first;
		}
		++first;
	}
	return -1;
}

/*----------------------------------------------------------------------------*-
Function:
	Itter_AddInternal
Params:
	&start - Array start index.
	&count - Number of items in the itterator.
	array[] - Itterator data.
	value - Item to add.
Return:
	-
Notes:
	Adds a value to a given itterator set.
-*----------------------------------------------------------------------------*/

stock
	Itter_AddInternal(&start, &count, array[], value)
{
	if (array[value] != -1)
	{
		return 0;
	}
	++count;
	if (start == -1)
	{
		start = value;
	}
	else if (start > value)
	{
		array[value] = start;
		start = value;
	}
	else
	{
		new
			cur = start,
			last;
		do
		{
			last = cur;
			cur = array[cur];
			if (cur > value)
			{
				array[value] = cur;
				array[last] = value;
				return 1;
			}
		}
		while (cur != -1);
		array[last] = value;
	}
	return 1;
}

/*----------------------------------------------------------------------------*-
Function:
	Itter_RemoveInternal
Params:
	&start - Array start index.
	&count - Number of items in the itterator.
	array[] - Itterator data.
	value - Item to remove.
Return:
	-
Notes:
	Removes a value from an itterator.
-*----------------------------------------------------------------------------*/

stock
	Itter_RemoveInternal(&start, &count, array[], value)
{
	if (start == -1)
	{
		return 0;
	}
	if (start == value)
	{
		start = array[value];
	}
	else
	{
		new
			cur = start;
		while (array[cur] != value)
		{
			cur = array[cur];
			if (cur == -1)
			{
				return 0;
			}
		}
		array[cur] = array[value];
	}
	array[value] = -1;
	--count;
	return 1;
}

/*----------------------------------------------------------------------------*-
Function:
	Itter_ClearInternal
Params:
	&start - Array start index.
	&count - Number of items in the itterator.
	array[] - Itterator data.
Return:
	-
Notes:
	Resets an iterator.
-*----------------------------------------------------------------------------*/

stock
	Itter_ClearInternal(&start, &count, array[])
{
	if (start != -1)
	{
		new
			cur = start,
			next = array[cur];
		start = -1;
		count = 0;
		while (next != -1)
		{
			array[cur] = -1;
			cur = next;
			next = array[cur];
		}
	}
}

/*----------------------------------------------------------------------------*-
Function:
	Itter_InitInternal
Params:
	array[][] - Itterator array to initialise.
	s0 - Size of first dimension.
	s1 - Size of second dimension.
Return:
	-
Notes:
	Multi-dimensional arrays can't be initialised at compile time, so need to be
	done at run time, which is slightly annoying.
-*----------------------------------------------------------------------------*/

stock
	Itter_InitInternal(arr[][], s0, s1)
{
	for (new i = 0; i != s0; ++i)
	{
		for (new j = 0; j != s1; ++j)
		{
			arr[i][j] = -1;
		}
	}
}