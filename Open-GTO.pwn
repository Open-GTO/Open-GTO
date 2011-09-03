/*
Project Name:	San Andreas - Multiplayer: Open - Grand Theft Online (Open-GTO).

Current version:	1.0.0 alpha
SA-MP Versions:		0.3c

Web site:	http://open-gto.ru/

Date Created GTO:		12 August 2006
Date start Open-GTO: 	5 November 2009

-------------------------------------------------------------------
							  GTO
-------------------------------------------------------------------
0.0.0 Ц 0.5.7		Iain Gilbert
0.5.7 Ц 0.5.8		Peter Steenbergen (j1nx)
0.5.8 Ц 0.5.9		Robin Kikkert (dejavu), Lajos Pacsek (Asturel)
0.5.9 Ц 0.6.0		SCALOLaz
-------------------------------------------------------------------
							Open-GTO
-------------------------------------------------------------------
0.0.0 Ц 1.0.0		Current: ZiGGi
					Previous: GhostTT, heufix, Elbi, SLICK
-------------------------------------------------------------------
*/

#include <..\compiler\includes\a_samp>		// samp
#include "utils\foreach"
#include "config"							// дефайны и настройки по умолчанию
#include "utils\mxINI"
#include "base"								// holds base script values
#include "logging"							// logging handler
#include "utils\gtoutils"					// misc used utils
#include "arrays"
#include "lang"
#include "player\level"
#include "player\money"
#include "player\health"
#include "player\vip"
#include "player\weapon"
#include "bank"								// bank money to keep it on death
#include "fightstyles"
#include "account"							// account handler
#include "player"							// holds player values
#include "weapons"							// weapons and ammunation shop
#include "vehicles"							// vehicles
#include "zones"
#include "world"							// functions for zone, location, world, etc
#include "commands"							// command handler
#include "gang"								// gang handler
#include "housing"							// housing handler
#include "business"							// business handler
#include "streamers\mapicon_stream"
#include "streamers\checkpoint_stream"
#include "race"								// race handler, manages and runs all rasces
#include "deathmatch"						// deathmatch handler
#include "payday" 							// pay players money based on level
#include "groundhold"						// hold ground to gain money, pirate ship, etc
#include "admin\functions"
#include "admin\admin_func"
#include "admin\mod_func"
#include "admin\admin_commands"				// admin commands
#include "admin\admin_commands_race"		// admin commands for race creation/minipulation
#include "admin\admin_commands_dm"			// admin commands for deathmatch creation/minipulation
#include "admin\admin_commands_sys"
#include "admin\adm_commands"
#include "admin\mod_commands"
#include "missions\trucker"
#include "protections\antiidle"
#include "protections\antirconhack"
#include "protections\antihightping"
#include "protections\chatguard"
#include "protections\banweapons"
//#include "testserver"
#include "click"
#include "services\fastfood"
#include "services\bar"
#include "interior"
#include "usermenu"
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
{
	print("\n\n\n\
		-------------------------------------------------------------\n\
		\n\
		\t\tRunning Open-GTO "#VERSION"\n\
		\n\
		_____________________________________________________________\n\
	");
	print("\
		Created by: Iain Gilbert (Bogart)\n\
		Continued by:\n\
			\tPeter Steenbergen (j1nx)\n\
			\tRobin Kikkert (Dejavu)\n\
			\tLajos Pacsek (asturel)\n\
			\tDmitry Frolov (FP)\n\
			\tOpen-GTO Team:\n\
				\t\tCurrent: ZiGGi, SLICK\n\
				\t\tPrevious: GhostTT, heufix, Elbi\n\
	");
	print("\
		_____________________________________________________________\n\
		\n\
		Translated to Russian by Dmitry Borisoff (Beginner)\n\
		Visit us at http://open-gto.ru/\n\
		\n\
		------------------------------------------------------------\n\
	");
}

public OnGameModeInit()
{
	SetGameModeText("Open-GTO "#VERSION);
	// Initialize everything that needs it
	lang_OnGameModeInit();
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
	groundhold_OnGameModeInit();
	business_OnGameModeInit();
	housing_OnGameModeInit();
	interior_OnGameModeInit();
	bank_OnGameModeInit();
	fights_OnGameModeInit();
    trucker_OnGameModeInit();
	// services
	fastfood_OnGameModeInit();
	bar_OnGameModeInit();
	//
	level_OnGameModeInit();
    antiidle_OnGameModeInit();
    antihightping_OnGameModeInit();
    health_OnGameModeInit();
    money_OnGameModeInit();
	chatguard_OnGameModeInit();
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
	GameMSG("SERVER: Mapicons, Objects and Pickups init");

	new hour,minute;
	gettime(hour,minute);
	WorldTime = hour;
	SetWorldTime(hour);
	GameMSG("SERVER: Worldtime set as %d",hour);
	SetWeather(10);
	SetTimer("OneSecTimer",1000,1); // 1 second
	SetTimer("FiveSecondTimer",5000,1); // 5 second
	SetTimer("OneMinuteTimer",60000,1); // 1 minute
	SetTimer("TenMinuteTimer",600000,1); // 10 minute
	SetTimer("OneHourTimer",3600000,1); // 1 hour
	SetTimer("WorldSave",WORLD_SAVE_TIME,1);
	GameMSG("SERVER: Timers started");
	SpawnWorld();
	
	WorldSave();
	GameMSG("SERVER: Open-GTO "#VERSION" initialization complete. [%02d:%02d]",hour,minute);
	return 1;
}

public OnGameModeExit()
{
	WorldSave();
	new hour,minute;
	gettime(hour,minute);
	GameMSG("SERVER: Open-GTO "#VERSION" turned off. [%02d:%02d]",hour,minute);
	return 1;
}

public OnPlayerConnect(playerid)
{
    if(IsPlayerNPC(playerid)) return 1;
	player_OnPlayerConnect(playerid);
   	account_OnPlayerConnect(playerid);
	chatguard_OnPlayerConnect(playerid);
	
#if defined _testserver_included
	testserver_OnPlayerConnect(playerid);
#endif
	return 1;
}

public OnPlayerDisconnect(playerid,reason)
{
	if(playerid == INVALID_PLAYER_ID || IsPlayerNPC(playerid)) return 1;
	player_OnPlayerDisconnect(playerid,reason);
	trucker_OnPlayerDisconnect(playerid, reason);
	chatguard_OnPlayerDisconnect(playerid,reason);
	gh_OnPlayerDisconnect(playerid,reason);
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case account_Log_DialogID,account_Reg_DialogID:
		{
			account_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
		case house_DialogID, houses_DialogID, houses_upgrades_DialogID, houses_setrent_DialogID:
		{
			housing_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
		case bis_DialogID, bis_Msg_DialogID:
		{
			business_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
		}
		case fightstyles_DialogID, fightstyles_user_DialogID:
		{
			fights_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
		case user_menu_DialogID, user_menu_Return_DialogID, vehicle_menu_DialogID, spawnselect_menu_DialogID:
		{
			usermenu_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
		case weapons_Select_DialogID, weapons_Buy_DialogID:
		{
			weapons_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
		case bank_Start_DialogID, bank_FirstList_DialogID, bank_Withdraw_DialogID, bank_Deposit_DialogID:
		{
			bank_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
		case click_DialogID, click_Resp_DialogID:
		{
			click_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
		case bar_DialogID:
		{
			bar_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
		case fastfood_DialogID:
		{
			fastfood_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
		}
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	click_OnPlayerClickPlayer(playerid, clickedplayerid);
	return 1;
}

public OnPlayerPickUpPickup(playerid,pickupid)
{
    return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	dm_OnPlayerEnterCheckpoint(playerid);
	trucker_OnPlayerEnterCheckpoint(playerid);
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
	if(killerid == INVALID_PLAYER_ID)
		GameMSG("player: %s(%d): has died > Reason: (%d)",oGetPlayerName(playerid),playerid,reason);
	else
		GameMSG("player: %s(%d): has killed player %s(%d)> Reason: (%d)",oGetPlayerName(killerid),killerid,oGetPlayerName(playerid),playerid,reason);

	SendDeathMessage(killerid,playerid,reason);
	
	if(killerid == INVALID_PLAYER_ID) return 1;
	
	banweapons_OnPlayerDeath(playerid, killerid, reason);
	if(!IsPlayerInAnyDM(playerid))
	{
		player_OnPlayerDeath(playerid,killerid,reason);
		player_OnPlayerKill(killerid,playerid,reason);
		trucker_OnPlayerDeath(playerid,killerid,reason);
		gang_OnPlayerDeath(playerid,killerid,reason);
		weapon_OnPlayerDeath(playerid,killerid,reason);
		PlayCrimeReportForPlayer(killerid,killerid,random(18)+3);
		PlayCrimeReportForPlayer(playerid,killerid,random(18)+3);
 	}
	else
	{
		deathmatch_OnPlayerDeath(playerid,killerid);
		deathmatch_OnPlayerKill(killerid,playerid,reason);
	}
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if(!IsPlayerConnected(playerid) || IsPlayerNPC(playerid)) return 1;
	SetPlayerSkin(playerid,GetPlayerSkinModel(playerid));
	if(GetPlayerJailed(playerid) == 1)
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
	if(GetPlayerMuteTime(playerid) != 0)
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][14]);
		SetPlayerWantedLevel(playerid, 3);
	}
	new dmid = GetPlayerDM(playerid);
	if(dmid == INVALID_DM_ID || DMPlayerStats[playerid][dm_player_active] == 0)
		player_OnPlayerSpawn(playerid);
	else
		deathmatch_OnPlayerSpawn(playerid,dmid);
	SetPlayerColor(playerid,PlayerGangColour(playerid));
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	// ‘икс дл€ выбора классов(поломалось в 0.3a)
	if(classid != 0)
	{
		SetPlayerSkinModel(playerid,GetPlayerSkin(playerid));
		SetPVarInt(playerid,"player_class_zero",1);
	}
	else if(GetPVarInt(playerid,"player_class_zero") == 1) // classid == 0 тоже можно выбирать
	{
		SetPlayerSkinModel(playerid,GetPlayerSkin(playerid));
		SetPVarInt(playerid,"player_class_zero",0);
	}
	//
	new req_id = GetPVarInt(playerid,"RequestPlace");
	SetPlayerInterior(playerid,RequestPlace[req_id][Interior]);
	SetPlayerPos(playerid,RequestPlace[req_id][player_X],RequestPlace[req_id][player_Y],RequestPlace[req_id][player_Z]);
	SetPlayerFacingAngle(playerid,RequestPlace[req_id][player_A]);
	SetPlayerCameraPos(playerid,RequestPlace[req_id][camera_pos_X],RequestPlace[req_id][camera_pos_Y],RequestPlace[req_id][camera_pos_Z]);
	SetPlayerCameraLookAt(playerid,RequestPlace[req_id][camera_look_X],RequestPlace[req_id][camera_look_Y],RequestPlace[req_id][camera_look_Z]);
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if(GetPVarInt(playerid,"IsLogin") != 1) return 0;
	return 1;
}

public OnPlayerCommandText(playerid,cmdtext[])
{
	// commands.inc
	command_register(cmdtext,"/help",5,commands);
	command_register(cmdtext,"/commands",9,commands);
	command_register(cmdtext,"/kill",5,commands);
	command_register(cmdtext,"/info",5,commands);
	command_register(cmdtext,"/objective",10,commands);
	command_register(cmdtext,"/dropammo",9,commands);
	command_register(cmdtext,"/sound",6,commands);
	command_register(cmdtext,"/stats",6,commands);
	command_register(cmdtext,"/status",7,commands);
	command_register(cmdtext,"/stat",5,commands);
	command_register(cmdtext,"/level",6,commands);
	command_register(cmdtext,"/version",8,commands);
	command_register(cmdtext,"/admins",7,commands);
	command_register(cmdtext,"/time",5,commands);
	command_register(cmdtext,"/skydive",8,commands);
	command_register(cmdtext,"/sk",3,commands);
	command_register(cmdtext,"/dance",6,commands);
	command_register(cmdtext,"/handsup",8,commands);
	command_register(cmdtext,"/piss",5,commands);
	command_register(cmdtext,"/smoke",6,commands);
	
	// account
	command_register(cmdtext,"/changepass",11,account);
	command_register(cmdtext,"/changenick",11,account);
	command_register(cmdtext,"/savechar",9,account);
	
	// gangs
	command_register(cmdtext,"/g",2,gang);
	command_register(cmdtext,"/gang",5,gang);
	
	// race
	command_register(cmdtext,"/races",6,race);
	command_registerNR(cmdtext,"/race",5,race);
	
	// admin race
	command_registerNR(cmdtext,"/race",5,AdminRace);
	
	// rcon admins
	command_registerNR(cmdtext,"/cmdlist",8,Admin);
	command_registerNR(cmdtext,"/about",6,Admin);
	command_register(cmdtext,"/int",4,Admin);
	command_register(cmdtext,"/carinfo",8,Admin);
	command_register(cmdtext,"/carrep",7,Admin);
	command_register(cmdtext,"/go",3,Admin);
	command_register(cmdtext,"/an",3,Admin);
	command_register(cmdtext,"/payday",7,Admin);
	command_register(cmdtext,"/boom",5,Admin);
	command_register(cmdtext,"/setskin",8,Admin);
	command_register(cmdtext,"/ssay",5,Admin);
	command_register(cmdtext,"/skydiveall",11,Admin);
	command_register(cmdtext,"/disarm",7,Admin);
	command_register(cmdtext,"/disarmall",10,Admin);
	command_register(cmdtext,"/paralyzeall",12,Admin);
	command_register(cmdtext,"/deparalyzeall",14,Admin);
	command_register(cmdtext,"/remcash",8,Admin);
	command_register(cmdtext,"/remcashall",11,Admin);
	command_register(cmdtext,"/setlvl",7,Admin);
	command_register(cmdtext,"/setstatus",10,Admin);
	command_register(cmdtext,"/allowport",10,Admin);
	command_register(cmdtext,"/setvip",7,Admin);
	command_register(cmdtext,"/underwater",11,Admin);
	command_register(cmdtext,"/ahideme",8,Admin);
	command_register(cmdtext,"/ashowme",8,Admin);

	// dm
	command_register(cmdtext,"/deathmatches",13,dm);
	command_register(cmdtext,"/dms",4,dm);
	command_registerNR(cmdtext,"/dm",3,dm);
	
	// admin dm
	command_registerNR(cmdtext,"/dm",3,dm);
	
	// admin sys
	command_register(cmdtext,"/sys",4,AdminSys);
	
	// admins
	command_registerNR(cmdtext,"/cmdlist",8,Adm);
	command_registerNR(cmdtext,"/about",6,Adm);
	command_register(cmdtext,"/say",4,Adm);
	command_register(cmdtext,"/pinfo",6,Adm);
	command_register(cmdtext,"/admincnn",9,Adm);
	command_register(cmdtext,"/akill",6,Adm);
	command_register(cmdtext,"/tele-set",9,Adm);
	command_register(cmdtext,"/tele-loc",9,Adm);
	command_register(cmdtext,"/tele-to",8,Adm);
	command_register(cmdtext,"/tele-here",10,Adm);
	command_register(cmdtext,"/tele-hereall",13,Adm);
	command_register(cmdtext,"/tele-xyzi",10,Adm);
	command_register(cmdtext,"/sethealth",10,Adm);
	command_register(cmdtext,"/setarm",7,Adm);
	command_register(cmdtext,"/givexp",7,Adm);
	command_register(cmdtext,"/agivecash",10,Adm);
	command_register(cmdtext,"/givegun",8,Adm);
	command_register(cmdtext,"/paralyze",9,Adm);
	command_register(cmdtext,"/deparalyze",11,Adm);
	command_register(cmdtext,"/showpm",7,Adm);
	command_register(cmdtext,"/getip",6,Adm);
	
	// moderators
	command_registerNR(cmdtext,"/cmdlist",8,Mod);
	command_registerNR(cmdtext,"/about",6,Mod);
	command_register(cmdtext,"/plist",6,Mod);
	command_register(cmdtext,"/remcar",7,Mod);
	command_register(cmdtext,"/kick",5,Mod);
	command_register(cmdtext,"/carresp",8,Mod);
	command_register(cmdtext,"/mute",5,Mod);
	command_register(cmdtext,"/unmute",7,Mod);
	command_register(cmdtext,"/jail",5,Mod);
	command_register(cmdtext,"/unjail",7,Mod);
	command_register(cmdtext,"/mole",5,Mod);
	command_register(cmdtext,"/spec",5,Mod);
	command_register(cmdtext,"/spec-off",9,Mod);
	
	// business
	//business_OnPlayerCommandText(playerid,cmdtext);
	
	// housing
	command_register(cmdtext,"/ganghouses",11,housing);
	
	// vehicles
	command_register(cmdtext,"/vmenu",6,vehicles);

	new logstring[MAX_STRING];
	format(logstring,sizeof(logstring),"Player: %s"CHAT_SHOW_ID": %s",oGetPlayerName(playerid),playerid,cmdtext);
	WriteLog(CMDLog,logstring);
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if(chatguard_OnPlayerText(playerid,text) == 0) return 0;
	
	new string[MAX_STRING];
	switch(text[0])
	{
	    case '!':
	    {
			if(GetPVarInt(playerid,"GangID") == 0 || GetPlayerMuteTime(playerid) > 0)    //»грок заткнут
			{
				SendClientMessage(playerid,COLOUR_RED,lang_texts[1][14]);
				return 0;
			}
			if(strlen(text[1]) < 2) return 1;
			format(string,sizeof(string), "%s"CHAT_SHOW_ID" банде: {FFFFFF}%s", oGetPlayerName(playerid), playerid, text[1]);
			SendGangMessage(GetPVarInt(playerid,"GangID"),string,COLOUR_GANG_CHAT);
			format(string,sizeof(string), "Player: %s"CHAT_SHOW_ID": <GANG CHAT>: %s",oGetPlayerName(playerid),playerid,text[1]);
			WriteLog(ChatLog,string);
			return 0;
		}
		case '@','"':
		{
			if(strlen(text[1]) < 2) return 1;
			SendClientMessageToAdmins(playerid,text[1]);
			format(string,sizeof(string), "Player: %s"CHAT_SHOW_ID": <ADMIN TALK>: %s",oGetPlayerName(playerid),playerid,text[1]);
			WriteLog(ChatLog,string);
			return 0;
		}
		case '#','є':
		{
			if(strlen(text[1]) < 2) return 1;
			SendClientMessageToModers(playerid,text[1]);
			format(string,sizeof(string), "Player: %s"CHAT_SHOW_ID": <MODERATOR TALK>: %s",oGetPlayerName(playerid),playerid,text[1]);
			WriteLog(ChatLog,string);
			return 0;
		}
	}
	if(GetPlayerMuteTime(playerid) != 0)  //«аткнут
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[1][14]);
		return 0;
	}
	format(string, sizeof(string),"%s"CHAT_SHOW_ID": {FFFFFF}%s",oGetPlayerName(playerid),playerid,text);
	SendClientMessageToAll(GetPlayerColor(playerid),string);
	format(string,sizeof(string),"Player: %s"CHAT_SHOW_ID": %s",oGetPlayerName(playerid),playerid,text);
	WriteLog(ChatLog,string);
	return 0;
}

forward WeatherUpdate();
public WeatherUpdate()
{
	if(sysweath != 1) return 1;
	new rand = random(19);
	SetWeather(rand);
	GameMSG("SERVER: Weather set to %d",rand);
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	if( PRESSED( KEY_FIRE ) )
	{
		if( GetPVarInt(playerid,"bar_Drinking") == 1 ) return bar_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
	}
	if( PRESSED ( KEY_USING ) )
	{
		if( IsPlayerAtEnterExit(playerid) ) return interior_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		if( IsPlayerAtHouse(playerid) ) return housing_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		if( IsPlayerAtBusiness(playerid) ) return business_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		if( IsPlayerAtBank(playerid) != -1 ) return bank_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		if( IsPlayerAtAmmunation(playerid) != -1 ) return weapons_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		if( GetPlayerFightTrenerID(playerid) != -1 ) return fights_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		if( IsPlayerAtFastFood(playerid) ) return fastfood_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		if( IsPlayerAtBar(playerid) ) return bar_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		show_User_Menu(playerid);
		return 1;
	}
	if( PRESSED( KEY_SUBMISSION ) )
	{
		if( IsPlayerInAnyVehicle(playerid) ) trucker_OnPlayerKeyStateChange(playerid,newkeys,oldkeys);
		return 1;
	}
	return 1;
}

public OnPlayerSelectedMenuRow(playerid,row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerStateChange(playerid,newstate,oldstate)
{
	if(newstate == PLAYER_STATE_DRIVER)
	{
	#if defined OLD_ENGINE_DO
		if(GetPlayerVehicleSeat(playerid) == 0)
			vehicle_Engine(GetPlayerVehicleID(playerid),VEHICLE_PARAMS_ON);
	#endif
		trucker_OnPlayerStateChange(playerid,newstate,oldstate);
		vip_OnPlayerStateChange(playerid,newstate,oldstate);
	}
	return 1;
}

public OnPlayerExitVehicle(playerid,vehicleid)
{
#if defined OLD_ENGINE_DO
	if(GetPlayerVehicleSeat(playerid) == 0)
		vehicle_Engine(vehicleid,VEHICLE_PARAMS_OFF);
#endif
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

public OnPlayerStreamIn(playerid, forplayerid)
{
    return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
    return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid,playerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
    return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
    return 1;
}

public OnVehicleDeath(vehicleid,killerid)
{
	trucker_OnVehicleDeath(vehicleid,killerid);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
    antirconhack_OnRconLoginAttempt(ip,password,success);
	return 1;
}

