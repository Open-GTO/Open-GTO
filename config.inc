/*
	
	Описание: дефайны и настройки по умолчанию
	Автор: ZiGGi

*/

#if defined _config_included
	#endinput
#endif

#define _config_included
#pragma library config

#pragma tabsize 0

#define VERSION					"v1.0.0 alpha(5)" // версия мода
#define GTO_FILES_FORMAT		".gto" // расширение(.txt для операционных систем на базе Linux не рекомендуется)
#define GTO_FILES_FOLDER		"Open-GTO/" // папка, где хранятся файлы Open-GTO, в scriptfiles/

#define LANGUAGE_USE			"Russian" // используемый язык
#define LANG_BB_N				"[n]" // переход на новую строку

#define MAX_STRING				128 // не менять, кол-во символов для большинства строковых переменных
#define MAX_NAME				40 // кол-во символов для имён, паролей и пр.

#define MAX_MONEY				50000000 // максимальное кол-во денег, которое может иметь игрок
#define UpdatePlayerScore(%1)	SetPlayerScore(%1,GetPVarInt(%1,"Level"))
#define MAX_SEND_SYMBOLS		64 // максимальное кол-во отправляемых символов в ПМ через click

#define INVALID_QUEST_ID		0
#define MAX_QUESTS				255

#define WORLD_SAVE_TIME			300000		// 1 раз, за это кол-во милисекунд выполняется сохранение всего мира(300000 - 5 минут)
#define GameMSG_USE_PRINT // если задефайнен, то функция GameMSG выводит сообщение в консоль сервера

// цены
#define HOUSE_UPGRADE_ARMOUR_COST 100000
#define HOUSE_UPGRADE_HEALTH_COST 100000

// логи(параметры по умолчанию) 1 вкл, 0 выкл
#define LOGGING_GAME_LOG_ENABLED		1
#define LOGGING_DEBUG_LOG_ENABLED		1
#define LOGGING_CHAT_LOG_ENABLED		1
#define LOGGING_CMD_LOG_ENABLED			1
// античит(параметры по умолчанию)
#define ANTI_HEALTH_HACK_ENABLED	1 // защита от чита на здоровье
#define ANTI_MONEY_HACK_ENABLED		1 // защита от чита на деньги

#define ANTI_HIGHT_PING_ENABLED		1 // защита от высокого пинга
#define MAXIMAL_PING				500 // пинг, при котором кикает

#define ANTI_IDLE_ENABLED	1 // защита от АФК на точках прокачки
#define MAX_IDLE_TIME		5 // кол-во минут, после которых кикает

#define CHAT_GUARD_ENABLED	1 // защита чата

// прочее
#define MAX_GULPS			5 // максимальное количество глотков из бутылки

#define GROUNDHOID_ENABLED	1 // точки прокачки

#define BANK_PROFIT		2 // процент начисления прибыли за хранение в банке
#define MAX_BANK_MONEY	15000000 // максимальное количество денег, которых можно хранить в банке

#define PAYDAY_BASE_PAY		500 // PayDay тариф на деньги
#define PAYDAY_CUTOFF		MAX_MONEY / 2 // кол-во денег, при котором перестаётся даваться деньги за PayDay
#define	PAYDAY_PAYXP		200 // PayDay тариф на опыт

#define CHAT_SHOW_ID		"(%d)" // вид отображения ида в чате

#define BAN_WEAPONS			38, 39 // запрещённое оружие

// стандартные иконки(by wups, модфицированно)
#define MAPICON_PAY_SPRAY
//#define MAPICON_BARBER
#define MAPICON_PIZZA
#define MAPICON_BURGER
#define MAPICON_CLUCKIN
//#define MAPICON_TATTOO
#define MAPICON_CLOTHES
#define MAPICON_AMMUNATION
#define MAPICON_TUNE
//

// дома
//#define BUY_ALL_HOUSES // позволяет запретить покупать чужие дома(если закомментировано то чужие дома можно покупать)
//

// бизнесы
#define MAX_UPGRADELEVEL	30 // максимальный уровень поднятия бизнеса
#define UPGRADE_TARIF		10 // тариф денег на повышение уровня, формула: (Businesses[id][Business_Upgrade]+1) * Businesses[id][Business_Value] * UPGRADE_TARIF
//

// транспорт
#define CARS_RESPAWN_TIME	600 // время респавна транспорта в секундах (86400 - один день) 
#define OLD_ENGINE_DO // сел в транспорт - завелось, вышел - заглох(если задефайнено)
//

// банды
#define MAX_GANGS			MAX_PLAYERS // максимум банд онлайн
#define MAX_GANG_SIZE		12 // максимальное кол-во людей в банде

#define MIN_GANG_NAME_LEN	3 // минимальная длина имени банды
#define MAX_GANG_NAME_LEN	40 // максимальная длина имени банды

#define GANG_CREATE_COST	100000 // кол-во денег, требуемое на создание банды
#define GANG_COLOUR_COST	20000 // кол-во денег, требуемое на смены цвета банды
//

// игрок
#define MIN_HEALTH				35.0 // минимальное(стартовое) кол-во здоровья
#define MAX_HEALTH				100.0 // максимальное кол-во здоровья
#define HEALTH_TARIF			4.0	// степень увеличения максимального здоровья от лвл, формула: MIN_HEALTH + (GetPlayerLevel(playerid) * HEALTH_TARIF)
#define HEALTH_REGEN_TARIF		0.4 // степень кол-ва востанавливаемого здоровья, формула: (GetPlayerLevel(id) * HEALTH_REGEN_TARIF)

