/*

	About: text list system
	Author:	ziggi

	Instalation:
		Include this file after a_samp.inc

	Usage:
		TextListCreate:example_tl(playerid)
		{
			new items[][TEXTLIST_MAX_ITEM_NAME] = {
				"Test 1",
				"Big Test 2"
			};

			TextList_Open(playerid, TextList:example_tl, items, sizeof(items), "Example header");
		}

		TextListResponse:example_tl(playerid, itemid, itemvalue[])
		{
			new string[128];
			format(string, sizeof(string), " %d | %d | %s", playerid, itemid, itemvalue);
			SendClientMessage(playerid, -1, string);
			return 1;
		}

*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before textlist"
#endif

#if defined _textlist_included
	#endinput
#endif

#define _textlist_included
#pragma library textlist

/*

	Define const

*/

#define TEXTLIST_MAX_ITEMS 10
#define TEXTLIST_MAX_ITEM_NAME 32
#define TEXTLIST_MAX_FUNCTION_NAME 31

/*

	Define functions

*/

#define TextListCreate:%0(%1) \
	forward tlc_%0(%1); \
	public tlc_%0(%1)

#define TextListResponse:%0(%1) \
	forward tlr_%0(%1); \
	public tlr_%0(%1)

#define TextList: #

/*

	Vars

*/

static
	bool:IsOpen[MAX_PLAYERS],
	ListCount[MAX_PLAYERS],
	FunctionName[MAX_PLAYERS][TEXTLIST_MAX_FUNCTION_NAME],
	TD_ListItemValue[MAX_PLAYERS][TEXTLIST_MAX_ITEMS][TEXTLIST_MAX_ITEM_NAME],
	PlayerText:TD_ListHeader[MAX_PLAYERS],
	PlayerText:TD_ListItem[MAX_PLAYERS][TEXTLIST_MAX_ITEMS];

/*

	Public functions

*/

stock TextList_Show(playerid, function[])
{
	new call_func[TEXTLIST_MAX_FUNCTION_NAME];
	strcat(call_func, "tlc_");
	strcat(call_func, function);

	CallLocalFunction(call_func, "i", playerid);
}

stock TextList_Close(playerid)
{
	IsOpen[playerid] = false;
	FunctionName[playerid][0] = '\0';

	PlayerTextDrawHide(playerid, TD_ListHeader[playerid]);
	PlayerTextDrawDestroy(playerid, TD_ListHeader[playerid]);

	for (new i = 0; i < TEXTLIST_MAX_ITEMS; i++) {
		TD_ListItemValue[playerid][i][0] = '\0';

		PlayerTextDrawHide(playerid, TD_ListItem[playerid][i]);
		PlayerTextDrawDestroy(playerid, TD_ListItem[playerid][i]);
	}

	CancelSelectTextDraw(playerid);
}

