// created by asturel based on business system, thx to Peter Steenbergen

#if defined _housing_included
#endinput
#endif

#define _housing_included
#pragma library housing

#include "base"
#include "account"
#include "player"
#include "world"

#define HouseDialogid 2
#define HousesDialogid 3

#define MAX_HOUSEBUYSELL 500000
#define MAX_HOUSES 161
#define int 15
//#define BUY_ALL_HOUSES // позволяет запретить покупать чужие дома

new Hous[MAX_PLAYERS],Haus[MAX_PLAYERS];
new house_icon[MAX_HOUSES],Text3D:House3DTextLabel[MAX_HOUSES],Float:HouseDistanceOfShowLabel=20.0;

enum HouseInfo {
	Houses_Name[MAX_STRING],// house name
	Houses_Cost,			// cost
	Houses_Owner[MAX_NAME],	// owner
	Houses_Gang[MAX_NAME],  // gang
	Houses_UpKeep,			// keep up
	Houses_UpKeepLeft,		// keep up left
	Houses_Buyout,			// Buyout price
	Houses_Interior,		// Interiors
	Houses_Car,				// Car
	Houses_Lock,			// Locked home or not
	Houses_Rentabil,		// Аренда офф/он
	Houses_RentName[MAX_NAME],	// Name of Renter
	Houses_RentCost,		// Цена аренды
	Houses_HealUpgrade,		//Heal Upgrade (Armour+Health)
	Float:Houses_PickupX,
	Float:Houses_PickupY,
	Float:Houses_PickupZ,
	Float:Houses_EnterX,	// Coord X
	Float:Houses_EnterY,	// Coord Y
	Float:Houses_EnterZ	// Coord Z
}

