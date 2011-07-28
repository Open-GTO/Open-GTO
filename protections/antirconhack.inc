//
// Made by ZiGGi
//


stock antirconhack_OnRconLoginAttempt(ip[], password[], success)
{
    if(!success)
    {
        printf(lang_texts[11][5],ip,password);
		for(new playerid=0,pip[16];playerid<=GetPlayerLastID();playerid++)
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
