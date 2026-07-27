#ifndef _DEBUGTOOLS_Z_H_
#define _DEBUGTOOLS_Z_H_

#include "Types_Z.h"

// Commands

void RegisterDebugCommand();

Bool DisplayMemStatus();
Bool DrawMemGraph();
Bool DeltaTime();
Bool ScreenShot();
Bool SwitchScreenMode();
Bool SetGammaRamp();
Bool SetBrightness();
Bool SetContrast();
Bool SetGAmma();
Bool SwapStartButton();
Bool BoxPatchClip();
Bool SetPosPerso();
Bool EnablePopupMenu();
Bool EnableProfiler();
Bool EnableDmaProfiler();
Bool EnableDebugMouse();
Bool DisableMouseForCamera();
Bool CleanTempPic();
Bool DisplayFps();
Bool EnableVSync();
Bool DisplayStream();
Bool DoSkelDynamic();
Bool DisplayFrame();
Bool DisableConsole();
Bool DisableIngameConsole();
Bool DisableAssert();
Bool DisableMusic();
Bool DisableMovie();
Bool SkipMovie();
Bool EnableL2R2();
Bool CheckMemoryEveryFrame();
Bool NbHandle();
Bool EnableBloomEffect();
Bool DrawMemInfos();

// Flags

enum EngineFlags {
    FL_ENGINE_NONE = 0 << 0,
    FL_FULLSCREEN = 1 << 0,
    FL_WIREFRAME = 1 << 1,
    FL_PROFILER = 1 << 2,
    FL_POS_PERSO = 1 << 3,
    FL_DISPLAY_FPS = 1 << 4,
    FL_DISABLE_CONSOLE = 1 << 6,
    FL_DISABLE_ASSERT = 1 << 7,
    FL_DISABLE_MUSIC = 1 << 8,
    FL_DISABLE_MOVIE = 1 << 9,
    FL_INGAME_CONSOLE = 1 << 11,
    FL_UNK_0x1000 = 1 << 12,
    FL_ENABLE_L2R2 = 1 << 13,
    FL_OCCLUDER_ON = 1 << 14,
    FL_MEMORY_VERIFY = 1 << 16,
    FL_STREAMZONE = 1 << 17,
    FL_UNK_0x40000 = 1 << 18,
    FL_POPUP_MENU = 1 << 20,
    FL_STACK_TRACE = 1 << 21,
    FL_MEMORY_INFOS = 1 << 22,
    FL_NO_TEXTURE = 1 << 23,
    FL_NO_OMNI = 1 << 24,
    FL_1X1_TEXTURE = 1 << 25,
    FL_1_TRIANGLE = 1 << 26,
    FL_LOCK_VIEW = 1 << 27,
    FL_DEBUG_SOUND = 1 << 29,
    FL_ENGINE_DEFAULT = FL_UNK_0x1000 | FL_OCCLUDER_ON | FL_UNK_0x40000
};

enum DebugFlags {
    FL_DEBUG_OCCLUDER_INV = 1 << 0,
    FL_DEBUG_NORMALS = 1 << 1,
    FL_DEBUG_SCENE_NODE = 1 << 2,
    FL_DEBUG_BSPHERE = 1 << 3,
    FL_DEBUG_INV_CULLCONE = 1 << 4,
    FL_DEBUG_NO_SOUND = 1 << 5,
    FL_DEBUG_SPLINE_COLLIDE = 1 << 6,
    FL_DEBUG_OCCLUDER_OUT = 1 << 7,
    FL_DEBUG_SKIN_STRIP = 1 << 8,
    FL_DEBUG_HEADLIGHT = 1 << 9,
    FL_DEBUG_HIDEPATCH0 = 1 << 10,
    FL_DEBUG_SLIDING_SURFACE = 1 << 11,
    FL_DEBUG_CULLCONE = 1 << 12,
    FL_DEBUG_SPLINE_CLING = 1 << 13,
    FL_DEBUG_LOD = 1 << 15,
    FL_DEBUG_BBOX = 1 << 16,
    FL_DEBUG_NO_FOG = 1 << 19,
    FL_DEBUG_SHADOWVOLUME = 1 << 20,
    FL_DEBUG_CLIPPING = 1 << 24,
    FL_DEBUG_ANIMATION = 1 << 25,
    FL_DEBUG_OBJECT = 1 << 26,
    FL_DEBUG_NODE = 1 << 27,
    FL_DEBUG_SKEL_NODE = 1 << 28,
    FL_DEBUG_RESOURCE = 1 << 29,
    FL_DEBUG_FADE_PERCENT = 1 << 30,
    FL_DEBUG_BONE_NODE = 1 << 31
};

enum CollisionFlags {
    FL_COLLISION_RAY = 1 << 0,
    FL_COLLISION_SPHERE = 1 << 1,
    FL_COLLISION_SKELETON = 1 << 2,
    FL_COLLISION_BOX_HIT = 1 << 3,
    FL_COLLISION_SPHERE_HIT = 1 << 4,
    FL_COLLISION_FACE = 1 << 5,
    FL_COLLISION_MOVING_SPHERE = 1 << 10,
    FL_COLLISION_CAPSULE = 1 << 11,
    FL_COLLISION_SPH_BOX_CYL = 1 << 13
};

enum MaterialFlags {
    FL_MATERIAL_DIFFUSE = 1 << 0,
    FL_MATERIAL_ENVMAP = 1 << 1,
    FL_MATERIAL_NORMAL_MAP = 1 << 2,
    FL_MATERIAL_SPECULAR_MAP = 1 << 3,
    FL_MATERIAL_ALL = FL_MATERIAL_DIFFUSE | FL_MATERIAL_ENVMAP | FL_MATERIAL_NORMAL_MAP | FL_MATERIAL_SPECULAR_MAP
};

enum GameFlags {
    FL_GAME_NONE = 0 << 0,           // 0
    FL_GAME_UNK_0x1 = 1 << 0,        // 0x1
    FL_GAME_PAL = 1 << 7,            // 0x80
    FL_GAME_UNK_0x100 = 1 << 8,      // 0x100
    FL_GAME_SCRIPT_ANY_FS = 1 << 10, // 0x400 - Can load .tsc from any filesystem
    FL_GAME_USE_CD = 1 << 11,        // 0x800 - Use CD for file loading
    FL_GAME_UNK_0x1000 = 1 << 12,    // 0x1000
    FL_GAME_UNK_0x2000 = 1 << 13,    // 0x2000
    FL_GAME_UNK_0x4000 = 1 << 14,    // 0x4000
    FL_GAME_2_FRAMES = 1 << 15,      // 0x8000
    FL_GAME_UNK_0x20000 = 1 << 17,   // 0x20000
    FL_GAME_UNK_0x40000 = 1 << 18    // 0x40000
};

#endif // _DEBUGTOOLS_Z_H_
