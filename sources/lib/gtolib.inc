/*

	About: grand theft online standart library
	Author: Iain Gilbert
	Co-author: ziggi

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before gtolib"
#endif

#if !defined _FOREACH_LOCAL_VERSION
	#error "Please include foreach before gtolib"
#endif

#if defined _gtolib_included
	#endinput
#endif

#define _gtolib_included
#pragma library gtolib

/*

	Defines

*/

#if !defined MAX_STRING
	#define MAX_STRING 128
#endif

#if !defined MAX_NAME
	#define MAX_NAME 40
#endif

#define MODE_INFO_VERSION 1
#define MODE_INFO_NAME    2

// additional mxINI-like functions
#define ini_fileRemove	fremove
#define ini_fileExist	fexist

//#define IS_IN_RANGE(%0,%1,%2) (((%0) - ((%1) + cellmin)) < (((%2) + 1) - ((%1) + cellmin)))
#define IS_IN_RANGE(%0,%1,%2) ((%0) >= (%1) && (%0) <= (%2))

/*

	Deprecated stuff

*/

#define GivePVarInt(%1,%2,%3) SetPVarInt(%1,%2, GetPVarInt(%1,%2)+(%3))
#define set(%0,%1) strmid(%0,%1, 0, 160, MAX_STRING + 32)

/*

	Key functions

*/

#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

#define HOLDING(%0) \
	((newkeys & (%0)) == (%0))

