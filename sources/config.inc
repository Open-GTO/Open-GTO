/*
	
	Описание: дефайны и настройки по умолчанию
	Автор: ziggi

*/

#if defined _config_included
	#endinput
#endif

#define _config_included
#pragma library config

#pragma tabsize 0
#pragma dynamic 13000

#define VERSION					"v1.1.7" // версия мода
#define GTO_FILES_FORMAT		".gto" // расширение(.txt для операционных систем на базе GNU/Linux не рекомендуется)
#define GTO_FILES_FOLDER		"Open-GTO/" // папка, где хранятся файлы Open-GTO, в scriptfiles/

#define LANGUAGE_USE			"Russian" // используемый язык

#define MAX_STRING				128 // не менять, кол-во символов для большинства строковых переменных
#define MAX_NAME				40 // кол-во символов для имён, паролей и пр.
#define MAX_IP					16 // максимальное количество символов для IP адреса

#define USE_PASSWORD_ENCRYPT // если задефайнено, то пароль в файле игрока будет шифроваться
#if defined USE_PASSWORD_ENCRYPT
new ENCRYPTION_KEY[] = "_C)2^K¬RPXjwE8b\\p{Df+=Q3(O*vgu9ZH[!1\"Le0->?/hSF<'xVzm]$4%5a`Jdny¦IUW@G7s6&l:k;i~ YsoMЈ.cBq#|}NrTtA, ";
#endif

#define SERVER_REAL_TIME			1 // мировое время соответствует текущему времени сервера
#define SERVER_REAL_TIME_OFFSET		0 // смещение времени относительно сервера

#define MAX_PLAYER_LOGIN_ATTEMPT	3 // максимальное количество попыток входа

#define MAX_MONEY				99999999 // максимальное кол-во денег, которое может иметь игрок
#define MIN_SEND_SYMBOLS		3 // минимальное кол-во отправляемых символов в ПМ
#define MAX_SEND_SYMBOLS		100 // максимальное кол-во отправляемых символов в ПМ

#define INVALID_QUEST_ID		0
#define MAX_QUESTS				255

#define WORLD_SAVE_TIME			300000		// 1 раз, за это кол-во милисекунд выполняется сохранение всего мира(300000 - 5 минут)
#define GameMSG_USE_PRINT // если задефайнен, то функция GameMSG выводит сообщение в консоль сервера
//#define DebugMSG_USE_PRINT // если задефайнен, то функция DMSG выводит сообщение в консоль сервера

// логи(параметры по умолчанию) 1 вкл, 0 выкл
#define LOGGING_GAME_LOG_ENABLED	1 // логирование серверных сообщений
#define LOGGING_DEBUG_LOG_ENABLED	1 // логирование отладочных сообщений
#define LOGGING_CHAT_LOG_ENABLED	1 // логирование сообщений в чат
#define LOGGING_CMD_LOG_ENABLED		1 // логирование введённых команд
// античит(параметры по умолчанию), в режиме тестирования...
#define ANTI_HEALTH_HACK_ENABLED	1 // защита от чита на здоровье
#define ANTI_MONEY_HACK_ENABLED		1 // защита от чита на деньги

#define ANTI_HIGHT_PING_ENABLED		1 // защита от высокого пинга
#define MAXIMAL_PING				500 // пинг, при котором кикает

#define ANTI_IDLE_ENABLED			1 // защита от АФК на точках прокачки
#define MAX_IDLE_TIME				5 // кол-во минут, после которых кикает

#define CHAT_GUARD_ENABLED			1 // защита чата
#define ANTI_JETPACK_ENABLED		1 // защита от использования JetPack
#define ANTI_RCON_HACK_ENABLED		1 // защита от взлома rcon пароля
#define ANTI_ARMOUR_HACK_ENABLED	1 // защита от чита на броню
#define ANTI_WEAPON_HACK_ENABLED	0 // защита от чита на оружие и патроны

#define ANTI_SPEED_HACK_ENABLED		0 // защита от speedhack, airbreak, телепортации(если наблюдаются ложные срабатывания, подрегулируйте параметр ниже)
#define ANTI_SPEED_HACK_CHECK_TIME	900 // время таймера для защиты от спидхака, на локалхосте актуально 1000
										// для более нагруженных серверов нужно устанавливать чуть меньшее значение


// QuidemSys - спидометр и бензин
#define QUIDEMSYS_ENABLED		1
#define QUIDEMSYS_TD_FONT		2 // используемый id шрифта в текстдраве
#define QUIDEMSYS_FUEL_SPEED	2 // количество бензина, заправляемого за 1 секунду
#define QUIDEMSYS_FUEL_TARIF	10 // цена одного литра бензина, в долларах