new Houses[MAX_HOUSES][HouseInfo] = {
//NAME, COST, OWNER, GANG, UpKeep, UpKeepLeft, BUYOUT, интерьер, Car, Lock, Rentabil, RentName, Rent Cost, Heal upgrade,eX,eY,eZ,pX,pY,pZ
//{"[LV]Four Dragons Mansion",1250000,"Unknown","Unknown",500,0,0,1,0,0,0,"Unknown",50000,0,	991.8711,-668.1383,121.9763,	247.5086,304.9248,999.1484},
{"[LV]Caligula Mansion",850000,"Unknown","Unknown",500,0,0,3,0,0,0,"Unknown",50000,0,	300.2951,-1154.5029,81.3909,	289.9850,307.3130,999.1484},
{"[LS]Willowfield",17000,"Unknown","Unknown",50,0,0,5,0,0,0,"Unknown",50000,0,	2486.8389,-1997.4480,13.8343,	319.4555,312.5038,999.1484},
{"[LS]Verdant Bluffs",17000,"Unknown","Unknown",50,0,0,2,0,0,0,"Unknown",50000,0,	1687.8954,-2100.4861,13.8343,	2451.1558,-1685.9690,1013.5078},
{"[LS]Jefferson",30000,"Unknown","Unknown",50,0,0,1,0,0,0,"Unknown",50000,0,	2108.2073,-1280.1827,25.6875,	2535.0583,-1674.4338,1015.4986},
{"[LS]Mulholland",190000,"Unknown","Unknown",250,0,0,2,0,0,0,"Unknown",50000,0,	1325.8687,-633.3270,109.1349,	272.0126,306.4208,999.1484},
{"[LS]Verona Beach",40000,"Unknown","Unknown",80,0,0,3,0,0,0,"Unknown",50000,0,	890.5464,-1638.7795,14.9616,	235.1457,1190.7142,1080.2578},
{"[LS]Santa Maria Beach",70000,"Unknown","Unknown",100,0,0,2,0,0,0,"Unknown",50000,0,	312.6361,-1772.2003,4.6282,	221.4334,1240.8158,1082.1406},
{"[LS]Palomino Creek",390000,"Unknown","Unknown",500,0,0,5,0,0,0,"Unknown",50000,0,	2238.4756,166.0871,28.1535,	239.2819,1114.1991,1080.9922},
{"[LS]Blueberry",30000,"Unknown","Unknown",50,0,0,15,0,0,0,"Unknown",50000,0,	206.6301,-109.8167,4.8965,	295.3137,1478.6494,1080.2578},
{"[LS]Dillimore",70000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	791.9865,-507.5847,18.0129,	328.9125,1482.2545,1084.4375},
{"[SF]Angel Pine",80000,"Unknown","Unknown",50,0,0,15,0,0,0,"Unknown",50000,0,	-2080.1499,-2312.3855,30.6250,	384.0571,1471.7700,1080.1875},
{"[SF]Whetstone",200000,"Unknown","Unknown",300,0,0,15,0,0,0,"Unknown",50000,0,	-1442.8309,-1541.9430,101.7578,	374.5086,1417.2700,1081.3281},
{"[SF]Hashbury",90000,"Unknown","Unknown",100,0,0,4,0,0,0,"Unknown",50000,0,	-2456.8408,-131.7211,26.0440,	306.1966,307.8190,1003.3047},
{"[SF]Queens",150000,"Unknown","Unknown",120,0,0,10,0,0,0,"Unknown",50000,0,	-2421.6580,335.3531,35.7626,	20.7300,1341.7350,1084.3750},
{"[SF]Chinatown",120000,"Unknown","Unknown",50,0,0,4,0,0,0,"Unknown",50000,0,	-2210.9714,723.4057,49.4141,	221.6921,1149.8457,1082.6094},
{"[SF]Paradiso",120000,"Unknown","Unknown",50,0,0,12,0,0,0,"Unknown",50000,0,	-2703.1348,818.3945,49.9844,	2324.6213,-1143.9209,1050.4922},
{"[SF]Calton Heights",200000,"Unknown","Unknown",300,0,0,1,0,0,0,"Unknown",50000,0,	-2103.9272,900.6589,76.7109,	247.5086,304.9248,999.1484},
{"[LV]El Quebradas",120000,"Unknown","Unknown",50,0,0,3,0,0,0,"Unknown",50000,0,	-1534.4202,2649.6294,55.8359,	289.9850,307.3130,999.1484},
{"[LV]Tierra Robada",120000,"Unknown","Unknown",50,0,0,5,0,0,0,"Unknown",50000,0,	-1045.4006,1552.8932,33.3112,	319.4555,312.5038,999.1484},
{"[LV]Fort Carson",130000,"Unknown","Unknown",100,0,0,2,0,0,0,"Unknown",50000,0,	-364.1446,1168.8844,19.7422,	2451.1558,-1685.9690,1013.5078},
{"[LV]Whitewood Estates",130000,"Unknown","Unknown",100,0,0,1,0,0,0,"Unknown",50000,0,	926.3783,2010.8345,11.4609,	2535.0583,-1674.4338,1015.4986},
{"[LV]Prickle Pine",250000,"Unknown","Unknown",200,0,0,2,0,0,0,"Unknown",50000,0,	1282.0720,2525.0994,10.8203,	272.0126,306.4208,999.1484},
{"[LV]Redsands West",230000,"Unknown","Unknown",100,0,0,3,0,0,0,"Unknown",50000,0,	1405.6171,1900.5596,11.4609,	235.1457,1190.7142,1080.2578},
{"[LV]Pirates In Mens Pants",46000,"Unknown","Unknown",20,0,0,2,0,0,0,"Unknown",50000,0,	1969.4005,1623.1444,12.8625,	221.4334,1240.8158,1082.1406},
{"[LV]The Camels Toe",46000,"Unknown","Unknown",20,0,0,5,0,0,0,"Unknown",50000,0,	2233.6375,1288.4495,10.8203,	239.2819,1114.1991,1080.9922},
{"[LV]Rockshore West",120000,"Unknown","Unknown",100,0,0,15,0,0,0,"Unknown",50000,0,	2446.2251,695.2101,11.4609,	295.3137,1478.6494,1080.2578},
{"[LV]The Clowns Pocket",99000,"Unknown","Unknown",20,0,0,15,0,0,0,"Unknown",50000,0,	2223.1616,1840.9108,10.8203,	328.9125,1482.2545,1084.4375},
{"[LV]Old Venturas Strip",46000,"Unknown","Unknown",30,0,0,15,0,0,0,"Unknown",50000,0,	2371.1787,2169.1052,10.8257,	384.0571,1471.7700,1080.1875},
{"[LV]Creek",110000,"Unknown","Unknown",50,0,0,15,0,0,0,"Unknown",50000,0,	2820.9004,2140.8269,14.6615,	374.5086,1417.2700,1081.3281},
{"[LS]st.Manhatan 12",850000,"Unknown","Unknown",100,0,0,4,0,0,0,"Unknown",50000,0,	2487.073,-1646.585,13.265,	306.1966,307.8190,1003.3047},
//{"[LS]st.Manhatan 32",17000,"Unknown","Unknown",150,0,0,9,0,0,0,"Unknown",50000,0,	2495.479,-1690.834,13.960,	260.9170,1242.6680,1084.2578},
{"[LS]st.Jarry 55",17000,"Unknown","Unknown",150,0,0,10,0,0,0,"Unknown",50000,0,	2396.199,-1647.489,12.732,	20.7300,1341.7350,1084.3750},
{"[LS]st.Carson 2",30000,"Unknown","Unknown",150,0,0,4,0,0,0,"Unknown",50000,0,	2328.642,-1681.613,13.999,		221.6921,1149.8457,1082.6094},
{"[LS]st.Mulholland 21",190000,"Unknown","Unknown",250,0,0,12,0,0,0,"Unknown",50000,0,	2067.023,-1700.507,13.343,	2324.6213,-1143.9209,1050.4922},
{"[LS]st.Verona Beach 3",40000,"Unknown","Unknown",180,0,0,1,0,0,0,"Unknown",50000,0,	1983.229,-1716.938,15.163,	247.5086,304.9248,999.1484},
{"[LS]st.Santa Maria Beach 18",70000,"Unknown","Unknown",110,0,0,3,0,0,0,"Unknown",50000,0,	1855.599,-1915.128,14.451,		289.9850,307.3130,999.1484},
{"[LS]st.Palomino Creek 54",390000,"Unknown","Unknown",150,0,0,5,0,0,0,"Unknown",50000,0,	1890.086,-2066.387,14.230,		319.4555,312.5038,999.1484},
{"[LS]st.Blueberry 33",30000,"Unknown","Unknown",150,0,0,2,0,0,0,"Unknown",50000,0,	1736.992,-2128.999,12.741,	2451.1558,-1685.9690,1013.5078},
{"[LS]st.Dillimore 8",70000,"Unknown","Unknown",150,0,0,1,0,0,0,"Unknown",50000,0,	1687.525,-2100.221,13.029,	2535.0583,-1674.4338,1015.4986},
{"[LS]st.Angel Pine 23",80000,"Unknown","Unknown",150,0,0,2,0,0,0,"Unknown",50000,0,	2463.217,-1996.910,12.883,	272.0126,306.4208,999.1484},
{"[LS]st.Whetstone 32",200000,"Unknown","Unknown",200,0,0,3,0,0,0,"Unknown",50000,0,	2691.140,-2017.328,12.728,	235.1457,1190.7142,1080.2578},
{"[LS]st.Hashbury 83",90000,"Unknown","Unknown",100,0,0,2,0,0,0,"Unknown",50000,0,	2630.277,-1071.674,68.813,	221.4334,1240.8158,1082.1406},
{"[LS]st.Doherty 14",50000,"Unknown","Unknown",150,0,0,2,0,0,0,"Unknown",50000,0,	2629.399,-1117.942,66.677,	221.4334,1240.8158,1082.1406},
{"[LS]st.Queens 17",150000,"Unknown","Unknown",120,0,0,5,0,0,0,"Unknown",50000,0,	2539.907,-1061.115,68.762,	239.2819,1114.1991,1080.9922},
{"[LS]st.Chinatown 21",120000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	2554.512,-958.501,81.871,	295.3137,1478.6494,1080.2578},
{"[LS]st.Paradiso 52",120000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	2444.380,-965.150,79.234,	328.9125,1482.2545,1084.4375},
{"[LS]st.Calton Heights 37",200000,"Unknown","Unknown",130,0,0,15,0,0,0,"Unknown",50000,0,	1107.973,-974.878,41.960,	384.0571,1471.7700,1080.1875},
{"[LS]st.El Quebradas 78",120000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	940.709,-846.031,92.949,	374.5086,1417.2700,1081.3281},
{"[LS]st.Tierra Robada 46",120000,"Unknown","Unknown",150,0,0,4,0,0,0,"Unknown",50000,0,	697.757,-1058.628,48.608,	306.1966,307.8190,1003.3047},
{"[LS]st.Fort Carson 58",130000,"Unknown","Unknown",100,0,0,9,0,0,0,"Unknown",50000,0,		527.331,-1177.296,58.004,	260.9170,1242.6680,1084.2578},
{"[LS]st.Whitewood Estates 23",130000,"Unknown","Unknown",100,0,0,10,0,0,0,"Unknown",50000,0,	817.150,-766.772,76.431,	20.7300,1341.7350,1084.3750},
{"[LS]st.Prickle Pine 53",250000,"Unknown","Unknown",200,0,0,4,0,0,0,"Unknown",50000,0,		1421.289,-884.693,49.824,	221.6921,1149.8457,1082.6094},
{"[LS]st.Redsands West 36",230000,"Unknown","Unknown",100,0,0,12,0,0,0,"Unknown",50000,0,	1326.435,-1092.259,27.171,	2324.6213,-1143.9209,1050.4922},
{"[LS]st.Pirates In Mens Pants 63",46000,"Unknown","Unknown",120,0,0,1,0,0,0,"Unknown",50000,0,		1242.796,-1098.000,27.178,	247.5086,304.9248,999.1484},
{"[LS]st.The Camels Toe 53",46000,"Unknown","Unknown",120,0,0,3,0,0,0,"Unknown",50000,0,	1183.146,-1076.684,30.866,	289.9850,307.3130,999.1484},
{"[LS]st.Rockshore West 23",120000,"Unknown","Unknown",100,0,0,5,0,0,0,"Unknown",50000,0,	969.566,-1542.730,12.788,	319.4555,312.5038,999.1484},
{"[LS]st.The Clowns Pocket 101",99000,"Unknown","Unknown",120,0,0,2,0,0,0,"Unknown",50000,0,	768.810,-1746.158,12.272,	2451.1558,-1685.9690,1013.5078},
{"[LS]st.Old Venturas Strip 37",46000,"Unknown","Unknown",130,0,0,1,0,0,0,"Unknown",50000,0,	650.999,-1623.480,14.194,	2535.0583,-1674.4338,1015.4986},
{"[LS-Elite]st.Twisty 1",1500000,"Unknown","Unknown",700,0,0,2,0,0,0,"Unknown",50000,0,		1503.053,-691.452,93.944,	272.0126,306.4208,999.1484},
{"[LS-Elite]st.Twisty 4",3900000,"Unknown","Unknown",750,0,0,3,0,0,0,"Unknown",50000,0,		1473.440,-901.453,54.226,	235.1457,1190.7142,1080.2578},
{"[LS-Elite]st.Twisty 13",2100000,"Unknown","Unknown",650,0,0,2,0,0,0,"Unknown",50000,0,	1290.428,-798.630,87.507,	221.4334,1240.8158,1082.1406},
{"[LS-Elite]st.Twisty 9",1000000,"Unknown","Unknown",700,0,0,5,0,0,0,"Unknown",50000,0,		1093.638,-804.572,106.613,	239.2819,1114.1991,1080.9922},
{"[LS-Elite]st.Twisty 17",1400000,"Unknown","Unknown",700,0,0,15,0,0,0,"Unknown",50000,0,	1051.354,-638.648,119.312,	295.3137,1478.6494,1080.2578},
{"[LS-Elite]st.Twisty 21",2000000,"Unknown","Unknown",590,0,0,15,0,0,0,"Unknown",50000,0,	986.791,-671.282,121.171,	328.9125,1482.2545,1084.4375},
{"[LS-Elite]st.Twisty 44",3000000,"Unknown","Unknown",650,0,0,15,0,0,0,"Unknown",50000,0,	874.889,-695.124,113.337,	384.0571,1471.7700,1080.1875},
//{"[LS-Elite]st.Twisty 63",2100000,"Unknown","Unknown",700,0,0,15,0,0,0,"Unknown",50000,0,	318.322,-1137.270,80.788,	374.5086,1417.2700,1081.3281},
{"[LS-Elite]st.Twisty 32",1500000,"Unknown","Unknown",700,0,0,4,0,0,0,"Unknown",50000,0,	252.211,-1227.374,73.802,	306.1966,307.8190,1003.3047},
{"[LS-Elite]st.Twisty 28",1200000,"Unknown","Unknown",700,0,0,9,0,0,0,"Unknown",50000,0,	220.443,-1251.679,77.509,	260.9170,1242.6680,1084.2578},
{"[LS-Elite]st.Twisty 19",1200000,"Unknown","Unknown",700,0,0,10,0,0,0,"Unknown",50000,0,	693.871,-1706.249,3.014,	20.7300,1341.7350,1084.3750},
{"[LS-Elite]st.Twisty 67",2000000,"Unknown","Unknown",700,0,0,4,0,0,0,"Unknown",50000,0,	166.367,-1317.702,69.546,	221.6921,1149.8457,1082.6094},
{"[SF]st.Blutter 64",300000,"Unknown","Unknown",150,0,0,12,0,0,0,"Unknown",50000,0,		-2027.734,-42.028,37.999,	2324.6213,-1143.9209,1050.4922},
{"[SF]st.Blutter 34",70000,"Unknown","Unknown",150,0,0,1,0,0,0,"Unknown",50000,0,		-2016.835,766.215,44.640,		247.5086,304.9248,999.1484},
{"[SF]st.Blutter 77",800000,"Unknown","Unknown",150,0,0,3,0,0,0,"Unknown",50000,0,		-2528.760,-143.549,19.230,	289.9850,307.3130,999.1484},
{"[SF]st.Ultrion 34",160000,"Unknown","Unknown",300,0,0,5,0,0,0,"Unknown",50000,0,		-2580.921,-119.258,5.473,	319.4555,312.5038,999.1484},
{"[SF]st.Festyval 47",180000,"Unknown","Unknown",100,0,0,2,0,0,0,"Unknown",50000,0,		-2620.736,-185.525,6.398,	2451.1558,-1685.9690,1013.5078},
{"[SF]st.Festyval 92",212000,"Unknown","Unknown",150,0,0,1,0,0,0,"Unknown",50000,0,		-2623.224,-99.661,6.398,	2535.0583,-1674.4338,1015.4986},
{"[SF]st.Tini 6",150000,"Unknown","Unknown",120,0,0,2,0,0,0,"Unknown",50000,0,		-2720.792,-124.644,3.530,	272.0126,306.4208,999.1484},
{"[SF]st.Jaina 43",120000,"Unknown","Unknown",150,0,0,3,0,0,0,"Unknown",50000,0,	-2684.688,-182.026,6.398,	235.1457,1190.7142,1080.2578},
{"[SF]st.Toreso 52",120000,"Unknown","Unknown",150,0,0,2,0,0,0,"Unknown",50000,0,	-2783.329,-99.717,9.249,	221.4334,1240.8158,1082.1406},
{"[SF]st.MiniLit 37",200000,"Unknown","Unknown",130,0,0,5,0,0,0,"Unknown",50000,0,	-2779.904,-185.424,9.257,	239.2819,1114.1991,1080.9922},
{"[SF]st.Simfony 72",120000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	-2780.863,73.764,6.374,	295.3137,1478.6494,1080.2578},
{"[SF]st.Tierra 36",120000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	-2783.453,189.806,9.249,	328.9125,1482.2545,1084.4375},
{"[SF]st.Frutty 32",130000,"Unknown","Unknown",100,0,0,15,0,0,0,"Unknown",50000,0,	-2373.403,627.190,32.621,	384.0571,1471.7700,1080.1875},
{"[SF]st.RussianHouse 12",130000,"Unknown","Unknown",100,0,0,15,0,0,0,"Unknown",50000,0,	-2371.354,705.971,34.365,	374.5086,1417.2700,1081.3281},
{"[SF]st.RussianHouse 23",250000,"Unknown","Unknown",200,0,0,4,0,0,0,"Unknown",50000,0,		-2446.600,817.687,34.374,	306.1966,307.8190,1003.3047},
{"[SF]st.Gimory 36",230000,"Unknown","Unknown",100,0,0,9,0,0,0,"Unknown",50000,0,		-2374.975,856.082,40.905,	260.9170,1242.6680,1084.2578},
{"[SF]st.RussianHouse 53",460200,"Unknown","Unknown",120,0,0,10,0,0,0,"Unknown",50000,0,	-2236.224,870.549,65.839,	20.7300,1341.7350,1084.3750},
{"[SF]st.The CameSF Toe 63",400000,"Unknown","Unknown",120,0,0,4,0,0,0,"Unknown",50000,0,	-2174.076,797.035,61.645,	221.6921,1149.8457,1082.6094},
{"[SF]st.RMine 21",120000,"Unknown","Unknown",100,0,0,12,0,0,0,"Unknown",50000,0,		-2016.753,994.915,49.152,	2324.6213,-1143.9209,1050.4922},
{"[SF]st.Strike 99",100000,"Unknown","Unknown",120,0,0,1,0,0,0,"Unknown",50000,0,		-2382.141,1305.989,17.773,	247.5086,304.9248,999.1484},
{"[SF]st.Old Fiero Strip 37",460000,"Unknown","Unknown",130,0,0,3,0,0,0,"Unknown",50000,0,		-2353.556,1250.259,27.800,	289.9850,307.3130,999.1484},
{"[SF]st.MiniLit 50",110000,"Unknown","Unknown",150,0,0,5,0,0,0,"Unknown",50000,0,		-2157.071,979.139,79.201,	319.4555,312.5038,999.1484},
{"[SF-Elite]st.Mortynate 33",3000000,"Unknown","Unknown",800,0,0,2,0,0,0,"Unknown",50000,0,		-2624.417,1410.827,6.288,	2451.1558,-1685.9690,1013.5078},
//{"[SF-Elite]st.Mortynate 13",2000000,"Unknown","Unknown",750,0,0,1,0,0,0,"Unknown",50000,0,		-2105.307,899.656,75.905,	2535.0583,-1674.4338,1015.4986},
{"[SF-Elite]st.Mortynate 34",3000000,"Unknown","Unknown",790,0,0,2,0,0,0,"Unknown",50000,0,		-2078.481,902.135,63.327,	272.0126,306.4208,999.1484},
{"[LV]st.TutiDruti 43",300000,"Unknown","Unknown",150,0,0,3,0,0,0,"Unknown",50000,0,	2397.535,655.037,10.655,	235.1457,1190.7142,1080.2578},
{"[LV]st.TutiDruti 23",190000,"Unknown","Unknown",250,0,0,2,0,0,0,"Unknown",50000,0,	2318.441,654.771,10.648,	221.4334,1240.8158,1082.1406},
{"[LV]st.Bellion 42",400000,"Unknown","Unknown",180,0,0,5,0,0,0,"Unknown",50000,0,		2346.741,735.190,10.663,	239.2819,1114.1991,1080.9922},
{"[LV]st.TutiDruti 22",700000,"Unknown","Unknown",110,0,0,15,0,0,0,"Unknown",50000,0,	2346.558,692.725,10.655,	295.3137,1478.6494,1080.2578},
{"[LV]st.Ferrary Creek 14",390000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	2448.982,742.452,10.655,	328.9125,1482.2545,1084.4375},
//{"[LV]st.Blueberry 37",300000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	2539.768,719.439,13.934,	384.0571,1471.7700,1080.1875},
{"[LV]st.Terramore 8",700000,"Unknown","Unknown",150,0,0,15,0,0,0,"Unknown",50000,0,	2004.935,2758.146,10.015,	374.5086,1417.2700,1081.3281},
{"[LV]st.Angel Pine 22",800000,"Unknown","Unknown",150,0,0,4,0,0,0,"Unknown",50000,0,	2049.476,2761.351,10.015,	306.1966,307.8190,1003.3047},
{"[LV]st.Whetstone 92",200000,"Unknown","Unknown",300,0,0,9,0,0,0,"Unknown",50000,0,	2093.317,731.819,10.648,	260.9170,1242.6680,1084.2578},
{"[LV]st.Hashbury 43",90000,"Unknown","Unknown",100,0,0,10,0,0,0,"Unknown",50000,0,		2013.653,774.227,10.655,	20.7300,1341.7350,1084.3750},
{"[LV]st.Doherty 63",50000,"Unknown","Unknown",150,0,0,10,0,0,0,"Unknown",50000,0,		2123.306,774.726,10.640,	20.7300,1341.7350,1084.3750},
{"[LV]st.Queens 37",150000,"Unknown","Unknown",120,0,0,4,0,0,0,"Unknown",50000,0,		984.356,1894.683,10.655,	221.6921,1149.8457,1082.6094},
{"[LV]st.TutiDruti 21",120000,"Unknown","Unknown",150,0,0,12,0,0,0,"Unknown",50000,0,	1029.716,1927.262,10.663,	2324.6213,-1143.9209,1050.4922},
{"[LV]st.Sent 22",120000,"Unknown","Unknown",150,0,0,1,0,0,0,"Unknown",50000,0,		887.278,1979.872,10.655,	247.5086,304.9248,999.1484},
{"[LV]st.Calton Heights 37",200000,"Unknown","Unknown",130,0,0,3,0,0,0,"Unknown",50000,0,	1032.377,2316.267,10.663,	289.9850,307.3130,999.1484},
{"[LV]st.Don Mona 93",120000,"Unknown","Unknown",150,0,0,5,0,0,0,"Unknown",50000,0,		1359.602,2566.876,10.015,	319.4555,312.5038,999.1484},
{"[LV]st.Tierra Robada 37",120000,"Unknown","Unknown",150,0,0,2,0,0,0,"Unknown",50000,0,	1229.857,2584.680,10.015,	2451.1558,-1685.9690,1013.5078},
{"[LV]st.Fort Carson 47",130000,"Unknown","Unknown",100,0,0,1,0,0,0,"Unknown",50000,0,	1555.484,2663.311,10.015,		2535.0583,-1674.4338,1015.4986},
{"[LV]st.Terramore 23a",130000,"Unknown","Unknown",100,0,0,2,0,0,0,"Unknown",50000,0,	1603.561,2679.075,10.015,		272.0126,306.4208,999.1484},
{"[LV]st.Prickle Pine 33",250000,"Unknown","Unknown",200,0,0,3,0,0,0,"Unknown",50000,0,		1618.664,2606.625,10.015,	235.1457,1190.7142,1080.2578},
{"[LV]st.Redsands West 38",230000,"Unknown","Unknown",100,0,0,2,0,0,0,"Unknown",50000,0,	1733.996,2700.667,10.015,	221.4334,1240.8158,1082.1406},
{"[LV]st.Pirates In Mens Pants 77",46000,"Unknown","Unknown",120,0,0,5,0,0,0,"Unknown",50000,0,		1967.051,1623.265,12.056,	239.2819,1114.1991,1080.9922},
{"[LV]st.Gvard Toe 53",46000,"Unknown","Unknown",120,0,0,15,0,0,0,"Unknown",50000,0,		2367.264,2124.049,10.014,	295.3137,1478.6494,1080.2578},
{"[LV]st.Rockshore West 23",120000,"Unknown","Unknown",100,0,0,15,0,0,0,"Unknown",50000,0,	2339.733,1621.617,10.015,	328.9125,1482.2545,1084.4375},
//{"[LV]st.BoomHack 1",99000,"Unknown","Unknown",120,0,0,15,0,0,0,"Unknown",50000,0,		2025.345,1916.193,11.525,	384.0571,1471.7700,1080.1875},
{"[LV]st.Old Strip Bar 37",46000,"Unknown","Unknown",130,0,0,15,0,0,0,"Unknown",50000,0,	2360.096,2378.085,10.015,	374.5086,1417.2700,1081.3281},
{"[LV]st.Marihuana 50",110000,"Unknown","Unknown",150,0,0,4,0,0,0,"Unknown",50000,0,		2609.640,2392.888,17.015,	306.1966,307.8190,1003.3047},
{"[LV-Elite]st.Million 12",3700000,"Unknown","Unknown",800,0,0,9,0,0,0,"Unknown",50000,0,	2535.262,998.084,13.467,	260.9170,1242.6680,1084.2578},
{"[LV-Elite]st.Million 32",1000000,"Unknown","Unknown",750,0,0,10,0,0,0,"Unknown",50000,0,	1493.112,720.433,10.015,	20.7300,1341.7350,1084.3750},
{"[LV-Elite]st.Million 17",2000000,"Unknown","Unknown",790,0,0,4,0,0,0,"Unknown",50000,0,	2557.671,1561.958,10.015,	221.6921,1149.8457,1082.6094},
{"[LV-Elite]st.Million 43",1500000,"Unknown","Unknown",800,0,0,12,0,0,0,"Unknown",50000,0,	2483.931,1528.259,10.193,	2324.6213,-1143.9209,1050.4922},
{"[LV-Elite]st.Million 22",2000000,"Unknown","Unknown",750,0,0,1,0,0,0,"Unknown",50000,0,	2581.533,1061.976,10.469,	247.5086,304.9248,999.1484},
{"[LV-Motel]Motel Room 2",50000,"Unknown","Unknown",50,0,0,3,0,0,0,"Unknown",50000,0,		1784.483,2866.863,13.460,	289.9850,307.3130,999.1484},
{"[LV-Motel]Motel Room 5",50000,"Unknown","Unknown",50,0,0,5,0,0,0,"Unknown",50000,0,		1740.401,2861.605,13.460,	319.4555,312.5038,999.1484},
{"[LV-Motel]Motel Room 21",50000,"Unknown","Unknown",50,0,0,2,0,0,0,"Unknown",50000,0,		1737.867,2802.571,13.468,	2451.1558,-1685.9690,1013.5078},
{"[LV-Motel]Motel Room 22",50000,"Unknown","Unknown",50,0,0,1,0,0,0,"Unknown",50000,0,		1830.707,2742.897,13.468,	2535.0583,-1674.4338,1015.4986},
{"[LV-Motel]Motel Room 37",50000,"Unknown","Unknown",50,0,0,2,0,0,0,"Unknown",50000,0,		2660.460,749.261,13.934,	272.0126,306.4208,999.1484},
{"[LV-Motel]Motel Room 93",50000,"Unknown","Unknown",50,0,0,3,0,0,0,"Unknown",50000,0,		2613.922,719.468,13.934,	235.1457,1190.7142,1080.2578},
{"[LV-Motel]Motel Room 48",50000,"Unknown","Unknown",50,0,0,2,0,0,0,"Unknown",50000,0,		2525.864,749.313,13.934,	221.4334,1240.8158,1082.1406},
//{"[LV-Motel]Motel Room 38",50000,"Unknown","Unknown",50,0,0,5,0,0,0,"Unknown",50000,0,		2578.610,719.615,10.015,	239.2819,1114.1991,1080.9922},
{"[LV-Motel]Motel Room 1",50000,"Unknown","Unknown",50,0,0,15,0,0,0,"Unknown",50000,0,		2820.047,2271.459,13.856,	295.3137,1478.6494,1080.2578},
{"[LV-Motel]Motel Room 33",50000,"Unknown","Unknown",50,0,0,15,0,0,0,"Unknown",50000,0,		2793.985,2222.337,13.856,	328.9125,1482.2545,1084.4375},
{"[LV-Motel]Motel Room 28",50000,"Unknown","Unknown",50,0,0,15,0,0,0,"Unknown",50000,0,		2549.181,2202.389,13.311,	384.0571,1471.7700,1080.1875},
{"[LV-Motel]Motel Room 77",46000,"Unknown","Unknown",46,0,0,15,0,0,0,"Unknown",50000,0,		2609.615,2143.072,13.311,	374.5086,1417.2700,1081.3281},
{"[LV-Motel]Motel Room 53",46000,"Unknown","Unknown",46,0,0,4,0,0,0,"Unknown",50000,0,		2637.650,1979.056,13.311,	306.1966,307.8190,1003.3047},
{"[LV-Motel]Motel Room 23",50000,"Unknown","Unknown",50,0,0,9,0,0,0,"Unknown",50000,0,		2652.686,2018.261,13.311,	260.9170,1242.6680,1084.2578},
{"[LV-Motel]Motel Room 11",99000,"Unknown","Unknown",99,0,0,10,0,0,0,"Unknown",50000,0,		2453.709,1419.763,10.101,	20.7300,1341.7350,1084.3750},
{"[LV-Motel]Motel Room 37",46000,"Unknown","Unknown",46,0,0,4,0,0,0,"Unknown",50000,0,		1904.929,664.622,13.468,	221.6921,1149.8457,1082.6094},
{"[LV-Motel]Motel Room 13",30000,"Unknown","Unknown",30,0,0,12,0,0,0,"Unknown",50000,0,		1929.276,741.587,13.468,	2324.6213,-1143.9209,1050.4922},
{"[BR]House Mymy",200000,"Unknown","Unknown",120,0,0,1,0,0,0,"Unknown",50000,0,		2510.728,11.165,27.636,		247.5086,304.9248,999.1484},
{"[BR]House Tester",150000,"Unknown","Unknown",120,0,0,3,0,0,0,"Unknown",50000,0,	2374.721,20.795,27.636,		289.9850,307.3130,999.1484},
{"[BR]House Violet",270000,"Unknown","Unknown",120,0,0,5,0,0,0,"Unknown",50000,0,	2249.135,-121.139,27.348,	319.4555,312.5038,999.1484},
{"[BR]House Milki",200000,"Unknown","Unknown",120,0,0,2,0,0,0,"Unknown",50000,0,	1294.500,174.799,20.105,	2451.1558,-1685.9690,1013.5078},
{"[BR]House Tykte",200000,"Unknown","Unknown",120,0,0,1,0,0,0,"Unknown",50000,0,	748.371,278.347,26.453,		2535.0583,-1674.4338,1015.4986},
{"[BR]House Bilka",200000,"Unknown","Unknown",120,0,0,2,0,0,0,"Unknown",50000,0,	252.318,-116.120,2.730,		272.0126,306.4208,999.1484},
{"[BR]House Camun",200000,"Unknown","Unknown",120,0,0,3,0,0,0,"Unknown",50000,0,	-2157.860,-2536.616,31.011,	235.1457,1190.7142,1080.2578},
{"[BR]House Geomance",190000,"Unknown","Unknown",120,0,0,2,0,0,0,"Unknown",50000,0,		-2220.680,-2401.287,31.777,		221.4334,1240.8158,1082.1406},
{"[BR]House Dodo",200000,"Unknown","Unknown",120,0,0,5,0,0,0,"Unknown",50000,0,		-2045.275,-2523.618,30.261,	239.2819,1114.1991,1080.9922},
{"[BR]House Ferrary",200000,"Unknown","Unknown",120,0,0,15,0,0,0,"Unknown",50000,0,	768.494,2006.660,5.261,		295.3137,1478.6494,1080.2578},
{"[BR]House Blatnoy",200000,"Unknown","Unknown",120,0,0,15,0,0,0,"Unknown",50000,0,	-37.578,1078.458,19.304,	328.9125,1482.2545,1084.4375},
{"[BR]House Feramon",230000,"Unknown","Unknown",120,0,0,15,0,0,0,"Unknown",50000,0,	-260.176,1047.628,20.134,	384.0571,1471.7700,1080.1875},
{"[BR]House Californy",200000,"Unknown","Unknown",120,0,0,15,0,0,0,"Unknown",50000,0,	-907.070,1543.662,25.109,	374.5086,1417.2700,1081.3281},
{"[BR]House Meteor",260000,"Unknown","Unknown",120,0,0,4,0,0,0,"Unknown",50000,0,	-1483.853,2702.008,55.449,	306.1966,307.8190,1003.3047},
{"[BR]House Neon",260000,"Unknown","Unknown",120,0,0,9,0,0,0,"Unknown",50000,0,		-149.406,2688.812,61.624,	260.9170,1242.6680,1084.2578},
{"[BR]House Figaro",200000,"Unknown","Unknown",120,0,0,10,0,0,0,"Unknown",50000,0,	-285.919,2757.093,61.707,	20.7300,1341.7350,1084.3750},
{"[BR]House Pycucat",290000,"Unknown","Unknown",120,0,0,4,0,0,0,"Unknown",50000,0,	-2389.537,2410.905,8.072,	221.6921,1149.8457,1082.6094},
{"[BR]House SAS",260000,"Unknown","Unknown",120,0,0,12,0,0,0,"Unknown",50000,0,	-2634.079,2403.307,10.475,	2324.6213,-1143.9209,1050.4922},
{"[BR]House WayV",200000,"Unknown","Unknown",120,0,0,1,0,0,0,"Unknown",50000,0,	-2524.967,2239.568,4.600,	247.5086,304.9248,999.1484},

{"[LS-Elite]Big House",20000000,"Unknown","Unknown",120,0,0,1,0,0,0,"Unknown",50000,0,	1122.7229,-2037.1871,69.8942,	247.5086,304.9248,999.1484},
{"[LV]st.Blueberry 37",200000,"Unknown","Unknown",120,0,0,5,0,0,0,"Unknown",50000,0,	559.1950,-1076.3453,72.9220,	247.5086,304.9248,999.1484},
{"[LS]Four Dragons Mansion",200000,"Unknown","Unknown",120,0,0,1,0,0,0,"Unknown",50000,0,				-2627.3208,2283.4939,8.3123,	247.5086,304.9248,999.1484},
{"[LV-Motel]Motel Room 38",200000,"Unknown","Unknown",120,0,0,5,0,0,0,"Unknown",50000,0,	2206.3157,691.5402,11.4609,	239.2819,1114.1991,1080.9922}
};

