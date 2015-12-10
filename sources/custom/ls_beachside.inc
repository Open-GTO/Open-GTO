/*
	Porting for Open-GTO by ziggi
*/
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Example Filterscript for the new LS BeachSide Building with Elevator
// --------------------------------------------------------------------
// Original elevator code by Zamaroht in 2010
//
// Updated by Kye in 2011
// * Added a sound effect for the elevator starting/stopping
//
// Edited by Matite in January 2015
// * Adapted the elevator code so it works in this new building and removed the
//   light pole at the underground car park entrance
//
// Updated to v1.03 by Matite in April 2015
// * Removed the code that removes the existing building map object and the lines
//   that create the new objects as the original building is now replaced with
//   the new one by SAMP instead (same as the LS Office building)
//
// Updated to v1.02 by Matite in February 2015
// * Added code for the new car park object and edited the elevator to
//   include the car park
//
// This script creates the new LS BeachSide building object, removes the
// existing GTASA building object, adds the new car park object and creates
// an elevator that can be used to travel between all levels.
//
// You can un-comment the OnPlayerCommandText callback below to enable a simple
// teleport command (/lsb) that teleports you to the LS BeachSide building.
//
// Warning...
// This script uses a total of:
// * 31 objects = 1 for the elevator, 2 for the elevator doors and 28 for the
//   elevator floor doors
// * 15 3D Text Labels = 14 on the floors and 1 in the elevator
// * 1 dialog (for the elevator - dialog ID 877)
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Defines
// -------

// Movement speed of the elevator
#define ELEVATOR_SPEED      (5.0)

// Movement speed of the doors
#define DOORS_SPEED         (5.0)

// Time in ms that the elevator will wait in each floor before continuing with the queue...
// be sure to give enough time for doors to open
#define ELEVATOR_WAIT_TIME  (5000)  

// Position defines
#define X_DOOR_R_OPENED     (289.542419)
#define X_DOOR_L_OPENED     (286.342407)
#define Y_DOOR_R_OPENED     (-1609.640991)
#define Y_DOOR_L_OPENED     (-1609.076049)

#define X_FDOOR_R_OPENED    (289.492431)
#define X_FDOOR_L_OPENED    (286.292419)
#define Y_FDOOR_R_OPENED    (-1609.870971)
#define Y_FDOOR_L_OPENED    (-1609.306030)

#define GROUND_Z_COORD      (18.755348)     // (33.825077)
#define X_ELEVATOR_POS      (287.942413)
#define Y_ELEVATOR_POS      (-1609.341064)

// Elevator state defines
#define ELEVATOR_STATE_IDLE     (0)
#define ELEVATOR_STATE_WAITING  (1)
#define ELEVATOR_STATE_MOVING   (2)

// Invalid floor define
#define INVALID_FLOOR           (-1)

// Used for chat text messages
#define COLOR_MESSAGE_YELLOW        0xFFDD00AA

#define ALERT_TIME 4
// -----------------------------------------------------------------------------
// Constants
// ---------

// Elevator floor names for the 3D text labels
static FloorNames[14][] =
{
	"Подземная парковка",
	"Цокольный этаж",
	"Первый этаж",
	"Второй этаж",
	"Третий этаж",
	"Четвёртый этаж",
	"Пятый этаж",
	"Шестой этаж",
	"Седьмой этаж",
	"Восьмой этаж",
	"Девятый этаж",
	"Десятый этаж",
	"Одиннадцатый этаж",
	"Двенадцатый этаж"
};

