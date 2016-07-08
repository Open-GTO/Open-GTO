/*

	About: player account header
	Author:	ziggi

*/

#if defined _player_account_h_included
	#endinput
#endif

#define _player_account_h_included


enum e_Account_Info {
#if defined PASSWORD_ENCRYPT_ENABLED
	e_aPassword[PASSWORD_HASH_LENGTH + 1],
	e_aPasswordSalt[PASSWORD_SALT_LENGTH + 1],
#else
	e_aPassword[MAX_PASS_LEN + 1],
#endif
	e_aIP[MAX_IP],
	e_aLoginTime,
	e_aCreationTime,
	e_aPlayedSeconds,
	e_aPremiumTime,
	e_aLanguage[MAX_LANG_NAME],
}
