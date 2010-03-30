//
// Created: 05.09.06
// Aurthor: Iain Gilbert
//


#if defined _weapons_included
#endinput
#endif

#define _weapons_included
#pragma library weapons

#include "base"
#include "logging"

#define MAX_BULLETS 2999
#define WeaponsSize 47
#define MAX_WEAPONS 46
#define AmmunationsCount 4

#define pistol_bullets 14
#define assault_bullets 30
#define shotgun_bullets 10
#define smg_bullets 100

new Menu:WeaponMenu,Menu:PistolMenu,Menu:MicroSMGMenu,Menu:ShotgunsMenu,Menu:AssaultMenu,WeapPickup[4];
new WeaponDB[MAX_STRING] = "GTO/Weapon/";
new Ammunations[AmmunationsCount][CoordInfo] = 
{
	{291.0004,-84.5168,1001.5156}, // LVammunation1
	{293.2710,-108.9645,1001.5156}, // LSAmmunation1
	{290.3177,-38.7184,1001.5156}, // LSAmmunation2
	{316.4356,-165.1367,999.6010} // север карты
};

enum WeaponInfo 
{
	Weapon_ID, // id number of weapon
	Weapon_Name[MAX_NAME], // weapon name
	Weapon_Allowed, // is weapon allowed
	Weapon_MaxBullets, // max bullets
	Weapon_Cost, // cost per bullet
	Weapon_Level // player level required to buy this weapon
}

new Weapons[WeaponsSize][WeaponInfo] = {
	//id, "Name" ,Allowed,bullets, cost , level
	{0, "Невооружен" , 0 , 1 , 0 , 0},
	{1, "Кастет" , 1 , 1 , 6000 , 3},
	{2, "Клюшка для гольфа" , 1 , 1 , 6000 , 2},
	{3, "Night Stick" , 0 , 1 , 10000 , 18},
	{4, "Нож" , 1 , 1 , 10000 , 4},
	{5, "Бейсбольная Бита" , 1 , 1 , 15000 , 7},
	{6, "Shovel" , 0 , 1 , 1000 , 0},
	{7, "Кий" , 1 , 1 , 1000 , 11},
	{8, "Меч Катана" , 1 , 1 , 20000 , 8},
	{9, "Бензопила" , 1 , 1 , 7500 , 6},
	{10, "Пурпурный Dildo" , 0 , 1 , 5000 , 9},
	{11, "Вибратор" , 1 , 1 , 5000 , 5},
	{12, "Длинный Белый Dildo" , 0 , 1 , 7000 , 15},
	{13, "Вибратор 2" , 1 , 1 , 5000 , 11},
	{14, "Букет Цветов" , 1 , 1 , 1000 , 12},
	{15, "Трость" , 1 , 1 , 1000 , 8},
	{16, "Граната" , 1 , 1 , 67500 , 43},
	{17, "Газовый Баллон" , 1 , 1 , 20000 , 5},
	{18, "Коктейль Молотова" , 1 , 1 , 30000 , 35},

	{19, "Неизвестное" , 0 , 1 , 1000 , 25},
	{20, "Неизвестное" , 0 , 1 , 1000 , 25},
	{21, "Неизвестное" , 0 , 1 , 1000 , 25},

	{22, "Пистолет" , 1 , 999 , 125 , 0},
	{23, "Пистолет с глушителем" , 1 , 999 , 125 , 10},
	{24, "Пустынный Орел" , 1 , 999 , 1200 , 12},
	{25, "Дробовик" , 1 , 999 , 1100 , 5},
	{26, "Разрывной Дробовик" , 1 , 999 , 2300 , 25},
	{27, "Боевой Дробовик" , 1 , 999 , 3750 , 26},
	{28, "Микро УЗИ (ПолуАвтомат)" , 1 , 999 , 2100 , 30},
	{29, "МП5(НК5) (ПолуАвтомат)" , 1 , 999 , 2200 , 32},
	{30, "АК47 (Автомат Калашникова)" , 1 , 999 , 1500 , 35},
	{31, "M4 (Автомат)" , 1 , 999 , 4300 , 36},
	{32, "Tec9 (ПолуАвтомат)" , 1 , 999 , 4150 , 29},
	{33, "Винтовка" , 1 , 999 , 11500 , 20},
	{34, "Снайперская Винтовка" , 1 , 1 , 41000 , 38},
	{35, "Гранатомет РПГ" , 1 , 1 , 99000 , 60},
	{36, "Реактивная Пусковая установка" , 1 , 1 , 99000 , 52},
	{37, "Огнемет" , 1 , 1 , 1000 , 28},
	{38, "Ручной Пулемет (Миниган)" , 1 , 1 , 5000 , 60},

	{39, "Заряженный Рюкзак" , 0 , 1 , 50000 , 62},
	{40, "Детонатор к Рюкзаку" , 0 , 1 , 100 , 62},

	{41, "Баллончик с краской" , 1 , 1 , 4000 , 5},
	{42, "Огнетушитель" , 1 , 1 , 1000 , 5},
	{43, "Фотокамера" , 1 , 1 , 6000 , 24},
	{44, "Очки ночного видения", 0 , 1 , 1000 , 0},
	{45, "Тепловые Очки" , 0 , 1 , 1000 , 0},
	{46, "Парашют" , 1 , 1 , 7000 , 1}
};

