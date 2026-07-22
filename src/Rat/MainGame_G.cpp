#include "MainGame_G.h"

#include "ActionHelper_G.h"
#include "CameraUser_Z.h"
#include "ClassManager_Z.h"
#include "ScriptInputManager_Z.h"
#include "FootPrints_G.h"
#include "IT_Condition.h"
#include "InGameFX_G.h"
#include "LiquidFlow_G.h"
#include "LoadingDraw_G.h"
#include "MemoryCardMgr_G.h"
#include "MissionManager_G.h"
#include "MusicManager_G.h"
#include "PaddleChecker_G.h"
#include "PermanentDraw.h"
#include "PersoLight_G.h"
#include "PointJump_G.h"
#include "Program_Z.h"
#include "Ropes_G.h"
#include "ScriptManager_G.h"
#include "Smell_G.h"
#include "TextGameDraw_G.h"
#include "VibrationManager_G.h"
#include "WanderingPath_G.h"
#include "UserCommand_G.h"

void ScriptInputManager_Z::Update(Float i_DeltaTime) { }

void GameProgramInit() {
    gData.ScriptInputMgr = NewL_Z(96) ScriptInputManager_Z;

    ScriptManager_G* l_ScriptManager = NewL_Z(99) ScriptManager_G;
    gData.ScriptMgr = l_ScriptManager;
    l_ScriptManager->GetCTFGameMgr().GetInputDef()->InitInputs();
}

void RegisterGameClasses() {
    REGISTER_CLASS("CameraUser_Z", "Manipulator_Z", CameraUser_Z::NewObject);

    // PlayerMove_G through MenuManager_G
    REGISTER_CLASS("LoadingDraw_C", "ManipulatorDraw", LoadingDraw_C::NewObject);
    REGISTER_CLASS("PermanentDraw", "ManipulatorDraw", PermanentDraw::NewObject);
    REGISTER_CLASS("TextGameDraw_G", "ManipulatorSceneDraw_Z", TextGameDraw_G::NewObject);
    REGISTER_CLASS("ActionHelper_G", "ManipulatorSceneDraw_Z", ActionHelper_G::NewObject);
    REGISTER_CLASS("FootPrints_G", "ManipulatorSceneDraw_Z", FootPrints_G::NewObject);
    REGISTER_CLASS("Ropes_G", "ManipulatorSceneDraw_Z", Ropes_G::NewObject);
    REGISTER_CLASS("PointJump_G", "Manipulator_Z", PointJump_G::NewObject);
    REGISTER_CLASS("Smell_G", "ManipulatorSceneDraw_Z", Smell_G::NewObject);
    REGISTER_CLASS("IT_Condition", "Manipulator_Z", IT_Condition::NewObject);
    REGISTER_CLASS("LiquidFlow_G", "ManipulatorSceneDraw_Z", LiquidFlow_G::NewObject);
    REGISTER_CLASS("PaddleChecker_G", "Manipulator", PaddleChecker_G::NewObject);

    // IT_COLLECTMove_G through RtcScripted_G
    REGISTER_CLASS("MemoryCardManager_C", "Manipulator", MemoryCardManager_C::NewObject);
    REGISTER_CLASS("VibrationManager_C", "Manipulator", VibrationManager_C::NewObject);
    REGISTER_CLASS("MusicManager_G", "Manipulator", MusicManager_G::NewObject);

    // ThrownMove_G through IT_CarryingMove
    REGISTER_CLASS("InGameFX_G", "ManipulatorSceneDraw_Z", InGameFX_G::NewObject);
    REGISTER_CLASS("PersoLight_G", "Manipulator", PersoLight_G::NewObject);

    // CreaturesBoneManip and IT_BreakMove
    REGISTER_CLASS("MissionManager_G", "Manipulator", MissionManager_G::NewObject);

    // CreaturesGroupMove_G through LevelManipulator_G
    REGISTER_CLASS("WanderingPath_G", "BaseObject_Z", WanderingPath_G::NewObject);

    // GusteauMove through DialogGroup_G
}

void RegisterGameCommands() {
    RegisterUserCommand();
}
