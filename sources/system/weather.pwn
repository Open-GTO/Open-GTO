/*

	About: automatic random weather changer
	Author: ziggi

*/

#if defined _weather_included
	#endinput
#endif

#define _weather_included

/*
	Defines
*/

#define MAX_WEATHER 21
#define MAX_WEATHER_NAME MAX_LANG_VALUE_STRING
#define INVALID_WEATHER_ID -1

/*
	Vars
*/

static
	LastTickCount,
	gWeatherTime = SYS_WEATHER_UPDATE,
	gWeather[] = {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 17, 18},
	gWeatherVarName[MAX_WEATHER][MAX_WEATHER_NAME] = {
			"WEATHER_EXTRASUNNY_LA",
			"WEATHER_SUNNY_LA",
			"WEATHER_EXTRASUNNY_SMOG_LA",
			"WEATHER_SUNNY_SMOG_LA",
			"WEATHER_CLOUDY_LA",
			"WEATHER_SUNNY_SF",
			"WEATHER_EXTRASUNNY_SF",
			"WEATHER_CLOUDY_SF",
			"WEATHER_RAINY_SF",
			"WEATHER_FOGGY_SF",
			"WEATHER_SUNNY_VEGAS",
			"WEATHER_EXTRASUNNY_VEGAS",
			"WEATHER_CLOUDY_VEGAS",
			"WEATHER_EXTRASUNNY_COUNTRYSIDE",
			"WEATHER_SUNNY_COUNTRYSIDE",
			"WEATHER_CLOUDY_COUNTRYSIDE",
			"WEATHER_RAINY_COUNTRYSIDE",
			"WEATHER_EXTRASUNNY_DESERT",
			"WEATHER_SUNNY_DESERT",
			"WEATHER_SANDSTORM_DESERT",
			"WEATHER_UNDERWATER"
		};

/*
	Config
*/

Weather_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Weather_ChangeTime", gWeatherTime);
}

Weather_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Weather_ChangeTime", gWeatherTime);
}

/*
	For timer
*/

stock Weather_Update()
{
	if (gWeatherTime == 0) {
		return 0;
	}

	new
		current_tick = GetTickCount();

	if (GetTickDiff(current_tick, LastTickCount) < gWeatherTime * 60 * 1000) {
		return 1;
	}

	LastTickCount = current_tick;

	Weather_SetRandom();
	return 1;
}

/*
	Random functions
*/

stock Weather_SetRandom()
{
	new weatherid = Weather_GetRadom();
	SetWeather(weatherid);
	Log(mainlog, INFO, "SERVER: Weather set to %d.", weatherid);
}

stock Weather_GetRadom()
{
	return gWeather[random( sizeof(gWeather) )];
}

/*
	Time functions
*/

stock Weather_GetTime()
{
	return gWeatherTime;
}

stock Weather_SetTime(time)
{
	gWeatherTime = time;
}


/*
	Name functions
*/

stock Weather_GetName(weatherid, Lang:lang, name[], const size = sizeof(name))
{
	if (!(0 < weatherid <= MAX_WEATHER)) {
		return 0;
	}
	return Lang_GetText(lang, gWeatherVarName[weatherid], name, size);
}

stock Weather_GetNameForPlayer(weatherid, playerid, name[], const size = sizeof(name))
{
	return Weather_GetName(weatherid, Lang_GetPlayerLang(playerid), name, size);
}