stock HousesLoadAll()
{
	new file_housing;
	if(ini_Exist(ConfigDB))
	{
		file_housing = ini_Open(ConfigDB);
		ini_Get(file_housing,"Houses_DB",HousesDB);
		ini_GetFloat(file_housing,"House_DistanceOfShowLabel",HouseDistanceOfShowLabel);
		ini_Close(file_housing);
	}
	if(!ini_Exist(HousesDB))
	{
		HousesSaveAll();
		return;
	}
	for(new i=0;i<MAX_HOUSES;i++)
	{
		new housesdbname[MAX_STRING];
		format(housesdbname,sizeof(housesdbname),"%s%s.txt",HousesDB,Houses[i][Houses_Name]);
		if(!ini_Exist(housesdbname))
		{
			new File:fhnd = fopen(housesdbname,io_write);
			fclose(fhnd);
			HousesSaveAll();
		}
		file_housing = ini_Open(housesdbname);
		ini_Get(file_housing,"Name",Houses[i][Houses_Name],MAX_STRING);
		ini_GetInt(file_housing,"Cost",Houses[i][Houses_Cost]);
		ini_Get(file_housing,"Owner",Houses[i][Houses_Owner],MAX_STRING);
		ini_Get(file_housing,"Gang",Houses[i][Houses_Gang],MAX_STRING);
		ini_GetInt(file_housing,"UpKeep",Houses[i][Houses_UpKeep]);
		ini_GetInt(file_housing,"UpKeepLeft",Houses[i][Houses_UpKeepLeft]);
		ini_GetInt(file_housing,"Buyout",Houses[i][Houses_Buyout]);
		ini_GetInt(file_housing,"Interior",Houses[i][Houses_Interior]);
		ini_GetInt(file_housing,"Car",Houses[i][Houses_Car]);
		ini_GetInt(file_housing,"Lock",Houses[i][Houses_Lock]);
		ini_GetInt(file_housing,"Rentabil",Houses[i][Houses_Rentabil]);
		ini_Get(file_housing,"RentName",Houses[i][Houses_RentName],MAX_STRING);
		ini_GetInt(file_housing,"RentCost",Houses[i][Houses_RentCost]);
		ini_GetInt(file_housing,"HealUpgrade",Houses[i][Houses_HealUpgrade]);
		ini_GetFloat(file_housing,"Coord_X",Houses[i][Houses_EnterX]);
		ini_GetFloat(file_housing,"Coord_Y",Houses[i][Houses_EnterY]);
		ini_GetFloat(file_housing,"Coord_Z",Houses[i][Houses_EnterZ]);
		ini_Close(file_housing);
	}
	return;
}

