/*
 *  sscanf 2.8.2
 *  Created by Y_Less, updated by Emmet_.
 *
 *  Version: MPL 1.1
 *
 *  The contents of this file are subject to the Mozilla Public License Version
 *  1.1 (the "License"); you may not use this file except in compliance with
 *  the License. You may obtain a copy of the License at
 *  [url]http://www.mozilla.org/MPL/[/url]
 *
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 *
 *  The Original Code is the sscanf 2.0 SA:MP plugin.
 *
 *  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
 *  Portions created by the Initial Developer are Copyright (C) 2010
 *  the Initial Developer. All Rights Reserved.
 *
 *  Contributor(s):
 *
 *  Special Thanks to:
 *
 *      SA:MP Team past, present and future
 */

#if defined _inc_a_npc
	#pragma library sscanf
#elseif !defined _samp_included
	#error Please include <a_npc> or <a_samp> first.
#endif

#define SSCANF:%0(%1) forward sscanf_%0(%1);public sscanf_%0(%1)

#if defined sscanf
	#error sscanf (possibly the PAWN version) already defined.
#endif

native sscanf(const data[], const format[], {Float,_}:...);
native unformat(const data[], const format[], {Float,_}:...) = sscanf;
native SSCANF_Init(players, invalid, len);
native SSCANF_Join(playerid, const name[], npc);
native SSCANF_Leave(playerid);
native SSCANF_IsConnected(playerid);

native SSCANF_Option(const name[], value);

stock const
	SSCANF_QUIET[] = "SSCANF_QUIET",
	OLD_DEFAULT_NAME[] = "OLD_DEFAULT_NAME",
	MATCH_NAME_PARTIAL[] = "MATCH_NAME_PARTIAL",
	CELLMIN_ON_MATCHES[] = "CELLMIN_ON_MATCHES",
	OLD_DEFAULT_KUSTOM[] = "OLD_DEFAULT_KUSTOM",
	OLD_DEFAULT_CUSTOM[] = "OLD_DEFAULT_CUSTOM";

static stock
	bool:SSCANF_gInit = false,
	SSCANF_g_sPlayers[MAX_PLAYERS char];

#if defined _inc_a_npc
	forward SSCANF_PlayerCheck();

	/*
	  OnNPCModeInit

	  Called when the script starts if it is a NPC mode, sets up the system,
	  then calls the "real" OnNPCModeInit (using the new ALS 2 hook method).
	*/

	public OnNPCModeInit()
	{
		SSCANF_Init(MAX_PLAYERS, INVALID_PLAYER_ID, MAX_PLAYER_NAME + 1);
		#if !defined SSCANF_NO_PLAYERS
			// Initialise the system.
			SSCANF_PlayerCheck();
			SetTimer("SSCANF_PlayerCheck", 1, 1);
		#endif
		#if defined SSCANF_OnNPCModeInit
			SSCANF_OnNPCModeInit();
		#endif
		return 1;
	}

	#if defined _ALS_OnNPCModeInit
		#undef OnNPCModeInit
	#else
		#define _ALS_OnNPCModeInit
	#endif
	#define OnNPCModeInit SSCANF_OnNPCModeInit
	#if defined SSCANF_OnNPCModeInit
		forward SSCANF_OnNPCModeInit();
	#endif

	/*
	  SSCANF_PlayerCheck

	  NPC modes have no "OnPlayerConnect callback, so we need to simulate one.
	*/

	#if !defined SSCANF_NO_PLAYERS
		public SSCANF_PlayerCheck()
		{
			for (new i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
				{
					if (!SSCANF_g_sPlayers{i})
					{
						new
							name[MAX_PLAYER_NAME + 1];
						GetPlayerName(i, name, sizeof(name));
						// We have no way to know if they are an NPC or not!
						SSCANF_Join(i, name, 0);
						SSCANF_g_sPlayers{i} = 1;
					}
				}
				else
				{
					if (SSCANF_g_sPlayers{i})
					{
						SSCANF_Leave(i);
						SSCANF_g_sPlayers{i} = 0;
					}
				}
			}
		}
	#endif
