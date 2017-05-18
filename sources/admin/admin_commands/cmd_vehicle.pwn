/*

	About: vehicle admin command
	Author: ziggi

*/

#if defined _admin_cmd_vehicle_included
	#endinput
#endif

#define _admin_cmd_vehicle_included

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
		Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_HELP");
		return 1;
	}

	new
		players[MAX_PLAYERS],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (strcmp(subcmd, "add", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_NOT_ALLOWED");
			return 1;
		}

		new modelid;

		if (sscanf(subparams, "k<vehicle>", modelid) || modelid == -1) {
			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_ADD_ERROR");
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
		SetVehicleNumberPlate(vehicleid, MODE_NAME);

		// put in
		PutPlayerInVehicle(playerid, vehicleid, 0);

		// message
		GetNearPlayers(x, y, z, 40.0, players);
		Lang_SendTextToPlayers(players, "ADMIN_COMMAND_VEHICLE_ADD_MESSAGE", playername, playerid, vehicleid);
	} else if (strcmp(subcmd, "remove", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_NOT_ALLOWED");
			return 1;
		}

		new vehicleid;

		if (sscanf(subparams, "I(0)", vehicleid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_REMOVE_ERROR");
			return 1;
		}

		if (vehicleid == 0) {
			vehicleid = GetPlayerVehicleID(playerid);
		}

		if (!IsValidVehicle(vehicleid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_REMOVE_ERROR");
			return 1;
		}

		// message
		GetVehicleNearPlayers(vehicleid, 40.0, players, .exclude_playerid = playerid);
		Lang_SendTextToPlayers(players, "ADMIN_COMMAND_VEHICLE_REMOVE_MESSAGE", playername, playerid, vehicleid);

		Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_REMOVE_MSG_SELF", vehicleid);

		// destroy
		DestroyVehicle(vehicleid);
	} else if (strcmp(subcmd, "respawn", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_NOT_ALLOWED");
			return 1;
		}

		if (strcmp(subparams, "all", true) == 0) {
			foreach (new vehid : Vehicle) {
				SetVehicleToRespawn(vehid);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_VEHICLE_RESPAWN_ALL_MSG", playername, playerid);
		} else {
			new vehicleid;

			if (sscanf(subparams, "I(0)", vehicleid)) {
				Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_RESPAWN_ERROR");
				return 1;
			}

			if (vehicleid == 0) {
				vehicleid = GetPlayerVehicleID(playerid);
			}

			if (!IsValidVehicle(vehicleid)) {
				Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_RESPAWN_ERROR");
				return 1;
			}

			// message
			GetVehicleNearPlayers(vehicleid, 40.0, players, .exclude_playerid = playerid);
			Lang_SendTextToPlayers(players, "ADMIN_COMMAND_VEHICLE_RESPAWN_MESSAGE", playername, playerid, vehicleid);

			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_RESPAWN_MSG_SELF", vehicleid);

			// respawn
			SetVehicleToRespawn(vehicleid);
		}
	} else if (strcmp(subcmd, "repair", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_NOT_ALLOWED");
			return 1;
		}

		if (strcmp(subparams, "all", true) == 0) {
			foreach (new vehid : Vehicle) {
				RepairVehicle(vehid);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_VEHICLE_REPAIR_ALL_MSG", playername, playerid);
		} else {
			new vehicleid;

			if (sscanf(subparams, "I(0)", vehicleid)) {
				Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_REPAIR_ERROR");
				return 1;
			}

			if (vehicleid == 0) {
				vehicleid = GetPlayerVehicleID(playerid);
			}

			if (!IsValidVehicle(vehicleid)) {
				Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_REPAIR_ERROR");
				return 1;
			}

			// message
			GetVehicleNearPlayers(vehicleid, 40.0, players, .exclude_playerid = playerid);
			Lang_SendTextToPlayers(players, "ADMIN_COMMAND_VEHICLE_REPAIR_MESSAGE", playername, playerid, vehicleid);

			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_REPAIR_MSG_SELF", vehicleid);

			// respawn
			RepairVehicle(vehicleid);
		}
	} else if (strcmp(subcmd, "info", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_NOT_ALLOWED");
			return 1;
		}

		new vehicleid;

		if (sscanf(subparams, "I(0)", vehicleid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_INFO_ERROR");
			return 1;
		}

		if (vehicleid == 0) {
			vehicleid = GetPlayerVehicleID(playerid);
		}

		if (!IsValidVehicle(vehicleid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_INFO_ERROR");
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
		Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_INFO_MESSAGE_0", vehicleid, name, model);
		Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_INFO_MESSAGE_1", health, x, y, z, angle);
	} else if (strcmp(subcmd, "health", true) == 0) {
		if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_NOT_ALLOWED");
			return 1;
		}

		new
			action[5],
			target[5],
			Float:amount;

		if (sscanf(subparams, "s[5]S()[5]F(1000.0)", action, target, amount)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_HEALTH_ERROR");
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
			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_HEALTH_ERROR");
			return 1;
		}

		// get action
		if (strcmp(action, "set", true) == 0) {
			if (vehicleid == 0) {
				foreach (new vehid : Vehicle) {
					SetVehicleHealth(vehid, amount);
				}

				Lang_SendTextToAll("ADMIN_COMMAND_VEHICLE_HEALTH_SET_ALL", playername, playerid, amount);
			} else {
				SetVehicleHealth(vehicleid, amount);

				GetVehicleNearPlayers(vehicleid, 40.0, players, .exclude_playerid = playerid);
				Lang_SendTextToPlayers(players, "ADMIN_COMMAND_VEHICLE_HEALTH_SET", playername, playerid, vehicleid);

				Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_HEALTH_SET_SELF", vehicleid, amount);
			}
		} else if (strcmp(action, "get", true) == 0) {
			GetVehicleHealth(vehicleid, amount);

			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_HEALTH_GET", vehicleid, amount);
		} else if (strcmp(action, "give", true) == 0) {
			new
				Float:current_health;

			if (vehicleid == 0) {
				foreach (new vehid : Vehicle) {
					GetVehicleHealth(vehid, current_health);
					SetVehicleHealth(vehid, current_health + amount);
				}

				Lang_SendTextToAll("ADMIN_COMMAND_VEHICLE_HEALTH_GIVE_ALL", playername, playerid, amount);
			} else {
				GetVehicleHealth(vehicleid, current_health);
				SetVehicleHealth(vehicleid, current_health + amount);

				GetVehicleNearPlayers(vehicleid, 40.0, players, .exclude_playerid = playerid);
				Lang_SendTextToPlayers(players, "ADMIN_COMMAND_VEHICLE_HEALTH_GIVE", playername, playerid, vehicleid, amount);

				Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_HEALTH_GIVE_SELF", vehicleid, amount);
			}
		} else {
			Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_HEALTH_ERROR");
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_VEHICLE_HELP");
	}

	return 1;
}