stock HousesSaveAll()
{
	new file_housing;
	if(!ini_Exist(ConfigDB)) file_housing = ini_Create(ConfigDB);
	else file_housing = ini_Open(ConfigDB);
	ini_Set(file_housing,"Houses_DB",HousesDB);
	ini_SetFloat(file_housing,"House_DistanceOfShowLabel",HouseDistanceOfShowLabel);
	ini_Close(file_housing);
	for(new housid=0;housid<MAX_HOUSES;housid++) SaveHous(housid);
	return 1;
}

stock SaveHous(housid)
{
	new housesdbname[MAX_STRING],file_housing;
	format(housesdbname,sizeof(housesdbname),"%s%s.txt",HousesDB,Houses[housid][Houses_Name]);
	if(!ini_Exist(housesdbname)) file_housing = ini_Create(housesdbname);
	else file_housing = ini_Open(housesdbname);
	ini_Set(file_housing,"Name",Houses[housid][Houses_Name]);
	ini_SetInt(file_housing,"Cost",Houses[housid][Houses_Cost]);
	ini_Set(file_housing,"Owner",Houses[housid][Houses_Owner]);
	ini_Set(file_housing,"Gang",Houses[housid][Houses_Gang]);
	ini_SetInt(file_housing,"UpKeep",Houses[housid][Houses_UpKeep]);
	ini_SetInt(file_housing,"UpKeepLeft",Houses[housid][Houses_UpKeepLeft]);
	ini_SetInt(file_housing,"Buyout",Houses[housid][Houses_Buyout]);
	ini_SetInt(file_housing,"Interior",Houses[housid][Houses_Interior]);
	ini_SetInt(file_housing,"Car",Houses[housid][Houses_Car]);
	ini_SetInt(file_housing,"Lock",Houses[housid][Houses_Lock]);
	ini_SetInt(file_housing,"Rentabil",Houses[housid][Houses_Rentabil]);
	ini_Set(file_housing,"RentName",Houses[housid][Houses_RentName]);
	ini_SetInt(file_housing,"RentCost",Houses[housid][Houses_RentCost]);
	ini_SetFloat(file_housing,"Coord_X",Houses[housid][Houses_EnterX]);
	ini_SetFloat(file_housing,"Coord_Y",Houses[housid][Houses_EnterY]);
	ini_SetFloat(file_housing,"Coord_Z",Houses[housid][Houses_EnterZ]);
	ini_Close(file_housing);
	return 1;
}
stock housing_OnGameModeInit()
{
	HousesLoadAll();
	new string[MAX_STRING];
	for(new id=0;id<MAX_HOUSES;id++)
	{
		CreatePickup(1273,1,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ]+0.5);	//зеленый домик
		CreatePickup(1559,1,Houses[id][Houses_EnterX],Houses[id][Houses_EnterY],Houses[id][Houses_EnterZ]+0.5);
		house_icon[id] = CreateStreamMapIcon(31,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ]);
		format(string,sizeof(string),"Дом: %s\nСтоимость: %d\nКуплен: %s\nАрендуется: %s",Houses[id][Houses_Name],Houses[id][Houses_Cost],Houses[id][Houses_Owner],Houses[id][Houses_RentName]);
		House3DTextLabel[id] = Create3DTextLabel(string,COLOUR_WHITE,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ]+0.75,HouseDistanceOfShowLabel,0,1);
	}
	WriteLog(GameLog,"Housing checkpoints initialised");
}