#else
	/*
	  OnFilterScriptInit

	  Called when the script starts if it is a filterscript, sets up the system,
	  then calls the "real" OnFilterScriptInit (using the new ALS 2 hook
	  method).
	*/

	public OnFilterScriptInit()
	{
	    new
     		name[MAX_PLAYER_NAME + 1];

		SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, sizeof(name));
		SSCANF_gInit = true;

		// Check if there are any players that aren't initialized.
		for (new i = 0; i < MAX_PLAYERS; i ++)
		{
		    if (IsPlayerConnected(i) && !SSCANF_IsConnected(i))
		    {
		        GetPlayerName(i, name, sizeof(name));
		        SSCANF_Join(i, name, IsPlayerNPC(i));
		    }
		}

		#if defined SSCANF_OnFilterScriptInit
			SSCANF_OnFilterScriptInit();
		#endif
		return 1;
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit SSCANF_OnFilterScriptInit
	#if defined SSCANF_OnFilterScriptInit
		forward SSCANF_OnFilterScriptInit();
	#endif

	/*
	  OnGameModeInit

	  Called when the script starts if it is a gamemode.  This callback is also
	  called in filterscripts so we don't want to reinitialise the system in
	  that case.
	*/

	public OnGameModeInit()
	{
		if (!SSCANF_gInit)
		{
		    new
		        name[MAX_PLAYER_NAME + 1];

			SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, sizeof(name));
			SSCANF_gInit = true;

			// Check if there are any players that aren't initialized.
			for (new i = 0; i < MAX_PLAYERS; i ++)
			{
			    if (IsPlayerConnected(i) && !SSCANF_IsConnected(i))
			    {
			        GetPlayerName(i, name, sizeof(name));
			        SSCANF_Join(i, name, IsPlayerNPC(i));
			    }
			}
		}
		#if defined SSCANF_OnGameModeInit
			SSCANF_OnGameModeInit();
		#endif
		return 1;
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit SSCANF_OnGameModeInit
	#if defined SSCANF_OnGameModeInit
		forward SSCANF_OnGameModeInit();
	#endif

	/*
	  OnPlayerConnect

	  Called when a player connects.  Actually increments an internal count so
	  that if a script ends and "OnPlayerDisconnect" is called then "sscanf"
	  still knows that the player is really connected.  Also stores their name
	  internally.
	*/

	public OnPlayerConnect(playerid)
	{
		new
			name[MAX_PLAYER_NAME + 1];
		GetPlayerName(playerid, name, sizeof(name));
		SSCANF_Join(playerid, name, IsPlayerNPC(playerid));
		#if defined SSCANF_OnPlayerConnect
			SSCANF_OnPlayerConnect(playerid);
		#endif
		return 1;
	}

	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect SSCANF_OnPlayerConnect
	#if defined SSCANF_OnPlayerConnect
		forward SSCANF_OnPlayerConnect(playerid);
	#endif

	/*
	  OnPlayerDisconnect

	  Called when a player disconnects, or when a script is ended.
	*/

	public OnPlayerDisconnect(playerid, reason)
	{
		#if defined SSCANF_OnPlayerDisconnect
			SSCANF_OnPlayerDisconnect(playerid, reason);
		#endif
		SSCANF_Leave(playerid);
		return 1;
	}

	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect SSCANF_OnPlayerDisconnect
	#if defined SSCANF_OnPlayerDisconnect
		forward SSCANF_OnPlayerDisconnect(playerid, reason);
	#endif
#endif

#define SSCANF_Init
#define SSCANF_Join
#define SSCANF_Leave

#define extract%0->%1; EXTRN%1;unformat(_:EXTRZ:EXTRV:EXTRX:%0,##,%1,,);
#define unformat(_:EXTRZ:EXTRV:EXTRX:%0,##,%1);%2else if (unformat(_:EXTRV:EXTRX:%0,##,%1))

#define EXTRV:EXTRX:%0<%3>##,%9new%1,%2) EXTRY:%0##P<%3>,|||%1|||%2)
#define EXTRX:%0##,%9new%1,%2) EXTRY:%0##,|||%1|||%2)
#define EXTRY: EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:

#define EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3=%9|||%4) %6_EXTRO:%0##%1,%2|||%3=%9|||%4)
#define EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%3=%9|||%4) __EXTRO:%0##%1,%2|||%3=%9|||%4)
#define EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3[%7]|||%4) %6_EXTRW:%0##%1,%2|||%3[%7]|||%4)
#define EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%3[%7]|||%4) __EXTRW:%0##%1,%2|||%3|||%4)
#define EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3|||%4) %6_EXTRN:%0##%1,%2|||%3|||%4)
#define EXTR3:EXTR4:%0##%1,,%2||||||%4) %0##%1,%2)
#define EXTR4:%0##%1,%2|||%3|||%4) __EXTRN:%0##%1,%2|||%3|||%4)