#define CHANGE_NICK_COST			10000 // кол-во денег, требуемых для смены ника
#define MIN_NICK_LEN				3
#define CHANGE_PASS_COST			5000 // кол-во денег, требуемых для смены пароля
#define MIN_PASS_LEN				3
// МИССИИ
	// trucker
	#define TRUCKER_GIVE_XP_RATE	5 // кол-во опыта за завоз, формула: ( (GetPlayerLevel(playerid)+1) * 2 )^2 * trucker_GIVE_XP_RATE
	#define TRUCKER_RUNNER_MISSION_TIME 300000 // время перерыва между миссиями, в милисекундах
	//
	
//
// при смерти
#define PLAYER_XP_DEATH_MINUS_PROC		3 // сколько процентов XP уберается от уже имеющегося опыта
#define PLAYER_MONEY_DEATH_MINUS_PROC	20 // сколько процентов денег уберается

// стартовые параметры игрока
#define PLAYER_START_MONEY		1500 // кол-во стартовых денег
#define PLAYER_START_WEAPON_1	WEAPON_COLT45 // ид первого стартового оружия
#define PLAYER_START_WEAPON_2	0 // ид второго стартового оружия
#define PLAYER_START_WEAPON_3	0 // ид третьего стартового оружия

#define MAX_COUNTDOWN		5 // кол-во секунд для отсчёта гонок и дм
#define KEY_USING			KEY_WALK // Кнопка 'Ходьба' - отвечает за открытие диалогов(банк, покупка чегонибудь...), входов/выходов
#define KEY_NAME			"Ходьба"
#define PICKUP_TEXT			"\n{33AA33}Нажмите кнопку '{FFFFFF}"KEY_NAME"{33AA33}'"
#define CreatePickupText(%0,%1,%2,%3)	Create3DTextLabel(%0PICKUP_TEXT,0x00FFFFEE,%1,%2,%3,20.0,0,1)
// диалоги
enum {
	message_DialogID,
	account_Reg_DialogID, account_Log_DialogID,
	bank_Start_DialogID, bank_FirstList_DialogID, bank_Withdraw_DialogID, bank_Deposit_DialogID,
	click_DialogID, click_Resp_DialogID,
	house_DialogID, houses_DialogID, houses_upgrades_DialogID,
	bis_DialogID, bis_Info_DialogID, bis_Msg_DialogID,
	fightstyles_DialogID, fightstyles_user_DialogID,
	user_menu_DialogID, user_menu_Return_DialogID, vehicle_menu_DialogID,
	weapons_Select_DialogID, weapons_Buy_DialogID,
	bar_DialogID,
	fastfood_DialogID,
}

// прочее
// 'переименование' функций mxINI
#define ini_Create		ini_createFile
#define ini_Open		ini_openFile
#define ini_Close		ini_closeFile
#define ini_Set			ini_setString
#define ini_SetInt		ini_setInteger
#define ini_SetFloat	ini_setFloat
#define ini_Get			ini_getString
#define ini_GetInt		ini_getInteger
#define ini_GetFloat	ini_getFloat
#define ini_Remove		ini_removeKey
#define ini_Remov(%0)	fremove(%0)
#define ini_Exist(%0)	fexist(%0)

// цвета
#define COLOUR_GREY 0xAFAFAFAA // grey
#define COLOUR_GREEN 0x33AA33AA // green
#define COLOUR_LIGHTGREEN 0x9ACD32AA //lightgreen
#define COLOUR_RED 0xAA3333AA // red
#define COLOUR_LIGHTRED 0xFF6347AA // lightred
#define COLOUR_YELLOW 0xFFFF00AA // yellow
#define COLOUR_WHITE 0xFFFFFFAA // white
#define COLOUR_BLUE 0x33AAFFAA // blue
#define COLOUR_LIGHTBLUE 0x33CCFFAA // LightBlue
#define COLOUR_ORANGE 0xFF9900AA //orange
#define COLOUR_PURPLE 0xC2A2DAAA //purple
#define COLOUR_CRIMSON 0xDC143CAA
#define COLOUR_LIMEGREEN 0x32CD32AA
#define COLOUR_AQUA 0x00FFFFAA
#define COLOUR_FLBLUE 0x6495EDAA
#define COLOUR_MAGENTA 0xFF00FFFF
#define COLOUR_GOLD 0xB8860BAA
#define COLOUR_PINK 0xFFC0CBAA
#define COLOUR_MISC 0xAFAFAFAA // Grey - Misc
#define COLOUR_CHAT 0xFFFFFFAA // White - Chat
#define COLOUR_PLAYER 0xAAAAAAAA // White - Player
#define COLOUR_XP_GOOD 0x00C7FFAA // Blue - XP
#define COLOUR_XP_BAD 0x00008BAA // Blue - XP
#define COLOUR_COMBAT_GOOD 0xAA3333AA // Red - Combat
#define COLOUR_COMBAT_BAD 0xBB3333AA // Red - Combat
#define COLOUR_MONEY_GOOD 0xFFFF00AA // Yellow - Money
#define COLOUR_MONEY_BAD 0xDAA520AA // Yellow - Money
#define COLOUR_RACE 0x005407AA // green - dark
#define COLOUR_RACE_BAD 0x545407AA // yellow-green-dark
#define COLOUR_GANG 0xFF8C00AA // orange
#define COLOUR_GANG_CHAT 0xFFA500AA // orange
#define COLOUR_DM 0xFF0066AA // red - purple
#define COLOUR_DM_BAD 0xFF0066AA // red - purple

// по кнопкам
#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))