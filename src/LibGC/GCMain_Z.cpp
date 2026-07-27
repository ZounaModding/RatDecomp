#include "GCMain_Z.h"
#include "Assert_Z.h"
#include "ErrorLanguage_Z.h"
#include "LowMemory_Z.h"
#include "GCConsole_Z.h"
#include "SurfaceGC_Z.h"
#include "GCMcManager_Z.h"
#include "StreamManager_Z.h"
#include "Movie_Z.h"
#include "SoundManager_Z.h"
#include "Renderer_Z.h"
#include "MaterialGC_Z.h"
#include "SkinGC_Z.h"
#include "SkelGC_Z.h"
#include "LodGC_Z.h"
#include "MeshGC_Z.h"
#include "RotShapeGC_Z.h"
#include "SurfaceGC_Z.h"
#include "ParticlesGC_Z.h"
#include "WarpGC_Z.h"
#include "InGameObjectsGC_Z.h"
#include "Sound_Z.h"
#include "FlareGC_Z.h"
#include "GCMovieManipulator_Z.h"
#include "TextureManager_Z.h"
#include "ARAM_Z.h"
#include "GCRenderer_Z.h"
#include "GCInput_Z.h"
#include "GCSoundManager_Z.h"
#include "GCMovie_Z.h"
#include <dolphin.h>

#ifdef __MWERKS__
#include "Sys_Z.h"
#endif

GCGlobals gData;
Bool ResetPressed;
Bool RestartReset = TRUE;

Extern_Z void GetFlagsFromGame();
Extern_Z void MathInitTables();

ExternC_Z void __start();
ExternC_Z void* OSGetArenaHi();
ExternC_Z void* OSGetArenaLo();
ExternC_Z U8 _stack_end[];
ExternC_Z U8 _stack_addr[];
ExternC_Z U8 _db_stack_addr[];
ExternC_Z U8 _db_stack_end[];
ExternC_Z U8 _f_sbss2[];

void PrintMemoryStatus(Char* i_Comment) {
    U32 l_ElfStart = (U32)&__start;

    U32 l_ArenaLo = (U32)OSGetArenaLo();
    U32 l_ArenaHi = (U32)OSGetArenaHi();
    U32 l_ArenaSize = l_ArenaHi - l_ArenaLo;

    U32 l_FreeMem = MemManager.GetFreeMem();
    U32 l_HeapUsed = MemManager.GetHeapSize() - l_FreeMem;

    void* l_StackEnd = (void*)_stack_end;
    U32 l_ElfEnd = (U32)l_StackEnd - 1;
    void* l_StackAddr = (void*)_stack_addr;
    void* l_DbStackAddr = (void*)_db_stack_addr;
    void* l_DbStackEnd = (void*)_db_stack_end;

    OSReport("\n\n");

    OSReport("> %s :\n", i_Comment ? i_Comment : "Memory Status");

    OSReport(">              start       end         size         usage\n");

    OSReport(
        ">     ELF      0x%08x  0x%08x  %08d ko\n",
        l_ElfStart,
        l_ElfEnd - 1,
        (l_ElfEnd - l_ElfStart) >> 10
    );

    OSReport(
        ">     STACK    0x%08x  0x%08x  %08d ko\n",
        (U32)l_StackEnd,
        (U32)l_StackAddr,
        ((U32)l_StackAddr - (U32)l_StackEnd) >> 10
    );

    OSReport(
        ">     DBSTACK  0x%08x  0x%08x  %08d ko\n",
        (U32)l_DbStackEnd,
        (U32)l_DbStackAddr,
        ((U32)l_DbStackAddr - (U32)l_DbStackEnd) >> 10
    );

    OSReport(
        ">     HEAP     0x%08x  0x%08x  %08d ko  %.2f mo\n",
        l_ArenaLo,
        l_ArenaHi,
        l_ArenaSize >> 10,
        (Float)l_HeapUsed / (1024.0f * 1024.0f)
    );

    OSReport("\n\n");
}

// TODO: Finish matching
Bool PrintMemStatus() {
    PrintMemoryStatus("PrintMemStatus Command");
    return TRUE;
}

void CallBack_ResetPressed() {
    ResetPressed = TRUE;
}

S32 ConsoleConfiguration::GetAspectRatio() {
    return 0;
}