// Elevator floor Z heights
static Float:FloorZOffsets[14] =
{
    0.0, 		// Car Park
    15.069729,  // Ground Floor
    29.130733,	// First Floor
    33.630733,  // Second Floor = 29.130733 + 4.5
    38.130733,  // Third Floor = 33.630733 + 4.5
    42.630733,  // Fourth Floor = 38.130733 + 4.5
    47.130733,  // Fifth Floor = 42.630733 + 4.5
    51.630733,  // Sixth Floor = 47.130733 + 4.5
    56.130733,  // Seventh Floor = 51.630733 + 4.5
    60.630733,  // Eighth Floor = 56.130733 + 4.5
    65.130733,  // Ninth Floor = 60.630733 + 4.5
    69.630733,  // Tenth Floor = 65.130733 + 4.5
    74.130733,  // Eleventh Floor = 69.630733 + 4.5
    78.630733,  // Twelfth Floor = 74.130733 + 4.5
};

// -----------------------------------------------------------------------------
// Variables
// ---------

// Stores the created object numbers of the elevator, the elevator doors and
// the elevator floor doors so they can be destroyed when the filterscript
// is unloaded
new Obj_Elevator, Obj_ElevatorDoors[2], Obj_FloorDoors[14][2];
	
// Stores a reference to the 3D text labels used on each floor and inside the
// elevator itself so they can be detroyed when the filterscript is unloaded
new Text3D:Label_Elevator, Text3D:Label_Floors[14];

// Stores the current state of the elevator (ie ELEVATOR_STATE_IDLE,
// ELEVATOR_STATE_WAITING or ELEVATOR_STATE_MOVING)
new ElevatorState;

// Stores the current floor the elevator is on or heading to... if the value is
// ELEVATOR_STATE_IDLE or ELEVATOR_STATE_WAITING this is the current floor. If
// the value is ELEVATOR_STATE_MOVING then it is the floor it's moving to
new	ElevatorFloor;  

// Stores the elevator queue for each floor
new ElevatorQueue[14];

// Stores who requested the floor for the elevator queue...
// FloorRequestedBy[floor_id] = playerid;  (stores who requested which floor)
new	FloorRequestedBy[14];

// Used for a timer that makes the elevator move faster after players start
// surfing the object
new ElevatorBoostTimer;

// -----------------------------------------------------------------------------
// Function Forwards
// -----------------

// Public:
forward CallElevator(playerid, floorid);    // You can use INVALID_PLAYER_ID too.

// Private:
forward Elevator_Boost(floorid);        	// Increases the elevator speed until it reaches 'floorid'.
forward Elevator_TurnToIdle();

forward Beachside_DelayOpen();

forward Float:GetElevatorZCoordForFloor(floorid);
forward Float:GetDoorsZCoordForFloor(floorid);

// -----------------------------------------------------------------------------
// Callbacks
// ---------

Beachside_OnGameModeInit()
{
	// Reset the elevator queue
	ResetElevatorQueue();

	// Create the elevator object, the elevator doors and the floor doors
	Elevator_Initialize();

	// Exit here
	Log_Game("SERVER: LS BeachSide Building Elevator init");
	return 1;
}

Beachside_OnPlayerConnect(playerid)
{
	// Remove the lamp post at the underground car park entrance
	RemoveBuildingForPlayer(playerid, 1226, 265.481, -1581.1, 32.9311, 5.0);
	
	// Remove the night lights object (must be removed to also remove any
	// occulsion zones inside the building)
	RemoveBuildingForPlayer(playerid, 6518, 280.297, -1606.2, 72.3984, 250.0);

	// Exit here (return 1 so this callback is processed in other scripts)
	return 1;
}

