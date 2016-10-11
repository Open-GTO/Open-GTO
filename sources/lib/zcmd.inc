/**********************************
 *                                *
 *   @Author:      ZeeX           *
 *   @Version:     0.3.1          *
 *   @Released:    31/10/2009     *
 *                                *
 *   @Modified by: ziggi          *
 *   @Released:    10/10/2016     *
 *                                *
 **********************************/

#if defined _zcmd_included
	#endinput
#endif
#define _zcmd_included

/*
	Defines
*/

#define ZCMD_MAX_COMMAND 28
#define ZCMD_MAX_PARAMS  126

/*
	Defined functions
*/

#define COMMAND:%1(%2)          \
			forward cmd_%1(%2); \
			public cmd_%1(%2)

#define CMD:%1(%2) \
			COMMAND:%1(%2)

#define command(%1,%2,%3) \
			COMMAND:%1(%2, %3)

#define cmd(%1,%2,%3) \
			COMMAND:%1(%2, %3)

#if !defined isnull
	#define isnull(%1) \
				((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

/*
	Forwards
*/

forward OnPlayerCommandReceived(playerid, cmdtext[]);
forward OnPlayerCommandPerformed(playerid, cmdtext[], success);

/*
	OnPlayerCommandText
*/

public OnPlayerCommandText(playerid, cmdtext[])
{
	#if defined OnPlayerCommandReceived
		if (!OnPlayerCommandReceived(playerid, cmdtext)) {
			return 1;
		}
	#endif

	new
		pos = 4,
		funcname[ZCMD_MAX_COMMAND + 4] = "cmd_",
		params[ZCMD_MAX_PARAMS];

	if (sscanf(cmdtext[1], "s[" #ZCMD_MAX_COMMAND "]S()[" #ZCMD_MAX_PARAMS "]", funcname[4], params)) {
		return 0;
	}

	if (isnull(params)) {
		params[0] = '\1';
	}

	while (funcname[pos]) {
		funcname[pos] = tolower(funcname[pos]);
		pos++;
	}

	#if defined OnPlayerCommandPerformed
		return OnPlayerCommandPerformed(playerid, cmdtext, CallLocalFunction(funcname, "is", playerid, params));
	#else
		return CallLocalFunction(funcname, "is", playerid, params);
	#endif
}

#if defined _ALS_OnPlayerCommandText
	#undef OnPlayerCommandText
#else
	#define _ALS_OnPlayerCommandText
#endif

#define OnPlayerCommandText zcmd_OnPlayerCommandText
#if defined zcmd_OnPlayerCommandText
	forward zcmd_OnPlayerCommandText(playerid, cmdtext[]);
#endif
