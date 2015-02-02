/*

	About: vehicle radio
	Author: ziggi
	
*/

#if defined _vehicle_radio_included
  #endinput
#endif

#define _vehicle_radio_included
#pragma library vehicle_radio


enum VehicleRadioInfo {
	vehradio_Name[MAX_NAME],
	vehradio_URL[MAX_STRING],
}

new vehicle_radio[][VehicleRadioInfo] = {
	{"Зайцев FM", "http://radio.zaycev.fm:9002/ZaycevFM(128).m3u"},
	{"Кисс FM", "http://radiogrom.com/online/ua_fm/kiss_fm_radio_online.m3u"},
	{"Rock 2", "http://skycast.su:2007/2420"},
	{"Бинарное Радио", "http://binradio.ru:24000/bin.m3u"}
};

static vehicle_RadioStatus[MAX_VEHICLES] = {-1, ...};


stock vh_radio_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER) {
		StopAudioStreamForPlayer(playerid);
	}

	if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER) {
		new radioid = GetVehicleRadio( GetPlayerVehicleID(playerid) );
		if (radioid >= 0) {
			PlayAudioStreamForPlayer(playerid, vehicle_radio[radioid][vehradio_URL]);
		}
	}
}

DialogCreate:VehicleRadio(playerid)
{
	new string[MAX_NAME * (sizeof(vehicle_radio) + 1 + 8)] = "{FF0000}Выключить радио\n{FFFFFF}";
	new current_radioid = GetVehicleRadio( GetPlayerVehicleID(playerid) );

	for (new i = 0; i < sizeof(vehicle_radio); i++) {
		if (i == current_radioid) {
			strcat(string, "{FFFFFF}", sizeof(string));
		} else {
			strcat(string, "{CCCCCC}", sizeof(string));
		}
		strcat(string, vehicle_radio[i][vehradio_Name], sizeof(string));
		strcat(string, "\n", sizeof(string));
	}
	
	Dialog_Open(playerid, Dialog:VehicleRadio, DIALOG_STYLE_LIST, "Радио", string, "Выбрать", "Назад");
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

stock DisableVehicleRadio(vehicleid)
{
	vehicle_RadioStatus[vehicleid] = -1;
	
	foreach (new playerid : Player) {
		if (vehicleid == GetPlayerVehicleID(playerid)) {
			StopAudioStreamForPlayer(playerid);
			SendClientMessage(playerid, COLOUR_SKYBLUE, "* Радио выключено");
		}
	}
	return 1;
}

stock SetVehicleRadio(vehicleid, radioid)
{
	vehicle_RadioStatus[vehicleid] = radioid;

	new string[MAX_STRING];
	format(string, sizeof(string), "* Текущая радиостанция: %s", vehicle_radio[radioid][vehradio_Name]);
	
	foreach (new playerid : Player) {
		if (vehicleid == GetPlayerVehicleID(playerid)) {
			PlayAudioStreamForPlayer(playerid, vehicle_radio[radioid][vehradio_URL]);
			SendClientMessage(playerid, COLOUR_SKYBLUE, string);
		}
	}
	return 1;
}

stock GetVehicleRadio(vehicleid)
{
	return vehicle_RadioStatus[vehicleid];
}