// Optional specifiers.
#define __EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1I"("#%9")"#,%2,%3|||%4|||%5)
#define Float_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1F"("#%9")"#,%2,%3|||%4|||%5)
#define player_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1U"("#%9")"#,%2,%3|||%4|||%5)
#define string_EXTRO:%0##%1,%2|||%3[%7]=%9|||%4,%5) EXTRY:%0##%1S"("#%9")"#[%7],%2,%3|||%4|||%5)

// Normal specifiers (the double underscore is to work for "_:".
#define __EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1i,%2,%3|||%4|||%5)
#define Float_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1f,%2,%3|||%4|||%5)
#define player_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1u,%2,%3|||%4|||%5)
//#define string_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1s[%7],%2,%3|||%4|||%5)

// Array versions of normal specifiers.
#define __EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<i>[%7],%2,%3|||%4|||%5)
#define Float_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<f>[%7],%2,%3|||%4|||%5)
#define player_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<u>[%7],%2,%3|||%4|||%5)
#define string_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1s[%7],%2,%3|||%4|||%5)

// Get rid of excess leading space which causes warnings.
#define EXTRN%0new%1; new%1;

#if !defined string
    #define string:
#endif

#define player:

#define hex:%0;unformat(_:EXTRZ:EXTRX:%1); %0;unformat(_:EXTRZ:EXTRX:%1);
#define hex_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1H"("#%9")"#,%2,%3|||%4|||%5)
#define hex_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1h,%2,%3|||%4|||%5)
#define hex_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<h>[%7],%2,%3|||%4|||%5)

#define bin:
#define bin_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1B"("#%9")"#,%2,%3|||%4|||%5)
#define bin_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1b,%2,%3|||%4|||%5)
#define bin_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<b>[%7],%2,%3|||%4|||%5)

#define kustom:%0<%1> %0
#define kustom_EXTRO:%0##%1,%2|||%3<%8>=%9|||%4,%5) EXTRY:%0##%1K<%8>"("#%9")"#,%2,%3|||%4|||%5)
#define kustom_EXTRN:%0##%1,%2|||%3<%8>|||%4,%5) EXTRY:%0##%1k<%8>,%2,%3|||%4|||%5)
//#define bin_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<b>[%7],%2,%3|||%4|||%5)