stock TextList_Open(playerid, function[], list_items[][], list_count, header[] = "", Float:pos_x = 89.0, Float:pos_y = 140.0)
{
	if (TextList_IsOpen(playerid)) {
		TextList_Close(playerid);
	}

	if (list_count > TEXTLIST_MAX_ITEMS) {
		printf("Error: so big list count value (%d, max is %d).", list_count, TEXTLIST_MAX_ITEMS);
		list_count = TEXTLIST_MAX_ITEMS;
	}

	IsOpen[playerid] = true;
	ListCount[playerid] = list_count;
	strmid(FunctionName[playerid], function, 0, strlen(function), sizeof(FunctionName[]));

	if (strlen(header) != 0) {
		TD_ListHeader[playerid] = CreatePlayerTextDraw(playerid, pos_x, pos_y, header);
		PlayerTextDrawLetterSize(playerid, TD_ListHeader[playerid], 0.3, 1.5);
		PlayerTextDrawTextSize(playerid, TD_ListHeader[playerid], 13.0, 135.0 + 8.0);
		PlayerTextDrawAlignment(playerid, TD_ListHeader[playerid], 2);
		PlayerTextDrawColor(playerid, TD_ListHeader[playerid], -1);
		PlayerTextDrawUseBox(playerid, TD_ListHeader[playerid], 1);
		PlayerTextDrawBoxColor(playerid, TD_ListHeader[playerid], 0xB71C1CAA);
		PlayerTextDrawSetShadow(playerid, TD_ListHeader[playerid], 0);
		PlayerTextDrawSetOutline(playerid, TD_ListHeader[playerid], 0);
		PlayerTextDrawBackgroundColor(playerid, TD_ListHeader[playerid], 255);
		PlayerTextDrawFont(playerid, TD_ListHeader[playerid], 1);
		PlayerTextDrawSetProportional(playerid, TD_ListHeader[playerid], 1);
		
		PlayerTextDrawShow(playerid, TD_ListHeader[playerid]);
	}

	for (new i = 0; i < list_count; i++) {
		strmid(TD_ListItemValue[playerid][i], list_items[i], 0, strlen(list_items[i]), TEXTLIST_MAX_ITEM_NAME);

		TD_ListItem[playerid][i] = CreatePlayerTextDraw(playerid, pos_x, pos_y + (i + 1) * 20.0, list_items[i]);
		PlayerTextDrawLetterSize(playerid, TD_ListItem[playerid][i], 0.3, 1.5);
		PlayerTextDrawTextSize(playerid, TD_ListItem[playerid][i], 13.0, 135.000000);
		PlayerTextDrawAlignment(playerid, TD_ListItem[playerid][i], 2);
		PlayerTextDrawColor(playerid, TD_ListItem[playerid][i], -1);
		PlayerTextDrawUseBox(playerid, TD_ListItem[playerid][i], 1);
		PlayerTextDrawBoxColor(playerid, TD_ListItem[playerid][i], 0x212121A0);
		PlayerTextDrawSetShadow(playerid, TD_ListItem[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TD_ListItem[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TD_ListItem[playerid][i], 255);
		PlayerTextDrawFont(playerid, TD_ListItem[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TD_ListItem[playerid][i], 1);
		PlayerTextDrawSetSelectable(playerid, TD_ListItem[playerid][i], true);

		PlayerTextDrawShow(playerid, TD_ListItem[playerid][i]);
	}

	SelectTextDraw(playerid, -5963521);
}

stock TextList_IsOpen(playerid)
{
	return _:IsOpen[playerid];
}

/*

	Private functions

*/

public OnPlayerDisconnect(playerid, reason)
{
	if (TextList_IsOpen(playerid)) {
		TextList_Close(playerid);
	}

	CallLocalFunction("TL_OnPlayerDisconnect", "ii", playerid, reason);
	return 1;
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect TL_OnPlayerDisconnect
forward OnPlayerDisconnect(playerid, reason);


public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	if (TextList_IsOpen(playerid)) {
		new list_id = -1;

		for (new i = 0; i < ListCount[playerid]; i++) {
			if (TD_ListItem[playerid][i] == playertextid) {
				list_id = i;
			}
		}

		if (list_id == -1) {
			return 0;
		}

		new call_func[TEXTLIST_MAX_FUNCTION_NAME];
		strcat(call_func, "tlr_");
		strcat(call_func, FunctionName[playerid]);
		
		if (funcidx(call_func) != -1) {
			CallLocalFunction(call_func, "iis", playerid, list_id, TD_ListItemValue[playerid][list_id]);
		}
	}
	
	CallLocalFunction("TL_OPClickPlayerTextDraw", "ii", playerid, _:playertextid);
	return 1;
}

#if defined _ALS_OPClickPlayerTextDraw
	#undef OPClickPlayerTextDraw
#else
	#define _ALS_OPClickPlayerTextDraw
#endif
#define OPClickPlayerTextDraw TL_OPClickPlayerTextDraw
forward OPClickPlayerTextDraw(playerid, PlayerText:playertextid);
