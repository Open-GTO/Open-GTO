/*
	
	Описание: автоматическая, рандомная смена погоды
	Автор: ZiGGi

*/

#if defined _weather_included
	#endinput
#endif

#define _weather_included
#pragma library weather


forward weather_WeatherUpdate();
public weather_WeatherUpdate()
{
	if (sys_weather == 0) return 0;
	static timer_ticks;
	timer_ticks++;
	if (sys_weather == timer_ticks)
	{
		timer_ticks = 0;
		new weatherid;
		switch ( random(2) )
		{
			case 0: weatherid = mathrandom(0,7);
			case 1: weatherid = mathrandom(9,18);
		}
		SetWeather(weatherid);
		GameMSG("SERVER: Weather set to %d",weatherid);
	}
	return 1;
}