stock WeaponLoadAll()
{
	new file;
	if(ini_Exist(ConfigDB))
	{
		file = ini_Open(ConfigDB);
		ini_Get(file,"Weapon_DB",WeaponDB);
		ini_Close(file);
	}
	if(!ini_Exist(WeaponDB))
	{
	    print("WeaponSaveAll();");
		WeaponSaveAll();
		return;
	}
	for(new i=0;i<WeaponsSize;i++)
	{
		new weapondbname[MAX_STRING];
		format(weapondbname,sizeof(weapondbname),"%s%s.txt",WeaponDB,Weapons[i][Weapon_Name]);
		file = ini_Open(weapondbname);
		ini_Get(file,"Name",Weapons[i][Weapon_Name],MAX_STRING);
        ini_GetInt(file,"IsAllowed",Weapons[i][Weapon_Allowed]);
        ini_GetInt(file,"Level",Weapons[i][Weapon_Level]);
        ini_GetInt(file,"Price",Weapons[i][Weapon_Cost]);
		ini_Close(file);
	}
	return;
}

stock WeaponSaveAll()
{
	new file;
	if(ini_Exist(ConfigDB))
	{
		file = ini_Open(ConfigDB);
		ini_Set(file,"Weapon_DB",WeaponDB);
		ini_Close(file);
	}
	for(new i=0;i<WeaponsSize;i++)
	{
		new weapondbname[MAX_STRING];
		format(weapondbname,sizeof(weapondbname),"%s%s.txt",WeaponDB,Weapons[i][Weapon_Name]);
		if(!ini_Exist(weapondbname))
		{
			new File:fhnd = fopen(weapondbname,io_write);
			fclose(fhnd);
		}
		file = ini_Open(weapondbname);
		ini_Set(file,"Name",Weapons[i][Weapon_Name]);
		ini_SetInt(file,"IsAllowed",Weapons[i][Weapon_Allowed]);
		ini_SetInt(file,"Level",Weapons[i][Weapon_Level]);
		ini_SetInt(file,"Price",Weapons[i][Weapon_Cost]);
		ini_Close(file);
	}
	return;
}

stock weapons_OnGameModeInit()
{
	WeaponLoadAll();
	WeaponMenuInit();
	for(new id=0;id<AmmunationsCount;id++)
	{
		WeapPickup[id] = CreatePickup(1247,1,Ammunations[id][Coord_X],Ammunations[id][Coord_Y],Ammunations[id][Coord_Z]);
	}
	WriteLog(GameLog,"Ammunition checkpoints initialised");
}

stock weapons_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys
	if(newkeys == KEY_USING)
	{
		for(new id=0;id<AmmunationsCount;id++)
		{
			if(IsPlayerInRangeOfPoint(playerid,2,Ammunations[id][Coord_X],Ammunations[id][Coord_Y],Ammunations[id][Coord_Z]))
			{
				TogglePlayerControllable(playerid,false);
				ShowMenuForPlayer(WeaponMenu,playerid);
			}
		}
	}
	return 1;
}

stock IsPlayerAtAmmunation(playerid)
{
	for(new id=0;id<AmmunationsCount;id++)
	{
		if(IsPlayerInRangeOfPoint(playerid,2,Ammunations[id][Coord_X],Ammunations[id][Coord_Y],Ammunations[id][Coord_Z])) return 1;
	}
	return 0;
}

