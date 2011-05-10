//
// Made by ZiGGi
//


stock antirconhack_OnRconLoginAttempt(ip[], password[], success)
{
    if(!success)
    {
        printf(lang_texts[11][5],ip, password);
		for(new playerid=GetMaxPlayers()-1,pip[16];playerid>=0;--playerid)
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