#define RELEASED(%0) \
	(((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

/*

	Time functions

*/

#if defined timestamp_to_date

stock timestamp_to_format_date(unix_timestamp)
{
	new year, month, day, hour, minute, second;
	timestamp_to_date(unix_timestamp, year, month, day, hour, minute, second);

	new string[20];
	format(string, sizeof(string), "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
	return string;
}

#endif

/*

	Player functions

*/

stock PlayerPlaySoundOnPlayer(playerid, soundid)
{
	return PlayerPlaySound(playerid, soundid, 0.0, 0.0, 0.0);
}

stock PlaySoundForPlayersInRange(soundid, Float:range, Float:x, Float:y, Float:z)
{
	foreach (new i : Player) {
	    if (IsPlayerInRangeOfPoint(i, range, x, y, z)) {
		    PlayerPlaySound(i, soundid, x, y, z);
	    }
	}
}

stock GetPlayersCount()
{
	return Iter_Count(Player);
}

stock SetPlayerPosEx(playerid, Float:x, Float:y, Float:z, Float:angle, interior, world = 0)
{
	SetPlayerInterior(playerid, interior);
	SetPlayerFacingAngle(playerid, angle);
	SetPlayerVirtualWorld(playerid, world);
	SetPlayerPos(playerid, x, y, z);
	SetCameraBehindPlayer(playerid);
}

stock SendMessageToBesidePlayers(message[], Float:range, Float:x, Float:y, Float:z)
{
	foreach (new playerid : Player) {
		if (IsPlayerInRangeOfPoint(playerid, range, x, y, z)) {
			SendClientMessage(playerid, -1, message);
		}
	}
}

/*

	Chat functions

*/

stock Chat_Clear(playerid)
{
	for (new i = 0; i < 20; i++) {
		SendClientMessage(playerid, -1, "");
	}
}

stock Chat_ClearAll()
{
	for (new i = 0; i < 20; i++) {
		SendClientMessageToAll(-1, "");
	}
}

/*

	Coords functions

*/

stock Float:GetDistanceBetweenPoints(Float:x1, Float:y1, Float:z1, Float:x2, Float:y2, Float:z2)
{
	return VectorSize(x1 - x2, y1 - y2, z1 - z2);
}

stock GetCoordsBefore(Float:x, Float:y, Float:a, Float:distance, &Float:res_x, &Float:res_y)
{
	res_x = x + (distance * floatsin(-a, degrees));
	res_y = y + (distance * floatcos(-a, degrees));
}

stock GetCoordsBeforePlayer(playerid, Float:distance, &Float:x, &Float:y)
{
	new
		Float:x,
		Float:y,
		Float:z,
		Float:angle;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, angle);

	GetCoordsBefore(x, y, angle, distance, x, y);
}

stock GetVehicleCoordsBeforePlayer(&Float:x, &Float:y, &Float:z, &Float:a, playerid, modelid = -1)
{
	new
		Float:wide, Float:long, Float:high,
		Float:wheel_x, Float:wheel_y, Float:wheel_z;

	if (modelid != -1) {
		GetVehicleModelInfo(modelid, VEHICLE_MODEL_INFO_SIZE, wide, long, high);
		GetVehicleModelInfo(modelid, VEHICLE_MODEL_INFO_WHEELSFRONT, wheel_x, wheel_y, wheel_z);
	} else {
		wide = 1.5;
		high = 1.0;
	}

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	GetCoordsBefore(x, y, a, wide + 0.5, x, y);

	z += 0.1;
	if (wheel_z != 0.0) {
		z -= high / 2 + wheel_z;
	}

	a += 90.0;
	if (a > 360.0) {
		a -= 360.0;
	}
}

/*

	String functions

*/

stock StripNL(str[]) // by Y_Less
{
	new i = strlen(str);
	while (i-- && str[i] <= ' ') {
		str[i] = '\0';
	}
}

stock FixAscii(text[])
{
	for (new i = 0; text[i] != '\0'; i++) {
		text[i] &= 0xFF;
	}
}

stock InsertSpacesInString(string[], const size = sizeof(string), const between_pos = 3)
{
	for (new i = 1, length = strlen(string); i < length; i++) {
		if ((length - i) % between_pos == 0) {
			strins(string, " ", i, size);
			i++;
			length++;
		}
	}
}


/*
	
	GenerateRandomString(result_str[], length, size = sizeof(result_str), alphabet[] = DEFAULT_ALPHABET, alphabet_size = sizeof(alphabet))

*/

#if !defined DEFAULT_ALPHABET
	#define DEFAULT_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#endif

stock GenerateRandomString(result_str[], length, size = sizeof(result_str), alphabet[] = DEFAULT_ALPHABET, alphabet_size = sizeof(alphabet))
{
	result_str[0] = '\0';

	if (length > size) {
		length = size - 1;
	}
	
	if (length < 1) {
		return 0;
	}

	for (new i = 0; i < length; i++) {
		result_str[i] = alphabet[ random(alphabet_size) ];
	}

	return 1;
}

/*
	
	About: strcmp empty string fix
	Author: Y_Less

*/

stock strcmpEx(const string1[], const string2[], bool:ignorecase=false, length=cellmax)
{
	if (string1[0])
	{
		if (string2[0])
		{
			return strcmp(string1, string2, ignorecase, length);
		}
		else
		{
			// Positive as st1 is greater (strcmp spec).
			return 1;
		}
	}
	else
	{
		if (string2[0])
		{
			// Negative as str2 is greater.
			return -1;
		}
		else
		{
			// The strings are the same (empty).
			return 0;
		}
	}
}
#define strcmp strcmpEx

stock FormatPosition(position)
{
	static pos_string[] = {"st!", "nd!", "rd!", "th"};
	new string[8];

	valstr(string, position);

	if (IS_IN_RANGE(position, 1, 3)) {
		strcat(string, pos_string[position - 1]);
	} else if (position > 3) {
		strcat(string, pos_string[3]);
	}

	return string;
}

// deprecated
stock strcharsplit(const string[], &index, seperator=' ')
{
	new
		result[20],
		i = 0;

	if (index != 0 && string[index] != '\0') {
		index++;
	}

	while (string[index] && string[index] != seperator && string[index] != '\r' && string[index] != '\n') {
		result[i++] = string[index++];
	}

	return result;
}

#if !defined ALLOWED_NICK_SYMBOLS
	#define ALLOWED_NICK_SYMBOLS 'a'..'z','A'..'Z','0'..'9','_','[',']','.','=','(',')'
#endif

stock NameCharCheck(playername[MAX_PLAYER_NAME + 1])
{
	if (!strcmp(playername, "Unknown", true) || IsNumeric(playername)) {
		return 0;
	}

	for (new i = 0; playername[i] != '\0'; i++) {
		switch (playername[i]) {
			case ALLOWED_NICK_SYMBOLS: {
				continue;
			}
			default: {
				return 0;
			}
		}
	}

	if (IsIpAddress(playername)) {
		return 0;
	}

	return 1;
}

#if !defined ALLOWED_STRING_SYMBOLS
	#define ALLOWED_STRING_SYMBOLS 'a'..'z','A'..'Z','0'..'9','_','[',']','=','(',')',' '
#endif

stock IsValidChars(string[])
{
	for (new i = 0; string[i] != '\0'; i++) {
		switch (string[i]) {
			case ALLOWED_STRING_SYMBOLS: {
				continue;
			}
			default: {
				return 0;
			}
		}
	}
	return 1;
}

stock IsNumeric(const string[])
{
	for (new i = 0; string[i] != '\0'; i++) {
		if (!IS_IN_RANGE(string[i], '0', '9')) {
			return 0;
		}
	}
	return 1;
}

stock IsIpAddress(string[])
{
	new
		octet,
		j = 100;

	for (new i = 0; string[i] != '\0'; i++) {
		if (j != 0 && IS_IN_RANGE(string[i], '0', '9')) {
			octet += (string[i] - '0') * j;
			j /= 10;
		} else if (string[i] == '.') {
			if (IS_IN_RANGE(octet, 0, 255)) {
				j = 100;
				octet = 0;
			} else {
				return 0;
			}
		} else {
			return 0;
		}
	}
	return 1;
}

/*

	Math functions

*/

stock mathrandom(min, max)
{
	return (random(max - min + 1) + min);
}

stock pow(value, degree)
{
	new result = 1;
	while (degree--) {
		result *= value;
	}
	return result;
}

stock GetVehicleSpeed(vehicleid)
{
	new
		Float:x,
		Float:y,
		Float:z;

	GetVehicleVelocity(vehicleid, x, y, z);

	return floatround( floatsqroot(x*x + y*y + z*z) * 100 );
}

/*

	For UpdateVehicleDamageStatus

*/

stock decode_panels(panels,&flp,&frp,&rlp,&rrp,&windshield,&front_bumper,&rear_bumper)
{
	flp = panels >> 0 & 0xF;
	frp = panels >> 4 & 0xF;
	rlp = panels >> 8 & 0xF;
	rrp = panels >> 12 & 0xF;
	windshield = panels >> 16 & 0xF;
	front_bumper = panels >> 20 & 0xF;
	rear_bumper = panels >> 24 & 0xF;
}

stock decode_doors(doors,&driver_door,&passenger_door,&bonnet,&boot)
{
	bonnet = doors & 0x7;
	boot = doors >> 8 & 0x7;
	driver_door = doors >> 16 & 0x7;
	passenger_door = doors >> 24 & 0x7;
}

stock decode_lights(lights,&light1,&light2,&light3,&light4)
{
	light1 = lights >> 0 & 0x1;
	light2 = lights >> 1 & 0x1;
	light3 = lights >> 2 & 0x1;
	light4 = lights >> 3 & 0x1;
}

stock decode_tires(tires,&tyre1,&tyre2,&tyre3,&tyre4)
{
	tyre1 = tires >> 0 & 0x1;
	tyre2 = tires >> 1 & 0x1;
	tyre3 = tires >> 2 & 0x1;
	tyre4 = tires >> 3 & 0x1;
}

stock encode_panels(flp, frp, rlp, rrp, windshield, front_bumper, rear_bumper)
{
	return flp | (frp << 4) | (rlp << 8) | (rrp << 12) | (windshield << 16) | (front_bumper << 20) | (rear_bumper << 24);
}

stock encode_doors(bonnet, boot, driver_door, passenger_door, behind_driver_door, behind_passenger_door)
{
	#pragma unused behind_driver_door
	#pragma unused behind_passenger_door
	return bonnet | (boot << 8) | (driver_door << 16) | (passenger_door << 24);
}

stock encode_lights(light1, light2, light3, light4)
{
	return light1 | (light2 << 1) | (light3 << 2) | (light4 << 3);
}

stock encode_tires(tire1, tire2, tire3, tire4)
{
	return tire1 | (tire2 << 1) | (tire3 << 2) | (tire4 << 3);
}

stock encode_tires_bike(rear, front)
{
	return rear | (front << 1);
}