stock oGetWeaponName(weaponid)
{
	new name[MAX_NAME];
	new lookupid=GetWeaponLookupID(weaponid);
	if(lookupid == -1) return name;
	set(name,Weapons[lookupid][Weapon_Name]);
	return name;
}

stock GetWeaponMaxAmmo(weaponid)
{
	new ammo;
	new lookupid=GetWeaponLookupID(weaponid);
	if(lookupid == -1) return ammo;
	ammo = Weapons[lookupid][Weapon_MaxBullets];
	return ammo;
}

stock IsPlayerAllowedWeapon(playerid,weaponid)
{
	new lookupid=GetWeaponLookupID(weaponid);
	if(lookupid == -1) return 0;
	if(Weapons[lookupid][Weapon_Allowed] == 0) return 0;
	if(Weapons[lookupid][Weapon_Level] > GetPlayerLevel(playerid)) return 0;
	return 1;
}

stock IsWeaponHandToHand(weaponid)
{
	new weaponlookupid = GetWeaponLookupID(weaponid);
	if(weaponlookupid == -1) return 0;
	if(Weapons[weaponlookupid][Weapon_MaxBullets] == 1) return 1;
	return 0;
}

stock GetWeaponCost(weaponid)
{
	new weaponlookupid = GetWeaponLookupID(weaponid);
	if(weaponlookupid == -1) return 0;
	return Weapons[weaponlookupid][Weapon_Cost];
}

stock GetWeaponLookupID(weaponid)
{
	new lookupid = -1;
	for(new i=0;i<WeaponsSize;i++)
	{
		if(weaponid == Weapons[i][Weapon_ID]) lookupid = i;
	}
	return lookupid;
}

stock IsWeapon(weaponid)
{
	if((weaponid >= 0) && (weaponid <= 46)) return 1;
	return 0;
}

stock IsStoreWeapon(weaponid)
{
	switch(weaponid)
	{
	    case 22..34,16..18,41..43,37,39: return 1;
	}
	return 0;
}

stock SendPlayerVendorWeapons(playerid)
{
	new string[MAX_STRING];
	SendClientMessage(playerid,COLOUR_GREEN,lang_texts[16][5]);
	for(new weaponid=1;weaponid<MAX_WEAPONS;weaponid++)
	{
		if(IsPlayerAllowedWeapon(playerid,weaponid))
		{
			format(string,sizeof(string),lang_texts[16][6],oGetWeaponName(weaponid),GetWeaponCost(weaponid),weaponid);
			SendPlayerScrollingText(playerid, string);
		}
	}
	return 1;
}

stock BuyWeapon(playerid,weaponid,bullets)
{
	new string[MAX_STRING];
	if((weaponid < 1) || (weaponid > 46))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[16][7]);
		return 0;
	}
	if((bullets < 1) || (bullets > MAX_BULLETS))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[16][8]);
		return 0;
	}
	if(!IsPlayerAtAmmunation(playerid))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[16][9]);
		return 0;
	}
	if(!IsPlayerAllowedWeapon(playerid,weaponid))
	{
		format(string,sizeof(string),lang_texts[16][10],oGetWeaponName(weaponid));
		SendClientMessage(playerid,COLOUR_RED,string);
		return 0;
	}
	new purchasecost = GetWeaponCost(weaponid) * bullets;
	if(purchasecost < 1)
	{
		format(string,sizeof(string),lang_texts[16][11],oGetWeaponName(weaponid));
		SendClientMessage(playerid,COLOUR_RED,string);
		return 0;
	}
	if(oGetPlayerMoney(playerid) < purchasecost)
	{
		format(string,sizeof(string),lang_texts[16][12],purchasecost);
		SendClientMessage(playerid,COLOUR_RED,string);
		return 0;
	}
	oGivePlayerMoney(playerid,-purchasecost,1);
	oGivePlayerWeapon(playerid,weaponid,bullets);
	if(bullets > 1) format(string,sizeof(string),lang_texts[16][13],bullets,oGetWeaponName(weaponid),purchasecost);
	else format(string,sizeof(string),lang_texts[16][14],oGetWeaponName(weaponid),purchasecost);
	SendClientMessage(playerid,COLOUR_RED,string);
	SendClientMessage(playerid,COLOUR_RED,lang_texts[16][15]);
	return 1;
}