Beachside_OnObjectMoved(objectid)
{
	// Create variables
	new Float:x, Float:y, Float:z;
	
	// Loop
	for(new i; i < sizeof(Obj_FloorDoors); i ++)
	{
		// Check if the object that moved was one of the elevator floor doors
		if(objectid == Obj_FloorDoors[i][0])
		{
			GetObjectPos(Obj_FloorDoors[i][0], x, y, z);

			// Some floor doors have shut, move the elevator to next floor in queue:
			if (y < Y_DOOR_L_OPENED - 0.5)
			{
				Elevator_MoveToFloor(ElevatorQueue[0]);
				RemoveFirstQueueFloor();
			}
		}
	}

	if(objectid == Obj_Elevator)   // The elevator reached the specified floor.
	{
		KillTimer(ElevatorBoostTimer);  // Kills the timer, in case the elevator reached the floor before boost.

		FloorRequestedBy[ElevatorFloor] = INVALID_PLAYER_ID;

		Elevator_OpenDoors();
		Floor_OpenDoors(ElevatorFloor);

		GetObjectPos(Obj_Elevator, x, y, z);
		Label_Elevator	= CreateDynamic3DTextLabel("{CCCCCC}Нажмите '{FFFFFF}"KEY_NAME"{CCCCCC}' для использования лифта", 0xCCCCCCAA, X_ELEVATOR_POS + 1.6, Y_ELEVATOR_POS - 1.85, z - 0.4, 4.0, .testlos = 1);

		ElevatorState 	= ELEVATOR_STATE_WAITING;
		SetTimer("Elevator_TurnToIdle", ELEVATOR_WAIT_TIME, 0);
	}

	return 1;
}

Beachside_OnPlayerKeySC(playerid, newkeys, oldkeys)
{
	#pragma unused newkeys, oldkeys
	// Check if the player is not in a vehicle and pressed the conversation yes key (Y by default)
	if (IsPlayerInAnyVehicle(playerid)) {
		return 0;
	}

	// Create variables and get the players current position
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	// Check if the player is using the button inside the elevator
	if (pos[1] > (Y_ELEVATOR_POS - 1.8) && pos[1] < (Y_ELEVATOR_POS + 1.8) && pos[0] < (X_ELEVATOR_POS + 1.8) && pos[0] > (X_ELEVATOR_POS - 1.8))
	{
		// The player is using the button inside the elevator
		// --------------------------------------------------
		
		// Show the elevator dialog to the player
		Dialog_Show(playerid, Dialog:ElevatorMenu);
		return 1;
	}
	else
	{
		// Check if the player is using the button on one of the floors
		if(pos[1] < (Y_ELEVATOR_POS - 1.81) && pos[1] > (Y_ELEVATOR_POS - 3.8) && pos[0] > (X_ELEVATOR_POS + 1.21) && pos[0] < (X_ELEVATOR_POS + 3.8))
		{
			// The player is most likely using an elevator floor button... check which floor
			// -----------------------------------------------------------------------------
			
			// Create variable with the number of floors to check (total floors minus 1)
			new i = 13;

			// Loop
			while(pos[2] < GetDoorsZCoordForFloor(i) + 3.5 && i > 0)
				i --;

			if(i == 0 && pos[2] < GetDoorsZCoordForFloor(0) + 2.0)
				i = -1;

			if (i <= 12)
			{
				// Check if the elevator is not moving (idle or waiting)
				if (ElevatorState != ELEVATOR_STATE_MOVING)
				{
					// Check if the elevator is already on the floor it was called from
					if (ElevatorFloor == i + 1)
					{
						// Display gametext message to the player
						if (gettime() >= GetPVarInt(playerid, "ls_beachside_call_time")) {
							SetPVarInt(playerid, "ls_beachside_call_time", gettime() + ALERT_TIME);
							Message_Alert(playerid, "Elevator", "LS BeachSide Elevator is already on this floor.~n~Walk inside and press '"KEY_NAME"'", ALERT_TIME * 1000);
						}

						// Exit here (return 1 so this callback is processed in other scripts)
						return 1;
					}
				}

				// Call function to call the elevator to the floor
				CallElevator(playerid, i + 1);

				// Display gametext message to the player
				if (gettime() >= GetPVarInt(playerid, "ls_beachside_call_time")) {
					SetPVarInt(playerid, "ls_beachside_call_time", gettime() + ALERT_TIME);
					Message_Alert(playerid, "Elevator", "LS BeachSide Elevator has been called.~n~Please wait", ALERT_TIME * 1000);
				

					// Create variable for formatted message
					new strTempString[100];
					
					// Check if the elevator is moving
					if (ElevatorState == ELEVATOR_STATE_MOVING)
					{
						format(strTempString, sizeof(strTempString), "* Лифт The LS BeachSide был вызван... В настоящее время он движется к '%s'.", FloorNames[ElevatorFloor]);
					}
					else
					{
						format(strTempString, sizeof(strTempString), "* Лифт The LS BeachSide был вызван... Сейчас он на '%s'.", FloorNames[ElevatorFloor]);
					}
					
					// Display formatted chat text message to the player
					SendClientMessage(playerid, COLOR_MESSAGE_YELLOW, strTempString);
				}
				// Exit here (return 1 so this callback is processed in other scripts)
				return 1;
			}
		}
	}
	
	return 0;
}

