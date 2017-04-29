/*

	About: color functions
	Author: ziggi

*/

#if defined _color_included
	#endinput
#endif

#define _color_included


/*
	Color defines
*/

// gto colors
#define COLOR_GREY 0xAFAFAFFF
#define COLOR_GREEN 0x33FF33FF
#define COLOR_LIGHTGREEN 0x9ACD32FF
#define COLOR_RED 0xAA3333FF
#define COLOR_LIGHTRED 0xFF6347FF
#define COLOR_YELLOW 0xFFFF00FF
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLUE 0x0000FFFF
#define COLOR_LIGHTBLUE 0x33CCFFFF
#define COLOR_SKYBLUE 0x87CEEBFF
#define COLOR_ORANGE 0xFF9900FF
#define COLOR_PURPLE 0xC2A2DAFF
#define COLOR_MISC 0xAFAFAFFF
#define COLOR_PLAYER 0xAAAAAAFF
#define COLOR_XP_GOOD 0x00C7FFFF
#define COLOR_XP_BAD 0x00008BFF
#define COLOR_MONEY_GOOD 0xFFFF00FF
#define COLOR_RACE 0x005407FF
#define COLOR_RACE_BAD 0x545407FF
#define COLOR_GANG 0xFF8C00FF
#define COLOR_GANG_CHAT 0xFFA500FF
#define COLOR_DM 0xFF0066FF
#define COLOR_DM_BAD 0xFF0066FF
#define COLOR_PM 0xFFCC22FF
#define WARN_MS_COLOR 0xFF5050FF

// material colors (https://www.google.com/design/spec/style/color.html#color-color-palette)
#define COLOR_BLUEGREY_200 0xB0BEC5FF
#define COLOR_TEAL_400 0x26A69AFF
#define COLOR_RED_500 0xF44336FF
#define COLOR_BLUE_500 0x2196F3FF

// vehicle compatible colors
#define COLOR_BLACK 0x000000FF
#define COLOR_RED_DEVIL 0x840410FF
#define COLOR_GOLDEN_BELL 0xD78E10FF
#define COLOR_DEEP_BLUSH 0xEC6AAEFF
#define COLOR_JELLY_BEAN 0x2C89AAFF
#define COLOR_FUCHSIA_BLUE 0x8A4DBDFF
#define COLOR_APPLE 0x35963AFF
#define COLOR_EAST_BAY 0x464C8DFF
#define COLOR_OLIVINE 0x8CB972FF
#define COLOR_MATISSE 0x1E4C99FF
#define COLOR_EUCALYPTUS 0x1E9948FF
#define COLOR_EASTERN_BLUE 0x1E9999FF
#define COLOR_VIN_ROUGE 0x993E4DFF
#define COLOR_VAN_CLEEF 0x481A0EFF
#define COLOR_OLD_BRICK 0x991E1EFF
#define COLOR_GOBLIN 0x368452FF
#define COLOR_ROPE 0x99581EFF
#define COLOR_PRAIRIE_SAND 0x99311EFF
#define COLOR_VIDA_LOCA 0x4C991EFF
#define COLOR_HACIENDA 0x96821DFF
#define COLOR_CEDAR 0x3B141FFF
#define COLOR_DISCO 0x7E1A6CFF
#define COLOR_GREEN_HOUSE 0x27450DFF
#define COLOR_BOTTLE_GREEN 0x071F24FF
#define COLOR_POTTERS_CLAY 0x8A653AFF
#define COLOR_MOCHA 0x732617FF
#define COLOR_LOCHINVAR 0x319490FF
#define COLOR_CHAMBRAY 0x355D8EFF
#define COLOR_MAROON_OAK 0x4E0E27FF
#define COLOR_TUNA 0x3B3E42FF

/*
	Param defines
*/

#define MAX_COLOR_NAME 16

/*
	gColors
*/

enum e_Colors_Info {
	e_cName[MAX_COLOR_NAME],
	e_cCode,
	e_cVehicle,
}

