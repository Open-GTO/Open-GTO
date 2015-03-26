/*

	About: property lang system
	Author: ziggi

*/

#if defined _zlang_included
	#endinput
#endif

#define _zlang_included
#pragma library zlang


#define MAX_TEXT_ENTRIES  1024

#define MAX_VAR_STRING    32
#define MAX_VALUE_STRING  128

#define LANG_VIRTUAL_MACHINE 1


#define _(%0) Lang_GetText(#%0)


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
		varname[MAX_VAR_STRING],
		string[MAX_VALUE_STRING + MAX_VAR_STRING];

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
	new result[MAX_VALUE_STRING];
	if (!isnull(varname)) {
		new exist = getproperty(LANG_VIRTUAL_MACHINE, .value = bernstein(varname), .string = result);
		if (exist == 0) {
			strcat(result, "Error: lang value ");
			strcat(result, varname);
			strcat(result, " not found.");
		} else {
			strunpack(result, result);
		}
	} else {
		result = "Error: lang varname is null";
	}
	return result;
}

stock Lang_DeleteText(varname[])
{
	if (!isnull(varname)) {
		return deleteproperty(LANG_VIRTUAL_MACHINE, .value = bernstein(varname));
	}
	return 0;
}
