//
// Created:     05.09.06
// Aurthor:    Iain Gilbert
//

#if defined _gtoutils_included
  #endinput
#endif

#define _gtoutils_included
#pragma library gtoutils

#include "base"

forward SendPlayerScrollingText(playerid,text[]);
forward TextScroller();
forward strcharsplit(string[], &index,seperator);
forward strchar(string[2]);
forward CharReplace(findchar,newchar,string[MAX_STRING]);
forward Vary(source,varyamount);
forward strcomp(str1[],str2[],bool:ignorecase);
forward NameCharCheck(string[MAX_STRING]);
forward SendClientMessageToRegistered(colour, message[]);
forward ConvertSeconds(time);
forward encryptbykey(string[],key[MAX_STRING]);
forward PlayerCount();
forward settime();

PlayerPlaySoundOnPlayer(playerid,soundid)
{
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	PlayerPlaySound(playerid,soundid,playerx,playery,playerz);
}

Float:GetPlayerX(playerid)
{
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	return playerx;
}

Float:GetPlayerY(playerid)
{
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	return playery;
}

Float:GetPlayerZ(playerid)
{
	new Float:playerx,Float:playery,Float:playerz;
	GetPlayerPos(playerid,playerx,playery,playerz);
	return playerz;
}

FormatPosition(position)
{
	new string[MAX_STRING];
	if(position == 1) format(string, sizeof(string), "%dst!",position);
	else if (position == 2) format(string, sizeof(string), "%dnd!",position);
	else if (position == 3) format(string, sizeof(string), "%drd!",position);
	else format(string, sizeof(string), "%dth",position);
	return string;
}

#define TEXT_SCROLL_SPEED 1000
#define TEXT_SCROLL_SIZE 1023
new ScrollingText[MAX_PLAYERS][TEXT_SCROLL_SIZE]; // hmmmmm

public SendPlayerScrollingText(playerid,text[])
{
	for(new i=0;i<strlen(text);i++)
	{
	    if(text[i] == 0) break;
	    if(text[i] == strchar("\n")) text[i] = strchar("_");
	}
	if(strlen(text) > 150) //  not allowed
	{
		SendClientMessage(playerid, COLOUR_MISC, text);
  		return;
	}
	new wbreak;
	while ((strlen(ScrollingText[playerid]) > (TEXT_SCROLL_SIZE  - strlen(text))) && (wbreak < 5))
	{
		ScrollText(playerid);
		wbreak++;
	}
	if (wbreak >=5) strdel(ScrollingText[playerid], 0, strlen(ScrollingText[playerid])-1);
	strins(ScrollingText[playerid],text,strlen(ScrollingText[playerid]),TEXT_SCROLL_SIZE-1);
	strins(ScrollingText[playerid],"\n",strlen(ScrollingText[playerid]),TEXT_SCROLL_SIZE-1);
}

ScrollText(id)
{
	if(IsPlayerConnected(id))
	{
	    if(strlen(ScrollingText[id]) == 0) return;
		new string[MAX_STRING],idx,colour;
	    set(string,strcharsplit(ScrollingText[id],idx,strchar("\n")));
	    if(strlen(string) < 1) return;
	    if((string[0] == strchar("~")) && (string[2] == strchar("~")))
	    {
	        if(string[1] == strchar("r")) colour = COLOUR_RED;
	        else if(string[1] == strchar("g")) colour = COLOUR_GREEN;
            else if(string[1] == strchar("b")) colour = COLOUR_BLUE;
            else if(string[1] == strchar("y")) colour = COLOUR_YELLOW;
            strdel(string, 0, 3);
        }
        if(colour != 0) SendClientMessage(id, colour, string);
        else SendClientMessage(id,COLOUR_MISC,string);
		strdel(ScrollingText[id], 0, idx);
	}
	else strdel(ScrollingText[id], 0, strlen(ScrollingText[id])-1);
	return;
}

public TextScroller()
{
	for(new id=0;id<MAX_PLAYERS;id++)
	{
		if(IsPlayerConnected(id)) ScrollText(id);
	}
}

strcharsplit(string[], &index,seperator)
{
	new result[MAX_STRING];
	new idxpos = index;
	for (new i=index;i<strlen(string);i++)
	{
	    index++;
		if (string[i] == seperator) break;
		if (string[i] == 0) break;
		result[i-idxpos] = string[i];
	}
	return result;
}

public strchar(string[2])
{
	return string[0];
}

public Vary(source,varyamount)
{
	new rnd = random((varyamount*2)+1) - varyamount;
	source = source + rnd;
	return source;
}

