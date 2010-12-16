/*
Project Name:	San Andreas - Multiplayer: Grand Theft Online (GTO).

Current version:	0.6.4
SA-MP Versions:		0.3b R2

Web site:	http://gto.zziggi.ru/

Date Created:		12 August 2006
Date start edit: 	5 November 2009(by Russian Scripter's)

Created by: 	Iain Gilbert (Bogart)
Continued by: 	Peter Steenbergen (j1nx)
				Robin Kikkert (Dejavu)
				Lajos Pacsek (asturel)
				Dmitry Frolov (FP)
				GTO-Rus Team(®): GhostTT, heufix, Elbi, ZiGGi

*/

#include <..\compiler\includes\a_samp>		// samp
#include "base"								// holds base script values
#include "utils\gtoutils"					// misc used utils
#include "utils\mxINI"
#include "logging"							// logging handler
#include "lang"
#include "account"							// account handler
#include "player"							// holds player values
#include "weapons"							// weapons and ammunation shop
#include "vehicles"							// vehicles
#include "world"							// functions for zone, location, world, etc
#include "commands"							// command handler
#include "gang"								// gang handler
#include "housing"							// housing handler
#include "business"							// business handler
#include "race"								// race handler, manages and runs all rasces
#include "deathmatch"						// deathmatch handler
#include "bank"								// bank money to keep it on death
#include "payday" 							// pay players money based on level
#include "groundhold"						// hold ground to gain money, pirate ship, etc
#include "admin\admin_func"
#include "admin\mod_func"
#include "admin\admin_commands"				// admin commands
#include "admin\admin_commands_race"		// admin commands for race creation/minipulation
#include "admin\admin_commands_dm"			// admin commands for deathmatch creation/minipulation
#include "admin\admin_commands_sys"
#include "admin\adm_commands"
#include "admin\mod_commands"
#include "streamers\mapicon_stream"
#include "misc\mission"
#include "protections\antiidle"
#include "protections\antirconhack"
#include "protections\antihealthhack"
#include "protections\antimoneyhack"
#include "protections\antihightping"
#include "protections\antivehicle"
//#include "testserver"
#include "click"
#include "fightstyles"
// Races
#tryinclude "races\race_monstertruck"		// Race: Monstertruck Madness
#tryinclude "races\race_thestrip"			// Race: Burnin Down The Strip. - Just a strait sprint down the strip and back
#tryinclude "races\race_riversiderun"		// Race: Riverside Run. - Beat the clock down a riverside dirt track.
#tryinclude "races\race_fleethecity"		// Race: Flee The City. - race to air strip
#tryinclude "races\race_lostinsmoke"		// Race: Lost in Smoke - a quick race around the city.
#tryinclude "races\race_backstreetbang"		// Race: Backstreet bang
#tryinclude "races\race_flyingfree"			// Race: Flying Free
#tryinclude "races\race_murderhorn"			// Race: Murderhorn - by |Insane|
#tryinclude "races\race_roundwego"			// Race: Airport Round We Go - by |Insane|
#tryinclude "races\race_striptease"			// Race: Strip Tease - by |Insane|
#tryinclude "races\race_countrycruise"		// Race: Countryside cruise
#tryinclude "races\race_thegrove"			// Race: Tearin Up The Grove
#tryinclude "races\race_mullholland"		// Race: Mullholland Getaway
#tryinclude "races\race_lv_julius"   		// RACE: BIG CIRCLE OF 3
#tryinclude "races\race_ls_trailer1"        // RACE: Trailers race
#tryinclude "races\race_sf_fuckinwood1"
#tryinclude "races\race_ls_majestic1"

// Deathmatches
#tryinclude "deathmatches\dm_air"			// Deathmatch - Base Jump
#tryinclude "deathmatches\dm_area51"		// Deathmatch - Area 51
#tryinclude "deathmatches\dm_badandugly"	// Deathmatch - The Bad and the Ugly
#tryinclude "deathmatches\dm_bluemountains"	// Deathmatch - Blue Mountains
#tryinclude "deathmatches\dm_cargoship"		// Deathmatch - Cargo Ship
#tryinclude "deathmatches\dm_dildo"			// Deathmatch - Dildo Farm Revenge
#tryinclude "deathmatches\dm_mbase"			// Deathmatch - Millitary Base
#tryinclude "deathmatches\dm_minigunmadness"// Deathmatch - Minigun Madness
#tryinclude "deathmatches\dm_poolday"		// Deathmatch - fy_poolday
#tryinclude "deathmatches\dm_usnavy"		// Deathmatch - The US Navy

