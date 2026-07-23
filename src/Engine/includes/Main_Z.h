#ifndef _MAIN_Z_H_
#define _MAIN_Z_H_
#include "Error_Z.h"
#include "Name_Z.h"
#include "String_Z.h"
#include "DebugTools_Z.h"
#include "Types_Z.h"
#include "Language_Z.h"

#define WhereAmI_Z(wai) strcpy(WhereAmI, "_CoreMainLoop(): " wai)

#define NewMgr_Z(mgr, class, line)                       \
    do {                                                 \
        MemManager.m_FreeMemCached = s_GetFreeMem();     \
        if (!mgr) {                                      \
            mgr = NewL_Z(line) class;                    \
            MemManager.m_FreeMemCached = s_GetFreeMem(); \
        }                                                \
    } while (0)

#define NewMgrInit_Z(mgr, class, line)                                                                           \
    do {                                                                                                         \
        MemManager.m_FreeMemCached = s_GetFreeMem();                                                             \
        if (!mgr) {                                                                                              \
            mgr = NewL_Z(line) class;                                                                            \
            if (!mgr->Init()) {                                                                                  \
                Console_Z::PrintBoxString("Fatal error", GetStringTabError(GetLanguage(), error_eight), #class); \
                exit(0);                                                                                         \
            }                                                                                                    \
            MemManager.m_FreeMemCached = s_GetFreeMem();                                                         \
        }                                                                                                        \
    } while (0)

#define NewMgrInitArgs_Z(mgr, class, line, ...)                                                                  \
    do {                                                                                                         \
        MemManager.m_FreeMemCached = s_GetFreeMem();                                                             \
        if (!mgr) {                                                                                              \
            mgr = NewL_Z(line) class;                                                                            \
            if (!mgr->Init(__VA_ARGS__)) {                                                                       \
                Console_Z::PrintBoxString("Fatal error", GetStringTabError(GetLanguage(), error_eight), #class); \
                exit(0);                                                                                         \
            }                                                                                                    \
            MemManager.m_FreeMemCached = s_GetFreeMem();                                                         \
        }                                                                                                        \
    } while (0)

ExternC_Z void memset(void* __s, S32 __c, U32 __n);

// Extern Platform Functions

Extern_Z void PrintMemoryStatus(Char* i_Msg);
Extern_Z Bool EndFrame();
Extern_Z void RegisterLowLevelClasses();
Extern_Z void LowLevelInitProgram();
Extern_Z void LowLevelUpdateProgram(Float i_DeltaTime);
Extern_Z void LowLevelCloseProgram();

// Extern Game Functions

Extern_Z void RegisterGameClasses();
Extern_Z void GameProgramInit();
Extern_Z void RegisterGameCommands();

// Engine Main Functions

void _CoreMainLoop();
Bool ProgramMain();
void ProgramDefaultFlag();
void ProgramInit();
void CloseProgram();
void RegisterClasses();
void RegisterGlobalCommands();

// Global commands

Bool ExitApp();
Bool Source();
Bool BSource();
Bool SetFrame();
Bool Help();
Bool Pause();
Bool LoadObjectLib();
Bool RemoveObjectLib();
Bool LoadMaterialLib();
Bool RemoveMaterialLib();
Bool LoadFont();
Bool RemoveFont();
Bool LoadSysRtc();
Bool RemoveSysRtc();
Bool InitRandomSeed();
Bool SetBlockFrame();
Bool SetTimeFactor();
Bool PrintFreeMem();
Bool AddJoyStick();

// Forward declarations

class ManipulatorManager_Z;
class AnimationManager_Z;
class SoundManager_Z;
class ABC_ScriptManager;
class XRamManager_Z;
class Mat4x4Buffer_Z;
class Console_Z;
class Renderer_Z;
class GameManager_Z;
class EffectManager_Z;
class SystemDatas_Z;
class StreamManager_Z;
class ClassManager_Z;
class WorldManager_Z;
class MaterialManager_Z;
class ColSurfaceCache_Z;
class ColTriangleCache_Z;
class ObjectBankManager_Z;
class InputPlatForm_Z;
class Movie_Z;
class InputEngine_Z;
class SaveGame_Z;
class ParticlesManager_Z;
class SurfaceCache_Z;
class VolatileMgr_Z;
class NetManager_Virtual_Z;

struct Date_Z {
    Char m_Unk_0x0;
    Char m_Unk_0x1;
    Char m_Unk_0x2;
    Char m_Unk_0x3;
    Char m_Unk_0x4;
    U16 m_Unk_0x6;

    Date_Z() {
        m_Unk_0x0 = 0;
        m_Unk_0x1 = 0;
        m_Unk_0x2 = 0;
        m_Unk_0x3 = 0;
        m_Unk_0x4 = 0;
        m_Unk_0x6 = 0;
    }
};

class Globals {
public:
    virtual ~Globals() { };
    virtual Bool GetMgrSize(const Name_Z& i_Name, S32& i_Size1, S32& i_Size2);

    virtual void Minimize() { }

    virtual Date_Z GetDate();

    void InitTime();
    Float GetOneFrameTime();
    Float GetDeltaTime();

    void* UnkMgr_0x4;
    Console_Z* Cons;
    Renderer_Z* MainRdr;
    ClassManager_Z* ClassMgr;
    WorldManager_Z* WorldMgr;
    MaterialManager_Z* MaterialMgr;
    ABC_ScriptManager* ScriptMgr;
    ColSurfaceCache_Z* ColSurfaceCache;
    void* UnkMgr_0x24;
    ColTriangleCache_Z* ColTriangleCache;
    Mat4x4Buffer_Z* MatrixBuffer;
    ManipulatorManager_Z* ManipMgr;
    GameManager_Z* GameMgr;
    void* UnkMgr_0x38;
    AnimationManager_Z* AnimMgr;
    EffectManager_Z* EffectMgr;
    SystemDatas_Z* SystemDatas;
    ObjectBankManager_Z* ObjectBankMgr;
    InputPlatForm_Z* InputMgr;
    SoundManager_Z* SoundMgr;
    Movie_Z* MovieMgr;
    InputEngine_Z* ScriptInputMgr;
    SaveGame_Z* SavingMgr;
    ParticlesManager_Z* ParticlesMgr;
    void* UnkMgr_0x64;
    SurfaceCache_Z* SurfaceCache;
    void* UnkMgr_0x6c;
    StreamManager_Z* StreamMgr;
    VolatileMgr_Z* VolatileMgr;
    void* UnkMgr_0x78;
    NetManager_Virtual_Z* NetMgr;
    void* UnkMgr_0x80;
    XRamManager_Z* XRamMgr;
    S32 m_UnkS32_0x88;
    Bool m_Running;
    Bool m_BlockFrame;
    Bool m_IsPlatformAgnostic; // $SABE: Never set to TRUE, I suppose it was used in BF Write mode
    Bool m_UpdatingResource;
    Float m_FrameBlockCountDownTime;
    Float m_FrameBlockTime;
    Float m_TimeFactor;
    Bool m_UnPauseRequested;
    String_Z<ARRAY_CHAR_MAX> m_DBPath;
    String_Z<ARRAY_CHAR_MAX> m_DFPath;
    String_Z<ARRAY_CHAR_MAX> m_BFPathList;
    String_Z<ARRAY_CHAR_MAX> m_BFMatchList;
    String_Z<ARRAY_CHAR_MAX> m_AppPath;
    String_Z<ARRAY_CHAR_MAX> m_UnkString_0x59d;
    String_Z<ARRAY_CHAR_MAX> m_LocalSavePath; // In Roaming on windows
    U32 m_EngineFlag;
    U32 m_MaterialFlag; // Which channels of materials are enabled
    U32 m_GameFlag;
    U32 m_Unk_0x7ac;
    U64 m_ErrorCodes;
    U32 m_FrameCount;
    Float m_Timer;
    U32 m_Unk_0x7c0;
    Bool m_TimerNotCalibrated;
    Float m_TargetSecondsPerFrame;
    Float m_AbsoluteTime;
    S16 m_Unk_0x7d0;
    S16 m_Unk_0x7d2;
    S16 m_Unk_0x7d4;
    S16 m_Unk_0x7d6;
    S16 m_Unk_0x7d8;
    S16 m_Unk_0x7da;
    S32 m_Unk_0x7dc;

    void SetEngineFlag(U32 i_Flag) { m_EngineFlag = i_Flag; }

    void EnableEngineFlag(U32 i_Flag) { m_EngineFlag |= i_Flag; }

    void DisableEngineFlag(U32 i_Flag) { m_EngineFlag &= ~i_Flag; }

    Bool IsEngineFlag(U32 i_Flag) const { return (m_EngineFlag & i_Flag) != 0; }

    void ToggleEngineFlag(U32 i_Flag) { m_EngineFlag ^= i_Flag; }

    void SetMaterialFlag(U32 i_Flag) { m_MaterialFlag = i_Flag; }

    void EnableMaterialFlag(U32 i_Flag) { m_MaterialFlag |= i_Flag; }

    void DisableMaterialFlag(U32 i_Flag) { m_MaterialFlag &= ~i_Flag; }

    // TODO: Properly define members for all this so we don't gotta move stuff around
    inline Globals() {
        m_Unk_0x7d0 = -1;
        m_Unk_0x7d2 = 0;
        m_Unk_0x7d4 = 1;
        m_Unk_0x7da = 8;
        m_Unk_0x7d6 = 60;
        m_Unk_0x7d8 = 480;
        // $SABE: Probably a macro
        memset(&UnkMgr_0x4, 0, sizeof(XRamManager_Z*) + (U32)&XRamMgr - (U32)&UnkMgr_0x4);
        m_TimerNotCalibrated = TRUE;
        m_AbsoluteTime = 0.0f;
        m_TargetSecondsPerFrame = 0.0f;
        m_AppPath.StrCpy(".\\");
        m_LocalSavePath.StrCpy("Ace");
        m_FrameCount = 0;
        m_Unk_0x7ac = 0;
        m_ErrorCodes = ERROR_CODE_NONE;
        m_Running = FALSE;
        m_BlockFrame = FALSE;
        m_UnPauseRequested = FALSE;
        m_UpdatingResource = FALSE;
        m_IsPlatformAgnostic = FALSE;
        SurfaceCache = NULL;
        UnkMgr_0x64 = NULL;
        UnkMgr_0x80 = NULL;
        m_Timer = 0.0f;
        m_EngineFlag = FL_ENGINE_NONE;
        m_GameFlag = FL_GAME_NONE;
        m_TimeFactor = 1.0f;
        m_Unk_0x7c0 = 0;
    }
};

class ConsoleConfiguration {
public:
    static LanguageEnum_Z GetLanguage();
    // $SABE: Probably returns an enum
    static S32 GetAspectRatio();
};

#endif
