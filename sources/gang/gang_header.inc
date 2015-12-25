/*
	
	About: gang header
	Author: ziggi

*/

#if defined _gang_header_included
	#endinput
#endif

#define _gang_header_included
#pragma library gang_header

/*
	Defines
*/

#define INVALID_GANG_ID -1
#define INVALID_MEMBER_ID -1

/*
	Enums
*/

enum GangMemberRank {
	GangMemberLeader,
	GangMemberHelper,
	GangMemberSoldier,
}