stock IsPlayerAtHouse(playerid)
{
	for(new id=0;id<MAX_HOUSES;id++)
	{
		if(
		IsPlayerInRangeOfPoint(playerid,2,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ])
		||
		IsPlayerInRangeOfPoint(playerid,2,Houses[id][Houses_EnterX],Houses[id][Houses_EnterY],Houses[id][Houses_EnterZ])
		) return 1;
	}
	return 0;
}

stock GetHouseID(playerid)
{
	new lookupid=-1;
	for(new id=0;id<MAX_HOUSES;id++) if(IsPlayerInRangeOfPoint(playerid,1.5,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ])) lookupid = id;
	return lookupid;
}

stock IsPlayerRenter(playerid)
{
	for(new id=0;id<MAX_HOUSES;id++) if(strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true) == 0) return true;
	return false;
}

stock housing_OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
	#pragma unused inputtext
	if(dialogid == HouseDialogid)
	{
		if(response)
		{
			switch(Hous[playerid])
			{
				case 1:ShowPlayerDialog(playerid,HousesDialogid,DIALOG_STYLE_LIST,"Действия:","Купить дом\nВойти в дом\nАрендовать комнату\n","Выбрать","Отмена");
				case 2:ShowPlayerDialog(playerid,HousesDialogid,DIALOG_STYLE_LIST,"Действия:","Продать дом\nВойти в дом\nВыгнать арендатора\n","Выбрать","Отмена");
				case 3:ShowPlayerDialog(playerid,HousesDialogid,DIALOG_STYLE_LIST,"Действия:","Купить дом\nВойти в дом\nАрендовать комнату\n","Выбрать","Отмена");
				case 4:ShowPlayerDialog(playerid,HousesDialogid,DIALOG_STYLE_LIST,"Действия:","Купить дом\nВойти в дом\nПрекратить арендовать\nЗаплатить за аренду","Выбрать","Отмена");
			}
		}
		else Hous[playerid] = 0;
	}
	if(dialogid == HousesDialogid)
	{
		if(response)
		{
			if(listitem == 0)
			{
				if(Hous[playerid] != 2)	FHouseBuy(playerid);
				else FHouseSell(playerid);
			}
			if(listitem == 1) 
			{
				new id = GetHouseID(playerid);
				if(id > -1)
				{
					if(Houses[id][Houses_Lock] == 1) return SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][56]);
					else 
					{
						SetPlayerPosEx(playerid,Houses[id][Houses_EnterX],Houses[id][Houses_EnterY],Houses[id][Houses_EnterZ],0,Houses[id][Houses_Interior]);
						Haus[playerid] = id;
						SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][73]);
					}
				}
			}
			if(listitem == 2)
			{
				switch(Hous[playerid])
				{
					case 1,3:RentRoom(playerid);
					case 2,4:DeleteRenter(playerid);
				}
			}
		}
		Hous[playerid] = 0;
	}
	return 1;
}

