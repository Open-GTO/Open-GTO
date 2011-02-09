//
// Created:     01.02.11
// Aurthor:    ZiGGi
//

#if defined _fastfood_included
#endinput
#endif

#define _fastfood_included
#pragma library fastfood


#define fastfood_DialogID 10

new FastFoods[][CoordInfo] = {
	{373.4858,-119.4608,1001.4922}, // LS pizza
	{377.4426,-67.5379,1001.5078}, // LS burger
	{369.3667,-6.3469,1001.8516} // LS chicken
};

enum FoodInfo {
	food_name[MAX_NAME],
	food_cost,
	Float:food_hp
}
new Food[][FoodInfo] = {
	{"Гнилой корень",10,10.0},
	{"Петрушка с хреном",50,25.0}
};

stock fastfood_OnGameModeInit()
{
	for(new ff_id=0;ff_id<sizeof(FastFoods);ff_id++)
	{
	    CreatePickup(1445,23,FastFoods[ff_id][Coord_X],FastFoods[ff_id][Coord_Y],FastFoods[ff_id][Coord_Z]);
	}
	return 1;
}

stock fastfood_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys,newkeys
	fastfood_ShowDialog(playerid);
	return 1;
}

stock fastfood_ShowDialog(playerid)
{
	new string[MAX_STRING];
	for(new i=0;i<sizeof(Food);i++)
		format(string,sizeof(string),"%s%s(цена: %d, насыщение: %0.0f)\n",string,Food[i][food_name],Food[i][food_cost],Food[i][food_hp]);
	return ShowPlayerDialog(playerid,fastfood_DialogID,DIALOG_STYLE_LIST,
		"Меню покупки еды",
		string,
		"Съесть","Отмена"
	);
}

stock fastfood_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused response,inputtext
	if(dialogid == fastfood_DialogID)
	{
		if(oGetPlayerMoney(playerid) < Food[listitem][food_cost])
			return show_Msg_Dialog(playerid,"Покупка еды","Вам не хватает денег!","ОК");
		oGivePlayerMoney(playerid,-Food[listitem][food_cost],1);
		if(Food[listitem][food_hp] + oGetPlayerHealth(playerid) > GetMaxHealth(playerid))
			oSetPlayerHealth(playerid,GetMaxHealth(playerid));
		else
			oSetPlayerHealth(playerid,oGetPlayerHealth(playerid) + Food[listitem][food_hp]);
		new string[MAX_STRING];
		format(string,sizeof(string),"Вы съели %s, за %d.",Food[listitem][food_name],Food[listitem][food_cost]);
		show_Msg_Dialog(playerid,"Покупка еды",string,"ОК");
		return 1;
	}
	return 1;
}

stock IsPlayerAtFastFood(playerid)
{
	for(new ff_id=0;ff_id<sizeof(FastFoods);ff_id++)
	{
		if(IsPlayerInRangeOfPoint(playerid,2,FastFoods[ff_id][Coord_X],FastFoods[ff_id][Coord_Y],FastFoods[ff_id][Coord_Z]))
			return 1;
	}
	return 0;
}