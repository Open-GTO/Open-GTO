/*
	
	About: automatic random weather changer
	Author: ziggi

*/

#if defined _weather_included
	#endinput
#endif

#define _weather_included

/*
	Vars
*/

static
	LastTickCount,
	gWeatherTime = SYS_WEATHER_UPDATE,
	gWeather[] = {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 17, 18};

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