main()
{	// CREDITS NOT DELETE !!!!!!!!!!!!!!!!!
	print(" ");
	print(" ");
	print("\n-----------------------------------------------------------\n");
	print("Running GTO "#VERSION"\n");
	print("_____________________________________________________________\n");
	print("Created by: Iain Gilbert (Bogart)\nContinued by: Peter Steenbergen (j1nx)\n\t\tRobin Kikkert (Dejavu)\n\t\tLajos Pacsek (asturel)");
	print("\t\tDmitry Frolov (FP)\nGTO-Rus Team(®): GhostTT, heufix, Elbi, ZiGGi");
	print("_____________________________________________________________\n");
	print("Translated to Russian by Dmitry Borisoff (Beginner)");
	print("Visit us at http://gto.zziggi.ru/");
	print("\n------------------------------------------------------------\n");
}   // CREDITS NOT DELETE !!!!!!!!!!!!!!!!!

public OnGameModeInit()
{
	SetGameModeText("GTO "#VERSION);
	// Initialize everything that needs it
	logging_OnGameModeInit();
	base_OnGameModeInit();
	account_OnGameModeInit();
	player_OnGameModeInit();
	gang_OnGameModeInit();
	payday_OnGameModeInit();
    weapons_OnGameModeInit();
	vehicles_OnGameModeInit();
    race_OnGameModeInit();
	deathmatch_OnGameModeInit();
	housing_OnGameModeInit();
	business_OnGameModeInit();
	bank_OnGameModeInit();
    groundhold_OnGameModeInit();
	lang_OnGameModeInit();
	fights_OnGameModeInit();
    mission_OnGameModeInit();
    antiidle_OnGameModeInit();
    antihightping_OnGameModeInit();
    antihealthhack_OnGameModeInit();
    antimoneyhack_OnGameModeInit();
	//
	race_thestrip_init();
	race_riversiderun_init();
	race_fleethecity_init();
	race_lostinsmoke_init();
	race_backstreetbang_init();
	race_flyingfree_init();
	race_murderhorn_init();
	race_roundwego_init();
	race_striptease_init();
	race_monstertruck_init();
	race_countrycruise_init();
	race_thegrove_init();
	race_mullholland_init();
	race_lv_julius_init();
	race_ls_trailer1_init();
	race_sf_fuckinwood1_init();
	race_ls_majestic1_init();
	//
	dm_air_init();
	dm_area51_init();
	dm_badandugly_init();
	dm_bluemountains_init();
	dm_cargoship_init();
	dm_dildo_init();
	dm_mbase_init();
	dm_minigunmadness_init();
	dm_poolday_init();
	dm_usnavy_init();

	#tryinclude "misc\mapicon"
	#tryinclude "misc\pickups"
	#tryinclude "misc\objects"
	printf("SERVER: Objects and Pickups init");

	new hour,minute,second;
	gettime(hour,minute,second);
	WorldTime = hour;
	SetWorldTime(WorldTime);
	printf("SERVER: Worldtime set as %d",WorldTime);
	SetWeather(10);
	SetTimer("OneSecTimer",1000,1); // 1 second
	SetTimer("FiveSecondTimer",5000,1); // 5 second
	SetTimer("OneMinuteTimer",60000,1); // 1 minute
	SetTimer("OneHourTimer",3600000,1); // 1 hour
	SpawnWorld();
	WorldSave();
	printf("SERVER: GTO "#VERSION" initialization complete. [%02d:%02d]",hour,minute);
	WriteLog(GameLog,"SERVER: GTO "#VERSION" initialization complete. ");
	return 1;
}

public OnGameModeExit()
{
	WorldSave();
	return 1;
}

public OnPlayerConnect(playerid)
{
    if(IsPlayerNPC(playerid)) return 1;
	player_OnPlayerConnect(playerid);
   	account_OnPlayerConnect(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid,reason)
{
	if(playerid == INVALID_PLAYER_ID || IsPlayerNPC(playerid)) return 1;
	player_OnPlayerDisconnect(playerid,reason);
	mission_OnPlayerDisconnect(playerid, reason);
	WorldSave();
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	account_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
	housing_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
	business_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	click_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
	bank_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
	fights_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	click_OnPlayerClickPlayer(playerid, clickedplayerid);
	return 1;
}

public OnPlayerPickUpPickup(playerid,pickupid)
{
//	WeaponPickup(playerid,pickupid);
    return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	dm_OnPlayerEnterCheckpoint(playerid);
	mission_OnPlayerEnterCheckpoint(playerid);
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    race_OnPlayerEnterCheckpoint(playerid);
    return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if(!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return 1;

	new logstring[MAX_STRING];
	if(killerid == INVALID_PLAYER_ID) format(logstring, sizeof (logstring), "player: %d: %s: has died > Reason: (%d)",playerid,oGetPlayerName(playerid),reason);
	else format(logstring, sizeof (logstring), "player: %d: %s: has killed player %s(%d)> Reason: (%d)",killerid,oGetPlayerName(killerid),oGetPlayerName(playerid),playerid,reason);
	WriteLog(GameLog,logstring);

	SendDeathMessage(killerid,playerid,reason);

	mission_OnPlayerDeath(playerid,killerid,reason);
	gang_OnPlayerDeath(playerid,killerid,reason);
	
	if(!IsPlayerInAnyDM(playerid))
	{
		player_OnPlayerDeath(playerid,killerid,reason);
		player_OnPlayerKill(killerid,playerid,reason);
//		DropPlayerWeapons(playerid);
		PlayCrimeReportForPlayer(killerid,killerid,random(18)+3);
		PlayCrimeReportForPlayer(playerid,killerid,random(18)+3);
 	}
	else deathmatch_OnPlayerDeath(playerid,killerid);

	if(killerid != INVALID_PLAYER_ID) return 1;
	{
		if(!IsPlayerInAnyDM(playerid)) player_OnPlayerKill(killerid, playerid,reason);
		else deathmatch_OnPlayerKill(killerid,playerid,reason);
	}
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if(!IsPlayerConnected(playerid)) return 1;
	SetPlayerSkin(playerid,Player[playerid][SkinModel]);
	if(Player[playerid][Jailed] == 1)
	{
		SetPlayerInterior(playerid,6);
		SetPlayerPos(playerid,265.1273,77.6823,1001.0391);
	    SetPlayerFacingAngle(playerid,0);
	    SetPlayerWantedLevel(playerid, 6);
		SetPlayerArmour(playerid, 0);
		SetPlayerHealth(playerid, 10);
		ResetPlayerMoney(playerid);
		ResetPlayerWeapons(playerid);
		PlayerPlaySound(playerid,1082,198.3797,160.8905,1003.0300);
	}
	if(Player[playerid][MuteTime] != 0)
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][14]);
		SetPlayerWantedLevel(playerid, 3);
	}
	new dmid = GetPlayerDM(playerid);
	if(dmid == INVALID_DM_ID || DMPlayerStats[playerid][dm_player_active] == 0) player_OnPlayerSpawn(playerid);
	else deathmatch_OnPlayerSpawn(playerid,dmid);
	SetPlayerColor(playerid,PlayerGangColour(playerid));
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	// Фикс для выбора классов(поломалось в 0.3a)
	SetPVarInt(playerid,"player_class_zero",0);
	if(classid != 0)
	{
		Player[playerid][SkinModel] = GetPlayerSkin(playerid);
		SetPVarInt(playerid,"player_class_zero",1);
	}
	else if(GetPVarInt(playerid,"player_class_zero") == 1) // classid == 0 тоже можно выбирать
	{
		Player[playerid][SkinModel] = GetPlayerSkin(playerid);
		SetPVarInt(playerid,"player_class_zero",0);
	}
	//
	switch(City[playerid])
	{
	    case 0:
	    {
			SetPlayerInterior(playerid,11);
			SetPlayerPos(playerid,508.7362,-87.4335,998.9609);
			SetPlayerFacingAngle(playerid,0.0);
	    	SetPlayerCameraPos(playerid,508.7362,-83.4335,998.9609);
			SetPlayerCameraLookAt(playerid,508.7362,-87.4335,998.9609);
		}
		case 1:
		{
			SetPlayerInterior(playerid,3);
			SetPlayerPos(playerid,-2673.8381,1399.7424,918.3516);
			SetPlayerFacingAngle(playerid,181.0);
	    	SetPlayerCameraPos(playerid,-2673.2776,1394.3859,918.3516);
			SetPlayerCameraLookAt(playerid,-2673.8381,1399.7424,918.3516);
		}
		case 2:
		{
			SetPlayerInterior(playerid,3);
			SetPlayerPos(playerid,349.0453,193.2271,1014.1797);
			SetPlayerFacingAngle(playerid,286.25);
	    	SetPlayerCameraPos(playerid,352.9164,194.5702,1014.1875);
			SetPlayerCameraLookAt(playerid,349.0453,193.2271,1014.1797);
		}
	}
	return 1;
}

public OnPlayerCommandText(playerid,cmdtext[])
{
	commands_OnPlayerCommandText(playerid,cmdtext);
	account_OnPlayerCommandText(playerid,cmdtext);
	gang_OnPlayerCommandText(playerid,cmdtext);
	weapons_OnPlayerCommandText(playerid,cmdtext);
	race_OnPlayerCommandText(playerid,cmdtext);
	AdminRace_OnPlayerCommandText(playerid,cmdtext);
	Admin_OnPlayerCommandText(playerid,cmdtext); //rcon admins
	dm_OnPlayerCommandText(playerid,cmdtext);
	AdminDM_OnPlayerCommandText(playerid,cmdtext);
	AdminSys_OnPlayerCommandText(playerid,cmdtext); //SYSTEM
	Adm_OnPlayerCommandTextr(playerid,cmdtext); //admins
	Mod_OnPlayerCommandText(playerid,cmdtext); //moderators
	business_OnPlayerCommandText(playerid,cmdtext);
	housing_OnPlayerCommandText(playerid,cmdtext);
	vehicles_OnPlayerCommandText(playerid,cmdtext);

	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"Player: %s[%d]: %s",oGetPlayerName(playerid),playerid,cmdtext);
	WriteLog(CMDLog,logstring);
	return 1;
}

public OnPlayerText(playerid, text[])
{
	new string[MAX_STRING];
	switch(text[0])
	{
	    case '!':
	    {
			if((PlayerGangid[playerid] == 0) || (Player[playerid][MuteTime] != 0))    //Игрок заткнут
			{
				SendClientMessage(playerid,COLOUR_RED,lang_texts[1][14]);
				return 0;
			}
			strmid(string,text,1,strlen(text));
			if(!strlen(string)) return 1;
			format(string,sizeof(string), "%s[%d] банде: %s", oGetPlayerName(playerid), playerid, string);
			SendGangMessage(PlayerGangid[playerid],string,COLOUR_GANG_CHAT);
			format(string,sizeof(string), "Player: %s[%d]: <GANG CHAT>:   %s",oGetPlayerName(playerid),playerid,text);
			WriteLog(ChatLog,string);
			return 0;
		}
		case '@':
		{
			adminfunc_OnPlayerText(playerid,text);
			format(string,sizeof(string), "Player: %s[%d]: <ADMIN TALK>:   %s",oGetPlayerName(playerid),playerid,text);
			WriteLog(ChatLog,string);
			return 0;
		}
		case '#':
		{
			modfunc_OnPlayerText(playerid,text);
			format(string,sizeof(string), "Player: %s[%d]: <MODERATOR TALK>:   %s",oGetPlayerName(playerid),playerid,text);
			WriteLog(ChatLog,string);
			return 0;
		}
	}
	if(Player[playerid][MuteTime] != 0)  //Заткнут
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][14]);
		return 0;
	}
	format(string, sizeof(string),"[%d]: %s", playerid, text);
	SendPlayerMessageToAll(playerid, string);
	format(string,sizeof(string),"Player: %s[%d]: %s",oGetPlayerName(playerid),playerid,text);
	WriteLog(ChatLog,string);
	return 0;
}