DialogCreate:ElevatorMenu(playerid)
{
	new string[16 * sizeof(ElevatorQueue)];
	for (new i; i < sizeof(ElevatorQueue); i ++)
	{
		if (FloorRequestedBy[i] != INVALID_PLAYER_ID)
			strcat(string, "{FF0000}");

		strcat(string, FloorNames[i]);
		strcat(string, "\n");
	}
	Dialog_Open(playerid, Dialog:ElevatorMenu, DIALOG_STYLE_LIST, "LS BeachSide Elevator", string, "Принять", "Отмена");
}

DialogResponse:ElevatorMenu(playerid, response, listitem, inputtext[])
{
	if(!response)
		return 0;

	if(FloorRequestedBy[listitem] != INVALID_PLAYER_ID || IsFloorInQueue(listitem))
		Message_Alert(playerid, "Elevator", "The floor is already in the queue");
	else if(DidPlayerRequestElevator(playerid))
		Message_Alert(playerid, "Elevator", "The floor is already in the queue");
	else
		CallElevator(playerid, listitem);

	return 1;
}

// ------------------------ Functions ------------------------
stock Elevator_Initialize()
{
	// Create the elevator and elevator door objects
	Obj_Elevator 			= CreateDynamicObject(18755, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD, 0.000000, 0.000000, 80.000000);
	Obj_ElevatorDoors[0] 	= CreateDynamicObject(18757, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD, 0.000000, 0.000000, 80.000000);
	Obj_ElevatorDoors[1] 	= CreateDynamicObject(18756, X_ELEVATOR_POS, Y_ELEVATOR_POS, GROUND_Z_COORD, 0.000000, 0.000000, 80.000000);

	// Create the 3D text label for inside the elevator
	Label_Elevator = CreateDynamic3DTextLabel("{CCCCCC}Нажмите '{FFFFFF}"KEY_NAME"{CCCCCC}' для использования лифта", 0xCCCCCCAA, X_ELEVATOR_POS + 1.6, Y_ELEVATOR_POS - 1.85, GROUND_Z_COORD - 0.4, 4.0, .testlos = 1);

	// Create variables
	new string[128], Float:z;

	// Loop
	for (new i; i < sizeof(Obj_FloorDoors); i ++)
	{
		// Create elevator floor door objects
		Obj_FloorDoors[i][0] 	= CreateDynamicObject(18757, X_ELEVATOR_POS, Y_ELEVATOR_POS - 0.245, GetDoorsZCoordForFloor(i) + 0.05, 0.000000, 0.000000, 80.000000);
		Obj_FloorDoors[i][1] 	= CreateDynamicObject(18756, X_ELEVATOR_POS, Y_ELEVATOR_POS - 0.245, GetDoorsZCoordForFloor(i) + 0.05, 0.000000, 0.000000, 80.000000);

		// Format string for the floor 3D text label
		format(string, sizeof(string), "{CCCCCC}[%s]\n{CCCCCC}Нажмите '{FFFFFF}"KEY_NAME"{CCCCCC}' для вызова", FloorNames[i]);

		// Get label Z position
		z = GetDoorsZCoordForFloor(i);

		// Create floor label
		Label_Floors[i] = CreateDynamic3DTextLabel(string, 0xCCCCCCAA, X_ELEVATOR_POS + 2, Y_ELEVATOR_POS -3, z - 0.2, 10.5, .testlos = 1);
	}

	// Open the car park floor doors and the elevator doors
	// in SetTimer, because MoveObject does not work in OnGameModeInit
	SetTimer("Beachside_DelayOpen", 500, 0);

	// Exit here
	return 1;
}

