//
// Created:		04.10.06
// Aurthor:		Iain Gilbert
// Updated in 16.12.2010 by ZiGGi

#if defined _testserver_included
	#endinput
#endif

#define _testserver_included
#pragma library testserver


stock testserver_OnPlayerConnect(playerid)
{
	SendClientMessage(playerid, COLOUR_MISC, "Добро пожаловать в Open - Grand Theft Online "#VERSION);
	SendClientMessage(playerid, COLOUR_MISC, "Этот сервер имеет некоторые изменения, поэтому возможно всё.");
	SendClientMessage(playerid, COLOUR_MISC, "Все самые последние изменения в моде Open-GTO будут тестироваться здесь.");
	SendClientMessage(playerid, COLOUR_MISC, "Ошибки, крэши, торможения - это обычное дело для тестового режима.");
	SendClientMessage(playerid, COLOUR_MISC, "Если вы увидели то, чего недолжно быть или у вас есть предложения, то обратитесь администратору сервера.");
}