// TODO: Finish matching
int main(int argc, char* argv[]) {
    void* l_StackAddr = (void*)_stack_addr;
    void* l_StackEnd = (void*)_stack_end;
    void* l_DbStackAddr = (void*)_db_stack_addr;
    void* l_DbStackEnd = (void*)_db_stack_end;

    memset(l_StackEnd, 0x41, (U32)l_StackAddr - (U32)l_StackEnd - 0x4000);

    memset(l_DbStackEnd, 0x41, (U32)l_DbStackAddr - (U32)l_DbStackEnd - 0x4000);

    OSSetResetCallback(CallBack_ResetPressed);
    InitProgram(argc, argv);

    REGISTERCOMMAND("PrintMemStatus", PrintMemStatus);

    ProgramMain();
    ShutProgram();

    OSResetSystem(0, 0, FALSE);
    return 1;
}

Bool EndFrame() {
    if (ResetPressed) {
        if (OSGetResetButtonState()) {
            return FALSE;
        }
        U32 l_LastStreamError = gData.StreamMgr->GetLastError();
        U32 l_LastMovieError = gData.MovieMgr->GetLastError();
        if (l_LastStreamError == STR_ERROR_FATAL || l_LastMovieError == STR_ERROR_FATAL) {
            return FALSE;
        }
        if (((GCMcManager_Z*)gData.SavingMgr)->GetIsBusy()) {
            gData.SavingMgr->Shut();
        }
        if ((l_LastStreamError == 0) && (l_LastMovieError == 0)) {
            RestartReset = 1;
            VISetBlack(1);
            VIFlush();
            VIWaitForRetrace();
            return 1;
        }
        RestartReset = FALSE;
        gData.SoundMgr->Shut();
        Delete_Z gData.SoundMgr;
        gData.SoundMgr = NULL;
        gData.MainRdr->Shut();
        Delete_Z gData.MainRdr;
        gData.MainRdr = NULL;
        VISetBlack(1);
        VIFlush();
        VIWaitForRetrace();
        OSResetSystem(TRUE, 0, FALSE);
    }
    return FALSE;
}

void InitProgram(int i_Argc, Char** i_Argv) {
    OSInit();
    ExceptionHandler();
    MemManager.Init();
    gData.m_GameFlag = FL_GAME_NONE;
    GetFlagsFromGame();
    gData.Cons = NewL_Z(283) GCConsole_Z;
    if (!gData.Cons->InitConsole()) {
        exit(0x14);
    }
    gData.Cons->Init();
    gData.Cons->SetVar("_GC");
    gData.Cons->SetVar("_FORCEBFREAD");
    MathInitTables();
    strcpy(gData.m_AliasPath, ".\\");
    DVDInit();
}

void ShutProgram() {
    Delete_Z gData.Cons;
}

LanguageEnum_Z ConsoleConfiguration::GetLanguage() {
    if (OSGetFontEncode() == OS_FONT_ENCODE_SJIS) {
        return LANG_JAPANESE_Z;
    }
    if (VIGetTvFormat() == VI_NTSC) {
        return LANG_ENGLISH_Z;
    }
    switch (OSGetLanguage()) {
        case 1:
            return LANG_GERMAN_Z;
        case 2:
            return LANG_FRENCH_Z;
        case 3:
            return LANG_SPANISH_Z;
        case 4:
            return LANG_ITALIAN_Z;
        case 5:
            return LANG_DUTCH_Z;
        default:
            return LANG_ENGLISH_Z;
    }
}