static gColors[][e_Colors_Info] = {
	{"Black", COLOR_BLACK, 0},
	{"White", COLOR_WHITE, 1},
	{"Red Devil", COLOR_RED_DEVIL, 3},
	{"Golden Bell", COLOR_GOLDEN_BELL, 6},
	{"Deep Blush", COLOR_DEEP_BLUSH, 126},
	{"Jelly Bean", COLOR_JELLY_BEAN, 135},
	{"Fuchsia Blue", COLOR_FUCHSIA_BLUE, 136},
	{"Apple", COLOR_APPLE, 137},
	{"East Bay", COLOR_EAST_BAY, 139},
	{"Olivine", COLOR_OLIVINE, 145},
	{"Matisse", COLOR_MATISSE, 152},
	{"Eucalyptus", COLOR_EUCALYPTUS, 154},
	{"Eastern Blue", COLOR_EASTERN_BLUE, 155},
	{"Vin Rouge", COLOR_VIN_ROUGE, 161},
	{"Van Cleef", COLOR_VAN_CLEEF, 168},
	{"Old Brick", COLOR_OLD_BRICK, 175},
	{"Goblin", COLOR_GOBLIN, 191},
	{"Rope", COLOR_ROPE, 219},
	{"Prairie Sand", COLOR_PRAIRIE_SAND, 222},
	{"Vida Loca", COLOR_VIDA_LOCA, 226},
	{"Hacienda", COLOR_HACIENDA, 228},
	{"Cedar", COLOR_CEDAR, 230},
	{"Disco", COLOR_DISCO, 233},
	{"Green House", COLOR_GREEN_HOUSE, 235},
	{"Bottle Green", COLOR_BOTTLE_GREEN, 236},
	{"Potters Clay", COLOR_POTTERS_CLAY, 238},
	{"Mocha", COLOR_MOCHA, 239},
	{"Lochinvar", COLOR_LOCHINVAR, 240},
	{"Chambray", COLOR_CHAMBRAY, 246},
	{"Maroon Oak", COLOR_MAROON_OAK, 249},
	{"Tuna", COLOR_TUNA, 251}
};

const MAX_COLOR_COUNT = sizeof(gColors);

/*
	Public functions
*/

stock GetColorEmbeddingCode(color, code[], const size = sizeof(code))
{
	format(code, size, "%06x", color >>> 8);
}

stock GetPlayerEmbeddingCode(playerid, code[], const size = sizeof(code))
{
	GetColorEmbeddingCode(GetPlayerColor(playerid), code, size);
}

stock ret_GetPlayerEmbeddingCode(playerid)
{
	new code[7];
	GetPlayerEmbeddingCode(playerid, code);
	return code;
}

stock Color_GetEmbeddingCode(id, code[], const size = sizeof(code))
{
	GetColorEmbeddingCode(gColors[id][e_cCode], code, size);
}

stock Color_ReturnEmbeddingCode(id)
{
	new code[7];
	GetColorEmbeddingCode(gColors[id][e_cCode], code);
	return code;
}

stock Color_GetCode(id)
{
	return gColors[id][e_cCode];
}

stock Color_GetVehicleCode(id)
{
	return gColors[id][e_cVehicle];
}

stock Color_GetName(id, name[], size = sizeof(name))
{
	strcpy(name, gColors[id][e_cName], size);
}

stock Color_ReturnName(id)
{
	new name[MAX_COLOR_NAME];
	Color_GetName(id, name);
	return name;
}

stock Color_GetIdByName(name[])
{
	for (new i = 0; i < MAX_COLOR_COUNT; i++) {
		if (strcmp(name, gColors[i][e_cName], true) == 0) {
			return i;
		}
	}
	return -1;
}

stock Color_GetCodeByName(name[])
{
	for (new i = 0; i < MAX_COLOR_COUNT; i++) {
		if (strcmp(name, gColors[i][e_cName], true) == 0) {
			return gColors[id][e_cCode];
		}
	}
	return -1;
}
