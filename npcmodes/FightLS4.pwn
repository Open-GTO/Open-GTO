#define RECORDING "FightLS4"
#define RECORDING_TYPE 2
#include <a_npc>

main(){}

public OnRecordingPlaybackEnd() StartRecordingPlayback(RECORDING_TYPE, RECORDING);
public OnNPCSpawn() StartRecordingPlayback(RECORDING_TYPE, RECORDING);
public OnNPCModeExit() StopRecordingPlayback();

