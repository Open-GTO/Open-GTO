/*

	About: gang header
	Author: ziggi

*/

#if defined _gang_header_included
	#endinput
#endif

#define _gang_header_included

/*
	Defines
*/

#define INVALID_GANG_ID -1
#define INVALID_MEMBER_ID -1

#if !defined MAX_GANG
	#define MAX_GANG MAX_PLAYERS
#endif

#if !defined MAX_GANG_SIZE
	#define MAX_GANG_SIZE 12
#endif

#if !defined MIN_GANG_SIZE
	#define MIN_GANG_SIZE 3
#endif

#if !defined MAX_GANG_NAME
	#define MAX_GANG_NAME 40
#endif

#if !defined MAX_GANG_MOTD
	#define MAX_GANG_MOTD 64
#endif

#if !defined GANG_CREATE_COST
	#define GANG_CREATE_COST 500000
#endif

#if !defined GANG_COLOR_COST
	#define GANG_COLOR_COST 20000
#endif

#if !defined MAX_GANG_BANK
	#define MAX_GANG_BANK 2000000000
#endif