SSCANF:weapon(string[])
{
	if ('0' <= string[0] <= '9') {
		new ret = strval(string);
		if (0 <= ret <= 18 || 22 <= ret <= 46) {
			return ret;
		}
	}

	static const
		weapon_ids[23][7] = {
			{30, -1}, // a
			{1, 5, -1}, // b
			{9, 15, 27, 43, -1}, // c
			{24, 40, -1}, // d
			{-1}, // e
			{42, 37, 14, 0, -1}, // f
			{2, 16, -1}, // g
			{-1}, // h
			{-1}, // i
			{-1}, // j
			{8, 4, 12, -1}, // k
			{-1}, // l
			{31, 28, 38, 36, 18, 29}, // m
			{3, 44, -1}, // n
			{-1}, // o
			{46, 22, 7, 10, -1}, // p
			{-1}, // q
			{33, 35, -1}, // r
			{39, 26, 25, 6, 23, 34, 41}, // s
			{17, 32, 45, -1}, // t
			{0, 28, -1}, // u
			{-1}, // v
			{11, 13, -1} // w
		},
		weapon_names[47][3][20] = {
			{"Fis", "Unarmed", -1}, // 0
			{"Bra", "BK", -1}, // 1
			{"Gol", "GC", -1}, // 2
			{"Nightstick", "NS", "Nit"}, // 3
			{"Kni", -1, -1}, // 4
			{"Bas", -1, -1}, // 5
			{"Show", -1, -1}, // 6
			{"Poo", "PC", -1}, // 7
			{"Kat", -1, -1}, // 8
			{"Cha", "CS", -1}, // 9
			{"Pur", "PD", -1}, // 10
			{"Dil", -1, -1}, // 11
			{"Vib", -1, -1}, // 12
			{"Silv", "SV", -1}, // 13
			{"Flo", -1, -1}, // 14
			{"Can", -1, -1}, // 15
			{"Gre", -1, -1}, // 16
			{"Tea", "TG", -1}, // 17
			{"Mol", "MC", -1}, // 18
			{-1, -1, -1},
			{-1, -1, -1},
			{-1, -1, -1},
			{"Pis", -1, -1}, // 22
			{"Sil", "SP", -1}, // 23
			{"De", -1, -1}, // 24
			{"Sho", "SG", -1}, // 25
			{"Saw", "SS", -1}, // 26
			{"Com", "CS", -1}, // 27
			{"Mic", "Uzi", -1}, // 28
			{"MP", -1, -1}, // 29
			{"AK", -1, -1}, // 30
			{"M4", -1, -1}, // 31
			{"Tec", -1, -1}, // 32
			{"Cou", "CR", "Rif"}, // 33
			{"Sni", "SR", -1}, // 34
			{"RPG", -1, -1}, // 35
			{"HS", "Heat", -1}, // 36
			{"Fla", "FT", -1}, // 37
			{"Min", -1, -1}, // 38
			{"Sac", "SC", -1}, // 39
			{"Det", -1, -1}, // 40
			{"Spr", -1, -1}, // 41
			{"Fir", "FE", -1}, // 42
			{"Cam", -1, -1}, // 43
			{"Nig", "NVG", -1}, // 44
			{"The", "TG", -1}, // 45
			{"Par", -1, -1} // 46
		};

	new
		i,
		j,
		weapon_id,
		id_index = tolower(string[0]) - 'a';

	if (!(0 <= id_index < sizeof(weapon_ids))) {
		return -1;
	}

	for (i = 0; i < sizeof(weapon_ids[]); i++) {
		weapon_id = weapon_ids[id_index][i];
		if (weapon_id == -1) {
			break;
		}

		for (j = 0; j < sizeof(weapon_names[]); j++) {
			if (weapon_names[weapon_id][j][0] == -1) {
				break;
			}

			if (!strcmp(string, weapon_names[weapon_id][j], true, strlen(weapon_names[weapon_id][j]))) {
				return weapon_id;
			}
		}
	}

	return -1;
}

