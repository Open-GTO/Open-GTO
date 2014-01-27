/*

	About: dialog system
	Author: ziggi

*/

#define DialogCreate:%0(%1) \
	forward dcreate_%0(%1); \
	public dcreate_%0(%1)

#define DialogResponse:%0(%1) \
	forward drespon_%0(%1); \
	public drespon_%0(%1)

#define Dialog: #


stock Dialog_Close(playerid)
{
	DeletePVar(playerid, "Dialog_IsOpen");
	DeletePVar(playerid, "Dialog_Function");

	ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, " ", " ", " ", "");
	return 0;
}

stock Dialog_IsOpen(playerid)
{
	return GetPVarInt(playerid, "Dialog_IsOpen") == 1 ? 1 : 0;
}

stock Dialog_Open(playerid, function[], style, caption[], info[], button1[], button2[])
{
	SetPVarInt(playerid, "Dialog_IsOpen", 1);
	SetPVarString(playerid, "Dialog_Function", function);

	ShowPlayerDialog(playerid, 32700, style, caption, info, button1, button2);
}

stock Dialog_Show(playerid, function[])
{
	SetPVarInt(playerid, "Dialog_IsOpen", 1);
	SetPVarString(playerid, "Dialog_Function", function);

	new call_func[40];
	strcat(call_func, "dcreate_");
	strcat(call_func, function);

	CallLocalFunction(call_func, "d", playerid);
}

stock Dialog_Message(playerid, caption[], info[], button1[])
{
	Dialog_Open(playerid, Dialog:Message, DIALOG_STYLE_MSGBOX, caption, info, button1, "");
	return 1;
}

stock Dialog_MessageEx(playerid, response[], caption[], info[], button1[], button2[])
{
	Dialog_Open(playerid, response, DIALOG_STYLE_MSGBOX, caption, info, button1, button2);
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	new input[128];

	strcat(input, inputtext);

	if (input[0] == '\0') {
		input[0] = '\1';
	} else {
		for (new i = 0, len = strlen(input); i != len; i++) {
			if (input[i] == '%') {
				input[i] = '#';
			}
		}
	}

	if (dialogid == 32700 && Dialog_IsOpen(playerid)) {
		new call_func[40];
		strcat(call_func, "drespon_");

		new function[32];
		GetPVarString(playerid, "Dialog_Function", function, sizeof(function));

		strcat(call_func, function);
		
		if (funcidx(call_func) != -1) {
			CallLocalFunction(call_func, "ddds", playerid, response, listitem, input);
		}
	}
	#if defined GTODIALOG_OnDialogResponse
		GTODIALOG_OnDialogResponse(playerid, dialogid, response, listitem, input);
	#endif
	return 0;
}

#define OnDialogResponse GTODIALOG_OnDialogResponse

#if defined GTODIALOG_OnDialogResponse
	forward GTODIALOG_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif
