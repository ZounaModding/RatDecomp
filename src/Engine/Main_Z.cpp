#include "Program_Z.h"
#include "Random_Z.h"
#include "Fonts_Z.h"
#include "ClassManager_Z.h"
#include "World_Z.h"
#include "GenWorld_Z.h"
#include "SubWorld_Z.h"
#include "Object_Z.h"
#include "Points_Z.h"
#include "Node_Z.h"
#include "Bitmap_Z.h"
#include "Spline_Z.h"
#include "SplineZone_Z.h"
#include "Occluder_Z.h"
#include "CameraZone_Z.h"
#include "Light_Z.h"
#include "LightData_Z.h"
#include "HFog_Z.h"
#include "Omni_Z.h"
#include "Emiter_Z.h"
#include "Tree_Z.h"
#include "CollisionVol_Z.h"
#include "Camera_Z.h"
#include "Game_Z.h"
#include "GameObj_Z.h"
#include "MaterialAnim_Z.h"
#include "MaterialObj_Z.h"
#include "AnimFrame_Z.h"
#include "Rtc_Z.h"
#include "Graph_Z.h"
#include "UserDefine_Z.h"
#include "Txt_Z.h"
#include "Animation_Z.h"
#include "PlayAnim_Z.h"
#include "DynamicAnim2_Z.h"
#include "SequenceAnim_Z.h"
#include "SequenceMixAnim_Z.h"
#include "CameraOccluder_Z.h"
#include "CameraStream_Z.h"
#include "CameraEngineZone_Z.h"
#include "ObjectMove_Z.h"
#include "ObjectThrow_Z.h"
#include "LodMove_Z.h"
#include "VehiculeMove_Z.h"
#include "ObjectsGame_Z.h"
#include "Menu3D_Z.h"
#include "Menu2D_Z.h"
#include "RtcPlayer_Z.h"
#include "Ghost_Z.h"
#include "SelfRunning_Z.h"
#include "InGameDatas_Z.h"
#include "SuperSpray_Z.h"
#include "InGameObjectsManipulator_Z.h"
#include "StreamFile_Z.h"
#include "ConsoleInterp_Z.h"
#include "ObjectsMove_Z.h"
#include "ObjectsBounce_Z.h"
#include "ObjectsBreak_Z.h"
#include "VolatileMem_Z.h"
#include "StreamManager_Z.h"
#include "Renderer_Z.h"
#include "SoundManager_Z.h"
#include "InputEngine_Z.h"
#include "SaveGame_Z.h"
#include "ABC_ScriptManager.h"
#include "GameManager_Z.h"
#include "ManipulatorManager_Z.h"
#include "EffectManager_Z.h"
#include "ParticlesManager_Z.h"
#include "MaterialManager_Z.h"
#include "XRamManager_Z.h"
#include "DebugTools_Z.h"
#include "ErrorLanguage_Z.h"
#include "NetManager_Virtual_Z.h"
#include "Movie_Z.h"
#include "World_Z.h"
#include "AnimationManager_Z.h"
#include "SystemDatas_Z.h"
#include "ObjectBankManager_Z.h"
#include "SurfaceCache_Z.h"
#include "TriangleCache_Z.h"
#include "Timer_Z.h"
#include "KSys_Z.h"
#include "Main_Z.h"

Char WhereAmI[128];
VBool InMainLoop;

void _CoreMainLoop() {
    static Float dTime;
    static Char init;
    Bool l_Do30Fps;

    WhereAmI_Z("Beginning");

    if (!init) {
        init = TRUE;
        dTime = FRAME_TIME_NTSC;
    }

    if (gData.m_BlockFrame) {
        gData.m_FrameBlockCountDownTime -= dTime;
        if (gData.m_FrameBlockCountDownTime > 0.0f) {
            dTime = gData.GetDeltaTime();
            return;
        }
        Float l_TargetSecondsPerFrame = gData.m_TargetSecondsPerFrame;
        if (l_TargetSecondsPerFrame) {
            dTime = gData.GetOneFrameTime() * l_TargetSecondsPerFrame;
        }
        else {
            dTime = FRAME_TIME_NTSC;
        }
    }

    WhereAmI_Z("Before UpdateVolatileBlocks()");
    gData.VolatileMgr->UpdateVolatileBlocks(dTime);

    WhereAmI_Z("Before StreamMgr->Update()");
    gData.StreamMgr->Update(dTime);

    WhereAmI_Z("Before BeginRender()");
    gData.MainRdr->BeginRender();

    WhereAmI_Z("Before InitFrame()");
    gData.SoundMgr->InitFrame();

    WhereAmI_Z("Before SavingMgr->Update()");
    gData.SavingMgr->Update(dTime);

    WhereAmI_Z("Before InputMgr->UpdateInput()");
    gData.InputMgr->UpdateInput(dTime);

    WhereAmI_Z("Before ScriptInputMgr->Update()");
    gData.ScriptInputMgr->Update(dTime);

    WhereAmI_Z("Before ScriptMgr->Update()");
    gData.ScriptMgr->Update(dTime);

    WhereAmI_Z("Before GameMgr->Update()");
    gData.GameMgr->Update(dTime);

    WhereAmI_Z("Before ManipulatorMgr->Update()");
    gData.ManipulatorMgr->Update(dTime);

    WhereAmI_Z("Before EffectMgr->Update()");
    gData.EffectMgr->Update(dTime);

    WhereAmI_Z("Before SoundMgr->Update()");
    gData.SoundMgr->Update(dTime);

    WhereAmI_Z("Before MainRdr->Draw()");
    gData.MainRdr->Draw(dTime);

    WhereAmI_Z("Before ParticlesMgr->Update()");
    gData.ParticlesMgr->Update(dTime);

    WhereAmI_Z("Before LowLevelUpdateProgram()");
    LowLevelUpdateProgram(dTime);

    WhereAmI_Z("Before  gData.GetDeltaTime()");
    dTime = gData.GetDeltaTime();

    ClassManager_Z* l_ClassMgr = gData.ClassMgr;
    l_Do30Fps = l_ClassMgr->GetSetDeltaTimeTo30fps();
    l_ClassMgr->SetSetDeltaTimeTo30fps(FALSE);

    if (l_Do30Fps || gData.m_UnPauseRequested) {
        if (gData.m_UnPauseRequested) {
            WhereAmI_Z("Before UnpauseAppRequested()");
            gData.m_UnPauseRequested = FALSE;
            WhereAmI_Z("Before ScriptMgr->EndPause()");
            gData.ScriptMgr->EndPause();
            WhereAmI_Z("Before ManipulatorMgr->EndPause()");
            gData.ManipulatorMgr->EndPause();
            WhereAmI_Z("Before MaterialMgr->EndPause()");
            gData.MaterialMgr->EndPause();
            WhereAmI_Z("Before GameMgr->EndPause()");
            gData.GameMgr->EndPause();
            WhereAmI_Z("Before InputMgr->EndPause()");
            gData.InputMgr->EndPause();
        }
        dTime = 1.0f / 30.0f;
    }

    gData.m_Timer += dTime;
    gData.m_FrameCount++;

    WhereAmI_Z("Before Cons->Update()");
    gData.Cons->Update(dTime);

    WhereAmI_Z("Before ClassMgr->Update()");
    gData.ClassMgr->Update(dTime);

    WhereAmI_Z("Before ClassMgr->Update()");
    gData.XRamMgr->Update(dTime);

    WhereAmI_Z("Before Z_Verify()");
    if (gData.m_EngineFlag & FL_MEMORY_VERIFY) {
        ASSERTL_Z(Z_Verify(), "memory intergrity error", 277);
    }

    WhereAmI_Z("Before MainRdr->EndRender()");
    gData.MainRdr->EndRender(dTime);

    WhereAmI_Z("Before MainRdr->WaitForRetrace()");
    gData.MainRdr->WaitForRetrace(dTime);

    if (gData.m_BlockFrame) {
        gData.m_FrameBlockCountDownTime = gData.m_FrameBlockTime;
    }

    WhereAmI_Z("End");
}

Bool ProgramMain() {
    ProgramInit();

    gData.InitTime();

    gData.Cons->InterpCommand("BSource User.tsc");

    gData.m_FrameCount++;

    for (;;) {
        if (!InMainLoop) {
            _CoreMainLoop();
            if (EndFrame()) {
                break;
            }
        }
    }

    CloseProgram();

    return TRUE;
}

void ProgramDefaultFlag() {
    U32 l_EngineFlag = gData.m_EngineFlag | FL_ENGINE_DEFAULT;
    U32 l_MaterialFlag = FL_MATERIAL_ALL;
    gData.m_MaterialFlag = l_MaterialFlag;
    gData.m_EngineFlag = l_EngineFlag;
}

void ProgramInit() {
    PrintMemoryStatus("Target Init");

    ComputeMathPrecision();

    ProgramDefaultFlag();

    gData.ScriptMgr = NULL;
    gData.UnkMgr_0x78 = NULL;
    gData.NetMgr = NULL;

    NewMgrL_Z(gData.ClassMgr, ClassManager_Z, 387);

    RegisterClasses();

    InitTabError();

    NewMgrInitL_Z(gData.StreamMgr, StreamManager_Z, 396);

    if (!gData.m_IsPlatformAgnostic) {
        LowLevelInitProgram();
    }

    if (!gData.MainRdr) {
        NewMgrL_Z(gData.MainRdr, Renderer_Z, 404);
    }

    if (!gData.NetMgr) {
        NewMgrL_Z(gData.NetMgr, NetManager_Virtual_Z, 405);
    }

    if (!gData.SoundMgr) {
        NewMgrInitL_Z(gData.SoundMgr, SoundManager_Z, 406);
    }

    if (!gData.InputMgr) {
        NewMgrInitL_Z(gData.InputMgr, InputPlatForm_Z, 407);
    }

    if (!gData.SavingMgr) {
        NewMgrInitL_Z(gData.SavingMgr, SaveGame_Z, 408);
    }

    if (!gData.MovieMgr) {
        NewMgrInitL_Z(gData.MovieMgr, Movie_Z, 409);
    }

    NewMgrL_Z(gData.VolatileMgr, VolatileMgr_Z, 411);
    NewMgrL_Z(gData.MaterialMgr, MaterialManager_Z, 412);
    NewMgrL_Z(gData.WorldMgr, WorldManager_Z, 413);
    NewMgrL_Z(gData.ColSurfaceCache, ColSurfaceCache_Z, 414);
    NewMgrL_Z(gData.ColTriangleCache, ColTriangleCache_Z, 416);
    NewMgrL_Z(gData.MatrixBuffer, Mat4x4Buffer_Z, 417);
    NewMgrL_Z(gData.ManipulatorMgr, ManipulatorManager_Z, 418);
    NewMgrL_Z(gData.GameMgr, GameManager_Z, 419);
    NewMgrL_Z(gData.AnimMgr, AnimationManager_Z, 420);
    NewMgrL_Z(gData.EffectMgr, EffectManager_Z, 421);
    NewMgrInitL_Z(gData.XRamMgr, XRamManager_Z, 422);

    PrintMemoryStatus("Zouna Init");

    GameProgramInit();

    PrintMemoryStatus("Game Init");

    NewMgrL_Z(gData.SystemDatas, SystemDatas_Z, 430);
    NewMgrL_Z(gData.ObjectBankMgr, ObjectBankManager_Z, 431);
    NewMgrL_Z(gData.ParticlesMgr, ParticlesManager_Z("ParticlesManager"), 432);

    gData.ScriptMgr->Init();
    gData.MainRdr->InitViewport(1);

    RegisterGlobalCommands();
}

void CloseProgram() {
}

void RegisterClasses() {
    REGISTER_CLASS("ResourceObject_Z", "BaseObject_Z", NULL);
    REGISTER_CLASS("World_Z", "ResourceObject_Z", World_Z::NewObject);
    REGISTER_CLASS("GenWorld_Z", "Object_Z", GenWorld_Z::NewObject);
    REGISTER_CLASS("SubWorld_Z", "ResourceObject_Z", SubWorld_Z::NewObject);
    REGISTER_CLASS("Object_Z", "ResourceObject_Z", NULL);
    REGISTER_CLASS("ObjectData_Z", "ResourceObject_Z", NULL);
    REGISTER_CLASS("Points_Z", "Object_Z", Points_Z::NewObject);
    REGISTER_CLASS("Node_Z", "Object_Z", Node_Z::NewObject);
    REGISTER_CLASS("Bitmap_Z", "Object_Z", Bitmap_Z::NewObject);
    REGISTER_CLASS("Spline_Z", "Object_Z", Spline_Z::NewObject);
    REGISTER_CLASS("SplineZone_Z", "Object_Z", SplineZone_Z::NewObject);
    REGISTER_CLASS("Occluder_Z", "SplineZone_Z", Occluder_Z::NewObject);
    REGISTER_CLASS("CameraZone_Z", "SplineZone_Z", CameraZone_Z::NewObject);
    REGISTER_CLASS("Light_Z", "Object_Z", Light_Z::NewObject);
    REGISTER_CLASS("LightData_Z", "ObjectData_Z", LightData_Z::NewObject);
    REGISTER_CLASS("HFog_Z", "Object_Z", HFog_Z::NewObject);
    REGISTER_CLASS("HFogData_Z", "ObjectData_Z", HFogData_Z::NewObject);
    REGISTER_CLASS("Omni_Z", "Object_Z", Omni_Z::NewObject);
    REGISTER_CLASS("Emiter_Z", "Object_Z", Emiter_Z::NewObject);
    REGISTER_CLASS("Tree_Z", "Object_Z", Tree_Z::NewObject);
    REGISTER_CLASS("CollisionVol_Z", "Object_Z", CollisionVol_Z::NewObject);
    REGISTER_CLASS("Camera_Z", "Object_Z", Camera_Z::NewObject);
    REGISTER_CLASS("Game_Z", "Object_Z", Game_Z::NewObject);
    REGISTER_CLASS("GameObj_Z", "Object_Z", GameObj_Z::NewObject);
    REGISTER_CLASS("MaterialAnim_Z", "Material_Z", MaterialAnim_Z::NewObject);
    REGISTER_CLASS("MaterialObj_Z", "ResourceObject_Z", MaterialObj_Z::NewObject);
    REGISTER_CLASS("Fonts_Z", "ResourceObject_Z", Fonts_Z::NewObject);
    REGISTER_CLASS("AnimFrame_Z", "ResourceObject_Z", AnimFrame_Z::NewObject);
    REGISTER_CLASS("Rtc_Z", "ResourceObject_Z", Rtc_Z::NewObject);
    REGISTER_CLASS("Graph_Z", "ResourceObject_Z", Graph_Z::NewObject);
    REGISTER_CLASS("UserDefine_Z", "ResourceObject_Z", UserDefine_Z::NewObject);
    REGISTER_CLASS("PointsData_Z", "ObjectData_Z", NULL);
    REGISTER_CLASS("Txt_Z", "ResourceObject_Z", Txt_Z::NewObject);
    REGISTER_CLASS("Animation_Z", "ResourceObject_Z", Animation_Z::NewObject);
    REGISTER_CLASS_TYPE("Animation_Z", FL_CLASS_TYPE_STREAM_XRAM, FL_CLASS_TYPE_NONE);
    RegisterLowLevelClasses();
    REGISTER_CLASS("Manipulator_Z", "BaseObject_Z", NULL);
    REGISTER_CLASS("ManipulatorDraw_Z", "Manipulator_Z", NULL);
    REGISTER_CLASS("PlayAnim_Z", "Manipulator_Z", PlayAnim_Z::NewObject);
    REGISTER_CLASS("DynamicAnim_Z", "Manipulator_Z", NULL);
    REGISTER_CLASS("DynamicAnim2_Z", "DynamicAnim_Z", DynamicAnim2_Z::NewObject);
    REGISTER_CLASS("SequenceAnim_Z", "BaseObject_Z", SequenceAnim_Z::NewObject);
    REGISTER_CLASS("SequenceMixAnim_Z", "SequenceAnim_Z", SequenceMixAnim_Z::NewObject);
    REGISTER_CLASS("CameraOccluder_Z", "Manipulator_Z", CameraOccluder_Z::NewObject);
    REGISTER_CLASS("CameraStream_Z", "Manipulator_Z", CameraStream_Z::NewObject);
    REGISTER_CLASS("CameraEngine_Z", "Manipulator_Z", NULL);
    REGISTER_CLASS("CameraEngineZone_Z", "CameraEngine_Z", CameraEngineZone_Z::NewObject);
    REGISTER_CLASS("ObjectMove_Z", "Manipulator_Z", ObjectMove_Z::NewObject);
    REGISTER_CLASS("ObjectThrow_Z", "ObjectMove_Z", ObjectThrow_Z::NewObject);
    REGISTER_CLASS("LodMove_Z", "ObjectThrow_Z", LodMove_Z::NewObject);
    REGISTER_CLASS("VehiculeMove_Z", "LodMove_Z", VehiculeMove_Z::NewObject);
    REGISTER_CLASS("ObjectsGame_Z", "ObjectGame_Z", ObjectsGame_Z::NewObject);
    REGISTER_CLASS("ObjectsGameClip_Z", "ObjectGame_Z", ObjectsGameClip_Z::NewObject);
    REGISTER_CLASS("Menu_Z", "ManipulatorDraw_Z", Menu3D_Z::NewObject);
    REGISTER_CLASS("Menu3D_Z", "Menu_Z", Menu3D_Z::NewObject);
    REGISTER_CLASS("Menu2D_Z", "Menu_Z", Menu2D_Z::NewObject);
    REGISTER_CLASS("RtcPlayer_Z", "Manipulator_Z", RtcPlayer_Z::NewObject);
    REGISTER_CLASS("Ghost_Z", "Manipulator_Z", Ghost_Z::NewObject);
    REGISTER_CLASS("SelfRunning_Z", "Manipulator_Z", SelfRunning_Z::NewObject);
    REGISTER_CLASS("InGameDatas_Z", "ManipulatorDraw_Z", InGameDatas_Z::NewObject);
    REGISTER_CLASS("PlayParticles_Z", "Manipulator_Z", NULL);
    REGISTER_CLASS("SuperSpray_Z", "PlayParticles_Z", SuperSpray_Z::NewObject);
    REGISTER_CLASS("InGameObjectsManipulator_Z", "Manipulator_Z", InGameObjectsManipulator_Z::NewObject);
    REGISTER_CLASS("StreamFile_Z", "Manipulator_Z", StreamFile_Z::NewObject);
    REGISTER_CLASS("ConsoleInterp_Z", "Manipulator_Z", ConsoleInterp_Z::NewObject);
    REGISTER_CLASS("ObjectsMove_Z", "ObjectMove_Z", ObjectsMove_Z::NewObject);
    REGISTER_CLASS("ObjectsBounce_Z", "ObjectsMove_Z", ObjectsBounce_Z::NewObject);
    REGISTER_CLASS("ObjectsBreak_Z", "ObjectsBounce_Z", ObjectsBreak_Z::NewObject);
    RegisterGameClasses();
}

void RegisterGlobalCommands() {
    REGISTERCOMMAND("eXit", ExitApp);
    REGISTERCOMMAND("SouRCe", Source);
    REGISTERCOMMAND("BSouRCe", BSource);
    REGISTERCOMMAND("SetFrame", SetFrame);
    REGISTERCOMMAND("Help", Help);
    REGISTERCOMMAND("Pause", Pause);
    REGISTERCOMMAND("SetDBPath", SetDBPath);
    REGISTERCOMMAND("SetDFPath", SetDFPath);
    REGISTERCOMMAND("LoadObjectLib", LoadObjectLib);
    REGISTERCOMMAND("RemoveObjectLib", RemoveObjectLib);
    REGISTERCOMMAND("LoadMaterialLib", LoadMaterialLib);
    REGISTERCOMMAND("RemoveMaterialLib", RemoveMaterialLib);
    REGISTERCOMMAND("LoadFOnt", LoadFont);
    REGISTERCOMMAND("RemoveFOnt", RemoveFont);
    REGISTERCOMMAND("LoadSysRtc", LoadSysRtc);
    REGISTERCOMMAND("RemoveSysRtc", RemoveSysRtc);
    REGISTERCOMMAND("CheckHandles", CheckHandles);
    REGISTERCOMMAND("AsynchCheckHandles", AsynchCheckHandles);
    REGISTERCOMMAND("MemoryGraphColor", (CommandProc)MemoryGraphColor);
    REGISTERCOMMAND("InitRandomSeed", InitRandomSeed);
    REGISTERCOMMAND("SetBlockFrame", SetBlockFrame);
    REGISTERCOMMAND("SetTimeFactor", SetTimeFactor);
    REGISTERCOMMAND("PrintFreeMem", PrintFreeMem);
    REGISTERCOMMANDC("SetBFPath", SetBFPath, " [Target] [Source]");
    REGISTERCOMMANDC("EnableBF", EnableBF, " [Read/Write]");
    REGISTERCOMMANDC("AddJoyStick", AddJoyStick, "Ajoute un joystick");

    RegisterGameMgrCommand();
    RegisterDebugCommand();
    RegisterGameCommands();
}