stock weapons_OnPlayerCommandText(playerid,text[]) // process player commands
{
	if(!IsPlayerRegistered(playerid)) return 0; // non registered players dont use weapon commands
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(strlen(cmd) == 0) return 0;
	if(!strcmp(cmd,"/weapons",true))
	{
		SendPlayerOwnedWeapons(playerid);
		return 1;
	}

	if(!strcmp(cmd,"/weapon",true))
	{
		set(cmd,strcharsplit(text, idx,strchar(" ")));

		if(!strcmp(cmd,"help",true)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[16][21]);
		if(!strcmp(cmd,"list",true))
		{
			if(!IsPlayerAtAmmunation(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[16][22]);
			SendPlayerVendorWeapons(playerid);
			return 1;
		}
		if(!strcmp(cmd,"buy",true))
		{
			if(!IsPlayerAtAmmunation(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[16][23]);
			set(cmd,strcharsplit(text, idx,strchar(" ")));
			if(strlen(cmd) == 0) return SendPlayerVendorWeapons(playerid);
			new weaponid = strval(cmd);
			if(weaponid <= 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[16][25]);
			new bullets;
			set(cmd,strcharsplit(text, idx,strchar(" ")));
			if(strlen(cmd) == 0) bullets = 1;
			else bullets = strval(cmd);
			if(bullets < 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[16][26]);
			BuyWeapon(playerid,weaponid,bullets);
			return 1;
		}
	}
	return 0;
}

stock WeaponMenuInit()
{
	new cost[MAX_STRING],costt;
	WeaponMenu = CreateMenu("Ammu-nation",1,50.0,200.0,100.0,200.0);
	AddMenuItem(WeaponMenu,0,"Pistols");
	AddMenuItem(WeaponMenu,0,"Micro SMG");
	AddMenuItem(WeaponMenu,0,"Shotguns");
	AddMenuItem(WeaponMenu,0,"Armor");
	AddMenuItem(WeaponMenu,0,"Assault");
	
	PistolMenu = CreateMenu("Pistols",2,100.0,200.0,100.0,200.0);
	SetMenuColumnHeader(PistolMenu,0,"Weapon");
	SetMenuColumnHeader(PistolMenu,1,"Cost");
	AddMenuItem(PistolMenu,0,"9mm");
	costt = GetWeaponCost(WEAPON_COLT45)*pistol_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(PistolMenu,1,cost);
	AddMenuItem(PistolMenu,0,"9mm silenced");
	costt = GetWeaponCost(WEAPON_SILENCED)*pistol_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(PistolMenu,1,cost);
	AddMenuItem(PistolMenu,0,"Deagle");
	costt = GetWeaponCost(WEAPON_DEAGLE)*pistol_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(PistolMenu,1,cost);
	
	MicroSMGMenu = CreateMenu("Micro-SMG",2,100.0,200.0,100.0,200.0);
	SetMenuColumnHeader(MicroSMGMenu,0,"Weapon");
	SetMenuColumnHeader(MicroSMGMenu,1,"Cost");
	AddMenuItem(MicroSMGMenu,0,"Uzi");
	costt = GetWeaponCost(WEAPON_SHOTGUN)*smg_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(MicroSMGMenu,1,cost);
	AddMenuItem(MicroSMGMenu,0,"MP5");
	costt = GetWeaponCost(WEAPON_SAWEDOFF)*smg_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(MicroSMGMenu,1,cost);
	AddMenuItem(MicroSMGMenu,0,"Tec9");
	costt = GetWeaponCost(WEAPON_SHOTGSPA)*smg_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(MicroSMGMenu,1,cost);
	
	ShotgunsMenu = CreateMenu("Shotguns",2,100.0,200.0,100.0,200.0);
	SetMenuColumnHeader(ShotgunsMenu,0,"Weapon");
	SetMenuColumnHeader(ShotgunsMenu,1,"Cost");
	AddMenuItem(ShotgunsMenu,0,"Shotgun");
	costt = GetWeaponCost(WEAPON_SHOTGUN)*shotgun_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(ShotgunsMenu,1,cost);
	AddMenuItem(ShotgunsMenu,0,"Sawedoff");
	costt = GetWeaponCost(WEAPON_SAWEDOFF)*shotgun_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(ShotgunsMenu,1,cost);
	AddMenuItem(ShotgunsMenu,0,"Shotgspa");
	costt = GetWeaponCost(WEAPON_SHOTGSPA)*shotgun_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(ShotgunsMenu,1,cost);
	
	AssaultMenu = CreateMenu("Assault",2,100.0,200.0,50.0,100.0);
	SetMenuColumnHeader(AssaultMenu,0,"Weapon");
	SetMenuColumnHeader(AssaultMenu,1,"Cost");
	AddMenuItem(AssaultMenu,0,"Ak47");
	costt = GetWeaponCost(WEAPON_AK47)*assault_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(AssaultMenu,1,cost);
	AddMenuItem(AssaultMenu,0,"M4");
	costt = GetWeaponCost(WEAPON_M4)*assault_bullets;
	format(cost,sizeof(cost),"%d",costt);
	AddMenuItem(AssaultMenu,1,cost);
}
		
stock weapons_OnPlayerSelectedMenuRow(playerid,row)
{
	new Menu:Current = GetPlayerMenu(playerid),bullets;
	if(Current == WeaponMenu) 
	{
		switch(row) 
		{
			case 0: ShowMenuForPlayer(PistolMenu,playerid);
			case 1: ShowMenuForPlayer(MicroSMGMenu,playerid);
			case 2: ShowMenuForPlayer(ShotgunsMenu,playerid);
			case 3: 
			{
				SetPlayerArmour(playerid,100);
				ShowMenuForPlayer(WeaponMenu,playerid);
			}
			case 4: ShowMenuForPlayer(AssaultMenu,playerid);
		}
	}
	if(Current == PistolMenu)
	{
		bullets = 14;
		switch(row) 
		{
			case 0: 
			{
				BuyWeapon(playerid,22,bullets);
				ShowMenuForPlayer(PistolMenu,playerid);
			}
			case 1: 
			{
				BuyWeapon(playerid,23,bullets);
				ShowMenuForPlayer(PistolMenu,playerid);
			}
			case 2: 
			{
				BuyWeapon(playerid,24,bullets);
				ShowMenuForPlayer(PistolMenu,playerid);
			}
		}
	}
	if(Current == MicroSMGMenu)
	{
		bullets = 100;
		switch(row) 
		{
			case 0: 
			{
				BuyWeapon(playerid,28,bullets);
				ShowMenuForPlayer(MicroSMGMenu,playerid);
			}
			case 1: 
			{
				BuyWeapon(playerid,29,bullets);
				ShowMenuForPlayer(MicroSMGMenu,playerid);
			}
			case 2: 
			{
				BuyWeapon(playerid,32,bullets);
				ShowMenuForPlayer(MicroSMGMenu,playerid);
			}
		}
	}
	if(Current == ShotgunsMenu)
	{
		bullets = 10;
		switch(row) 
		{
			case 0: 
			{
				BuyWeapon(playerid,25,bullets);
				ShowMenuForPlayer(ShotgunsMenu,playerid);
			}
			case 1: 
			{
				BuyWeapon(playerid,26,bullets);
				ShowMenuForPlayer(ShotgunsMenu,playerid);
			}
			case 2: 
			{
				BuyWeapon(playerid,27,bullets);
				ShowMenuForPlayer(ShotgunsMenu,playerid);
			}
		}
	}
	if(Current == AssaultMenu)
	{
		bullets = 30;
		switch(row) 
		{
			case 0: 
			{
				BuyWeapon(playerid,30,bullets);
				ShowMenuForPlayer(AssaultMenu,playerid);
			}
			case 1: 
			{
				BuyWeapon(playerid,31,bullets);
				ShowMenuForPlayer(AssaultMenu,playerid);
			}
		}
	}
	return 1;
}

stock weapons_OnPlayerExitedMenu(playerid)
{
	new Menu:tmpMenu = GetPlayerMenu(playerid);
	if(tmpMenu != WeaponMenu) ShowMenuForPlayer(WeaponMenu,playerid);
	else 
	{
		HideMenuForPlayer(tmpMenu,playerid);
		TogglePlayerControllable(playerid,true);
	}
	return 1;
}
