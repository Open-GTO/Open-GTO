/*
	
	About: automatic random weather changer
	Author: ziggi

*/

#if defined _weather_included
	#endinput
#endif

#define _weather_included
#pragma library weather

static
	sys_weather = SYS_WEATHER_UPDATE,
	weather_array[] = {0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 12, 13, 14, 15, 17, 18};

stock weather_Update()
{
	if (sys_weather == 0) {
		return 0;
	}

	static timer_ticks;
	timer_ticks++;

	if (timer_ticks >= sys_weather) {
		timer_ticks = 0;

		weather_SetRandom();
	}
	return 1;
}

stock weather_SetRandom()
{
	new weatherid = weather_array[random( sizeof(weather_array) )];
	SetWeather(weatherid);
	GameMSG("SERVER: Weather set to %d", weatherid);
}

stock weather_GetTime()
{
	return sys_weather;
}

stock weather_SetTime(time)
{
	sys_weather = time;
}