stock RentRoom(playerid)
{
	new id = GetHouseID(playerid),string[MAX_STRING];
	if(id <= -1) return 1;
	if(IsPlayerRenter(playerid)) return 1;
	if(Houses[id][Houses_Rentabil] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][68]);
	if(strlen(PlayerGangName[playerid]) > 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][79]);
	if(!strcmp(Houses[id][Houses_RentName],"Unknown",true))
	{
		if(oGetPlayerMoney(playerid) < 50000) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][57]);
		set(Houses[id][Houses_RentName],oGetPlayerName(playerid));
		oGivePlayerMoney(playerid,-Houses[id][Houses_RentCost],0);
		format(string,sizeof(string),"%s%s.txt",HousesDB,Houses[id][Houses_Name]);
		new file_housing = ini_Open(string);
		ini_SetInt(file_housing,"Rentabil",1);
		ini_Set(file_housing,"RentName",oGetPlayerName(playerid));
		ini_Close(file_housing);
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][55]);
		UpdatePlayerStreamMapIcon(playerid,house_icon[id]);
	}
	else
	{
		format(string,sizeof(string),lang_texts[8][54],Houses[id][Houses_RentName]);
		SendClientMessage(playerid,COLOUR_RED,string);
	}
	return 1;
}

stock DeleteRenter(playerid)
{
	new id = GetHouseID(playerid);
	if(strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true) == 0) 
	{
		SendClientMessage(playerid,COLOUR_WHITE,lang_texts[8][58]);
		for(new i=GetMaxPlayers()-1;i>=0;--i)
			if(IsPlayerConnected(i) && !strcmp(Houses[id][Houses_RentName],oGetPlayerName(i),true))
				SendClientMessage(i,COLOUR_LIGHTRED,lang_texts[8][60]);
	}
	else if(!strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true))
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][59]);
	if(id > -1) set(Houses[id][Houses_RentName],"Unknown");
	UpdatePlayerStreamMapIcon(playerid,house_icon[id]);
	return 1;
}

stock housing_OnPlayerCommandText(playerid,text[]) 
{
	if(!IsPlayerConnected(playerid)) return 0;
	new cmd[20],idx;
	set(cmd,strcharsplit(text, idx,strchar(" ")));
	if(strlen(cmd) == 0) return 0;

	if((!strcmp(cmd,"/houseinfo",true)) || (!strcmp(cmd,"/hinfo",true)))
	{
		if(!IsPlayerAtHouse(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][2]);
		FHouseInfo(playerid);
		return 1;
	}
	if((!strcmp(cmd,"/housebuy",true)) || (!strcmp(cmd,"/hbuy",true)))
	{
		if(!IsPlayerAtHouse(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][14]);
		FHouseBuy(playerid);
		return 1;
	}
	if((!strcmp(cmd,"/housesell",true)) || (!strcmp(cmd,"/hsell",true)))
	{
		if(!IsPlayerAtHouse(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][22]);
		FHouseSell(playerid);
		return 1;
	}
	if(!strcmp(cmd,"/myhouses",true)) FHouseMyHouses(playerid);
	if(!strcmp(cmd,"/houses",true)) FHouseHouses(playerid);
	if(!strcmp(cmd,"/upkeep",true))
	{
		if(!IsPlayerAtHouse(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][43]); 
		FHouseKeep(playerid);
		return 1;
	}
	if(!strcmp(cmd,"/heal",true))
	{
		if(Haus[playerid] <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][61]);
		new id = Haus[playerid];
		if(Houses[id][Houses_HealUpgrade] != 0)
		{
			SetPlayerArmour(playerid,100);
			SetPlayerHealth(playerid,1000);
		}
		else SendClientMessage(playerid,COLOUR_RED,lang_texts[8][62]);
		return 1;
	}
	if(!strcmp(cmd,"/houseupgrade",true))
	{
		if(Haus[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][61]);
		new id = Haus[playerid];
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if(strlen(cmd) == 0) SendClientMessage(playerid,COLOUR_GREY,lang_texts[8][66]);
		if(!strcmp(cmd,"heal",true))
		{
			if(GetPlayerMoney(playerid) >= 100000)
			{
				if(Houses[id][Houses_HealUpgrade] != 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][65]);
				Houses[id][Houses_HealUpgrade] = 1;
				oGivePlayerMoney(playerid,-100000,0);
				SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][64]);
			}
			else SendClientMessage(playerid,COLOUR_RED,lang_texts[8][63]);
		}
		return 1;
	}
	if(!strcmp(cmd,"/setrentcost",true))
	{
		new id = GetHouseID(playerid),string[MAX_STRING];
		if((id > -1 || Haus[playerid] > -1) && !strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true))
		{
			id = Haus[playerid];
			new rentcost = strval(strcharsplit(text, idx,strchar(" ")));
			if(rentcost < 1) return SendClientMessage(playerid,COLOUR_GREY,lang_texts[8][71]);
			Houses[id][Houses_RentCost] = rentcost;
			format(string,sizeof(string),lang_texts[8][72],rentcost);
			SendClientMessage(playerid,COLOUR_LIGHTRED,string);
		}
		return 1;
	}
	if(!strcmp(cmd,"/hlock",true))
	{
		if(Haus[playerid] > -1) if(!strcmp(Houses[Haus[playerid]][Houses_Owner],oGetPlayerName(playerid),true)) Houses[Haus[playerid]][Houses_Lock] = 1;
		return 1;
	}
	if(!strcmp(cmd,"/hunlock",true))
	{
		new id = GetHouseID(playerid);
		if(id > -1) if(!strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true)) Houses[id][Houses_Lock] = 0;
		return 1;
	}
	if(!strcmp(cmd,"/rent",true))
	{
		new id = GetHouseID(playerid);
		set(cmd,strcharsplit(text, idx,strchar(" ")));
		if(strlen(cmd) < 1) SendClientMessage(playerid,COLOUR_RED,lang_texts[8][76]);
		else if(!strcmp(cmd,"on",true))
		{
			Houses[id][Houses_Rentabil] = 1;
			new filename_housing[MAX_STRING];
			format(filename_housing,sizeof(filename_housing),"%s%s.txt",HousesDB,Houses[id][Houses_Name]);
			if(ini_Exist(filename_housing))
			{
				new file_housing = ini_Open(filename_housing);
				ini_SetInt(file_housing,"Rentabil",Houses[id][Houses_Rentabil]);
				ini_Close(file_housing);
			}
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][74]);
			return 1;
		}
		else if(!strcmp(cmd,"off",true))
		{
			Houses[id][Houses_Rentabil] = 0;
			new filename_housing[MAX_STRING];
			format(filename_housing,sizeof(filename_housing),"%s%s.txt",HousesDB,Houses[id][Houses_Name]);
			if(ini_Exist(filename_housing))
			{
				new file_housing = ini_Open(filename_housing);
				ini_SetInt(file_housing,"Rentabil",Houses[id][Houses_Rentabil]);
				ini_Close(file_housing);
			}
			SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][75]);
			return 1;
		}
		return 1;
	}
	return 0;
}

