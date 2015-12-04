/*
	
	About: color functions
	Author: ziggi

*/

#if defined _color_included
	#endinput
#endif

#define _color_included
#pragma library color


// gto colors
#define COLOUR_GREY 0xAFAFAFFF
#define COLOUR_GREEN 0x33FF33FF
#define COLOUR_LIGHTGREEN 0x9ACD32FF
#define COLOUR_RED 0xAA3333FF
#define COLOUR_LIGHTRED 0xFF6347FF
#define COLOUR_YELLOW 0xFFFF00FF
#define COLOUR_WHITE 0xFFFFFFFF
#define COLOUR_BLUE 0x0000FFFF
#define COLOUR_LIGHTBLUE 0x33CCFFFF
#define COLOUR_SKYBLUE 0x87CEEBFF
#define COLOUR_ORANGE 0xFF9900FF
#define COLOUR_PURPLE 0xC2A2DAFF
#define COLOUR_MISC 0xAFAFAFFF
#define COLOUR_PLAYER 0xAAAAAAFF
#define COLOUR_XP_GOOD 0x00C7FFFF
#define COLOUR_XP_BAD 0x00008BFF
#define COLOUR_MONEY_GOOD 0xFFFF00FF
#define COLOUR_RACE 0x005407FF
#define COLOUR_RACE_BAD 0x545407FF
#define COLOUR_GANG 0xFF8C00FF
#define COLOUR_GANG_CHAT 0xFFA500FF
#define COLOUR_DM 0xFF0066FF
#define COLOUR_DM_BAD 0xFF0066FF
#define COLOUR_PM 0xFFCC22FF
#define WARN_MS_COLOR 0xFF5050FF

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

#define COLOR_COUNT sizeof(gColors)
#define MAX_COLOR_NAME 16

enum e_Colors_Info {
	e_cName[MAX_COLOR_NAME],
	e_cCode,
	e_cVehicle,
}

static gColors[][e_Colors_Info] = {
	{"Black", COLOR_BLACK, 0},
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

stock Color_GetCount()
{
	return sizeof(gColors);
}

stock Color_GetEmbeddingCode(id, code[], size = sizeof(code))
{
	format(code, size, "%06x", gColors[id][e_cCode] >>> 8);
}

stock Color_ReturnEmbeddingCode(id)
{
	new code[8];
	format(code, sizeof(code), "%06x", gColors[id][e_cCode] >>> 8);
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
	strmid(name, gColors[id][e_cName], 0, strlen(gColors[id][e_cName]), size);
}

stock Color_ReturnName(id)
{
	new name[MAX_COLOR_NAME];
	strmid(name, gColors[id][e_cName], 0, strlen(gColors[id][e_cName]));
	return name;
}

stock Color_GetIdByName(name[])
{
	for (new i = Color_GetCount() - 1; i != -1; i--) {
		if (strcmp(name, gColors[i][e_cName], true) == 0) {
			return i;
		}
	}
	return -1;
}

stock Color_GetCodeByName(name[])
{
	for (new i = Color_GetCount() - 1; i != -1; i--) {
		if (strcmp(name, gColors[i][e_cName], true) == 0) {
			return gColors[id][e_cCode];
		}
	}
	return -1;
}
