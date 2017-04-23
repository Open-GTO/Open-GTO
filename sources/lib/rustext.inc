/*
	About: rustext include
	Author: ziggi
*/

#if defined _rustext_included
	#endinput
#endif
#define _rustext_included
#pragma library rustext

// Enums
enum RussifierType {
	RussifierType_Disabled = -1,
	RussifierType_SanLtd = 0,
	RussifierType_OneC,
	RussifierType_Rush,
	RussifierType_Unknown1,
	RussifierType_Unknown2,
	RussifierType_Unknown3,
	RussifierType_MG,
	RussifierType_Community,
	RussifierType_Ukraine,
	RussifierType_Hungarian,
}

// Natives
native GetRussifierVersion(version[], const size = sizeof(version));
native GetRussifierText(RussifierType:type, string[], string_return[], const size = sizeof(string_return));

native SetPlayerRussifierType(playerid, RussifierType:type);
native RussifierType:GetPlayerRussifierType(playerid);

native SetDefaultRussifierType(RussifierType:type);
native RussifierType:GetDefaultRussifierType();

// Update checker
#if !defined HTTP
	#tryinclude <a_http>
#endif

#if !defined HTTP
	#tryinclude "a_http"
#endif

#if !defined HTTP
	#tryinclude "../a_http"
#endif

#if defined HTTP && !defined RUSTEXT_DISABLE_VERSION_CHECK
	#if defined FILTERSCRIPT
		public OnFilterScriptInit()
		{
			RusText_SendVersionRequest();

			#if defined RusText_OnFilterScriptInit
				return RusText_OnFilterScriptInit();
			#else
				return 1;
			#endif
		}
		#if defined _ALS_OnFilterScriptInit
			#undef OnFilterScriptInit
		#else
			#define _ALS_OnFilterScriptInit
		#endif

		#define OnFilterScriptInit RusText_OnFilterScriptInit
		#if defined RusText_OnFilterScriptInit
			forward RusText_OnFilterScriptInit();
		#endif
	#else
		public OnGameModeInit()
		{
			RusText_SendVersionRequest();

			#if defined RusText_OnGameModeInit
				return RusText_OnGameModeInit();
			#else
				return 1;
			#endif
		}
		#if defined _ALS_OnGameModeInit
			#undef OnGameModeInit
		#else
			#define _ALS_OnGameModeInit
		#endif

		#define OnGameModeInit RusText_OnGameModeInit
		#if defined RusText_OnGameModeInit
			forward RusText_OnGameModeInit();
		#endif
	#endif

	static stock RusText_SendVersionRequest()
	{
		HTTP(0, HTTP_GET,
		     "ziggi.org/github/lastrelease.php?owner=ziggi&repo=rustext&elem=tag_name",
		     "",
		     "RusText_OnRequestResponse");
	}

	forward RusText_OnRequestResponse(index, response_code, data[]);
	public RusText_OnRequestResponse(index, response_code, data[])
	{
		if (index == 0 && response_code == 200) {
			new length = strlen(data);
			if (length == 0) {
				return;
			}

			new last_version[16], plugin_version[16];
			strmid(last_version, data, 1, length);
			GetRussifierVersion(plugin_version);

			if (strcmp(last_version, plugin_version) != 0) {
				printf("\n** Please update your rustext plugin to %s from here: https://github.com/ziggi/rustext/releases.\n", data);
			}
		}
	}
#endif
