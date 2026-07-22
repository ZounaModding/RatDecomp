#include "UserCommand_G.h"

#include "Console_Z.h"
#include "LoadingDraw_G.h"
#include "Program_Z.h"
#include "ScriptManager_G.h"

void RegisterUserCommand() {
    REGISTERCOMMAND("SetLoadingDraw", SetLoadingDraw);
}

Bool SetLoadingDraw() {
    if (strcmp(gData.Cons->GetStrParam(1), "OFF") == 0) {
        gScriptMgr->GetLoadingDraw()->StopAnimLoading(FALSE);
    }
    else {
        gScriptMgr->GetLoadingDraw()->StopAnimLoading(TRUE);
    }
    return TRUE;
}
