/*

	Title: Click handler system
	Created: 14.01.2014
	Author: ziggi

*/

#define DIALOG_STYLE_NONE	-1
#define MAX_DIALOG_CAPTION_SIZE	64

enum click_dialogArray_Info {
	cda_style,
	cda_caption[MAX_DIALOG_CAPTION_SIZE],
	cda_info[512],
	cda_button1[16],
	cda_button2[16],
	cda_statusLevel,
	cda_function[32],
}

static click_dialogArray[][click_dialogArray_Info] = {
	// player
	{DIALOG_STYLE_INPUT, "Отправить деньги", "Введите сумму $", "Отправить", "Назад", STATUS_LEVEL_PLAYER, "pl_click_SendCash"},
	{DIALOG_STYLE_INPUT, "Личное сообщение", "Введите сообщение", "Отправить", "Назад", STATUS_LEVEL_PLAYER, "pl_click_SendMessage"},
	{DIALOG_STYLE_INPUT, "Жалоба на игрока", "Введите текст", "Отправить", "Назад", STATUS_LEVEL_PLAYER, "pl_click_SendReport"},

	// moder
	{DIALOG_STYLE_INPUT, "Кикнуть", "Введите причину", "Кикнуть", "Назад", STATUS_LEVEL_MODER, "adm_click_KickPlayer"},
	{DIALOG_STYLE_INPUT, "Заглушить", "Введите время, на которое нужно заткнуть игрока", "Заткнуть", "Назад", STATUS_LEVEL_MODER, "adm_click_MutePlayer"},
	{DIALOG_STYLE_NONE, "Разглушить", "", "", "", STATUS_LEVEL_MODER, "adm_click_UnMutePlayer"},
	{DIALOG_STYLE_INPUT, "Посадить в тюрьму", "Введите время заключения", "Посадить", "Назад", STATUS_LEVEL_MODER, "adm_click_JailPlayer"},
	{DIALOG_STYLE_NONE, "Выпустить из тюрьмы", "", "", "", STATUS_LEVEL_MODER, "adm_click_UnJailPlayer"},

	// admin
	{DIALOG_STYLE_NONE, "Информация", "", "", "", STATUS_LEVEL_ADMIN, "adm_click_InfoPlayer"},
	{DIALOG_STYLE_NONE, "Убить", "", "", "", STATUS_LEVEL_ADMIN, "adm_click_KillPlayer"},
	{DIALOG_STYLE_NONE, "Телепортироваться к игроку", "", "", "", STATUS_LEVEL_ADMIN, "adm_click_TeleportToPlayer"},
	{DIALOG_STYLE_NONE, "Телепортировать к себе", "", "", "", STATUS_LEVEL_ADMIN, "adm_click_TeleportToMe"},
	{DIALOG_STYLE_INPUT, "Изменить здоровье", "Введите значение", "Изменить", "Назад", STATUS_LEVEL_ADMIN, "adm_click_SetHealth"},
	{DIALOG_STYLE_INPUT, "Изменить броню", "Введите значение", "Изменить", "Назад", STATUS_LEVEL_ADMIN, "adm_click_SetArmour"},
	{DIALOG_STYLE_INPUT, "Изменить уровень", "Введите новый уровень", "Изменить", "Назад", STATUS_LEVEL_ADMIN, "adm_click_SetLevel"},
	{DIALOG_STYLE_INPUT, "Дать опыт", "Введите количество опыта", "Дать", "Назад", STATUS_LEVEL_ADMIN, "adm_click_GiveXP"},
	{DIALOG_STYLE_INPUT, "Дать денег", "Введите количество денег", "Дать", "Назад", STATUS_LEVEL_ADMIN, "adm_click_GiveMoney"},
	{DIALOG_STYLE_NONE, "Заморозить", "", "", "", STATUS_LEVEL_ADMIN, "adm_click_FreezePlayer"},
	{DIALOG_STYLE_NONE, "Разморозить", "", "", "", STATUS_LEVEL_ADMIN, "adm_click_UnFreezePlayer"},
	{DIALOG_STYLE_NONE, "Получить NetStats", "", "", "", STATUS_LEVEL_ADMIN, "adm_click_GetNetStats"}
};

/*

	Callback's

*/

stock click_OnPlayerClickPlayer(playerid, clickedplayerid)
{
	if (playerid == clickedplayerid) {
		return show_menu(playerid);
	}
	
	click_SetPlayerClickedID(playerid, clickedplayerid);

	Dialog_Show(playerid, Dialog:PlayerClick);
	return 1;
}

DialogCreate:PlayerClick(playerid)
{
	new listitems[MAX_DIALOG_CAPTION_SIZE * sizeof(click_dialogArray)];
	new player_status = player_GetStatus(playerid);

	for (new i = 0; i < sizeof(click_dialogArray); i++) {
		if (player_status >= click_dialogArray[i][cda_statusLevel]) {
			strcat(listitems, click_dialogArray[i][cda_caption]);
			strcat(listitems, "\n");
		}
	}

	Dialog_Open(playerid, Dialog:PlayerClick, DIALOG_STYLE_LIST, "Выберите действие", listitems, "Выбрать", "Отмена");
}

DialogResponse:PlayerClick(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	new id = click_GetIdByListitem(player_GetStatus(playerid), listitem);

	if (click_dialogArray[id][cda_style] == -1) {
		click_CallFunction(playerid, id, listitem, inputtext);
	} else {
		click_SetResponseID(playerid, id);
		Dialog_Show(playerid, Dialog:PlayerClickResponse);
	}
	return 1;
}

DialogCreate:PlayerClickResponse(playerid)
{
	new dialogid = click_GetResponseID(playerid);

	Dialog_Open(playerid, Dialog:PlayerClickResponse, click_dialogArray[dialogid][cda_style],
		click_dialogArray[dialogid][cda_caption],
		click_dialogArray[dialogid][cda_info],
		click_dialogArray[dialogid][cda_button1], click_dialogArray[dialogid][cda_button2]
	);
}

DialogResponse:PlayerClickResponse(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:PlayerClick);
		return 1;
	}

	new id = click_GetResponseID(playerid);

	if (player_GetStatus(playerid) >= click_dialogArray[id][cda_statusLevel]) {
		click_CallFunction(playerid, id, listitem, inputtext);
	}
	return 1;
}

/*

	Function's

*/

stock click_CallFunction(playerid, dialogid, listitem, inputtext[])
{
	new clickedid = click_GetPlayerClickedID(playerid);
	CallLocalFunction(click_dialogArray[dialogid][cda_function], "ddds", playerid, clickedid, listitem, inputtext);
}

stock click_GetIdByListitem(player_status, listitem)
{
	new id = 0;

	for (new i = 0; i < sizeof(click_dialogArray); i++) {
		if (player_status >= click_dialogArray[i][cda_statusLevel]) {
			if (listitem == id) {
				return id;
			}

			id++;
		}
	}
	return -1;
}

stock click_SetResponseID(playerid, id) {
	SetPVarInt(playerid, "click_ResponseType", id);
}

stock click_GetResponseID(playerid) {
	return GetPVarInt(playerid, "click_ResponseType");
}

stock click_GetPlayerClickedID(playerid)
{
	return GetPVarInt(playerid, "click_ClickedPlayerid");
}

stock click_SetPlayerClickedID(playerid, clickedid)
{
	SetPVarInt(playerid, "click_ClickedPlayerid", clickedid);
}