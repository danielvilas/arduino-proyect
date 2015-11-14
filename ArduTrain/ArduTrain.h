#ifndef ArduTrain_h
#define ArduTrain_h

typedef enum tArduTrainCommands {
    atcmd_GenError=0,
    atcmd_Connect=1,
    atcmd_Disconnect=2,
    atcmd_Echo=3,
    atcdm_SetChannel=4,
} ArduTrainCommands;

typedef enum tArduTrainStatus {
    atst_NoError=0,
    atst_Crc=1,
    atst_Pad=2,
    atst_Op=3
} ArduTrainStatus;


#endif

