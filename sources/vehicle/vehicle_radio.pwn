/*

	About: vehicle radio
	Author: ziggi

*/

#if defined _vehicle_radio_included
  #endinput
#endif

#define _vehicle_radio_included

/*
	Defines
*/

#if !defined MAX_RADIO_NAME
	#define MAX_RADIO_NAME 64
#endif

#if !defined MAX_RADIO_URL
	#define MAX_RADIO_URL 128
#endif

#define INVALID_RADIO_ID -1

/*
	Enums
*/

enum e_RadioInfo {
	e_riName[MAX_RADIO_NAME],
	e_riUrl[MAX_RADIO_URL],
}

/*
	Vars
*/

static
	gRadioList[][e_RadioInfo] = {
		{"Zaycev FM", "http://radio.zaycev.fm:9002/ZaycevFM(128).m3u"},
		{"Smooth Jazz Tampa", "http://us3.internet-radio.com:8007/listen.pls"},
		{"Classic Rock Florida", "http://us2.internet-radio.com:8046/listen.pls"},
		{"Ambient Radio", "http://uk2.internet-radio.com:31491/listen.pls"},
		{"MEGATON CAFE RADIO", "http://us2.internet-radio.com:8443/listen.pls"},
		{"PONdENDS.COM", "http://s7.voscast.com:7000/listen.pls?sid=1"},
		{"FOREX CHILL MUSIC", "http://aska.ru-hoster.com:8053/autodj.m3u"},
		{"Rock Around The Block Radio", "http://uk6.internet-radio.com:8465/listen.pls"},
		{"FUN45s.com", "http://173.193.205.96:7032/listen.pls?sid=1"},
		{"The UK 1950s Radio Station", "http://91.121.134.23:8648/listen.pls?sid=1"},
		{"1FM", "http://uk2.internet-radio.com:8358/listen.pls"},
		{"KBRD", "http://205.134.192.90:680/listen.pls?sid=1"},
		{"Pigpen Radio", "http://uk6.internet-radio.com:8213/listen.pls"},
		{"Rainbow Sound UK", "http://uk5.internet-radio.com:8163/listen.pls"},
		{"Box UK Radio", "http://uk2.internet-radio.com:8145/listen.pls"}
	};

static
	gRadioID[MAX_VEHICLES] = {INVALID_RADIO_ID, ...};

/*
	OnPlayerStateChange
*/

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER) {
		StopAudioStreamForPlayer(playerid);
	}

	if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER) {
		new radioid = GetVehicleRadio( GetPlayerVehicleID(playerid) );
		if (radioid != INVALID_RADIO_ID) {
			PlayAudioStreamForPlayer(playerid, gRadioList[radioid][e_riUrl]);
		}
	}

	#if defined VRadio_OnPlayerStateChange
		return VRadio_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif

#define OnPlayerStateChange VRadio_OnPlayerStateChange
#if defined VRadio_OnPlayerStateChange
	forward VRadio_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

/*
	Dialog
*/

DialogCreate:VehicleRadio(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING + MAX_RADIO_NAME * (sizeof(gRadioList) + 8)],
		vehicleid,
		current_radioid;

	vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		return;
	}

	current_radioid = GetVehicleRadio(vehicleid);
	Lang_GetPlayerText(playerid, "VEHICLE_MENU_RADIO_ENABLE", string);

	for (new i = 0; i < sizeof(gRadioList); i++) {
		if (i == current_radioid) {
			strcat(string, "{FFFFFF}", sizeof(string));
		} else {
			strcat(string, "{CCCCCC}", sizeof(string));
		}
		strcat(string, gRadioList[i][e_riName], sizeof(string));
		strcat(string, "\n", sizeof(string));
	}

	Dialog_Open(playerid, Dialog:VehicleRadio, DIALOG_STYLE_LIST,
	            "VEHICLE_MENU_RADIO_HEADER",
	            string,
	            "BUTTON_SELECT", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
}

DialogResponse:VehicleRadio(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:VehicleMenu);
		return 1;
	}

	new vehicleid = GetPlayerVehicleID(playerid);

	if (listitem == 0) {
		DisableVehicleRadio(vehicleid);
	} else {
		SetVehicleRadio(vehicleid, listitem - 1);
	}
	return 1;
}

/*
	Functions
*/

stock DisableVehicleRadio(vehicleid)
{
	if (!IsValidVehicle(vehicleid)) {
		return 0;
	}

	gRadioID[vehicleid] = INVALID_RADIO_ID;

	new players[MAX_PLAYERS];
	GetPlayersInVehicle(vehicleid, players);

	StopAudioStreamForPlayers(players);
	Lang_SendTextToPlayers(players, "VEHICLE_RADIO_DISABLED");
	return 1;
}

stock SetVehicleRadio(vehicleid, radioid)
{
	if (!IsValidVehicle(vehicleid)) {
		return 0;
	}

	if (!(0 <= radioid < sizeof(gRadioList))) {
		return 0;
	}

	gRadioID[vehicleid] = radioid;

	new players[MAX_PLAYERS];
	GetPlayersInVehicle(vehicleid, players);

	PlayAudioStreamForPlayers(players, gRadioList[radioid][e_riUrl]);
	Lang_SendTextToPlayers(players, "VEHICLE_RADIO_CURRENT", gRadioList[radioid][e_riName]);
	return 1;
}

stock GetVehicleRadio(vehicleid)
{
	if (!IsValidVehicle(vehicleid)) {
		return INVALID_RADIO_ID;
	}
	return gRadioID[vehicleid];
}
