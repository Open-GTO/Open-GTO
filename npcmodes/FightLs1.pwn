#define RECORDING "FightLS1"
#define RECORDING_TYPE 2 
#include <a_npc>

main(){}

public OnNPCSpawn() StartRecordingPlayback(RECORDING_TYPE, RECORDING);
public OnNPCModeExit() StopRecordingPlayback();