// прочее
#define MAX_GULPS			3 // максимальное количество глотков из бутылки

// поклажи
#define SWAGUP_ENABLED		1 // включена-ли система поклаж
#define SWAGUP_MONEY_RATE	30 // параметр количества денег, за поднятие поклажи
#define SWAGUP_XP_RATE		30 // параметр количества опыта, за поднятие поклажи

// лотерея
#define LOTTERY_ENABLED		1 // включен-ли скрипт лотереи
#define LOTTERY_TIME		10 // время розыгрыша, в минутах
#define LOTTERY_WIN_MONEY	200000 // количество денег победителю в лотерее
#define LOTTERY_TICKET_COST	5000 // цена лотерейного билета

// skins
#define SKINS_COST			0 // цена покупки скина
#define SKINS_IGNORE		0, 74  // список игнорируемых скинов
#define SKINS_MAXID			299
#define SKINS_MINID			1

#define GROUNDHOLD_ENABLED		1 // точки прокачки
#define GROUNDHOLD_HOLD_TIME	2 // скорость потери точки при выходе из зоны

#define BANK_PROFIT			2 // процент начисления прибыли за хранение в банке
#define MAX_BANK_MONEY		1000000000 // максимальное количество денег, которых можно хранить в банке

#define PAYDAY_BASE_PAY		25 // PayDay тариф на деньги
#define PAYDAY_CUTOFF		MAX_MONEY / 2 // кол-во денег, при котором перестаётся даваться деньги за PayDay
#define	PAYDAY_PAYXP		200 // PayDay тариф на опыт

#define CHAT_SHOW_ID		"(%d)" // вид отображения ида в чате

#define WEAPON_SKILL_ENABLED	1 // включить прокачку оружия (если выключено, то оружие на максимуме)
#define WEAPON_SKILL_SPEED		10 // количество даваемого скилла за одно убийство
#define WEAPON_DROP_ENABLED		1 // включить выпадание оружия
#define WEAPON_DROP_TIME		10 // время, после которого удалятся пикапы, выпавшие из игрока, в секундах
#define WEAPON_DROP_BULL		20 // столько процентов количества патрон выпадет из игрока
#define MAX_DROPPED_WEAPONS		64 // максимальное количество выпавшего оружия единовременно(у всех игроков)

#define SKYDIVE_TIME		5 // время, через которое игром может вновь использовать /skydive, в минутах

#define SYS_WEATHER_UPDATE	20 // время, через которое обновится состояние погоды, в минутах, если 0, то выключено

#define TELEPORTS_PAUSE_TIME	300 // время после телепортирования, через которое будет разрешено телепортироваться, в секундах

// разрешённые символы в названиях
#define ALLOWED_STRING_SYMBOLS		'a'..'z','A'..'Z','0'..'9','_','[',']','=','(',')',' '
#define ALLOWED_STRING_SYMBOLS_STR	"'a'..'z','A'..'Z','0'..'9','_','[',']','=','(',')',' '"
#define ALLOWED_NICK_SYMBOLS		'a'..'z','A'..'Z','0'..'9','_','[',']','.','=','(',')'
#define ALLOWED_NICK_SYMBOLS_STR	"'a'..'z','A'..'Z','0'..'9','_','[',']','.','=','(',')'"
// стандартные иконки(by wups, модфицированно)
#define MAPICON_PAY_SPRAY
#define MAPICON_TUNE
//

// дома
#define MAX_PLAYER_HOUSES		3 // количество домов, которые может иметь игрок(много ставить не рекомендуется)
#define HOUSE_UPGRADE_ARMOUR_COST 100000
#define HOUSE_UPGRADE_HEALTH_COST 100000
//#define BUY_ALL_HOUSES // позволяет запретить покупать чужие дома(если закомментировано то чужие дома нельзя покупать)
#define HOUSE_UNLOGIN_SELL_DAYS		14 // количество дней, через которое дом будет продан, если игрок не входил в игру
//

// бизнесы
#define MAX_PLAYER_BUSINESS		3 // количество бизнесов, которые может иметь игрок(много ставить не рекомендуется)
#define MAX_BUSINESS_LEVEL		30 // максимальный уровень поднятия бизнеса
#define UPGRADE_TARIF			10 // тариф денег на повышение уровня, формула: (Businesses[id][Business_Upgrade]+1) * Businesses[id][Business_Value] * UPGRADE_TARIF
//#define BUY_ALL_BUSINESS // позволяет запретить покупать чужие бизнесы(если закомментировано то чужие бизнесы нельзя покупать)
#define BUSINESS_UNLOGIN_SELL_DAYS	14 // количество дней, через которое бизнес будет продан, если игрок не входил в игру
//

