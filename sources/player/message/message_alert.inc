/*

	About: player alert header
	Author:	ziggi

*/

#if defined _pl_alert_h_included
	#endinput
#endif

#define _pl_alert_h_included


/*
	Macros
*/

#if !defined ALERT_POS_X
	#define ALERT_POS_X 479.5
#endif

#if !defined ALERT_POS_Y
	#define ALERT_POS_Y 286.5
#endif

#if !defined ALERT_WIDTH
	#define ALERT_WIDTH 144
#endif

#if !defined MAX_CACHE_ALERT
	#define MAX_CACHE_ALERT 6
#endif

#define INVALID_ALERT_ID -1

/*
	Enums
*/

enum e_Alerts_Info {
	bool:e_aIsBusy,
	e_aIndex,
	e_aCaption[MAX_LANG_VALUE_STRING],
	e_aInfo[MAX_LANG_VALUE_STRING],
	e_aTime,
	e_aColor,
	bool:e_aWithSound,
}

enum (<<= 1) {
	MESSAGE_NOTVAR_ALL = -1,
	MESSAGE_NOTVAR_NONE = 0,
	MESSAGE_NOTVAR_CAPTION = 1,
	MESSAGE_NOTVAR_INFO,
}
