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

			TextList_Open(playerid, TextList:example_tl, items, sizeof(items), "Example header", "Button 1", "Button 2");
		}

		TextListResponse:example_tl(playerid, TextListType:response, itemid, itemvalue[])
		{
			new string[128];
			format(string, sizeof(string), " %d | %d | %d | %s", playerid, _:response, itemid, itemvalue);
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

	Enums

*/

enum TextListType {
	TextList_None,
	TextList_Button1,
	TextList_Button2,
	TextList_ListItem,
}

/*

	Vars

*/

static
	bool:IsOpen[MAX_PLAYERS],
	ListCount[MAX_PLAYERS],
	FunctionName[MAX_PLAYERS][TEXTLIST_MAX_FUNCTION_NAME],
	TD_ListItemValue[MAX_PLAYERS][TEXTLIST_MAX_ITEMS][TEXTLIST_MAX_ITEM_NAME],
	PlayerText:TD_ListHeader[MAX_PLAYERS],
	PlayerText:TD_ListItem[MAX_PLAYERS][TEXTLIST_MAX_ITEMS],
	PlayerText:TD_Button1[MAX_PLAYERS],
	PlayerText:TD_Button2[MAX_PLAYERS];

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

	PlayerTextDrawHide(playerid, TD_Button1[playerid]);
	PlayerTextDrawDestroy(playerid, TD_Button1[playerid]);
	TD_Button1[playerid] = PlayerText:0;

	PlayerTextDrawHide(playerid, TD_Button2[playerid]);
	PlayerTextDrawDestroy(playerid, TD_Button2[playerid]);
	TD_Button2[playerid] = PlayerText:0;

	for (new i = 0; i < TEXTLIST_MAX_ITEMS; i++) {
		TD_ListItemValue[playerid][i][0] = '\0';

		PlayerTextDrawHide(playerid, TD_ListItem[playerid][i]);
		PlayerTextDrawDestroy(playerid, TD_ListItem[playerid][i]);
	}

	CancelSelectTextDraw(playerid);
}

stock TextList_Open(playerid, function[], list_items[][], list_count, header[] = "",
                    button1[] = "", button2[] = "", Float:pos_x = 89.0, Float:pos_y = 140.0)
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

	new
		button1_length = strlen(button1),
		button2_length = strlen(button2);

	if (button1_length != 0 && button2_length != 0) {
		TD_ButtonCreate(playerid, TD_Button1[playerid], button1, pos_x - 36.0, pos_y + (list_count + 1) * 20.0);
		TD_ButtonCreate(playerid, TD_Button2[playerid], button2, pos_x + 36.0, pos_y + (list_count + 1) * 20.0);
	} else if (button1_length != 0) {
		TD_ButtonCreate(playerid, TD_Button1[playerid], button1, pos_x, pos_y + (list_count + 1) * 20.0);
	} else if (button2_length != 0) {
		TD_ButtonCreate(playerid, TD_Button2[playerid], button2, pos_x, pos_y + (list_count + 1) * 20.0);
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

static stock TD_ButtonCreate(playerid, &PlayerText:button, text[], Float:pos_x, Float:pos_y)
{
	button = CreatePlayerTextDraw(playerid, pos_x, pos_y, text);
	PlayerTextDrawLetterSize(playerid, button, 0.25, 1.4);
	PlayerTextDrawTextSize(playerid, button, 13.0, 68.0);
	PlayerTextDrawAlignment(playerid, button, 2);
	PlayerTextDrawColor(playerid, button, -1);
	PlayerTextDrawUseBox(playerid, button, 1);
	PlayerTextDrawBoxColor(playerid, button, 0x6D4C41AA);
	PlayerTextDrawSetShadow(playerid, button, 0);
	PlayerTextDrawSetOutline(playerid, button, 0);
	PlayerTextDrawBackgroundColor(playerid, button, 255);
	PlayerTextDrawFont(playerid, button, 1);
	PlayerTextDrawSetProportional(playerid, button, 1);
	PlayerTextDrawSetSelectable(playerid, button, true);

	PlayerTextDrawShow(playerid, button);
}

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
		new
			TextListType:response_type = TextListType:TextList_None,
			value[TEXTLIST_MAX_ITEM_NAME],
			list_id = -1;

		// check listitem
		for (new i = 0; i < ListCount[playerid]; i++) {
			if (TD_ListItem[playerid][i] == playertextid) {
				list_id = i;
				response_type = TextListType:TextList_ListItem;
			}
		}

		if (list_id != -1) {
			strmid(value, TD_ListItemValue[playerid][list_id], 0, strlen(TD_ListItemValue[playerid][list_id]), TEXTLIST_MAX_ITEM_NAME);
		} else {
			value[0] = '\1';
		}

		// check buttons
		if (TD_Button1[playerid] == playertextid) {
			response_type = TextListType:TextList_Button1;
		} else if (TD_Button2[playerid] == playertextid) {
			response_type = TextListType:TextList_Button2;
		}

		// check on errors
		if (response_type == TextListType:TextList_None) {
			return 0;
		}

		// call function
		new call_func[TEXTLIST_MAX_FUNCTION_NAME];
		strcat(call_func, "tlr_");
		strcat(call_func, FunctionName[playerid]);

		if (funcidx(call_func) != -1) {
			CallLocalFunction(call_func, "iiis", playerid, _:response_type, list_id, value);
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
