/*

	About: vehicle component system
	Author:	ziggi
	Thanks AirKite for component's data

*/

#if defined _vehicle_component_included
	#endinput
#endif

#define _vehicle_component_included
#pragma library vehicle_component


#define INVALID_COMPONENT_ID    -1
#define MAX_COMPONENTS          128
#define MAX_COMPONENT_NAME      64
#define MAX_COMPONENT_TYPES     14
#define MAX_COMPONENT_TYPE_NAME 16

enum component_Info {
	cu_Model = 0,
	cu_ComponentID = 1,
}

static component_array[][][MAX_COMPONENTS] = {
	{
		{400},
		{
			1008, 1009, 1010, 1013, 1018, 1019, 1020, 1021, 1024, 1025, 1073, 1074, 1075, 1076, 1077,
			1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{401},
		{
			1001, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1013, 1017, 1019, 1020, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1142, 1143, 1144
		}
	},
	{
		{404},
		{
			1000, 1002, 1007, 1008, 1009, 1010, 1013, 1016, 1017, 1019, 1020, 1021, 1025, 1073, 1074,
			1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{405},
		{
			1000, 1001, 1008, 1009, 1010, 1014, 1018, 1019, 1020, 1021, 1023, 1025, 1073, 1074, 1075,
			1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{410},
		{
			1001, 1003, 1007, 1008, 1009, 1010, 1013, 1017, 1019, 1020, 1021, 1023, 1024, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098
		}
	},
	{
		{415},
		{
			1001, 1003, 1007, 1008, 1009, 1010, 1017, 1018, 1019, 1023, 1025, 1073, 1074, 1075, 1076,
			1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{418},
		{
			1002, 1006, 1008, 1009, 1010, 1016, 1020, 1021, 1025, 1073, 1074, 1075, 1076, 1077, 1078,
			1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{420},
		{
			1001, 1003, 1004, 1005, 1008, 1009, 1010, 1019, 1021, 1025, 1073, 1074, 1075, 1076, 1077,
			1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{421},
		{
			1000, 1008, 1009, 1010, 1014, 1016, 1018, 1019, 1020, 1021, 1023, 1025, 1073, 1074, 1075,
			1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{422},
		{
			1007, 1008, 1009, 1010, 1013, 1017, 1019, 1020, 1021, 1025, 1073, 1074, 1075, 1076, 1077,
			1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{426},
		{
			1001, 1003, 1004, 1005, 1006, 1008, 1009, 1010, 1019, 1021, 1025, 1073, 1074, 1075, 1076,
			1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{436},
		{
			1001, 1003, 1006, 1007, 1008, 1009, 1010, 1013, 1017, 1019, 1020, 1021, 1022, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098
		}
	},
	{
		{439},
		{
			1001, 1003, 1007, 1008, 1009, 1010, 1013, 1017, 1023, 1025, 1073, 1074, 1075, 1076, 1077,
			1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098, 1142, 1143, 1144,
			1145
		}
	},
	{
		{477},
		{
			1006, 1007, 1008, 1009, 1010, 1017, 1018, 1019, 1020, 1021, 1025, 1073, 1074, 1075, 1076,
			1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{478},
		{
			1004, 1005, 1008, 1009, 1010, 1012, 1013, 1020, 1021, 1022, 1024, 1025, 1073, 1074, 1075,
			1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{489},
		{
			1000, 1002, 1004, 1005, 1006, 1008, 1009, 1010, 1013, 1016, 1018, 1019, 1020, 1024, 1025,
			1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096,
			1097, 1098
		}
	},
	{
		{491},
		{
			1003, 1007, 1008, 1009, 1010, 1014, 1017, 1018, 1019, 1020, 1021, 1023, 1025, 1073, 1074,
			1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098,
			1142, 1143, 1144, 1145
		}
	},
	{
		{492},
		{
			1000, 1004, 1005, 1006, 1008, 1009, 1010, 1016, 1025, 1073, 1074, 1075, 1076, 1077, 1078,
			1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{496},
		{
			1001, 1002, 1003, 1006, 1007, 1008, 1009, 1010, 1011, 1017, 1019, 1020, 1023, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1142, 1143
		}
	},
	{
		{500},
		{
			1008, 1009, 1010, 1013, 1019, 1020, 1021, 1024, 1025, 1073, 1074, 1075, 1076, 1077, 1078,
			1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{516},
		{
			1000, 1002, 1004, 1007, 1008, 1009, 1010, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1025,
			1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096,
			1097, 1098
		}
	},
	{
		{517},
		{
			1002, 1003, 1007, 1008, 1009, 1010, 1016, 1017, 1018, 1019, 1020, 1023, 1025, 1073, 1074,
			1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098,
			1142, 1143, 1144, 1145
		}
	},
	{
		{518},
		{
			1001, 1003, 1005, 1006, 1007, 1008, 1009, 1010, 1013, 1017, 1018, 1020, 1023, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1142, 1143, 1144, 1145
		}
	},
	{
		{527},
		{
			1001, 1007, 1008, 1009, 1010, 1014, 1015, 1017, 1018, 1020, 1021, 1025, 1073, 1074, 1075,
			1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{529},
		{
			1001, 1003, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1017, 1018, 1019, 1020, 1023, 1025,
			1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096,
			1097, 1098
		}
	},
	{
		{534},
		{
			1008, 1009, 1010, 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083,
			1084, 1085, 1087, 1096, 1097, 1098, 1100, 1101, 1106, 1122, 1123, 1124, 1125, 1126, 1127,
			1178, 1179, 1180, 1185
		}
	},
	{
		{535},
		{
			1008, 1009, 1010, 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083,
			1084, 1085, 1087, 1096, 1097, 1098, 1109, 1110, 1113, 1114, 1115, 1116, 1117, 1118, 1119,
			1120, 1121
		}
	},
	{
		{536},
		{
			1008, 1009, 1010, 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083,
			1084, 1085, 1087, 1096, 1097, 1098, 1103, 1104, 1105, 1107, 1108, 1128, 1181, 1182, 1183,
			1184
		}
	},
	{
		{540},
		{
			1001, 1004, 1006, 1007, 1008, 1009, 1010, 1017, 1018, 1019, 1020, 1023, 1024, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1142, 1143, 1144, 1145
		}
	},
	{
		{542},
		{
			1008, 1009, 1010, 1014, 1015, 1018, 1019, 1020, 1021, 1025, 1073, 1074, 1075, 1076, 1077,
			1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098, 1144, 1145
		}
	},
	{
		{546},
		{
			1001, 1002, 1004, 1006, 1007, 1008, 1009, 1010, 1017, 1018, 1019, 1023, 1024, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1142, 1143, 1144, 1145
		}
	},
	{
		{547},
		{
			1000, 1003, 1008, 1009, 1010, 1016, 1018, 1019, 1020, 1021, 1025, 1073, 1074, 1075, 1076,
			1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098, 1142, 1143
		}
	},
	{
		{549},
		{
			1001, 1003, 1007, 1008, 1009, 1010, 1011, 1012, 1017, 1018, 1019, 1020, 1023, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1142, 1143, 1144, 1145
		}
	},
	{
		{550},
		{
			1001, 1003, 1004, 1005, 1006, 1008, 1009, 1010, 1018, 1019, 1020, 1023, 1025, 1073, 1074,
			1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098,
			1142, 1143, 1144, 1145
		}
	},
	{
		{551},
		{
			1002, 1003, 1005, 1006, 1008, 1009, 1010, 1016, 1018, 1019, 1020, 1021, 1023, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098
		}
	},
	{
		{558},
		{
			1008, 1009, 1010, 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083,
			1084, 1085, 1087, 1088, 1089, 1090, 1091, 1092, 1093, 1094, 1095, 1096, 1097, 1098, 1163,
			1164, 1165, 1166, 1167, 1168
		}
	},
	{
		{559},
		{
			1008, 1009, 1010, 1025, 1065, 1066, 1067, 1068, 1069, 1070, 1071, 1072, 1073, 1074, 1075,
			1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098, 1158,
			1159, 1160, 1161, 1162, 1173
		}
	},
	{
		{560},
		{
			1008, 1009, 1010, 1025, 1026, 1027, 1028, 1029, 1030, 1031, 1032, 1033, 1073, 1074, 1075,
			1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098, 1138,
			1139, 1140, 1141, 1169, 1170
		}
	},
	{
		{561},
		{
			1008, 1009, 1010, 1025, 1055, 1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063, 1064, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1154, 1155, 1156, 1157
		}
	},
	{
		{562},
		{
			1008, 1009, 1010, 1025, 1034, 1035, 1036, 1037, 1038, 1039, 1040, 1041, 1073, 1074, 1075,
			1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098, 1146,
			1147, 1148, 1149, 1171, 1172
		}
	},
	{
		{565},
		{
			1008, 1009, 1010, 1025, 1045, 1046, 1047, 1048, 1049, 1050, 1051, 1052, 1053, 1054, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1150, 1151, 1152, 1153
		}
	},
	{
		{567},
		{
			1008, 1009, 1010, 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083,
			1084, 1085, 1087, 1096, 1097, 1098, 1102, 1129, 1130, 1131, 1132, 1133, 1186, 1187, 1188,
			1189
		}
	},
	{
		{575},
		{
			1008, 1009, 1010, 1025, 1042, 1043, 1044, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080,
			1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098, 1099, 1174, 1175, 1176, 1177
		}
	},
	{
		{576},
		{
			1008, 1009, 1010, 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083,
			1084, 1085, 1087, 1096, 1097, 1098, 1134, 1135, 1136, 1137, 1190, 1191, 1192, 1193
		}
	},
	{
		{580},
		{
			1001, 1006, 1007, 1008, 1009, 1010, 1017, 1018, 1020, 1023, 1025, 1073, 1074, 1075, 1076,
			1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{585},
		{
			1001, 1003, 1006, 1007, 1008, 1009, 1010, 1013, 1017, 1018, 1019, 1020, 1023, 1025, 1073,
			1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097,
			1098, 1142, 1143, 1144, 1145
		}
	},
	{
		{589},
		{
			1000, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1013, 1016, 1017, 1018, 1020, 1024, 1025,
			1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096,
			1097, 1098, 1144, 1145
		}
	},
	{
		{600},
		{
			1004, 1005, 1006, 1007, 1008, 1009, 1010, 1013, 1017, 1018, 1020, 1022, 1025, 1073, 1074,
			1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098
		}
	},
	{
		{603},
		{
			1001, 1006, 1007, 1008, 1009, 1010, 1017, 1018, 1019, 1020, 1023, 1024, 1025, 1073, 1074,
			1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083, 1084, 1085, 1087, 1096, 1097, 1098,
			1142, 1143, 1144, 1145
		}
	},
	{
		{
			402, 403, 406, 407, 408, 409, 411, 412, 413, 414, 416, 419, 423, 424, 427, 428, 429, 431,
			432, 433, 434, 435, 437, 438, 440, 441, 442, 443, 444, 445, 448, 450, 451, 455, 456, 459,
			461, 462, 463, 466, 467, 468, 470, 471, 474, 475, 479, 480, 481, 482, 483, 484, 485, 486,
			490, 494, 495, 498, 499, 502, 503, 504, 505, 506, 507, 508, 509, 510, 514, 515, 521, 522,
			523, 524, 525, 526, 528, 530, 531, 532, 533, 541, 552, 543, 544, 545, 554, 555, 556, 557,
			566, 568, 578, 571, 572, 573, 574, 579, 581, 582, 583, 584, 586, 587, 588, 591, 594, 596,
			597, 598, 599, 601, 602, 604, 605, 606, 607, 608, 609, 610, 611
		},
		{
			1008, 1009, 1010, 1025, 1073, 1074, 1075, 1076, 1077, 1078, 1079, 1080, 1081, 1082, 1083,
			1084, 1085, 1087, 1096, 1097, 1098
		}
	}
};

static component_type[MAX_COMPONENT_TYPES][MAX_COMPONENT_TYPE_NAME] = {
	"Spoiler",
	"Hood",
	"Roof",
	"Sideskirt",
	"Lamps",
	"Nitro",
	"Exhaust",
	"Wheels",
	"Stereo",
	"Hydraulics",
	"Front Bumper",
	"Rear Bumper",
	"Vent Right",
	"Vent Left"
};

static component_name[][MAX_COMPONENT_NAME] = {
	"Pro Spoiler",
	"Win Spoiler",
	"Drag Spoiler",
	"Alpha Spoiler",
	"Champ Scoop Hood",
	"Fury Scoop Hood",
	"Roof Scoop",
	"Right Sideskirt",
	"5x Nitro",
	"2x Nitro",
	"10x Nitro",
	"Race Scoop Hood",
	"Worx Scoop Hood",
	"Round Fog Lamps",
	"Champ Spoiler",
	"Race Spoiler",
	"Worx Spoiler",
	"Left Sideskirt",
	"Upswept Exhaust",
	"Twin Exhaust",
	"Large Exhaust",
	"Medium Exhaust",
	"Small Exhaust",
	"Fury Spoiler",
	"Square Fog Lamps",
	"Offroad Wheels",
	"Right Alien Sideskirt 'Sultan'",
	"Left Alien Sideskirt 'Sultan'",
	"Alien Exhaust 'Sultan'",
	"X-Flow Exhaust 'Sultan'",
	"Left X-Flow Sideskirt 'Sultan'",
	"Right X-Flow Sideskirt 'Sultan'",
	"Alien Roof Vent 'Sultan'",
	"X-Flow Roof Vent 'Sultan'",
	"Alien Exhaust 'Elegy'",
	"X-Flow Roof Vent 'Elegy'",
	"Right Alien Sideskirt 'Elegy'",
	"X-Flow Exhaust 'Elegy'",
	"Alien Roof Vent 'Elegy'",
	"Right X-Flow Sideskirt 'Elegy'",
	"Left Alien Sideskirt 'Elegy'",
	"Right X-Flow Sideskirt 'Elegy'",
	"Right Chrome Sideskirt 'Broadway'",
	"Slamin Exhaust 'Broadway'",
	"Chrome Exhaust 'Broadway'",
	"X-Flow Exhaust 'Flash'",
	"Alien Exhaust 'Flash'",
	"Right Alien Sideskirt 'Flash'",
	"Right X-Flow Sideskirt 'Flash'",
	"Alien Spoiler 'Flash'",
	"X-Flow Spoiler 'Flash'",
	"Left Alien Sideskirt 'Flash'",
	"Left X-Flow Sideskirt 'Flash'",
	"X-Flow Roof 'Flash'",
	"Alien Roof 'Flash'",
	"Alien Roof 'Stratum'",
	"Right Alien Sideskirt 'Stratum'",
	"Right X-Flow Sideskirt 'Stratum'",
	"Alien Spoiler 'Stratum'",
	"X-Flow Exhaust 'Stratum'",
	"X-Flow Spoiler 'Stratum'",
	"X-Flow Roof 'Stratum'",
	"Left Alien Sideskirt 'Stratum'",
	"Left X-Flow Sideskirt 'Stratum'",
	"Alien Exhaust 'Stratum'",
	"Alien Exhaust 'Jester'",
	"X-Flow Exhaust 'Jester'",
	"Alien Roof 'Jester'",
	"X-Flow Roof 'Jester'",
	"Right Alien Sideskirt 'Jester'",
	"Right X-Flow Sideskirt 'Jester'",
	"Left Alien Sideskirt 'Jester'",
	"Left X-Flow Sideskirt 'Jester'",
	"Shadow Wheels",
	"Mega Wheels",
	"Rimshine Wheels",
	"Wires Wheels",
	"Classic Wheels",
	"Twist Wheels",
	"Cutter Wheels",
	"Switch Wheels",
	"Grove Wheels",
	"Import Wheels",
	"Dollar Wheels",
	"Trance Wheels",
	"Atomic Wheels",
	"Stereo Sound",
	"Hydraulics",
	"Alien Roof 'Uranus'",
	"X-Flow Exhaust 'Uranus'",
	"Right Alien Sideskirt 'Uranus'",
	"X-Flow Roof 'Uranus'",
	"Alien Exhaust 'Uranus'",
	"Right X-Flow Sideskirt 'Uranus'",
	"Left Alien Sideskirt 'Uranus'",
	"Right X-Flow Sideskirt 'Uranus'",
	"Ahab Wheels",
	"Virtual Wheels",
	"Access Wheels",
	"Left Chrome Sideskirt 'Broadway'",
	"Chrome Grill Bullbar 'Remington'",
	"Left 'Chrome Flames' Sideskirt 'Remington'",
	"Left 'Chrome Strip' Sideskirt 'Savanna'",
	"Corvetible Roof 'Blade'",
	"Chrome Exhaust 'Blade'",
	"Slamin Exhaust 'Blade'",
	"Right 'Chrome Arches' Sideskirt 'Remington'",
	"Left 'Chrome Strip' Sideskirt 'Blade'",
	"Right 'Chrome Strip' Sideskirt 'Blade'",
	"Chrome Rear Bullbars 'Slamvan'",
	"Slamin Rear Bullbars 'Slamvan'",
	"Little Front Sign? 'Slamvan'",
	"Little Front Sign? 'Slamvan'",
	"Chrome Exhaust 'Slamvan'",
	"Slamin Exhaust 'Slamvan'",
	"Chrome Front Bullbars 'Slamvan'",
	"Slamin Front Bullbars 'Slamvan'",
	"Chrome Front Bumper 'Slamvan'",
	"Right 'Chrome Trim' Sideskirt 'Slamvan'",
	"Right 'Wheelcovers' Sideskirt 'Slamvan'",
	"Left 'Chrome Trim' Sideskirt 'Slamvan'",
	"Left 'Wheelcovers' Sideskirt 'Slamvan'",
	"Right 'Chrome Flames' Sideskirt 'Remington'",
	"Bullbar Chrome Bars 'Remington'",
	"Left 'Chrome Arches' Sideskirt 'Remington'",
	"Bullbar Chrome Lights 'Remongton'",
	"Chrome Exhaust 'Remington'",
	"Slamin Exhaust 'Remington'",
	"Vinyl Hardtop 'Blade'",
	"Chrome Exhaust 'Savanna'",
	"Hardtop Roof 'Savanna'",
	"Softtop Roof 'Savanna'",
	"Slamin Exhaust 'Savanna'",
	"Right 'Chrome Strip' Sideskirt 'Savanna'",
	"Right 'Chrome Strip' Sideskirt 'Tornado'",
	"Slamin Exhaust 'Tornado'",
	"Chrome Exhaust 'Tornado'",
	"Left 'Chrome Strip' Sideskirt 'Tornado'",
	"Alien Spoiler 'Sultan'",
	"X-Flow Spoiler 'Sultan'",
	"X-Flow Rear Bumper 'Sultan'",
	"Alien Rear Bumper 'Sultan'",
	"Left Oval Vents",
	"Right Oval Vents",
	"Left Square Vents",
	"Right Square Vents",
	"X-Flow Spoiler 'Elegy'",
	"Alien Spoiler 'Elegy'",
	"X-Flow Rear Bumper 'Elegy'",
	"Alien Rear Bumper 'Elegy'",
	"Alien Rear Bumper 'Flash'",
	"X-Flow Rear Bumper 'Flash'",
	"X-Flow Front Bumper 'Flash'",
	"Alien Front Bumper 'Flash'",
	"Alien Rear Bumper 'Stratum'",
	"Alien Front Bumper 'Stratum'",
	"X-Flow Rear Bumper 'Stratum'",
	"X-Flow Front Bumper 'Stratum'",
	"X-Flow Spoiler 'Jester'",
	"Alien Rear Bumper 'Jester'",
	"Alien Front Bumper 'Jester'",
	"X-Flow Rear Bumper 'Jester'",
	"Alien Spoiler 'Jester'",
	"X-Flow Spoiler 'Uranus'",
	"Alien Spoiler 'Uranus'",
	"X-Flow Front Bumper 'Uranus'",
	"Alien Front Bumper 'Uranus'",
	"X-Flow Rear Bumper 'Uranus'",
	"Alien Rear Bumper 'Uranus'",
	"Alien Front Bumper 'Sultan'",
	"X-Flow Front Bumper 'Sultan'",
	"Alien Front Bumper 'Elegy'",
	"X-Flow Front Bumper 'Elegy'",
	"X-Flow Front Bumper 'Jester'",
	"Chrome Front Bumper 'Broadway'",
	"Slamin Front Bumper 'Broadway'",
	"Chrome Rear Bumper 'Broadway'",
	"Slamin Rear Bumper 'Broadway'",
	"Slamin Rear Bumper 'Remington'",
	"Chrome Front Bumper 'Remington'",
	"Chrome Rear Bumper 'Remington'",
	"Slamin Front Bumper 'Blade'",
	"Chrome Front Bumper 'Blade'",
	"Slamin Rear Bumper 'Blade'",
	"Chrome Rear Bumper 'Blade'",
	"Slamin Front Bumper 'Remington'",
	"Slamin Rear Bumper 'Savanna'",
	"Chrome Rear Bumper 'Savanna'",
	"Slamin Front Bumper 'Savanna'",
	"Chrome Front Bumper 'Savanna'",
	"Slamin Front Bumper 'Tornado'",
	"Chrome Front Bumper 'Tornado'",
	"Chrome Rear Bumper 'Tornado'",
	"Slamin Rear Bumper 'Tornado'"
};

/*
	Component Type
*/

stock GetComponentTypeId(name[])
{
	for (new i = 0; i < sizeof(component_type); i++) {
		if (strcmp(component_type[i], name, true) == 0) {
			return i;
		}
	}

	return INVALID_COMPONENT_ID;
}

stock GetComponentTypeName(type, name[], size = sizeof(name))
{
	if (type < 0 || type >= sizeof(component_type)) {
		return 0;
	}

	strmid(name, component_type[type], 0, strlen(component_type[type]), size);
	return 1;
}

stock GetVehicleCompatibleTypes(model, array[MAX_COMPONENT_TYPES], &array_size)
{
	new
		components[MAX_COMPONENTS],
		components_size;

	new isok = GetVehicleCompatibleUpgrades(model, components, components_size);
	if (!isok) {
		return 0;
	}

	new
		type_id,
		bool:is_type_exist[MAX_COMPONENT_TYPES];

	array_size = 0;

	for (new i = 0; i < components_size; i++) {
		type_id = GetVehicleComponentType(components[i]);

		if (!is_type_exist[type_id]) {
			is_type_exist[type_id] = true;

			array[array_size++] = type_id;
		}
	}

	return 1;
}

/*
	Component Upgrade
*/

stock GetComponentName(componentid, cname[], size = sizeof(cname))
{
	new idx = componentid - 1000;
	if (idx < 0 || idx >= sizeof(component_name)) {
		return 0;
	}

	strmid(cname, component_name[idx], 0, strlen(component_name[idx]), size);
	return 1;
}

stock GetVehicleCompatibleUpgrades(model, array[MAX_COMPONENTS], &array_size)
{
	new id = GetComponentArrayID(model);
	if (id == INVALID_COMPONENT_ID) {
		return 0;
	}

	for (new i = 0; i < sizeof(array); i++) {
		if (component_array[id][cu_ComponentID][i] == 0) {
			array_size = i;
			return 1;
		}

		array[i] = component_array[id][cu_ComponentID][i];
	}

	return 0;
}

stock IsVehicleUpgradeCompatible(model, componentid)
{
	new id = GetComponentArrayID(model);
	if (id == INVALID_COMPONENT_ID) {
		return 0;
	}

	for (new i = 0; i < sizeof(component_array[][]); i++) {
		if (component_array[id][cu_ComponentID][i] == componentid) {
			return 1;
		}
	}

	return 0;
}

/*
	Private functions
*/

static stock GetComponentArrayID(model)
{
	new i, j;

	for (i = 0; i < sizeof(component_array); i++) {
		for (j = 0; j < sizeof(component_array[][]); j++) {
			if (component_array[i][cu_Model][j] == model) {
				return i;
			}
		}
	}

	return INVALID_COMPONENT_ID;
}
