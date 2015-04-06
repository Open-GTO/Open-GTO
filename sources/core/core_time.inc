/*

	About: time system
	Author: ziggi

*/

#if defined _core_time_included
  #endinput
#endif

#define _core_time_included
#pragma library core_time


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
	new hour = time_GetCurrentHour();

	if (is_real == 0) {
		hour++;
	} else {
		gettime(hour);
	}

	time_SetCurrentHour(hour);
}

stock time_GetCurrentHour()
{
	return current_hour;
}

stock time_SetCurrentHour(value)
{
	current_hour = value;

	if (is_real == 0) {
		if (current_hour > 23) {
			current_hour = 0;
		}
	} else {
		current_hour += real_offset;

		if (current_hour > 23) {
			current_hour -= 23;
		}
	}

	SetWorldTime(current_hour);
}

stock time_SetRealStatus(bool:isrealstatus)
{
	is_real = isrealstatus ? 1 : 0;
	if (is_real) {
		time_Sync();
	}
}