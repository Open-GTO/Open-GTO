/*
	About:	adaptation to Incognito Streamer
	Author: ziggi
*/

#if defined _incognito_streamer_included
	#endinput
#endif

#define _incognito_streamer_included

#if !defined Streamer_IncludeFileVersion
	#endinput
#endif

#define INCOGNITO_STREAMER_USED

// object
#define CreateObject CreateDynamicObject

/*
// mapicon
#define _streamer_mapicon_included
#define Mapicon_OnPlayerConnect(%0);
#define Mapicon_Stream(%0);

#define Mapicon_Create CreateDynamicMapIcon
#define Mapicon_Delete DestroyDynamicMapIcon
#define Mapicon_IsValid IsValidDynamicMapIcon


// checkpoint
#define _checkpoint_stream_included
#define Checkpoint_OnPlayerConnect(%0);
#define Checkpoint_Stream(%0);

#define Checkpoint_Create CreateDynamicCP
#define Checkpoint_Delete DestroyDynamicCP
#define Checkpoint_IsValid IsValidDynamicCP
#define Checkpoint_ToggleForPlayer TogglePlayerDynamicCP
#define Checkpoint_ToggleAllForPlayer TogglePlayerAllDynamicCPs
#define Checkpoint_IsPlayerIn IsPlayerInDynamicCP
#define Checkpoint_GetPlayerVisible GetPlayerVisibleDynamicCP
*/