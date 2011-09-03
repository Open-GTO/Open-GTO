//
// Made by ZiGGi
//


stock antirconhack_OnRconLoginAttempt(ip[], password[], success)
{
    if(!success)
    {
        printf(lang_texts[11][5],ip,password);
		new pip[16];
		foreach(Player,playerid)
        {
			GetPVarString(playerid,"IP",pip,sizeof(pip));
            if(!strcmp(ip,pip,false))
            {
                KickPlayer(playerid,lang_texts[11][6]);
				return 1;
            }
        }
    }
	return 1;
}
