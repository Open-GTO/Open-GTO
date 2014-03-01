/*
	
	About: color functions
	Author: ziggi

*/

#if defined _color_included
  #endinput
#endif

#define _color_included
#pragma library color


// цвета
#define COLOUR_GREY 0xAFAFAFAA // grey
#define COLOUR_GREEN 0x33AA33AA // green
#define COLOUR_LIGHTGREEN 0x9ACD32AA //lightgreen
#define COLOUR_RED 0xAA3333AA // red
#define COLOUR_LIGHTRED 0xFF6347AA // lightred
#define COLOUR_YELLOW 0xFFFF00AA // yellow
#define COLOUR_WHITE 0xFFFFFFAA // white
#define COLOUR_BLUE 0x0000FFAA // blue
#define COLOUR_LIGHTBLUE 0x33CCFFAA // LightBlue
#define COLOUR_ORANGE 0xFF9900AA //orange
#define COLOUR_PURPLE 0xC2A2DAAA //purple
#define COLOUR_CRIMSON 0xDC143CAA
//#define COLOUR_LIMEGREEN 0x32CD32AA
#define COLOUR_AQUA 0x00FFFFAA
#define COLOUR_FLBLUE 0x6495EDAA
#define COLOUR_MAGENTA 0xFF00FFAA
//#define COLOUR_GOLD 0xFFD700AA
#define COLOUR_PINK 0xFFC0CBAA
#define COLOUR_MISC 0xAFAFAFAA // Grey - Misc
#define COLOUR_CHAT 0xFFFFFFAA // White - Chat
#define COLOUR_PLAYER 0xAAAAAAAA // White - Player
#define COLOUR_XP_GOOD 0x00C7FFAA // Blue - XP
#define COLOUR_XP_BAD 0x00008BAA // Blue - XP
#define COLOUR_COMBAT_GOOD 0xAA3333AA // Red - Combat
#define COLOUR_COMBAT_BAD 0xBB3333AA // Red - Combat
#define COLOUR_MONEY_GOOD 0xFFFF00AA // Yellow - Money
#define COLOUR_MONEY_BAD 0xDAA520AA // Yellow - Money
#define COLOUR_RACE 0x005407AA // green - dark
#define COLOUR_RACE_BAD 0x545407AA // yellow-green-dark
#define COLOUR_GANG 0xFF8C00AA // orange
#define COLOUR_GANG_CHAT 0xFFA500AA // orange
#define COLOUR_DM 0xFF0066AA // red - purple
#define COLOUR_DM_BAD 0xFF0066AA // red - purple
#define COLOUR_PM 0xFFCC22FF
#define WARN_MS_COLOR 0xFF5050AA

//#define COLOUR_LIGHTCORAL 0xF08080AA
#define COLOUR_DARKRED 0x8B0000AA
#define COLOUR_DEEPPINK 0xFF1493AA
#define COLOUR_TOMATO 0xFF6347AA
#define COLOUR_OrangeRed 0xFF4500AA
#define COLOUR_DarkOrange 0xFF8C00AA
#define COLOUR_Moccasin 0xFFE4B5AA
#define COLOUR_Khaki 0xF0E68CAA
#define COLOUR_DarkViolet 0x9400D3AA
#define COLOUR_Indigo 0x4B0082AA
#define COLOUR_GreenYellow 0xADFF2FAA
#define COLOUR_Lime 0x00FF00AA
//#define COLOUR_LIMEGREEN 0x32CD32AA
#define COLOUR_SpringGreen 0x00FF7FAA
#define COLOUR_Olive 0x808000AA
#define COLOUR_Teal 0x008080AA
//#define COLOUR_Aquamarine 0x7FFFD4AA
#define COLOUR_SteelBlue 0x4682B4AA
#define COLOUR_SkyBlue 0x87CEEBAA
#define COLOUR_Tan 0xD2B48CAA
#define COLOUR_Peru 0xCD853FAA
#define COLOUR_Sienna 0xA0522DAA
//#define COLOUR_MistyRose 0xFFE4E1AA
#define COLOUR_DimGray 0x696969AA
#define COLOUR_Black 0x000000AA


enum ColorInfo {
	color_name[MAX_NAME],
	color_code,
	color_vehicle,
}

new colors_Array[][ColorInfo] = {
	//{"LightCoral",	COLOUR_LIGHTCORAL,	70},
	{"Crimson",	 	COLOUR_CRIMSON,		3},
	{"DarkRed",	 	COLOUR_DARKRED,		43},
	{"Pink",	 	COLOUR_PINK,		126},
	{"DeepPink",	COLOUR_DEEPPINK,	5},
	{"Tomato",	 	COLOUR_TOMATO,		70},
	{"OrangeRed",	COLOUR_OrangeRed,	6},
	{"DarkOrange",	COLOUR_DarkOrange,	61},
	//{"Gold",		COLOUR_GOLD,		6},
	{"Yellow",		COLOUR_YELLOW,		6},
	{"Moccasin",	COLOUR_Moccasin,	46},
	{"Khaki",		COLOUR_Khaki,		57},
	{"Magenta",		COLOUR_MAGENTA,		115},
	{"DarkViolet",	COLOUR_DarkViolet,	116},
	{"Indigo",		COLOUR_Indigo,		20},
	{"GreenYellow",	COLOUR_GreenYellow,	65},
	{"Lime",		COLOUR_Lime,		16},
	//{"LimeGreen",	COLOUR_LIMEGREEN,	86},
	{"SpringGreen",	COLOUR_SpringGreen,	144},
	{"Olive",		COLOUR_Olive,		104},
	{"Teal",		COLOUR_Teal,		10},
	{"Aqua",		COLOUR_AQUA,		93},
	//{"Aquamarine",	COLOUR_Aquamarine,	97},
	{"SteelBlue",	COLOUR_SteelBlue,	59},
	{"SkyBlue",		COLOUR_SkyBlue,		87},
	{"Blue",		COLOUR_BLUE,		79},
	{"Tan",			COLOUR_Tan,			99},
	{"Peru",		COLOUR_Peru,		61},
	{"Sienna",		COLOUR_Sienna,		62},
	{"White",		COLOUR_WHITE,		1},
	//{"MistyRose",	COLOUR_MistyRose,	126},
	//{"Black",		COLOUR_Black,		0},
	{"DimGray",		COLOUR_DimGray,		113}
};

#define COLOR_COUNT	sizeof(colors_Array)

stock color_GetEmbeddingCode(id)
{
	new code[8];
	format(code, sizeof(code), "%06x", colors_Array[id][color_code] >>> 8);
	return code;
}

stock color_GetCode(id)
{
	return colors_Array[id][color_code];
}

stock color_GetName(id)
{
	return colors_Array[id][color_name];
}

stock color_GetIdByName(name[])
{
	for (new i = 0; i < COLOR_COUNT; i++) {
		if (strcmp(name, colors_Array[i][color_name], true)) {
			return i;
		}
	}
	return -1;
}

stock color_GetCodeByName(name[])
{
	for (new i = 0; i < COLOR_COUNT; i++) {
		if (strcmp(name, colors_Array[i][color_name], true)) {
			return colors_Array[id][color_code];
		}
	}
	return -1;
}

stock color_GetVehicleCode(id)
{
	return colors_Array[id][color_vehicle];
}