SSCANF:vehicle(string[])
{
	// This function is VERY basic, needs VASTLY improving to detect variations.
	if ('0' <= string[0] <= '9')
	{
		new
			ret = strval(string);
		if (400 <= ret <= 611)
		{
			return ret;
		}
	}
	else if (!strcmp(string, "Landstalker", true)) return 400;
	else if (!strcmp(string, "Bravura", true)) return 401;
	else if (!strcmp(string, "Buffalo", true)) return 402;
	else if (!strcmp(string, "Linerunner", true)) return 403;
	else if (!strcmp(string, "Perenniel", true)) return 404;
	else if (!strcmp(string, "Sentinel", true)) return 405;
	else if (!strcmp(string, "Dumper", true)) return 406;
	else if (!strcmp(string, "Firetruck", true)) return 407;
	else if (!strcmp(string, "Trashmaster", true)) return 408;
	else if (!strcmp(string, "Stretch", true)) return 409;
	else if (!strcmp(string, "Manana", true)) return 410;
	else if (!strcmp(string, "Infernus", true)) return 411;
	else if (!strcmp(string, "Voodoo", true)) return 412;
	else if (!strcmp(string, "Pony", true)) return 413;
	else if (!strcmp(string, "Mule", true)) return 414;
	else if (!strcmp(string, "Cheetah", true)) return 415;
	else if (!strcmp(string, "Ambulance", true)) return 416;
	else if (!strcmp(string, "Leviathan", true)) return 417;
	else if (!strcmp(string, "Moonbeam", true)) return 418;
	else if (!strcmp(string, "Esperanto", true)) return 419;
	else if (!strcmp(string, "Taxi", true)) return 420;
	else if (!strcmp(string, "Washington", true)) return 421;
	else if (!strcmp(string, "Bobcat", true)) return 422;
	else if (!strcmp(string, "Mr Whoopee", true)) return 423;
	else if (!strcmp(string, "BF Injection", true)) return 424;
	else if (!strcmp(string, "Hunter", true)) return 425;
	else if (!strcmp(string, "Premier", true)) return 426;
	else if (!strcmp(string, "Enforcer", true)) return 427;
	else if (!strcmp(string, "Securicar", true)) return 428;
	else if (!strcmp(string, "Banshee", true)) return 429;
	else if (!strcmp(string, "Predator", true)) return 430;
	else if (!strcmp(string, "Bus", true)) return 431;
	else if (!strcmp(string, "Rhino", true)) return 432;
	else if (!strcmp(string, "Barracks", true)) return 433;
	else if (!strcmp(string, "Hotknife", true)) return 434;
	else if (!strcmp(string, "Article Trailer", true)) return 435;
	else if (!strcmp(string, "Previon", true)) return 436;
	else if (!strcmp(string, "Coach", true)) return 437;
	else if (!strcmp(string, "Cabbie", true)) return 438;
	else if (!strcmp(string, "Stallion", true)) return 439;
	else if (!strcmp(string, "Rumpo", true)) return 440;
	else if (!strcmp(string, "RC Bandit", true)) return 441;
	else if (!strcmp(string, "Romero", true)) return 442;
	else if (!strcmp(string, "Packer", true)) return 443;
	else if (!strcmp(string, "Monster", true)) return 444;
	else if (!strcmp(string, "Admiral", true)) return 445;
	else if (!strcmp(string, "Squallo", true)) return 446;
	else if (!strcmp(string, "Seasparrow", true)) return 447;
	else if (!strcmp(string, "Pizzaboy", true)) return 448;
	else if (!strcmp(string, "Tram", true)) return 449;
	else if (!strcmp(string, "Article Trailer 2", true)) return 450;
	else if (!strcmp(string, "Turismo", true)) return 451;
	else if (!strcmp(string, "Speeder", true)) return 452;
	else if (!strcmp(string, "Reefer", true)) return 453;
	else if (!strcmp(string, "Tropic", true)) return 454;
	else if (!strcmp(string, "Flatbed", true)) return 455;
	else if (!strcmp(string, "Yankee", true)) return 456;
	else if (!strcmp(string, "Caddy", true)) return 457;
	else if (!strcmp(string, "Solair", true)) return 458;
	else if (!strcmp(string, "Berkley's RC Van", true)) return 459;
	else if (!strcmp(string, "Skimmer", true)) return 460;
	else if (!strcmp(string, "PCJ-600", true)) return 461;
	else if (!strcmp(string, "Faggio", true)) return 462;
	else if (!strcmp(string, "Freeway", true)) return 463;
	else if (!strcmp(string, "RC Baron", true)) return 464;
	else if (!strcmp(string, "RC Raider", true)) return 465;
	else if (!strcmp(string, "Glendale", true)) return 466;
	else if (!strcmp(string, "Oceanic", true)) return 467;
	else if (!strcmp(string, "Sanchez", true)) return 468;
	else if (!strcmp(string, "Sparrow", true)) return 469;
	else if (!strcmp(string, "Patriot", true)) return 470;
	else if (!strcmp(string, "Quad", true)) return 471;
	else if (!strcmp(string, "Coastguard", true)) return 472;
	else if (!strcmp(string, "Dinghy", true)) return 473;
	else if (!strcmp(string, "Hermes", true)) return 474;
	else if (!strcmp(string, "Sabre", true)) return 475;
	else if (!strcmp(string, "Rustler", true)) return 476;
	else if (!strcmp(string, "ZR-350", true)) return 477;
	else if (!strcmp(string, "Walton", true)) return 478;
	else if (!strcmp(string, "Regina", true)) return 479;
	else if (!strcmp(string, "Comet", true)) return 480;
	else if (!strcmp(string, "BMX", true)) return 481;
	else if (!strcmp(string, "Burrito", true)) return 482;
	else if (!strcmp(string, "Camper", true)) return 483;
	else if (!strcmp(string, "Marquis", true)) return 484;
	else if (!strcmp(string, "Baggage", true)) return 485;
	else if (!strcmp(string, "Dozer", true)) return 486;
	else if (!strcmp(string, "Maverick", true)) return 487;
	else if (!strcmp(string, "SAN News Maverick", true)) return 488;
	else if (!strcmp(string, "Rancher", true)) return 489;
	else if (!strcmp(string, "FBI Rancher", true)) return 490;
	else if (!strcmp(string, "Virgo", true)) return 491;
	else if (!strcmp(string, "Greenwood", true)) return 492;
	else if (!strcmp(string, "Jetmax", true)) return 493;
	else if (!strcmp(string, "Hotring Racer", true)) return 494;
	else if (!strcmp(string, "Sandking", true)) return 495;
	else if (!strcmp(string, "Blista Compact", true)) return 496;
	else if (!strcmp(string, "Police Maverick", true)) return 497;
	else if (!strcmp(string, "Boxville", true)) return 498;
	else if (!strcmp(string, "Benson", true)) return 499;
	else if (!strcmp(string, "Mesa", true)) return 500;
	else if (!strcmp(string, "RC Goblin", true)) return 501;
	else if (!strcmp(string, "Hotring Racer", true)) return 502;
	else if (!strcmp(string, "Hotring Racer", true)) return 503;
	else if (!strcmp(string, "Bloodring Banger", true)) return 504;
	else if (!strcmp(string, "Rancher", true)) return 505;
	else if (!strcmp(string, "Super GT", true)) return 506;
	else if (!strcmp(string, "Elegant", true)) return 507;
	else if (!strcmp(string, "Journey", true)) return 508;
	else if (!strcmp(string, "Bike", true)) return 509;
	else if (!strcmp(string, "Mountain Bike", true)) return 510;
	else if (!strcmp(string, "Beagle", true)) return 511;
	else if (!strcmp(string, "Cropduster", true)) return 512;
	else if (!strcmp(string, "Stuntplane", true)) return 513;
	else if (!strcmp(string, "Tanker", true)) return 514;
	else if (!strcmp(string, "Roadtrain", true)) return 515;
	else if (!strcmp(string, "Nebula", true)) return 516;
	else if (!strcmp(string, "Majestic", true)) return 517;
	else if (!strcmp(string, "Buccaneer", true)) return 518;
	else if (!strcmp(string, "Shamal", true)) return 519;
	else if (!strcmp(string, "Hydra", true)) return 520;
	else if (!strcmp(string, "FCR-900", true)) return 521;
	else if (!strcmp(string, "NRG-500", true)) return 522;
	else if (!strcmp(string, "HPV1000", true)) return 523;
	else if (!strcmp(string, "Cement Truck", true)) return 524;
	else if (!strcmp(string, "Towtruck", true)) return 525;
	else if (!strcmp(string, "Fortune", true)) return 526;
	else if (!strcmp(string, "Cadrona", true)) return 527;
	else if (!strcmp(string, "FBI Truck", true)) return 528;
	else if (!strcmp(string, "Willard", true)) return 529;
	else if (!strcmp(string, "Forklift", true)) return 530;
	else if (!strcmp(string, "Tractor", true)) return 531;
	else if (!strcmp(string, "Combine Harvester", true)) return 532;
	else if (!strcmp(string, "Feltzer", true)) return 533;
	else if (!strcmp(string, "Remington", true)) return 534;
	else if (!strcmp(string, "Slamvan", true)) return 535;
	else if (!strcmp(string, "Blade", true)) return 536;
	else if (!strcmp(string, "Freight (Train)", true)) return 537;
	else if (!strcmp(string, "Brownstreak (Train)", true)) return 538;
	else if (!strcmp(string, "Vortex", true)) return 539;
	else if (!strcmp(string, "Vincent", true)) return 540;
	else if (!strcmp(string, "Bullet", true)) return 541;
	else if (!strcmp(string, "Clover", true)) return 542;
	else if (!strcmp(string, "Sadler", true)) return 543;
	else if (!strcmp(string, "Firetruck LA", true)) return 544;
	else if (!strcmp(string, "Hustler", true)) return 545;
	else if (!strcmp(string, "Intruder", true)) return 546;
	else if (!strcmp(string, "Primo", true)) return 547;
	else if (!strcmp(string, "Cargobob", true)) return 548;
	else if (!strcmp(string, "Tampa", true)) return 549;
	else if (!strcmp(string, "Sunrise", true)) return 550;
	else if (!strcmp(string, "Merit", true)) return 551;
	else if (!strcmp(string, "Utility Van", true)) return 552;
	else if (!strcmp(string, "Nevada", true)) return 553;
	else if (!strcmp(string, "Yosemite", true)) return 554;
	else if (!strcmp(string, "Windsor", true)) return 555;
	else if (!strcmp(string, "Monster \"A\"", true)) return 556;
	else if (!strcmp(string, "Monster \"B\"", true)) return 557;
	else if (!strcmp(string, "Uranus", true)) return 558;
	else if (!strcmp(string, "Jester", true)) return 559;
	else if (!strcmp(string, "Sultan", true)) return 560;
	else if (!strcmp(string, "Stratum", true)) return 561;
	else if (!strcmp(string, "Elegy", true)) return 562;
	else if (!strcmp(string, "Raindance", true)) return 563;
	else if (!strcmp(string, "RC Tiger", true)) return 564;
	else if (!strcmp(string, "Flash", true)) return 565;
	else if (!strcmp(string, "Tahoma", true)) return 566;
	else if (!strcmp(string, "Savanna", true)) return 567;
	else if (!strcmp(string, "Bandito", true)) return 568;
	else if (!strcmp(string, "Freight Flat Trailer (Train)", true)) return 569;
	else if (!strcmp(string, "Streak Trailer (Train)", true)) return 570;
	else if (!strcmp(string, "Kart", true)) return 571;
	else if (!strcmp(string, "Mower", true)) return 572;
	else if (!strcmp(string, "Dune", true)) return 573;
	else if (!strcmp(string, "Sweeper", true)) return 574;
	else if (!strcmp(string, "Broadway", true)) return 575;
	else if (!strcmp(string, "Tornado", true)) return 576;
	else if (!strcmp(string, "AT400", true)) return 577;
	else if (!strcmp(string, "DFT-30", true)) return 578;
	else if (!strcmp(string, "Huntley", true)) return 579;
	else if (!strcmp(string, "Stafford", true)) return 580;
	else if (!strcmp(string, "BF-400", true)) return 581;
	else if (!strcmp(string, "Newsvan", true)) return 582;
	else if (!strcmp(string, "Tug", true)) return 583;
	else if (!strcmp(string, "Petrol Trailer", true)) return 584;
	else if (!strcmp(string, "Emperor", true)) return 585;
	else if (!strcmp(string, "Wayfarer", true)) return 586;
	else if (!strcmp(string, "Euros", true)) return 587;
	else if (!strcmp(string, "Hotdog", true)) return 588;
	else if (!strcmp(string, "Club", true)) return 589;
	else if (!strcmp(string, "Freight Box Trailer (Train)", true)) return 590;
	else if (!strcmp(string, "Article Trailer 3", true)) return 591;
	else if (!strcmp(string, "Andromada", true)) return 592;
	else if (!strcmp(string, "Dodo", true)) return 593;
	else if (!strcmp(string, "RC Cam", true)) return 594;
	else if (!strcmp(string, "Launch", true)) return 595;
	else if (!strcmp(string, "Police Car (LSPD)", true)) return 596;
	else if (!strcmp(string, "Police Car (SFPD)", true)) return 597;
	else if (!strcmp(string, "Police Car (LVPD)", true)) return 598;
	else if (!strcmp(string, "Police Ranger", true)) return 599;
	else if (!strcmp(string, "Picador", true)) return 600;
	else if (!strcmp(string, "S.W.A.T.", true)) return 601;
	else if (!strcmp(string, "Alpha", true)) return 602;
	else if (!strcmp(string, "Phoenix", true)) return 603;
	else if (!strcmp(string, "Glendale Shit", true)) return 604;
	else if (!strcmp(string, "Sadler Shit", true)) return 605;
	else if (!strcmp(string, "Baggage Trailer \"A\"", true)) return 606;
	else if (!strcmp(string, "Baggage Trailer \"B\"", true)) return 607;
	else if (!strcmp(string, "Tug Stairs Trailer", true)) return 608;
	else if (!strcmp(string, "Boxville", true)) return 609;
	else if (!strcmp(string, "Farm Trailer", true)) return 610;
	else if (!strcmp(string, "Utility Trailer", true)) return 611;
	return -1;
}

// Fix the compiler crash when both the PAWN and Plugin versions of sscanf are
// found by renaming the old version at declaration.  (fixes.inc compatible
// naming scheme: "BAD_Function()").
#define sscanf(%0:...) BAD_sscanf(%0:...)
