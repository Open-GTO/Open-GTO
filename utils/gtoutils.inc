//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//
/*
	Модифицировал ZiGGi
*/

#if defined _gtoutils_included
  #endinput
#endif

#define _gtoutils_included
#pragma library gtoutils


#define command_register(%0,%1,%2,%3) if(!strcmp(%0,%1,true,%2) && ((%0[%2] == ' ') || (%0[%2] == 0))) return %3_OnPlayerCommandText(playerid,%0)
#define command_registerNR(%0,%1,%2,%3) if(!strcmp(%0,%1,true,%2) && ((%0[%2] == ' ') || (%0[%2] == 0))) %3_OnPlayerCommandText(playerid,%0)
#define GivePVarInt(%1,%2,%3) SetPVarInt(%1,%2,GetPVarInt(%1,%2)+%3)
#define set(%0,%1) strmid(%0,%1,0,160,MAX_STRING + 32)

#define BYTES_PER_CELL			4

stock GameMSG(fstring[], {Float, _}:...)
{
	static const STATIC_ARGS = 1;
	new n = (numargs() - STATIC_ARGS) * BYTES_PER_CELL;
	if(n)
	{
		new message[144],arg_start,arg_end;
		#emit CONST.alt			fstring
		#emit LCTRL				5
		#emit ADD
		#emit STOR.S.pri		arg_start

		#emit LOAD.S.alt		n
		#emit ADD
		#emit STOR.S.pri		arg_end
		do
		{
			#emit LOAD.I
			#emit PUSH.pri
			arg_end -= BYTES_PER_CELL;
			#emit LOAD.S.pri	arg_end
		}
		while(arg_end > arg_start);

		#emit PUSH.S			fstring
		#emit PUSH.C			144
		#emit PUSH.ADR			message

		n += BYTES_PER_CELL * 3;
		#emit PUSH.S			n
		#emit SYSREQ.C			format

		n += BYTES_PER_CELL;
		#emit LCTRL				4
		#emit LOAD.S.alt		n
		#emit ADD
		#emit SCTRL				4
	#if defined GameMSG_USE_PRINT
		printf(message);
	#endif
		WriteLog(GameLog,message);
	}
	else
	{
	#if defined GameMSG_USE_PRINT
		printf(fstring);
	#endif
		WriteLog(GameLog,fstring);
	}
	return 1;
}

stock PlayerPlaySoundOnPlayer(playerid,soundid)
{
	new Float:pos[3];
	GetPlayerPos(playerid,pos[0],pos[1],pos[2]);
	return PlayerPlaySound(playerid,soundid,pos[0],pos[1],pos[2]);
}

stock FormatPosition(position)
{
	new string[MAX_STRING];
	switch(position)
	{
		case 1: format(string, sizeof(string), "%dst!",position);
		case 2: format(string, sizeof(string), "%dnd!",position);
		case 3: format(string, sizeof(string), "%drd!",position);
		default: format(string, sizeof(string), "%dth",position);
	}
	return string;
}

stock strcharsplit(const string[], &index,seperator)
{
	new result[20],
		idxpos = index,
		len = strlen(string);
	for (new i=index;i<len;i++)
	{
	    index++;
		if (string[i] == seperator || string[i] == 0) break;
		result[i-idxpos] = string[i];
	}
	return result;
}

stock Vary(source,varyamount)
{
	return source + random((varyamount*2)+1) - varyamount;
}

stock NameCharCheck(string[MAX_PLAYER_NAME])
{
	if(!strcmp(string,"Unknown",true) || IsNumeric(string)) return 0;
	for(new i=strlen(string)-1;i>=0;i--)
	{
		switch(string[i])
		{
			case ALLOWED_NICK_SYMBOLS: continue;
			default: return 0;
		}
	}
	return 1;
}

