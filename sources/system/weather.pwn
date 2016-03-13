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
	gWeatherName[MAX_WEATHER][MAX_WEATHER_NAME char];

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
	OnGameModeInit
*/

Weather_OnGameModeInit()
{
	Weather_SetName(0, _(WEATHER_EXTRASUNNY_LA));
	Weather_SetName(1, _(WEATHER_SUNNY_LA));
	Weather_SetName(2, _(WEATHER_EXTRASUNNY_SMOG_LA));
	Weather_SetName(3, _(WEATHER_SUNNY_SMOG_LA));
	Weather_SetName(4, _(WEATHER_CLOUDY_LA));
	Weather_SetName(5, _(WEATHER_SUNNY_SF));
	Weather_SetName(6, _(WEATHER_EXTRASUNNY_SF));
	Weather_SetName(7, _(WEATHER_CLOUDY_SF));
	Weather_SetName(8, _(WEATHER_RAINY_SF));
	Weather_SetName(9, _(WEATHER_FOGGY_SF));
	Weather_SetName(10, _(WEATHER_SUNNY_VEGAS));
	Weather_SetName(11, _(WEATHER_EXTRASUNNY_VEGAS));
	Weather_SetName(12, _(WEATHER_CLOUDY_VEGAS));
	Weather_SetName(13, _(WEATHER_EXTRASUNNY_COUNTRYSIDE));
	Weather_SetName(14, _(WEATHER_SUNNY_COUNTRYSIDE));
	Weather_SetName(15, _(WEATHER_CLOUDY_COUNTRYSIDE));
	Weather_SetName(16, _(WEATHER_RAINY_COUNTRYSIDE));
	Weather_SetName(17, _(WEATHER_EXTRASUNNY_DESERT));
	Weather_SetName(18, _(WEATHER_SUNNY_DESERT));
	Weather_SetName(19, _(WEATHER_SANDSTORM_DESERT));
	Weather_SetName(20, _(WEATHER_UNDERWATER));
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

	if (current_tick - LastTickCount < gWeatherTime * 60 * 1000) {
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
	Log_Game("SERVER: Weather set to %d", weatherid);
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

stock Weather_GetName(weatherid, name[], const size = sizeof(name))
{
	if (MAX_WEATHER <= weatherid < 0) {
		return 0;
	}
	return strunpack(name, gWeatherName[weatherid], size);
}

stock Weather_SetName(weatherid, name[])
{
	if (MAX_WEATHER <= weatherid < 0) {
		return 0;
	}
	return strpack(gWeatherName[weatherid], name, MAX_WEATHER_NAME);
}