stock FHouseInfo(playerid) 
{
	new id=GetHouseID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][13]);
	new temp[MAX_STRING];
	format(temp,sizeof(temp),lang_texts[8][3],Houses[id][Houses_Name]);
	SendClientMessage(playerid,COLOUR_GREEN,temp);
	if(strcmp(Houses[id][Houses_Owner],"Unknown",true) && strcmp(Houses[id][Houses_Gang],"Unknown",true))
	{
		format(temp,sizeof(temp),lang_texts[8][4],Houses[id][Houses_Gang],Houses[id][Houses_Owner]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		new price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
		new UpKeep = Houses[id][Houses_UpKeep];
		if(strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true) || strcmp(Houses[id][Houses_Gang], PlayerGangName[playerid], true))
		{
			format(temp,sizeof(temp),lang_texts[8][5],price);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			format(temp,sizeof(temp),lang_texts[8][6],UpKeep);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
			format(temp,sizeof(temp),lang_texts[8][7],Houses[id][Houses_UpKeepLeft],UpKeep*2800);
			SendClientMessage(playerid,COLOUR_GREEN,temp);
		}
	}
	else
	{
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][9]);
		format(temp,sizeof(temp),lang_texts[8][10],Houses[id][Houses_Cost]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		format(temp,sizeof(temp),lang_texts[8][11],Houses[id][Houses_UpKeep],Houses[id][Houses_UpKeepLeft]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
	}
	return 1;
}

stock FHouseHouses(playerid)
{
	new temp[MAX_STRING];
	for(new id=0;id<MAX_HOUSES;id++)
	{
		if(strcmp(Houses[id][Houses_Owner],"Unknown",true))
		{
			format(temp,sizeof(temp),lang_texts[8][38],Houses[id][Houses_Name],Houses[id][Houses_Gang],Houses[id][Houses_Owner]);
			SendPlayerScrollingText(playerid,temp);
		} 
		else 
		{
			format(temp,sizeof(temp),lang_texts[8][39],Houses[id][Houses_Name]);
			SendPlayerScrollingText(playerid,temp);
		}
	}
	return 1;
}

stock FHouseBuy(playerid) 
{
	for(new i=0;i<MAX_HOUSES;i++)
		if(!strcmp(oGetPlayerName(playerid),Houses[i][Houses_Owner],true))
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][78]);
	new temp[MAX_STRING];
	new id = GetHouseID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][21]);
	if(!IsPlayerRegistered(playerid)) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][20]);
	if(PlayerGangid[playerid] == 0) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][19]);
	new price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
	if(Player[playerid][Money] < price) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][18]);
	if(!strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true))
		SendPlayerFormattedText(playerid,lang_texts[8][15],0,COLOUR_RED);
#if !defined BUY_ALL_HOUSES
	else if(strcmp(Houses[id][Houses_Owner],"Unknown",true))
		SendClientMessage(playerid,COLOUR_RED,"Этот дом уже куплен!");
#endif
	else
	{
		new owner=-1;
		for(new ownerid=GetMaxPlayers()-1;ownerid>=0;--ownerid)
			if(IsPlayerConnected(ownerid)) if(!strcmp(Houses[id][Houses_Owner],oGetPlayerName(ownerid),true))
				owner = ownerid;
		oGivePlayerMoney(playerid,0-price,1);
		if(owner > -1)
			return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][77]);
		else
		{
			new filename_housing[MAX_STRING];
			format(filename_housing,sizeof(filename_housing),"%s%s.txt",PlayerDB,Houses[id][Houses_Owner]);
			if(ini_Exist(filename_housing))
			{
				new file_housing = ini_Open(filename_housing);
				new tempmoney;
				ini_GetInt(file_housing,"Money",tempmoney);
				ini_SetInt(file_housing,"Money",tempmoney+price);
				ini_Close(file_housing);
			}
		}
		set(Houses[id][Houses_Owner],oGetPlayerName(playerid));
		set(Houses[id][Houses_Gang],PlayerGangName[playerid]);
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][69]);
		SendClientMessage(playerid,COLOUR_LIGHTRED,lang_texts[8][70]);
		Houses[id][Houses_Buyout] = 0;
		UpdateStreamMapIcon(house_icon[id]);
		new filename_housing[MAX_STRING],logstring[MAX_STRING];
		format(filename_housing,sizeof(filename_housing),"%s%s.txt",HousesDB,Houses[id][Houses_Name]);
		if(ini_Exist(filename_housing))
		{
		    new file_housing = ini_Open(filename_housing);
			ini_Set(file_housing,"Owner",Houses[id][Houses_Owner]);
			ini_Set(file_housing,"Gang",Houses[id][Houses_Gang]);
			ini_SetInt(file_housing,"Buyout",Houses[id][Houses_Buyout]);
			ini_Close(file_housing);
		}
		format(temp,sizeof(temp),lang_texts[8][17],Houses[id][Houses_Name]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		format(logstring,sizeof(logstring),"player: %d:  %s: bought the '%s' (house)",playerid,oGetPlayerName(playerid),Houses[id][Houses_Name]);
		WriteLog(GameLog,logstring);
	}
	return 1;
}

stock FHouseSell(playerid) 
{
	FHouseKeep(playerid);
	new temp[MAX_STRING];
	new id = GetHouseID(playerid);
	if(id <= -1) return SendClientMessage(playerid,COLOUR_RED,lang_texts[8][28]);
	if(!IsPlayerRegistered(playerid))
	{
		SendClientMessage(playerid,COLOUR_RED,lang_texts[8][26]);
		SendClientMessage(playerid,COLOUR_RED,lang_texts[8][27]);
		return 1;
	}
	if(strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true))
		SendPlayerFormattedText(playerid,lang_texts[8][23],0,COLOUR_RED);
	else
	{
		new price = ((Houses[id][Houses_Cost] + Houses[id][Houses_Buyout]) * 85) / 100;
		oGivePlayerMoney(playerid,price,1);
		set(Houses[id][Houses_Owner],"Unknown");
		set(Houses[id][Houses_Gang],"Unknown");
		Houses[id][Houses_Buyout] = 0;
		UpdateStreamMapIcon(house_icon[id]);
		SaveHous(id);
		format(temp,sizeof(temp),lang_texts[8][24],Houses[id][Houses_Name]);
		SendClientMessage(playerid,COLOUR_GREEN,temp);
		SendClientMessage(playerid,COLOUR_GREEN,lang_texts[8][25]);
		new logstring[MAX_STRING];
		format(logstring,sizeof(logstring),"Player: %s [ID %d]: sold the '%s' (house)",oGetPlayerName(playerid),playerid,Houses[id][Houses_Name]);
		WriteLog(GameLog,logstring);
	}
	return 1;
}

stock FHouseMyHouses(playerid) 
{
	new temp[MAX_STRING];
	new count = 0;
	for(new id=0;id<MAX_HOUSES;id++)
	{
		if(!strcmp(Houses[id][Houses_Gang],PlayerGangName[playerid],true))
		{
			count++;
			format(temp,sizeof(temp),lang_texts[8][40],Houses[id][Houses_Name],Houses[id][Houses_Owner],Houses[id][Houses_Cost]);
			SendPlayerScrollingText(playerid,temp);
			format(temp,sizeof(temp),lang_texts[8][41],Houses[id][Houses_UpKeep], Houses[id][Houses_UpKeepLeft]);
			SendPlayerScrollingText(playerid,temp);
		}
	}
	if(count < 1) SendPlayerFormattedText(playerid,lang_texts[8][42],0,COLOUR_RED);
	return 1;
}