public Beachside_DelayOpen()
{
	Elevator_OpenDoors();
	Floor_OpenDoors(0);
}

stock Elevator_OpenDoors()
{
	// Opens the elevator's doors.

	new Float:x, Float:y, Float:z;

	GetObjectPos(Obj_ElevatorDoors[0], x, y, z);
	MoveObject(Obj_ElevatorDoors[0], X_DOOR_L_OPENED, Y_DOOR_L_OPENED, z, DOORS_SPEED);
	MoveObject(Obj_ElevatorDoors[1], X_DOOR_R_OPENED, Y_DOOR_R_OPENED, z, DOORS_SPEED);

	return 1;
}

stock Elevator_CloseDoors()
{
	// Closes the elevator's doors.

	if(ElevatorState == ELEVATOR_STATE_MOVING)
		return 0;

	new Float:x, Float:y, Float:z;

	GetObjectPos(Obj_ElevatorDoors[0], x, y, z);
	MoveObject(Obj_ElevatorDoors[0], X_ELEVATOR_POS, Y_ELEVATOR_POS, z, DOORS_SPEED);
	MoveObject(Obj_ElevatorDoors[1], X_ELEVATOR_POS, Y_ELEVATOR_POS, z, DOORS_SPEED);

	return 1;
}

stock Floor_OpenDoors(floorid)
{
	// Opens the doors at the specified floor.

	MoveObject(Obj_FloorDoors[floorid][0], X_FDOOR_L_OPENED, Y_FDOOR_L_OPENED, GetDoorsZCoordForFloor(floorid) + 0.05, DOORS_SPEED);
	MoveObject(Obj_FloorDoors[floorid][1], X_FDOOR_R_OPENED, Y_FDOOR_R_OPENED, GetDoorsZCoordForFloor(floorid) + 0.05, DOORS_SPEED);
	
	PlaySoundForPlayersInRange(6401, 50.0, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid) + 5.0);

	return 1;
}

stock Floor_CloseDoors(floorid)
{
	// Closes the doors at the specified floor.

	MoveObject(Obj_FloorDoors[floorid][0], X_ELEVATOR_POS, Y_ELEVATOR_POS - 0.245, GetDoorsZCoordForFloor(floorid) + 0.05, DOORS_SPEED);
	MoveObject(Obj_FloorDoors[floorid][1], X_ELEVATOR_POS, Y_ELEVATOR_POS - 0.245, GetDoorsZCoordForFloor(floorid) + 0.05, DOORS_SPEED);
	
	PlaySoundForPlayersInRange(6401, 50.0, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid) + 5.0);

	return 1;
}

stock Elevator_MoveToFloor(floorid)
{
	// Moves the elevator to specified floor (doors are meant to be already closed).

	ElevatorState = ELEVATOR_STATE_MOVING;
	ElevatorFloor = floorid;

	// Move the elevator slowly, to give time to clients to sync the object surfing. Then, boost it up:
	MoveObject(Obj_Elevator, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetElevatorZCoordForFloor(floorid), 0.25);
	MoveObject(Obj_ElevatorDoors[0], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), 0.25);
	MoveObject(Obj_ElevatorDoors[1], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), 0.25);
	DestroyDynamic3DTextLabel(Label_Elevator);

	ElevatorBoostTimer = SetTimerEx("Elevator_Boost", 2000, 0, "i", floorid);

	return 1;
}

