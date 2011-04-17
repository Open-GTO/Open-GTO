//
// Created:		04.10.06
// Aurthor:		Iain Gilbert
// Updated in 16.12.2010 by ZiGGi

#if defined _testserver_included
	#endinput
#endif

#define _testserver_included
#pragma library testserver

#include "base"

stock testserver_OnPlayerConnect(playerid)
{
	SendPlayerScrollingText(playerid,"Добро пожаловать в Open - Grand Theft Online "#VERSION);
	SendPlayerScrollingText(playerid,"Этот сервер имеет некоторые изменения, поэтому возможно всё.");
	SendPlayerScrollingText(playerid,"Все самые последние изменения в моде Open-GTO будут тестироваться здесь.");
	SendPlayerScrollingText(playerid,"Ошибки, крэши, торможения - это обычное дело для тестового режима.");
	SendPlayerScrollingText(playerid,"Если вы увидели то, чего недолжно быть или у вас есть предложения, то обратитесь администратору сервера.");
}
