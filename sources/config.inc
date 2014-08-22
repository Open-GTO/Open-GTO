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

#define VERSION					"v2.0.0 alpha" // версия мода
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

#define ANTI_HEIGHT_PING_ENABLED	1 // защита от высокого пинга
#define MAXIMAL_PING				500 // пинг, при котором кикает

#define ANTI_IDLE_ENABLED			1 // защита от АФК на точках прокачки
#define MAX_IDLE_TIME				5 // кол-во минут, после которых кикает

#define CHAT_GUARD_ENABLED			1 // защита чата
#define ANTI_JETPACK_ENABLED		1 // защита от использования JetPack
#define ANTI_RCON_HACK_ENABLED		1 // защита от взлома rcon пароля
#define ANTI_ARMOUR_HACK_ENABLED	1 // защита от чита на броню
#define ANTI_WEAPON_HACK_ENABLED	1 // защита от чита на оружие и патроны

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

// admin
#define ADMIN_TELEPORT_ENABLED		1 // включена-ли система телепортации администрации по клику на карте
#define ADMIN_TELEPORT_MIN_STATUS	STATUS_LEVEL_ADMIN // минимальный уровень доступа для телепортации

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
#define PLAYER_DEFAULT_SKIN	95

#define GROUNDHOLD_ENABLED		1 // точки прокачки
#define GROUNDHOLD_HOLD_TIME	2 // скорость потери точки при выходе из зоны

#define BANK_PROFIT			2 // процент начисления прибыли за хранение в банке
#define BANK_PROFIT_VIP		(BANK_PROFIT * 2) // процент начисления прибыли за хранение в банке для игроков с VIP
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

#define VEHICLE_DAMAGE_ENABLE_TIRES // включает прокалывание шин у пустого транспорта
//#define VEHICLE_DAMAGE_ENABLE_DAMAGE // включает урон у пустого транспорта
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
#define HEALTH_TARIF			3.0	// степень увеличения максимального здоровья от лвл, формула: MIN_HEALTH + (GetPlayerLevel(playerid) * HEALTH_TARIF)
#define HEALTH_REGEN_TARIF		0.1 // степень кол-ва востанавливаемого здоровья, формула: (GetPlayerLevel(id) * HEALTH_REGEN_TARIF)

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

	// поклажи
	#define SWAGUP_ENABLED		1 // включена ли система поклаж
	#define SWAGUP_MONEY_RATE	30 // параметр количества денег, за поднятие поклажи
	#define SWAGUP_XP_RATE		30 // параметр количества опыта, за поднятие поклажи
	#define SWAGUP_PAUSE_TIME	480000
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

stock Text3D:CreatePickupText(text[], Float:pos_x, Float:pos_y, Float:pos_z)
{
	new string[MAX_STRING];
	format(string, sizeof(string), "%s%s", text, PICKUP_TEXT);
	return Create3DTextLabel(string, 0xFFFFFFFF, pos_x, pos_y, pos_z, 20.0, 0, 1);
}

enum clickText_Info {
	Text:sshop_left,
	Text:sshop_right,
	Text:sshop_buy,
	Text:sshop_list,
	Text:sshop_exit,
}

new Text:clickText[clickText_Info];

// прочее
#define SPECIAL_ACTION_PISS	68

// дополнительные функции 'для' mxINI
#define ini_fileRemove	fremove
#define ini_fileExist	fexist

// по кнопкам
#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