public Elevator_Boost(floorid)
{
	// Increases the elevator's speed until it reaches 'floorid'
	StopObject(Obj_Elevator);
	StopObject(Obj_ElevatorDoors[0]);
	StopObject(Obj_ElevatorDoors[1]);
	
	MoveObject(Obj_Elevator, X_ELEVATOR_POS, Y_ELEVATOR_POS, GetElevatorZCoordForFloor(floorid), ELEVATOR_SPEED);
	MoveObject(Obj_ElevatorDoors[0], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), ELEVATOR_SPEED);
	MoveObject(Obj_ElevatorDoors[1], X_ELEVATOR_POS, Y_ELEVATOR_POS, GetDoorsZCoordForFloor(floorid), ELEVATOR_SPEED);

	return 1;
}

public Elevator_TurnToIdle()
{
	ElevatorState = ELEVATOR_STATE_IDLE;
	ReadNextFloorInQueue();

	return 1;
}

stock RemoveFirstQueueFloor()
{
	// Removes the data in ElevatorQueue[0], and reorders the queue accordingly.

	for(new i; i < sizeof(ElevatorQueue) - 1; i ++)
		ElevatorQueue[i] = ElevatorQueue[i + 1];

	ElevatorQueue[sizeof(ElevatorQueue) - 1] = INVALID_FLOOR;

	return 1;
}

stock AddFloorToQueue(floorid)
{
 	// Adds 'floorid' at the end of the queue.

	// Scan for the first empty space:
	new slot = -1;
	for(new i; i < sizeof(ElevatorQueue); i ++)
	{
		if(ElevatorQueue[i] == INVALID_FLOOR)
		{
			slot = i;
			break;
		}
	}

	if(slot != -1)
	{
		ElevatorQueue[slot] = floorid;

	 	// If needed, move the elevator.
		if(ElevatorState == ELEVATOR_STATE_IDLE)
			ReadNextFloorInQueue();

		return 1;
	}

	return 0;
}

stock ResetElevatorQueue()
{
	// Resets the queue.

	for(new i; i < sizeof(ElevatorQueue); i ++)
	{
		ElevatorQueue[i] 	= INVALID_FLOOR;
		FloorRequestedBy[i] = INVALID_PLAYER_ID;
	}

	return 1;
}

stock IsFloorInQueue(floorid)
{
	// Checks if the specified floor is currently part of the queue.

	for(new i; i < sizeof(ElevatorQueue); i ++)
		if(ElevatorQueue[i] == floorid)
			return 1;

	return 0;
}

stock ReadNextFloorInQueue()
{
	// Reads the next floor in the queue, closes doors, and goes to it.

	if(ElevatorState != ELEVATOR_STATE_IDLE || ElevatorQueue[0] == INVALID_FLOOR)
		return 0;

	Elevator_CloseDoors();
	Floor_CloseDoors(ElevatorFloor);

	return 1;
}

stock DidPlayerRequestElevator(playerid)
{
	for(new i; i < sizeof(FloorRequestedBy); i ++)
		if(FloorRequestedBy[i] == playerid)
			return 1;

	return 0;
}

stock CallElevator(playerid, floorid)
{
	// Calls the elevator (also used with the elevator dialog).

	if(FloorRequestedBy[floorid] != INVALID_PLAYER_ID || IsFloorInQueue(floorid))
		return 0;

	FloorRequestedBy[floorid] = playerid;
	AddFloorToQueue(floorid);

	return 1;
}

stock Float:GetElevatorZCoordForFloor(floorid)
{
	// Return Z height value
	return (GROUND_Z_COORD + FloorZOffsets[floorid]);
}

stock Float:GetDoorsZCoordForFloor(floorid)
{
	// Return Z height value
	return (GROUND_Z_COORD + FloorZOffsets[floorid]);
}

