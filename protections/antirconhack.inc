//
//
//


stock antirconhack_OnRconLoginAttempt(ip[], password[], success)
{
    if(!success)
    {
        printf("Неудачный вход рконом с ip `%s`, использовал пароль: %s",ip, password);
        new pip[16];
        for(new i=0;i<MAX_PLAYERS;i++)
        {
            GetPlayerIp(i, pip, sizeof(pip));
            if(!strcmp(ip, pip, true))
            {
                SendClientMessage(i,COLOUR_RED,"Неправильный пароль. Досвидание!");
                Kick(i);
            }
        }
    }
	return 1;
}
