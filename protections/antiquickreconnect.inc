/*
    Этот FS не дает реконектится игроку быстрее чем через 5 сек.
    после выхода. Для проверки используется IP игрока.

    Скрипт оптимизирован, юзает динамическую память и
    надежную систему запрета доступа.

    MX_Master, 14.07.2010 2:46:25
*/

/*
aqr_OnPlayerConnect(playerid);
aqr_OnPlayerDisconnect(playerid,reason);
*/
const player_machine = 1, // ид виртуальной машины для хранения IP игроков, которые онлайн
      rec_delay_sec  = 5; // сек. после дисконекта, после котороых снова можно конектится


stock aqr_OnPlayerConnect ( playerid )
{
    new cur_str_ip[16];
    GetPlayerIp( playerid, cur_str_ip, 16 );
    setproperty( player_machine, _, playerid, cur_str_ip );
    return 1;
}

stock aqr_OnPlayerDisconnect ( playerid, reason )
{
    if ( reason == 0 ) return 1; // если был тайм аут соединения у игрока

    new saved_str_ip[16];
    getproperty(    player_machine, _, playerid, saved_str_ip );
    deleteproperty( player_machine, _, playerid );
    strunpack( saved_str_ip, saved_str_ip );

    new rcon_cmd[32];
    format( rcon_cmd, 32, "banip %s", saved_str_ip );
    SendRconCommand(rcon_cmd);

    SetTimerEx( "aqr_unban_ip", rec_delay_sec * 1000, 0, "s", saved_str_ip );

    return 1;
}



forward aqr_unban_ip ( ip[] );
public  aqr_unban_ip ( ip[] )
{
    new rcon_cmd[32];
    format( rcon_cmd, 32, "unbanip %s", ip );
    SendRconCommand(rcon_cmd);
}