#include "Game_Z.h"
#include "Console_Z.h"

void RegisterGameCommand() {
    REGISTERCOMMANDC("LoadSubLevel", Cmd_LoadSubLevel, " WorldName SubWorldName SubLevelId");
    REGISTERCOMMANDC("LoadSubData", Cmd_LoadSubData, " WorldName SubWorldName SubDataId");
    REGISTERCOMMANDC("RemoveSubData", Cmd_RemoveSubData, " WorldName SubDataId [CheckHandles=0=FALSE]");
    REGISTERCOMMANDC("RemoveSubLevel", Cmd_RemoveSubLevel, " WorldName SubLevelId");
}

Bool Cmd_LoadSubLevel() {
    return TRUE;
}

Bool Cmd_RemoveSubLevel() {
    return TRUE;
}
