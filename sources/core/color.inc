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
#define COLOUR_GREY 0xAFAFAFFF // grey
#define COLOUR_GREEN 0x33FF33FF // green
#define COLOUR_LIGHTGREEN 0x9ACD32FF //lightgreen
#define COLOUR_RED 0xAA3333FF // red
#define COLOUR_LIGHTRED 0xFF6347FF // lightred
#define COLOUR_YELLOW 0xFFFF00FF // yellow
#define COLOUR_WHITE 0xFFFFFFFF // white
#define COLOUR_BLUE 0x0000FFFF // blue
#define COLOUR_LIGHTBLUE 0x33CCFFFF // LightBlue
#define COLOUR_ORANGE 0xFF9900FF //orange
#define COLOUR_PURPLE 0xC2A2DAFF //purple
#define COLOUR_CRIMSON 0xDC143CFF
#define COLOUR_LIMEGREEN 0x32CD32FF
#define COLOUR_AQUA 0x00FFFFFF
#define COLOUR_FLBLUE 0x6495EDFF
#define COLOUR_MAGENTA 0xFF00FFFF
#define COLOUR_PINK 0xFFC0CBFF
#define COLOUR_MISC 0xAFAFAFFF // Grey - Misc
#define COLOUR_CHAT 0xFFFFFFFF // White - Chat
#define COLOUR_PLAYER 0xAAAAAAFF // White - Player
#define COLOUR_XP_GOOD 0x00C7FFFF // Blue - XP
#define COLOUR_XP_BAD 0x00008BFF // Blue - XP
#define COLOUR_MONEY_GOOD 0xFFFF00FF // Yellow - Money
#define COLOUR_MONEY_BAD 0xDAA520FF // Yellow - Money
#define COLOUR_RACE 0x005407FF // green - dark
#define COLOUR_RACE_BAD 0x545407FF // yellow-green-dark
#define COLOUR_GANG 0xFF8C00FF // orange
#define COLOUR_GANG_CHAT 0xFFA500FF // orange
#define COLOUR_DM 0xFF0066FF // red - purple
#define COLOUR_DM_BAD 0xFF0066FF // red - purple
#define COLOUR_PM 0xFFCC22FF
#define WARN_MS_COLOR 0xFF5050FF
#define COLOUR_DARKRED 0x8B0000FF
#define COLOUR_DEEPPINK 0xFF1493FF
#define COLOUR_TOMATO 0xFF6347FF
#define COLOUR_ORANGERED 0xFF4500FF
#define COLOUR_DARKORANGE 0xFF8C00FF
#define COLOUR_MOCCASIN 0xFFE4B5FF
#define COLOUR_KHAKI 0xF0E68CFF
#define COLOUR_DARKVIOLET 0x9400D3FF
#define COLOUR_INDIGO 0x4B0082FF
#define COLOUR_GREENYELLOW 0xADFF2FFF
#define COLOUR_LIME 0x00FF00FF
#define COLOUR_SPRINGGREEN 0x00FF7FFF
#define COLOUR_OLIVE 0x808000FF
#define COLOUR_TEAL 0x008080FF
#define COLOUR_AQUAMARINE 0x7FFFD4FF
#define COLOUR_STEELBLUE 0x4682B4FF
#define COLOUR_SKYBLUE 0x87CEEBFF
#define COLOUR_TAN 0xD2B48CFF
#define COLOUR_PERU 0xCD853FFF
#define COLOUR_SIENNA 0xA0522DFF
#define COLOUR_MISTYROSE 0xFFE4E1FF
#define COLOUR_DIMGRAY 0x696969FF
#define COLOUR_BLACK 0x000000FF


enum ColorInfo {
	color_name[MAX_NAME],
	color_code,
	color_vehicle,
}

new colors_Array[][ColorInfo] = {
	{"Crimson",	 	COLOUR_CRIMSON,		3},
	{"DarkRed",	 	COLOUR_DARKRED,		43},
	{"Pink",	 	COLOUR_PINK,		126},
	{"DeepPink",	COLOUR_DEEPPINK,	220},
	{"Tomato",	 	COLOUR_TOMATO,		183},
	{"OrangeRed",	COLOUR_ORANGERED,	222},
	{"DarkOrange",	COLOUR_DARKORANGE,	219},
	{"Yellow",		COLOUR_YELLOW,		6},
	{"Moccasin",	COLOUR_MOCCASIN,	200},
	{"Khaki",		COLOUR_KHAKI,		57},
	{"Magenta",		COLOUR_MAGENTA,		136},
	{"DarkViolet",	COLOUR_DARKVIOLET,	134},
	{"Indigo",		COLOUR_INDIGO,		211},
	{"GreenYellow",	COLOUR_GREENYELLOW,	241},
	{"Lime",		COLOUR_LIME,		128},
	{"LimeGreen",	COLOUR_LIMEGREEN,	229},
	{"SpringGreen",	COLOUR_SPRINGGREEN,	154},
	{"Olive",		COLOUR_OLIVE,		225},
	{"Teal",		COLOUR_TEAL,		130},
	{"Aqua",		COLOUR_AQUA,		135},
	{"Aquamarine",	COLOUR_AQUAMARINE,	155},
	{"SteelBlue",	COLOUR_STEELBLUE,	162},
	{"SkyBlue",		COLOUR_SKYBLUE,		166},
	{"Blue",		COLOUR_BLUE,		162},
	{"Tan",			COLOUR_TAN,			254},
	{"Peru",		COLOUR_PERU,		174},
	{"Sienna",		COLOUR_SIENNA,		239},
	{"White",		COLOUR_WHITE,		1},
	{"MistyRose",	COLOUR_MISTYROSE,	185},
	{"Black",		COLOUR_BLACK,		0},
	{"DimGray",		COLOUR_DIMGRAY,		252}
};

#define COLOR_COUNT	sizeof(colors_Array)

stock Color_GetEmbeddingCode(id)
{
	new code[8];
	format(code, sizeof(code), "%06x", colors_Array[id][color_code] >>> 8);
	return code;
}

stock Color_GetCode(id)
{
	return colors_Array[id][color_code];
}

stock Color_GetName(id)
{
	new name[MAX_NAME];
	set(name, colors_Array[id][color_name]);
	return name;
}

stock Color_GetIdByName(name[])
{
	for (new i = 0; i < COLOR_COUNT; i++) {
		if (!strcmp(name, colors_Array[i][color_name], true)) {
			return i;
		}
	}
	return -1;
}

stock Color_GetCodeByName(name[])
{
	for (new i = 0; i < COLOR_COUNT; i++) {
		if (!strcmp(name, colors_Array[i][color_name], true)) {
			return colors_Array[id][color_code];
		}
	}
	return -1;
}

stock Color_GetVehicleCode(id)
{
	return colors_Array[id][color_vehicle];
}
