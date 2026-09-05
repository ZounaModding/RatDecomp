#include "DebugTools_Z.h"
#include "Program_Z.h"
#include "Renderer_Z.h"
#include "Console_Z.h"
#include "Timer_Z.h"
#include <String_Z.h>

Float PreviousTime;

void RegisterDebugCommand() {
    REGISTERCOMMAND("BoxPatchClip", BoxPatchClip);
    REGISTERCOMMAND("PosPersoDebug", SetPosPerso);
    REGISTERCOMMAND("ScreenShot", ScreenShot);
    REGISTERCOMMANDC("SetGammaRamp", SetGammaRamp, "No param restore default");
    REGISTERCOMMANDC("SetBrightness", SetBrightness, "No param restore default");
    REGISTERCOMMANDC("SetContrast", SetContrast, "No param restore default");
    REGISTERCOMMANDC("SetGAmma", SetGAmma, "No param restore default");
    REGISTERCOMMAND("SwapStartButton", SwapStartButton);
    REGISTERCOMMAND("CleanTempPic", CleanTempPic);
    REGISTERCOMMAND("EnableProfiler", EnableProfiler);
    REGISTERCOMMAND("EnableDmaProfiler", EnableDmaProfiler);
    REGISTERCOMMAND("EnableDebugMOuse", EnableDebugMouse);
    REGISTERCOMMAND("DisableMouseForCamera", DisableMouseForCamera);
    REGISTERCOMMAND("SwitchScreenMode", SwitchScreenMode);
    REGISTERCOMMAND("DisplayFPS", DisplayFps);
    REGISTERCOMMAND("DisplayFrameBar", DisplayFrame);
    REGISTERCOMMAND("DisableConsole", DisableConsole);
    REGISTERCOMMAND("DisableIngameConsole", DisableIngameConsole);
    REGISTERCOMMAND("DisableAssert", DisableAssert);
    REGISTERCOMMAND("DisableMUsic", DisableMusic);
    REGISTERCOMMAND("DisableMOvie", DisableMovie);
    REGISTERCOMMAND("DisplaySTream", DisplayStream);
    REGISTERCOMMAND("EnablePopupMenu", EnablePopupMenu);
    REGISTERCOMMAND("EnableL2R2", EnableL2R2);
    REGISTERCOMMAND("EnableVSYnc", EnableVSync);
    REGISTERCOMMAND("CheckMemoryEveryFrame", CheckMemoryEveryFrame);
    REGISTERCOMMAND("SkipMOvie", SkipMovie);
    REGISTERCOMMAND("NbHandle", NbHandle);
    REGISTERCOMMAND("DoSkelDynamic", DoSkelDynamic);
    REGISTERCOMMAND("DrawMEMInfos", DrawMemInfos);
    REGISTERCOMMAND("DeltaTime", DeltaTime);
    REGISTERCOMMAND("EnableBloomEffect", EnableBloomEffect);
    REGISTERCOMMAND("DisplayMemStatus", DisplayMemStatus);
    REGISTERCOMMAND("DrawMemGraph", DrawMemGraph);
}

Bool DisplayMemStatus() {
    return FALSE;
}

Bool DrawMemGraph() {
    return TRUE;
}

Bool DeltaTime() {
    PreviousTime = GetAbsoluteTime();
    return TRUE;
}

Bool ScreenShot() {
    return FALSE;
}

Bool SwitchScreenMode() {
    return FALSE;
}

Bool SetGammaRamp() {
    return FALSE;
}

Bool SetBrightness() {
    return FALSE;
}

Bool SetContrast() {
    return FALSE;
}

Bool SetGAmma() {
    return FALSE;
}

Bool SwapStartButton() {
    return FALSE;
}

Bool BoxPatchClip() {
    return FALSE;
}

Bool SetPosPerso() {
    return FALSE;
}

Bool EnablePopupMenu() {
    if (gData.Cons->GetNbParam() > 2) {
        return FALSE;
    }

    if (gData.Cons->GetNbParam() < 2) {
        gData.ToggleEngineFlag(FL_POPUP_MENU);
    }

    if (gData.Cons->IsParamFloat(1)) {
        if (gData.Cons->GetParamFloat(1) == 0.0f) {
            gData.DisableEngineFlag(FL_POPUP_MENU);
        }
        else {
            gData.EnableEngineFlag(FL_POPUP_MENU);
        }
    }
    return TRUE;
}

Bool EnableProfiler() {
    return FALSE;
}

Bool EnableDmaProfiler() {
    return FALSE;
}

Bool EnableDebugMouse() {
    return FALSE;
}

Bool DisableMouseForCamera() {
    return FALSE;
}

Bool CleanTempPic() {
    return FALSE;
}

Bool DisplayFps() {
    if (gData.Cons->GetNbParam() != 2) {
        gData.ToggleEngineFlag(FL_DISPLAY_FPS);
        return TRUE;
    }
    if (gData.Cons->IsParamFloat(1)) {
        if (gData.Cons->GetParamFloat(1) == 0.0f) {
            gData.DisableEngineFlag(FL_DISPLAY_FPS);
        }
        else {
            gData.EnableEngineFlag(FL_DISPLAY_FPS);
        }
    }
    return TRUE;
}

Bool EnableVSync() {
    return FALSE;
}

Bool DisplayStream() {
    return FALSE;
}

Bool DoSkelDynamic() {
    return FALSE;
}

Bool DisplayFrame() {
    return FALSE;
}

Bool DisableConsole() {
    if (gData.Cons->GetNbParam() != 2) {
        gData.ToggleEngineFlag(FL_DISABLE_CONSOLE);
        return TRUE;
    }

    if (!stricmp(gData.Cons->GetStrParam(1), "ON")) {
        gData.EnableEngineFlag(FL_DISABLE_CONSOLE);
    }
    else {
        gData.DisableEngineFlag(FL_DISABLE_CONSOLE);
    }
    return TRUE;
}

Bool DisableIngameConsole() {
    return FALSE;
}

Bool DisableAssert() {
    return FALSE;
}

Bool DisableMusic() {
    return FALSE;
}

Bool DisableMovie() {
    return FALSE;
}

Bool SkipMovie() {
    return FALSE;
}

Bool EnableL2R2() {
    gData.ToggleEngineFlag(FL_ENABLE_L2R2);
    return TRUE;
}

Bool CheckMemoryEveryFrame() {
    return FALSE;
}

Bool NbHandle() {
    return FALSE;
}

Bool EnableBloomEffect() {
    if (gData.Cons->GetNbParam() == 1) {
        if (gData.MainRdr->IsEffectFlag(FL_EFFECT_BLOOM)) {
            gData.MainRdr->DisableEffectFlag(FL_EFFECT_BLOOM);
        }
        else {
            gData.MainRdr->EnableEffectFlag(FL_EFFECT_BLOOM);
        }
        return TRUE;
    }

    if (gData.Cons->GetNbParam() != 2) {
        return FALSE;
    }

    if (gData.Cons->IsParamFloat(1)) {
        if (gData.Cons->GetParamFloat(1) == 0.0f) {
            gData.MainRdr->DisableEffectFlag(FL_EFFECT_BLOOM);
        }
        else {
            gData.MainRdr->EnableEffectFlag(FL_EFFECT_BLOOM);
        }
    }
    return TRUE;
}

Bool DrawMemInfos() {
    return FALSE;
}