// Command functions

Bool AddJoyStick() {
    gData.InputMgr->AddDevice();
    return TRUE;
}

Bool ExitApp() {
    return FALSE;
}

Bool SetFrame() {
    return FALSE;
}

Bool SetBlockFrame() {
    return FALSE;
}

Bool PrintFreeMem() {
    MemManager.m_FreeMemCached = s_GetFreeMem();
    s_GetLargestFree();
    return TRUE;
}

Bool SetTimeFactor() {
    if (gData.Cons->GetNbParam() <= 2 && gData.Cons->IsParamFloat(1)) {
        Float l_TimeFactor = gData.Cons->GetParamFloat(1);
        if (l_TimeFactor > 0.0f && l_TimeFactor < 5.0f) {
            gData.m_TimeFactor = l_TimeFactor;
        }
    }
    return TRUE;
}

Bool InitRandomSeed() {
    if (gData.Cons->GetNbParam() >= 1 && gData.Cons->IsParamFloat(1)) {
        InitRandom(gData.Cons->GetParamFloat(1));
        return TRUE;
    }

    return FALSE;
}

// Globals methods

void Globals::InitTime() {
    if (m_TimerNotCalibrated) {
        CalibrateTimer();
    }
    m_AbsoluteTime = GetAbsoluteTime();
    m_TimerNotCalibrated = FALSE;
}

Float Globals::GetOneFrameTime() {
    if (m_GameFlag & FL_GAME_PAL) {
        return FRAME_TIME_PAL;
    }
    return FRAME_TIME_NTSC;
}

Float Globals::GetDeltaTime() {
    Float l_DeltaTime;

    if (gData.NetMgr) {
        NetMgr->FixNetTime();
        l_DeltaTime = NetMgr->GetDeltaTime();
    }
    else {
        l_DeltaTime = GetAbsoluteTime();
    }

    Float l_Fps;

    if (!gData.m_BlockFrame && m_TargetSecondsPerFrame) {
        l_DeltaTime = m_TargetSecondsPerFrame * GetOneFrameTime() + m_AbsoluteTime;
        l_Fps = l_DeltaTime - m_AbsoluteTime;
    }
    else {
        l_Fps = m_TimeFactor * (l_DeltaTime - m_AbsoluteTime);
    }

    m_AbsoluteTime = l_DeltaTime;

    if (gData.m_GameFlag & FL_GAME_PAL) {
        l_DeltaTime = Clamp(l_Fps, FRAME_TIME_PAL, 1.0f / 15.0f);
    }
    else {
        l_DeltaTime = Clamp(l_Fps, FRAME_TIME_NTSC, 1.0f / 15.0f);
    }

    return l_DeltaTime;
}

Date_Z Globals::GetDate() {
    return Date_Z();
}

Bool Globals::GetMgrSize(const Name_Z& i_Name, S32& i_Size1, S32& i_Size2) {
    i_Size1 = 0;
    i_Size2 = 0;
    return 0;
}
