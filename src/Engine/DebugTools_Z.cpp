#include "DebugTools_Z.h"
#include "Program_Z.h"
#include "Renderer_Z.h"

ExternC_Z int stricmp(const char* String1, const char* String2);

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

Bool EnablePopupMenu() {
    return TRUE;
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
