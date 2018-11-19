/**
 * Log plugin v0.4
 * by maddinat0r
 */

#if defined log_plugin_included
	#endinput
#endif
#define log_plugin_included


#if !defined E_LOGLEVEL
enum E_LOGLEVEL
{
	NONE = 0,
	DEBUG = 1,
	INFO = 2,
	WARNING = 4,
	ERROR = 8,
	
	ALL = ERROR | WARNING | INFO | DEBUG
};
#endif


native SetLogPluginLogLevel(E_LOGLEVEL:level);


native Logger:CreateLog(const name[], E_LOGLEVEL:level = INFO | WARNING | ERROR, bool:debuginfo = true);
native DestroyLog(Logger:logger);

native SetLogLevel(Logger:logger, E_LOGLEVEL:level);
native bool:IsLogLevel(Logger:logger, E_LOGLEVEL:level);

native Log(Logger:logger, E_LOGLEVEL:level, const msg[], {Float,_}:...);
