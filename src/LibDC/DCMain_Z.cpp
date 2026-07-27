#include "DCMain_Z.h"
#include "DCConsole_Z.h"
#include "LowMemory_Z.h"
#include "Material_Z.h"
#include "Assert_Z.h"
#include "DCRenderer_Z.h"
#include "ErrorLanguage_Z.h"
#include <stdarg.h>
#include <stdio.h>

Extern_Z void GetFlagsFromGame();

DCGlobals gData;

KOS_INIT_FLAGS(INIT_IRQ | INIT_CONTROLLER | INIT_CDROM | INIT_VMU | INIT_FS_ALL);

int main(int argc, char* argv[]) {
    dbglog(DBG_INFO, "> Ratatouille Dreamcast\n");

    InitProgram(argc, argv);

    ProgramMain();
    ShutProgram();

    return 0;
}

// Init

void InitProgram(int i_Argc, Char** i_Argv) {
    ExceptionHandler();
    MemManager.Init();
    GetFlagsFromGame();
    gData.Cons = New_Z DCConsole_Z;
    if (!gData.Cons->InitConsole()) {
        exit(0x14);
    }
    gData.Cons->Init();

    // Override game flags (TODO: Remove later)
    gData.m_GameFlag &= ~FL_GAME_2_FRAMES;
    gData.m_GameFlag &= ~FL_GAME_USE_CD;

    if (gData.m_GameFlag & FL_GAME_USE_CD) {
        strcpy(gData.m_AliasPath, "/cd/");
    }
    else {
        strcpy(gData.m_AliasPath, "/rd/");
    }
}

void LowLevelInitProgram() {
    NewMgrInitArgs_Z(gData.MainRdr, DCRenderer_Z, 640, 480);
}

// TODO: MaterialDC_Z might need to be a real class
static BaseObject_Z* NewMaterialDC_Z() {
    return New_Z Material_Z;
}

void RegisterLowLevelClasses() {
    REGISTER_CLASS("Material_Z", "BaseObject_Z", NewMaterialDC_Z);
}

// Update

void LowLevelUpdateProgram(Float i_DeltaTime) {
}

Bool EndFrame() {
    return FALSE;
}

void DCGlobals::Minimize() {
}

// Shut

void ShutProgram() {
    Delete_Z gData.Cons;
    gData.Cons = NULL;
}

void LowLevelCloseProgram() { }

// Util

void Report_Z(const Char* i_Format, ...) {
    Char l_Buffer[512];
    va_list l_Args;
    va_start(l_Args, i_Format);
    vsnprintf(l_Buffer, sizeof(l_Buffer), i_Format, l_Args);
    va_end(l_Args);
    dbglog(DBG_INFO, "%s", l_Buffer);
}

void PrintMemoryStatus(Char* i_Comment) {
    dbglog(DBG_INFO, "> %s :\n", i_Comment ? i_Comment : "Memory Status");
}

void* Sys_Z::MemCpyFrom(void* o_Dest, void* i_Src, const U32 i_Size) {
    return memcpy(o_Dest, i_Src, i_Size);
}

LanguageEnum_Z ConsoleConfiguration::GetLanguage() {
    return LANG_ENGLISH_Z;
}
