/*
	
	About: automatic random weather changer
	Author: ziggi

*/

#if defined _weather_included
	#endinput
#endif

#define _weather_included
#pragma library weather

static sys_weather = SYS_WEATHER_UPDATE;

stock weather_Update()
{
	if (sys_weather == 0) {
		return 0;
	}

	static timer_ticks;
	timer_ticks++;

	if (timer_ticks >= sys_weather) {
		timer_ticks = 0;

		new weatherid;

		switch ( random(2) ) {
			case 0: {
				weatherid = mathrandom(0, 7);
			}
			case 1: {
				weatherid = mathrandom(9, 18);
			}
		}

		SetWeather(weatherid);

		GameMSG("SERVER: Weather set to %d", weatherid);
	}
	return 1;
}

stock weather_GetTime()
{
	return sys_weather;
}

stock weather_SetTime(time)
{
	sys_weather = time;
}