public NameCharCheck(string[MAX_STRING])
{
	for (new i=0;i<MAX_STRING;i++)
	{
	    if(string[i]==0) return 1; // null
		if(string[i] < 32) return 0;
		if(string[i] > 125) return 0;
	}
	return 1;
}

stock SendPlayerFormattedText(playerid, const str[], define, colour)
{
	new tmpbuf[MAX_STRING];
	format(tmpbuf, sizeof(tmpbuf), str, define);
	SendClientMessage(playerid, colour, tmpbuf);
}

stock SendAllFormattedText(const str[], define)
{
	new tmpbuf[MAX_STRING];
	format(tmpbuf, sizeof(tmpbuf), str, define);
	SendClientMessageToAll(COLOUR_MISC, tmpbuf);
}

stock SendClientMessageToRegistered(colour, message[])
{
	for(new playerid=0;playerid<MAX_PLAYERS;playerid++)
	{
	    if(IsPlayerConnected(playerid))
	    {
	        if(IsPlayerRegistered(playerid)) SendClientMessage(playerid,colour,message);
		}
	}
}

ConvertSeconds(time)
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

public PlayerCount()
{
	new count = 0;
	for(new playerid=0;playerid<MAX_PLAYERS;playerid++)
	{
        if(IsPlayerConnected(playerid)) count++;
	}
	return count;
}

stock SetPlayerPosEx(playerid,Float:X,Float:Y,Float:Z,Float:Angle,Interior)
{
    SetPlayerInterior(playerid,Interior);
    SetPlayerFacingAngle(playerid,Angle);
    SetPlayerPos(playerid,X,Y,Z);
    SetCameraBehindPlayer(playerid);
    return 1;
}

new Float:x11,Float:y11,Float:z11;

stock PlaySoundForAll(soundid)
{
    for(new i=0;i<MAX_PLAYERS;i++)
	{
    	if(!IsPlayerConnected(i)) continue;
		PlayerPlaySound(i,soundid,x11,y11,z11);
	}
}

public settime()
{
    new second,minute,hour;
    //new string[256];
    gettime(hour,minute,second);
    //if (minute <= 9) format(string,25,"%d:0%d",hour,minute);
    //else format(string,25,"%d:%d",hour,minute);
    SetWorldTime(hour);
	return 1;
}

//------------------------- THIS COPY-PASTED FROM DUTILS -----------------------


/**
 *  Copies items from one array/string into another.
 *  @param   dest
 *           source
 *           count
 */
stock copy(dest[],source[],count) {
	dest[0]=0;
	if (count<0) return false;
	if (count>strlen(source)) count=strlen(source);
	new i=0;
	for (i=0;i<count;i++) {
		dest[i]=source[i];
		if (source[i]==0) return true;
	}
	dest[count]=0;
	return true;
}
/*
 * First version released by mike, this one created by DracoBlue
 * Has also a fix to use "-" and "+" in the beginning of the number.
 */
stock isNumeric(const string[]) {
	new length=strlen(string);
	if (length==0) return false;
	for (new i = 0; i < length; i++) {
		if (
		(string[i] > '9' || string[i] < '0' && string[i]!='-' && string[i]!='+') // Not a number,'+' or '-'
		|| (string[i]=='-' && i!=0)                                             // A '-' but not at first.
		|| (string[i]=='+' && i!=0)                                             // A '+' but not at first.
		) return false;
	}
	if (length==1 && (string[0]=='-' || string[0]=='+')) return false;
	return true;
}
/**
 *  Checks wether two strings are equal (case insensetive)
 *  @param   str1
 *           str2
 */
stock equal(str1[],str2[],bool:ignorecase) {
    if (strlen(str1)!=strlen(str2)) return false;
    if (strcmp(str1,str2,ignorecase)==0) return true;
    return false;
}
/*
*/
stock set(set_result[],set_source[])
{
	new count = strlen(set_source);
	for(new i=0;i<count;i++) set_result[i] = set_source[i];
	set_result[count] = 0;
}
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
/**
 *  Returns an element of a string splitted by ' ', default index is 0.
 *  @param   string
 *           index
 */
stock strtok(const string[], &index,seperator=' ')
{
	new length = strlen(string);
	new offset = index;
	new result[MAX_STRING];
	while ((index < length) && (string[index] != seperator) && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}

	result[index - offset] = EOS;
	if ((index < length) && (string[index] == seperator))
	{
		index++;
	}
	return result;
}
