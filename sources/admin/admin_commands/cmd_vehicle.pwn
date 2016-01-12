/*

	About: vehicle admin command
	Author: ziggi

*/

#if defined _admin_cmd_vehicle_included
	#endinput
#endif

#define _admin_cmd_vehicle_included
#pragma library admin_cmd_vehicle

COMMAND:v(playerid, params[])
{
	return cmd_vehicle(playerid, params);
}

COMMAND:vehicle(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subcmd[32],
		subparams[64];

	if (sscanf(params, "s[32]S()[64]", subcmd, subparams)) {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_HELP));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		playername[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(playerid, playername, sizeof(playername));

	if (strcmp(subcmd, "add", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		new modelid;

		if (sscanf(subparams, "k<vehicle>", modelid) || modelid == -1) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_ADD_ERROR));
			return 1;
		}

		// get info
		new Float:x, Float:y, Float:z, Float:r, interior, world;
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, r);
		interior = GetPlayerInterior(playerid);
		world = GetPlayerVirtualWorld(playerid);

		// create vehicle
		new vehicleid = CreateVehicle(modelid, x, y, z, r, -1, -1, VEHICLE_RESPAWN_TIME);
		LinkVehicleToInterior(vehicleid, interior);
		SetVehicleVirtualWorld(vehicleid, world);
		SetVehicleFuel(vehicleid, -1);

		// put in
		PutPlayerInVehicle(playerid, vehicleid, 0);

		// message
		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_ADD_MESSAGE), playername, playerid, vehicleid);
		SendMessageToNearPlayers(string, 40.0, x, y, z);
	} else if (strcmp(subcmd, "remove", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		new vehicleid;

		if (sscanf(subparams, "I(0)", vehicleid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_REMOVE_ERROR));
			return 1;
		}

		if (vehicleid == 0) {
			vehicleid = GetPlayerVehicleID(playerid);
		}

		if (!IsValidVehicle(vehicleid)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_REMOVE_ERROR));
			return 1;
		}

		// message
		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_REMOVE_MESSAGE), playername, playerid, vehicleid);
		SendMessageToNearVehiclePlayers(string, 40.0, vehicleid);

		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_REMOVE_MSG_SELF), vehicleid);
		SendClientMessage(playerid, -1, string);

		// destroy
		DestroyVehicle(vehicleid);
	} else if (strcmp(subcmd, "respawn", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		if (strcmp(subparams, "all", true) == 0) {
			for (new vehid = 1; vehid <= MAX_VEHICLES; vehid++) {
				SetVehicleToRespawn(vehid);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_RESPAWN_ALL_MSG), playername, playerid);
			SendClientMessageToAll(-1, string);
		} else {
			new vehicleid;

			if (sscanf(subparams, "I(0)", vehicleid)) {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_RESPAWN_ERROR));
				return 1;
			}

			if (vehicleid == 0) {
				vehicleid = GetPlayerVehicleID(playerid);
			}

			if (!IsValidVehicle(vehicleid)) {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_RESPAWN_ERROR));
				return 1;
			}

			// message
			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_RESPAWN_MESSAGE), playername, playerid, vehicleid);
			SendMessageToNearVehiclePlayers(string, 40.0, vehicleid);

			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_RESPAWN_MSG_SELF), vehicleid);
			SendClientMessage(playerid, -1, string);

			// respawn
			SetVehicleToRespawn(vehicleid);
		}
	} else if (strcmp(subcmd, "repair", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		if (strcmp(subparams, "all", true) == 0) {
			for (new vehid = 1; vehid <= MAX_VEHICLES; vehid++) {
				RepairVehicle(vehid);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_REPAIR_ALL_MSG), playername, playerid);
			SendClientMessageToAll(-1, string);
		} else {
			new vehicleid;

			if (sscanf(subparams, "I(0)", vehicleid)) {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_REPAIR_ERROR));
				return 1;
			}

			if (vehicleid == 0) {
				vehicleid = GetPlayerVehicleID(playerid);
			}

			if (!IsValidVehicle(vehicleid)) {
				SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_REPAIR_ERROR));
				return 1;
			}

			// message
			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_REPAIR_MESSAGE), playername, playerid, vehicleid);
			SendMessageToNearVehiclePlayers(string, 40.0, vehicleid);

			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_REPAIR_MSG_SELF), vehicleid);
			SendClientMessage(playerid, -1, string);

			// respawn
			RepairVehicle(vehicleid);
		}
	} else if (strcmp(subcmd, "info", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		new vehicleid;

		if (sscanf(subparams, "I(0)", vehicleid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_VEHICLE_INFO_ERROR));
			return 1;
		}

		if (vehicleid == 0) {
			vehicleid = GetPlayerVehicleID(playerid);
		}

		if (!IsValidVehicle(vehicleid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_VEHICLE_INFO_ERROR));
			return 1;
		}

		new
			Float:x,
			Float:y,
			Float:z,
			Float:angle,
			Float:health,
			model,
			name[MAX_NAME];

		GetVehiclePos(vehicleid, x, y, z);
		GetVehicleZAngle(vehicleid, angle);
		GetVehicleHealth(vehicleid, health);
		model = GetVehicleModel(vehicleid);
		GetVehicleModelName(vehicleid, name);

		// print
		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_INFO_MESSAGE_0), vehicleid, name, model);
		SendClientMessage(playerid, -1, string);

		format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_INFO_MESSAGE_1), health, x, y, z, angle);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "health", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
			SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_NOT_ALLOWED));
			return 1;
		}

		new
			action[5],
			target[5],
			Float:amount;

		if (sscanf(subparams, "s[5]S()[5]F(1000.0)", action, target, amount)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_VEHICLE_HEALTH_ERROR));
			return 1;
		}

		// get target
		new
			vehicleid = INVALID_VEHICLE_ID;

		if (isnull(target)) {
			vehicleid = GetPlayerVehicleID(playerid);
		} else if (strcmp(target, "all", true) == 0) {
			vehicleid = 0;
		} else if (IsNumeric(target)) {
			vehicleid = strval(target);
		}

		if (vehicleid != 0 && !IsValidVehicle(vehicleid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_VEHICLE_HEALTH_ERROR));
			return 1;
		}

		// get action
		if (strcmp(action, "set", true) == 0) {
			if (vehicleid == 0) {
				for (new vehid = 1; vehid <= MAX_VEHICLES; vehid++) {
					SetVehicleHealth(vehid, amount);
				}

				format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_HEALTH_SET_ALL), playername, playerid, amount);
				SendClientMessageToAll(-1, string);
			} else {
				SetVehicleHealth(vehicleid, amount);

				format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_HEALTH_SET), playername, playerid, vehicleid, amount);
				SendMessageToNearVehiclePlayers(string, 40.0, vehicleid);

				format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_HEALTH_SET_SELF), vehicleid, amount);
				SendClientMessage(playerid, -1, string);
			}
		} else if (strcmp(action, "get", true) == 0) {
			GetVehicleHealth(vehicleid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_HEALTH_GET), vehicleid, amount);
			SendClientMessage(playerid, -1, string);
		} else if (strcmp(action, "give", true) == 0) {
			new
				Float:current_health;

			if (vehicleid == 0) {
				for (new vehid = 1; vehid <= MAX_VEHICLES; vehid++) {
					GetVehicleHealth(vehid, current_health);
					SetVehicleHealth(vehid, current_health + amount);
				}

				format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_HEALTH_GIVE_ALL), playername, playerid, amount);
				SendClientMessageToAll(-1, string);
			} else {
				GetVehicleHealth(vehicleid, current_health);
				SetVehicleHealth(vehicleid, current_health + amount);

				format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_HEALTH_GIVE), playername, playerid, vehicleid, amount);
				SendMessageToNearVehiclePlayers(string, 40.0, vehicleid);

				format(string, sizeof(string), _(ADMIN_COMMAND_VEHICLE_HEALTH_GIVE_SELF), vehicleid, amount);
				SendClientMessage(playerid, -1, string);
			}
		} else {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_VEHICLE_HEALTH_ERROR));
		}
	} else {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_VEHICLE_HELP));
	}

	return 1;
}