stock FHouseKeep(playerid) 
{
	new id=GetHouseID(playerid);
	if(id > -1)
	{
		if(IsPlayerRegistered(playerid))
		{
			if(strcmp(Houses[id][Houses_Gang],PlayerGangName[playerid],true) == 0)
			{
				if(oGetPlayerMoney(playerid) > Houses[id][Houses_UpKeepLeft]) 
				{
					if(Houses[id][Houses_UpKeepLeft] > 0)
					{
						Player[playerid][Money] -= Houses[id][Houses_UpKeepLeft];
						SendPlayerFormattedText(playerid,  lang_texts[8][44] , 0,COLOUR_GREEN);
						Houses[id][Houses_UpKeepLeft] = 0;
						new filename_housing[MAX_STRING];
						format(filename_housing,sizeof(filename_housing),"%s%s.txt",HousesDB,Houses[id][Houses_Name]);
						if(ini_Exist(filename_housing))
						{
							new file_housing = ini_Open(filename_housing);
							ini_SetInt(file_housing,"UpKeepLeft",Houses[id][Houses_UpKeepLeft]);
							ini_Close(file_housing);
						}
					} 
					else SendPlayerFormattedText(playerid,lang_texts[8][45],0,COLOUR_RED);
				}
				else SendPlayerFormattedText(playerid,lang_texts[8][46],0,COLOUR_RED);
			} 
			else SendPlayerFormattedText(playerid,lang_texts[8][47],0,COLOUR_RED);
		} 
		else 
		{
			SendPlayerFormattedText(playerid,lang_texts[8][48],0,COLOUR_RED);
			SendPlayerFormattedText(playerid,lang_texts[8][49],0,COLOUR_RED);
		}
	} 
	else SendPlayerFormattedText(playerid,lang_texts[8][50],0,COLOUR_RED);
}

stock ganghouse(playerid) 
{
	new hova = -1,tmp[MAX_STRING];
	if(!IsPlayerRegistered(playerid) || !strlen(PlayerGangName[playerid])) return hova;
	for(new id=0;id<MAX_HOUSES;id++)
	{
		if(!strcmp(Houses[id][Houses_Gang],PlayerGangName[playerid],true)) 
		{
			hova = id;
			format(tmp,sizeof(tmp),lang_texts[8][51],Houses[id][Houses_Name],Houses[id][Houses_UpKeepLeft],Houses[id][Houses_UpKeep]*2800);
			SendClientMessage(playerid,COLOUR_RED,tmp);
		}
		if(!strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true)) hova = id;
	}
	return hova;
}

stock gang2house(playerid,i)
{
	SetPlayerPos(playerid,Houses[i][Houses_PickupX],Houses[i][Houses_PickupY],Houses[i][Houses_PickupZ]+0.5);
}

stock HouseKeepUp()
{
	new upkeepamount,upkeep,upkeepleft;
	for(new id=0;id<MAX_HOUSES;id++)
	{
		if(strcmp(Houses[id][Houses_Gang],"Unknown",true) != 0) 
		{
			upkeep = Houses[id][Houses_UpKeep];
			upkeepleft = Houses[id][Houses_UpKeepLeft];
			upkeepamount = upkeep + upkeepleft;
			if(upkeepamount < upkeep*2800) 
			{
				Houses[id][Houses_UpKeepLeft] = upkeepamount;
				new filename_housing[MAX_STRING];
				format(filename_housing,sizeof(filename_housing),"%s%s.txt",HousesDB,Houses[id][Houses_Name]);
				if(ini_Exist(filename_housing))
				{
					new file_housing = ini_Open(filename_housing);
					ini_SetInt(file_housing,"UpKeepLeft",upkeepamount);
					ini_Close(file_housing);
				}
			} 
			else 
			{ 
				new filename_housing[MAX_STRING];
				format(filename_housing,sizeof(filename_housing),"%s%s.txt",HousesDB,Houses[id][Houses_Name]);
				if(ini_Exist(filename_housing))
				{
					new file_housing = ini_Open(filename_housing);
					ini_Set(file_housing,"Gang","Unknown");
					ini_Set(file_housing,"Owner","Unknown");
					ini_Set(file_housing,"UpKeepLeft","0");
					ini_Close(file_housing);
				}   
			}
		} 
	}
}

stock housing_OnPlayerKeyStateChange(playerid,newkeys,oldkeys)
{
	#pragma unused oldkeys,newkeys
	for(new id=0;id<MAX_HOUSES;id++)
	{
		if(IsPlayerInRangeOfPoint(playerid,3,Houses[id][Houses_PickupX],Houses[id][Houses_PickupY],Houses[id][Houses_PickupZ]+0.5))
		{
			new string[MAX_STRING],string2[MAX_STRING],string3[MAX_STRING],string4[MAX_STRING],string5[MAX_STRING],string6[MAX_STRING],dialog[MAX_STRING];
			format(string,sizeof(string),lang_texts[8][3],Houses[id][Houses_Name]);
			new price = Houses[id][Houses_Cost] + Houses[id][Houses_Buyout];
			new UpKeep = Houses[id][Houses_UpKeep];
			if(strcmp(Houses[id][Houses_Owner],"Unknown",true) && strcmp(Houses[id][Houses_Gang],"Unknown",true))
			{
				if(strcmp(Houses[id][Houses_Owner],oGetPlayerName(playerid),true) || strcmp(Houses[id][Houses_Gang],PlayerGangName[playerid],true))
				{
					if(strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true) != 0) Hous[playerid] = 1;
					else Hous[playerid] = 4;
					format(string2,sizeof(string2),lang_texts[8][5],price);
					format(string3,sizeof(string3),lang_texts[8][6],UpKeep);
					format(string4,sizeof(string4),lang_texts[8][7],Houses[id][Houses_UpKeepLeft],UpKeep*2800);
					format(string5,sizeof(string5),lang_texts[8][4],Houses[id][Houses_Gang],Houses[id][Houses_Owner]);
					if(strcmp(Houses[id][Houses_RentName],"Unknown",true) == 0) format(string6,sizeof(string6),lang_texts[8][53],Houses[id][Houses_RentCost]);
					else format(string6,sizeof(string6),lang_texts[8][52],Houses[id][Houses_RentName]);
					format(dialog,sizeof(dialog),"%s\n%s\n%s\n%s\n%s\n",string2,string3,string4,string5,string6);
					ShowPlayerDialog(playerid,2,DIALOG_STYLE_MSGBOX,string,dialog,"Действия","Отмена");
				}
				else
				{
					format(string2,sizeof(string2),lang_texts[8][5],price);
					format(string3,sizeof(string3),lang_texts[8][6],UpKeep);
					if(strcmp(Houses[id][Houses_RentName],"Unknown",true) == 0) format(string4,sizeof(string4),lang_texts[8][53],Houses[id][Houses_RentCost]);
					else format(string4,sizeof(string4),lang_texts[8][52],Houses[id][Houses_RentName]);
					format(dialog,sizeof(dialog),"%s\n%s\n%s\n",string2,string3,string4);
					Hous[playerid] = 2;
					ShowPlayerDialog(playerid,2,DIALOG_STYLE_MSGBOX,string,dialog,"Действия","Отмена");
				}
			}
			else
			{
				if(strcmp(Houses[id][Houses_RentName],oGetPlayerName(playerid),true) != 0) Hous[playerid] = 3;
				else Hous[playerid] = 4;
				format(string2,sizeof(string2),lang_texts[8][9]);
				format(string3,sizeof(string3),lang_texts[8][10],Houses[id][Houses_Cost]);
				format(string4,sizeof(string4),lang_texts[8][11],Houses[id][Houses_UpKeep],Houses[id][Houses_UpKeepLeft]);
				format(string5,sizeof(string5),lang_texts[8][53],Houses[id][Houses_RentCost]);
				format(dialog,sizeof(dialog),"%s\n%s\n%s\n%s\n",string2,string3,string4,string5);
				ShowPlayerDialog(playerid,2,DIALOG_STYLE_MSGBOX,string,dialog,"Действия","Отмена");
			}
		}
		else if(IsPlayerInRangeOfPoint(playerid,2,Houses[id][Houses_EnterX],Houses[id][Houses_EnterY],Houses[id][Houses_EnterZ]))
		{
			SetPlayerInterior(playerid,0);
			gang2house(playerid,Haus[playerid]);
			Haus[playerid] = -1;
		}
	}
	return 1;
}

stock housing_Update3DTextLabelText()
{
	new string[MAX_STRING];
	for(new id=0;id<MAX_HOUSES;id++)
	{
		format(string,sizeof(string),"Дом: %s\nСтоимость: %d\nКуплен: %s\nАрендуется: %s",Houses[id][Houses_Name],Houses[id][Houses_Cost],Houses[id][Houses_Owner],Houses[id][Houses_RentName]);
		Update3DTextLabelText(House3DTextLabel[id],COLOUR_WHITE,string);
	}
	return 1;
}