void RegisterLowLevelClasses() {
    REGISTER_CLASS("Material_Z", "BaseObject_Z", MaterialGC_Z::NewObject);
    REGISTER_CLASS("Skin_Z", "Object_Z", SkinGC_Z::NewObject);
    REGISTER_CLASS("Skel_Z", "PointsData_Z", SkelGC_Z::NewObject);
    REGISTER_CLASS("Lod_Z", "Object_Z", LodGC_Z::NewObject);
    REGISTER_CLASS("LodData_Z", "ObjectData_Z", LodGCData_Z::NewObject);
    REGISTER_CLASS("Mesh_Z", "Points_Z", MeshGC_Z::NewObject);
    REGISTER_CLASS("MeshData_Z", "ObjectData_Z", MeshDataGC_Z::NewObject);
    REGISTER_CLASS("RotShape_Z", "Points_Z", RotShapeGC_Z::NewObject);
    REGISTER_CLASS("RotShapeData_Z", "ObjectData_Z", RotShapeGCData_Z::NewObject);
    REGISTER_CLASS("Surface_Z", "Points_Z", SurfaceGC_Z::NewObject);
    REGISTER_CLASS("SurfaceDatas_Z", "PointsData_Z", SurfaceDatas_Z::NewObject);
    REGISTER_CLASS("Particles_Z", "Object_Z", Particles_Z::NewObject);
    REGISTER_CLASS("ParticlesData_Z", "ObjectData_Z", ParticlesGCData_Z::NewObject);
    REGISTER_CLASS("Warp_Z", "BaseObject_Z", WarpGC_Z::NewObject);
    REGISTER_CLASS("InGameObjects_Z", "Manipulator_Z", InGameObjectsGC_Z::NewObject);
    REGISTER_CLASS("Sound_Z", "ResourceObject_Z", Sound_Z::NewObject);
    REGISTER_CLASS("Flare_Z", "Object_Z", FlareGC_Z::NewObject);
    REGISTER_CLASS("FlareData_Z", "ObjectData_Z", FlareDataGC_Z::NewObject);
    REGISTER_CLASS("MovieManipulator_Z", "ManipulatorDraw_Z", GCMovieManipulator_Z::NewObject);
    REGISTER_CLASS_TYPE("Mesh_Z", FL_CLASS_STREAM_XRAM, FL_CLASS_NONE);
}

// TODO: Finish matching
void LowLevelInitProgram() {
    NewMgrInitL_Z(gData.XRamMgr, AramManager_Z, 429);
    NewMgrInitArgsL_Z(gData.MainRdr, GCRenderer_Z, 430, 640, 480);
    NewMgrInitL_Z(gData.TextureMgr, TextureManager_Z, 431);
    NewMgrInitL_Z(gData.InputMgr, GCInput_Z, 433);
    NewMgrInitL_Z(gData.SoundMgr, GCSoundManager_Z, 437);
    NewMgrInitL_Z(gData.MovieMgr, GCMovie_Z, 438);
    NewMgrInitL_Z(gData.SavingMgr, GCMcManager_Z, 440);
    NewMgrL_Z(gData.SurfaceCache, SurfaceCache_Z, 445);
    NewMgrL_Z(gData.GCParticlesManager, GCParticlesManager_Z("GCParticlesManager_Z"), 446);
}

void LowLevelUpdateProgram(Float i_DeltaTime) {
    gData.SurfaceCache->Update();
}

void LowLevelCloseProgram() {
    Delete_Z gData.SurfaceCache;
    gData.SurfaceCache = NULL;
    Delete_Z gData.GCParticlesManager;
    gData.GCParticlesManager = NULL;
    Delete_Z gData.TextureMgr;
    gData.TextureMgr = NULL;
    Delete_Z gData.SavingMgr;
    gData.SavingMgr = NULL;
    Delete_Z gData.Cons;
    gData.Cons = NULL;
}

void GCGlobals::Minimize() {
    if (!GCParticlesManager) return;
    GCParticlesManager->Minimize();
}

Bool GCGlobals::GetMgrSize(const Name_Z& i_Name, S32& i_Size1, S32& i_Size2) {
    return Globals::GetMgrSize(i_Name, i_Size1, i_Size2) != FALSE;
}

// clang-format off
#ifdef __MWERKS__
asm void* Sys_Z::MemCpyFrom(void* o_Dest, void* i_Src, const U32 i_Size) {
    nofralloc
    dcbt r0, r4
    li r6, 0
    cmplw r6, r5
    beq done
    andi. r0, r5, 1
    cmplw r6, r0
    beq half_loop
    lbzx r0, r4, r6
    stbx r0, r3, r6
    addi r6, r6, 1
    cmplwi r5, 1
    beq done
half_loop: 
    lhzx r0, r4, r6
    sthx r0, r3, r6
    addi r6, r6, 2
    cmplw r6, r5
    blt half_loop
done:
    add r3, r4, r5
    blr
}
#else
void* Sys_Z::MemCpyFrom(void* o_Dest, void* i_Src, const U32 i_Size) {
    return memcpy(o_Dest, i_Src, i_Size);
}
#endif
// clang-format on