public WeatherUpdate()
{
	if(sysweath != 1) return 1;
	new rand = random(19);
	SetWeather(rand);
	printf("SERVER: Weather set to %d",rand);
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	housing_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
	business_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
	bank_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
	weapons_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
    fights_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
    mission_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
	return 1;
}

public OnPlayerSelectedMenuRow(playerid,row)
{
	weapons_OnPlayerSelectedMenuRow(playerid,row);
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	weapons_OnPlayerExitedMenu(playerid);
	return 1;
}

public OnPlayerStateChange(playerid,newstate,oldstate)
{
	mission_OnPlayerStateChange(playerid,newstate,oldstate);
	antivehicle_OnPlayerStateChange(playerid,newstate,oldstate);
	return 1;
}

public OnPlayerExitVehicle(playerid,vehicleid)
{
	modfunc_OnPlayerExitVehicle(playerid,vehicleid);
	return 1;
}

public OnPlayerInteriorChange(playerid,newinteriorid,oldinteriorid)
{
    modfunc_OnPlayerInteriorChange(playerid,newinteriorid,oldinteriorid);
    return 1;
}

public OnPlayerEnterVehicle(playerid,vehicleid,ispassenger)
{
	modfunc_OnPlayerEnterVehicle(playerid,vehicleid,ispassenger);
	return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid,playerid)
{
	return 1;
}

public OnVehicleDeath(vehicleid,killerid)
{
	mission_OnVehicleDeath(vehicleid,killerid);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
    antirconhack_OnRconLoginAttempt(ip,password,success);
	return 1;
}

