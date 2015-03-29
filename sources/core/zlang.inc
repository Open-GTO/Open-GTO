/*

	About: property lang system
	Author: ziggi

*/

#if defined _zlang_included
	#endinput
#endif

#define _zlang_included
#pragma library zlang


#define MAX_TEXT_ENTRIES       1024

#define MAX_MULTI_VAR_COUNT    10

#define MAX_LANG_VAR_STRING    48
#define MAX_LANG_VALUE_STRING  128
#define MAX_LANG_MULTI_STRING  (MAX_LANG_VALUE_STRING * MAX_MULTI_VAR_COUNT)

#define LANG_VIRTUAL_MACHINE   1


#define _(%0) Lang_GetText(#%0)
#define _m(%0) Lang_GetMultiText(#%0)


stock Lang_OnGameModeInit()
{
	Lang_LoadText(DATA_FILES_FOLDER"lang_ru_test.gto");
	return 1;
}

stock Lang_LoadText(filename[])
{
	new File:flang = fopen(filename, io_read);
	if (!flang) {
		printf("Error: no such language file '%s'", filename);
		return 0;
	}
	
	new
		i,
		sep_pos,
		varname[MAX_LANG_VAR_STRING],
		string[MAX_LANG_VALUE_STRING + MAX_LANG_VAR_STRING];

	while (fread(flang, string, sizeof(string))) {
		if (string[0] == '\0' || (string[0] == '/' && string[1] == '/')) {
			continue;
		}
		
		sep_pos = -1;
		for (i = 0; string[i] >= ' '; i++) {
			if (sep_pos == -1) {
				if (string[i] == ' ' && string[i + 1] == '=' && string[i + 2] == ' ') {
					strmid(varname, string, 0, i);
					sep_pos = i;
				}
			} else if (string[i] == '\\') {
				switch (string[i + 1]) {
					case 'n': {
						strdel(string, i, i + 1);
						string[i] = '\n';
					}
					case 't': {
						strdel(string, i, i + 1);
						string[i] = '\t';
					}
					case '%': {
						strdel(string, i, i + 1);
						strins(string, "%", i);
					}
					case 's': {
						strdel(string, i, i + 1);
						string[i] = ' ';
					}
					case '\\': {
						strdel(string, i, i + 1);
						string[i] = '\\';
					}
				}
			}
		}

		if (sep_pos != -1) {
			string[i] = '\0';
			Lang_SetText(varname, string[sep_pos + 3]);
		}
	}

	fclose(flang);
	return 1;
}

stock Lang_SetText(varname[], value[])
{
	if (isnull(varname)) {
		return 0;	
	}
	setproperty(LANG_VIRTUAL_MACHINE, .value = bernstein(varname), .string = value);
	return 1;
}

stock Lang_GetText(varname[])
{
	new string[MAX_LANG_VALUE_STRING];
	if (!isnull(varname)) {
		new exist = getproperty(LANG_VIRTUAL_MACHINE, .value = bernstein(varname), .string = string);
		if (exist == 0) {
			strcat(string, "Error: lang value ");
			strcat(string, varname);
			strcat(string, " not found.");
		} else {
			strunpack(string, string);
		}
	} else {
		string = "Error: lang varname is null";
	}
	return string;
}

stock Lang_GetMultiText(varname[])
{
	new string[MAX_LANG_MULTI_STRING];
	if (!isnull(varname)) {
		new
			exist,
			property_value[MAX_LANG_VALUE_STRING],
			property_name[MAX_LANG_VAR_STRING + 6];

		for (new i = 0; i < MAX_MULTI_VAR_COUNT; i++) {
			format(property_name, sizeof(property_name), "%s_%d", varname, i);
			exist = getproperty(LANG_VIRTUAL_MACHINE, .value = bernstein(property_name), .string = property_value);
			
			if (exist != 0) {
				strcat(string, property_value);
			} else {
				if (i == 0) {
					strcat(string, "Error: multi lang value ");
					strcat(string, varname);
					strcat(string, " not found.");
				}
				break;
			}
		}
	} else {
		string = "Error: multi lang varname is null";
	}
	return string;
}

stock Lang_DeleteText(varname[])
{
	if (!isnull(varname)) {
		return deleteproperty(LANG_VIRTUAL_MACHINE, .value = bernstein(varname));
	}
	return 0;
}
