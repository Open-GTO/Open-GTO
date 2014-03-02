/*

	About: time system
	Author: ziggi

*/

#if defined _sys_time_included
  #endinput
#endif

#define _sys_time_included
#pragma library sys_time


static
	is_real = SERVER_REAL_TIME,
	real_offset = SERVER_REAL_TIME_OFFSET,
	current_hour;

stock time_LoadConfig(file_config)
{
	ini_getInteger(file_config, "time_isreal", is_real);
	ini_getInteger(file_config, "time_real_offset", real_offset);
}

stock time_SaveConfig(file_config)
{
	ini_setInteger(file_config, "time_isreal", is_real);
	ini_setInteger(file_config, "time_real_offset", real_offset);
}

stock time_Sync()
{
	if (is_real == 0) {
		current_hour++;
		
		if (current_hour > 23) {
			current_hour = 0;
		}
	} else {
		new hour;
		gettime(hour);
		
		hour += real_offset;
		if (hour > 23) {
			hour -= 23;
		}
		
		current_hour = hour;
	}

	SetWorldTime(current_hour);
}

stock time_GetCurrentHour()
{
	return current_hour;
}