// транспорт
#define CARS_RESPAWN_TIME	600 // время респавна транспорта в секундах (86400 - один день) 
#define OLD_ENGINE_DO // сел в транспорт - завелось, вышел - заглох(если задефайнено)

// поставьте в VEHICLE_LIGHTS_ON_TIME и VEHICLE_LIGHTS_OFF_TIME значения 0, если хотите, чтобы фары включались в любое время
#define VEHICLE_LIGHTS_ON_TIME		16 // время(часов), когда фары транспорта будут автоматически включаться
#define VEHICLE_LIGHTS_OFF_TIME		7 // время(часов), когда фары транспорта будут автоматически выключаться
//
#define MAX_PLAYER_VEHICLES			4 // максимальное количество личного транспорта
#define VEHSHOP_CAR_CHANGE_TIME		3 // количество часов, через которое транспорт в магазине меняется

// банды
#define MAX_GANGS			MAX_PLAYERS // максимум банд онлайн
#define MAX_GANG_SIZE		12 // максимальное кол-во людей в банде

#define MIN_GANG_NAME_LEN	3 // минимальная длина имени банды
#define MAX_GANG_NAME_LEN	40 // максимальная длина имени банды

#define MAX_GANG_MOTD_LEN	64 // максимальная длина объявления банды

#define GANG_CREATE_COST	500000 // кол-во денег, требуемое на создание банды
#define GANG_COLOUR_COST	20000 // кол-во денег, требуемое на смены цвета банды

#define MAX_GANG_BANK		2000000000 // максимальное количество денег, которых можно хранить в банке банды
//

// игрок
#define MIN_HEALTH				35.0 // минимальное(стартовое) кол-во здоровья
#define MAX_HEALTH				100.0 // максимальное кол-во здоровья
#define HEALTH_TARIF			4.0	// степень увеличения максимального здоровья от лвл, формула: MIN_HEALTH + (GetPlayerLevel(playerid) * HEALTH_TARIF)
#define HEALTH_REGEN_TARIF		0.4 // степень кол-ва востанавливаемого здоровья, формула: (GetPlayerLevel(id) * HEALTH_REGEN_TARIF)

#define CHANGE_NICK_COST			100000 // кол-во денег, требуемых для смены ника
#define MIN_PLAYER_NAME				3
#define CHANGE_PASS_COST			5000 // кол-во денег, требуемых для смены пароля
#define MIN_PASS_LEN				3
#define MAX_PASS_LEN				40
// МИССИИ
	// trucker
	#define TRUCKER_ENABLED					1
	#define TRUCKER_XP_RATE					20 // кол-во опыта за завоз, формула: ( (GetPlayerLevel(playerid)+1) * 2 )^2 * trucker_GIVE_XP_RATE
	#define TRUCKER_MONEY_RATE				10
	#define TRUCKER_MONEY_PENALTY			2000 // кол-во денег забираемых при невыполнении договора
	#define TRUCKER_TIME_GET_TRAILER		120 // кол-во секунд, дающихся на прицепление фуры к тягачу
	#define TRUCKER_MISSION_TRY_COUNT		2 // кол-во проходений и использований миссий до перерыва
	#define TRUCKER_MISSION_TIME			480 // время дающееся на прохождение миссии
	#define TRUCKER_RUNNER_MISSION_TIME		300 // время перерыва между миссиями, в секундах
	#define TRUCKER_CAR_SPAWN_TIME			300
	//
	
//
// при смерти
#define PLAYER_XP_DEATH_MINUS_PROC		3 // сколько процентов XP убирается от уже имеющегося опыта
#define PLAYER_MONEY_DEATH_MINUS_PROC	20 // сколько процентов денег убирается
#define PLAYER_XP_KILL_TARIF			5 // отвечает за количество получаемого опыта при убийстве, подробнее в функции player_OnPlayerKill

// стартовые параметры игрока
#define FIRST_LEVEL				1 // уровень, который даётся игроку после регистрации
#define PLAYER_START_MONEY		1500 // кол-во стартовых денег

#define MAX_COUNTDOWN		4 // кол-во секунд для отсчёта гонок и дм
#define KEY_USING			KEY_YES // Кнопка 'Y' - отвечает за открытие диалогов(банк, покупка чегонибудь...), входов/выходов
#define KEY_NAME			"Согласие - Y"
#define PICKUP_TEXT			"\n{33AA33}Нажмите кнопку '{FFFFFF}"KEY_NAME"{33AA33}'"
#define CreatePickupText(%0,%1,%2,%3)	Create3DTextLabel(%0PICKUP_TEXT, 0xFFFFFFFF,%1,%2,%3, 20.0, 0, 1)