stock ConvertSeconds(time)
{
	new string[40],Float:minutes,Float:hours,seconds,minutes_int;
	if(time < 60) format(string, sizeof(string), "%d seconds", time);
	else if(time == 60)	string = "1 minute";
	else if(time > 60 && time < 3600)
	{
		minutes = time / 60;
		seconds = time % 60;
		format(string, sizeof(string), "%.0f minutes and %d seconds", minutes, seconds);
	}
	else if(time == 3600) string = "1 hour";
	else if(time > 3600)
	{
		hours = time / 3600;
		minutes_int = time % 3600;
		minutes = minutes_int / 60;
		seconds = minutes_int % 60;
		format(string,sizeof(string),"%s of hours, %s of minutes and %d seconds",hours,minutes,seconds);
	}
	return string;
}

stock PlayerCount()
{
	new count = 0;
	foreach(Player,playerid)
	{
		count++;
	}
	return count;
}

stock oSetPlayerPos(playerid,Float:X,Float:Y,Float:Z,Float:Angle,Interior,World=0)
{
    SetPlayerInterior(playerid,Interior);
	SetPlayerVirtualWorld(playerid,World);
    SetPlayerFacingAngle(playerid,Angle);
    SetPlayerPos(playerid,X,Y,Z);
    SetCameraBehindPlayer(playerid);
    return 1;
}

stock settime()
{
    new hour;
    gettime(hour);
    SetWorldTime(hour);
	return 1;
}

stock StringCharCheck(string[MAX_STRING])
{
	for(new i=strlen(string)-1;i>=0;i--)
	{
		switch(string[i])
		{
			case ALLOWED_STRING_SYMBOLS: continue;
			default: return 0;
		}
	}
	return 1;
}

stock IsNumeric(const string[])
{
	for (new i = 0, j = strlen(string); i < j; i++)
	{
		if (string[i] > '9' || string[i] < '0') return 0;
	}
	return 1;
}

stock loccmp(Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2)
{
	if ((x1 == x2) && (y1 == y2) && (z1 == z2)) return 1;
	return 0;
}

#if defined USE_PASSWORD_ENCRYPT
// encrypt(string[]) by Alex "Y_Less" Cole, Based on code by Jeremy Cole
// modified by ZiGGi
stock encrypt(string[])
{
	new size = strlen(ENCRYPTION_KEY),
		j = strlen(string),
		k = ((j >> 4) + 1) << 4,
		sum = j,
		tmp[] = {0, 0};
	for (new i = 0; i < k; i++)
	{
		tmp[0] = (i < j) ? string[i] : ' ';
		sum = ((sum + strfind(ENCRYPTION_KEY, tmp) + 1) % size);
		string[i] = ENCRYPTION_KEY[sum];
	}
}
#endif
//------------------------- THIS COPY-PASTED FROM DUTILS -----------------------
/**
 *  Return a timestamp
 */
stock Now() {
  new hour,minute,second;
  new year, month,day;
  gettime(hour, minute, second);
  getdate(year, month, day);
  return mktime(hour,minute,second,day,month,year);
}
/*
 * Originally created by mabako, tuned by DracoBlue
 */
stock mktime(hour,minute,second,day,month,year) {
	new timestamp2;

	timestamp2 = second + (minute * 60) + (hour * 3600);

	new days_of_month[12];

	if ( ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) ) {
			days_of_month = {31,29,31,30,31,30,31,31,30,31,30,31}; // Schaltjahr
		} else {
			days_of_month = {31,28,31,30,31,30,31,31,30,31,30,31}; // keins
		}
	new days_this_year = 0;
	days_this_year = day;
	if(month > 1) { // No January Calculation, because its always the 0 past months
		for(new i=0; i<month-1;i++) {
			days_this_year += days_of_month[i];
		}
	}
	timestamp2 += days_this_year * 86400;

	for(new j=1970;j<year;j++) {
		timestamp2 += 31536000;
		if ( ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0) )  timestamp2 += 86400; // Schaltjahr + 1 Tag
	}

	return timestamp2;
}