// диалоги
enum {
	message_DialogID,
	account_Reg_DialogID, account_Log_DialogID,
	bank_Start_DialogID, bank_FirstList_DialogID, bank_Withdraw_DialogID, bank_Deposit_DialogID,
	click_DialogID, click_Resp_DialogID,
	house_DialogID, houses_DialogID, houses_upgrades_DialogID, houses_setrent_DialogID, house_sell_accept_DialogID,
	bis_DialogID, bis_Msg_DialogID, bis_sell_accept_DialogID,
	fightstyles_DialogID, fightstyles_user_DialogID,
	user_menu_DialogID, user_menu_Return_DialogID,
		vehicle_menu_DialogID, spawnselect_menu_DialogID, settings_menu_DialogID, vehicle_color_menu_DialogID, vehicle_radio_menu_DialogID,
		changenick_menu_DialogID, changepass_menu_DialogID,
		gang_menu_DialogID, gang_create_menu_DialogID, gang_invite_menu_DialogID, gang_color_menu_DialogID, gang_motd_menu_DialogID, gang_wmoney_menu_DialogID, gang_dmoney_menu_DialogID,
		gang_kick_menu_DialogID, gang_exit_accept_menu_DialogID,
	weapons_Select_DialogID, weapons_Buy_DialogID,
	bar_DialogID,
	fastfood_DialogID,
	SkinShop_Buy_DialogID,
	trucker_DialogID, trucker_cancel_DialogID,
	teleport_menu_DialogID,
	vshop_DialogID,
	pveh_select_DialogID, pveh_do_DialogID,
	anims_lib_menu_DialogID, anims_anim_menu_DialogID,
}

// прочее

#define SPECIAL_ACTION_PISS	68

// типы спавнов
#define SPAWN_HOUSE_NONE	-1
#define SPAWN_HOUSE_GANG	-2

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
#define ini_Remov		fremove
#define ini_Exist		fexist

// цвета
#define COLOUR_GREY 0xAFAFAFAA // grey
#define COLOUR_GREEN 0x33AA33AA // green
#define COLOUR_LIGHTGREEN 0x9ACD32AA //lightgreen
#define COLOUR_RED 0xAA3333AA // red
#define COLOUR_LIGHTRED 0xFF6347AA // lightred
#define COLOUR_YELLOW 0xFFFF00AA // yellow
#define COLOUR_WHITE 0xFFFFFFAA // white
#define COLOUR_BLUE 0x0000FFAA // blue
#define COLOUR_LIGHTBLUE 0x33CCFFAA // LightBlue
#define COLOUR_ORANGE 0xFF9900AA //orange
#define COLOUR_PURPLE 0xC2A2DAAA //purple
#define COLOUR_CRIMSON 0xDC143CAA
//#define COLOUR_LIMEGREEN 0x32CD32AA
#define COLOUR_AQUA 0x00FFFFAA
#define COLOUR_FLBLUE 0x6495EDAA
#define COLOUR_MAGENTA 0xFF00FFAA
//#define COLOUR_GOLD 0xFFD700AA
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
#define COLOUR_PM 0xFFCC22FF
#define WARN_MS_COLOR 0xFF5050AA

//#define COLOUR_LIGHTCORAL 0xF08080AA
#define COLOUR_DARKRED 0x8B0000AA
#define COLOUR_DEEPPINK 0xFF1493AA
#define COLOUR_TOMATO 0xFF6347AA
#define COLOUR_OrangeRed 0xFF4500AA
#define COLOUR_DarkOrange 0xFF8C00AA
#define COLOUR_Moccasin 0xFFE4B5AA
#define COLOUR_Khaki 0xF0E68CAA
#define COLOUR_DarkViolet 0x9400D3AA
#define COLOUR_Indigo 0x4B0082AA
#define COLOUR_GreenYellow 0xADFF2FAA
#define COLOUR_Lime 0x00FF00AA
//#define COLOUR_LIMEGREEN 0x32CD32AA
#define COLOUR_SpringGreen 0x00FF7FAA
#define COLOUR_Olive 0x808000AA
#define COLOUR_Teal 0x008080AA
//#define COLOUR_Aquamarine 0x7FFFD4AA
#define COLOUR_SteelBlue 0x4682B4AA
#define COLOUR_SkyBlue 0x87CEEBAA
#define COLOUR_Tan 0xD2B48CAA
#define COLOUR_Peru 0xCD853FAA
#define COLOUR_Sienna 0xA0522DAA
//#define COLOUR_MistyRose 0xFFE4E1AA
#define COLOUR_DimGray 0x696969AA
#define COLOUR_Black 0x000000AA

// по кнопкам
#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
