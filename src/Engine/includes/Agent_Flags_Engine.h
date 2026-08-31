#ifndef _AGENTFLAGS_ENGINE_H_
#define _AGENTFLAGS_ENGINE_H_

#define FL_AGENT_UNK_0x1 ((U32)1 << 0)                 // 0x1 - Unknown
#define FL_AGENT_UNK_0x2 ((U32)1 << 1)                 // 0x2 - Unknown
#define FL_AGENT_UNK_0x4 ((U32)1 << 2)                 // 0x4 - Unknown
#define FL_AGENT_UNK_0x8 ((U32)1 << 3)                 // 0x8 - Unknown
#define FL_AGENT_UNK_0x10 ((U32)1 << 4)                // 0x10 - Unknown
#define FL_AGENT_LODDYN ((U32)1 << 5)                  // 0x20 - LodDynAgent_Z
#define FL_AGENT_UNK_0x40 ((U32)1 << 6)                // 0x40 - Unknown
#define FL_AGENT_ANIMATED ((U32)1 << 7)                // 0x80 - It's animated
#define FL_AGENT_ANIMATED_MIX ((U32)1 << 8)            // 0x100 - It's mixed anim (smoothing)
#define FL_AGENT_ANIMATED_SEQ ((U32)1 << 9)            // 0x200 - It's sequenced (rtc?/animframe?)
#define FL_AGENT_UNK_0x400 ((U32)1 << 10)              // 0x400 - Maybe skel collide
#define FL_AGENT_CAMERA ((U32)1 << 11)                 // 0x800 - CameraAgent_Z
#define FL_AGENT_PARTICLES ((U32)1 << 12)              // 0x1000 - ParticlesAgent_Z
#define FL_AGENT_RTC ((U32)1 << 13)                    // 0x2000 - RtcAgent_Z
#define FL_AGENT_NEED_SOUND ((U32)1 << 14)             // 0x4000 - Needs sound
#define FL_AGENT_UNK_0x8000 ((U32)1 << 15)             // 0x8000 - Unknown
#define FL_AGENT_THROWN ((U32)1 << 16)                 // 0x10000 - Being thrown
#define FL_AGENT_NO_SUSPEND ((U32)1 << 17)             // 0x20000 - Don't suspend
#define FL_AGENT_NO_RESTORE ((U32)1 << 18)             // 0x40000 - Don't restore
#define FL_AGENT_UNK_0x80000 ((U32)1 << 19)            // 0x80000 - Unknown
#define FL_AGENT_PAUSED ((U32)1 << 20)                 // 0x100000 - Will get executed even if ScriptMgr is paused
#define FL_AGENT_DEFAULT ((U32)1 << 21)                // 0x200000 - Unknown
#define FL_AGENT_RTC_PLAYING ((U32)1 << 22)            // 0x400000 - Rtc is playing
#define FL_AGENT_UNK_0x800000 ((U32)1 << 23)           // 0x800000 - Unknown
#define FL_AGENT_CAMERA_NO_SUSPEND ((U32)1 << 24)      // 0x1000000 - Don't suspend camera agent
#define FL_AGENT_HAS_EMITTER_FLAG_0x100 ((U32)1 << 25) // 0x2000000 - Has emitter flag 0x100
#define FL_AGENT_OBJECTMOVE_SET ((U32)1 << 26)         // 0x4000000 - ObjectMove was set for MovingAgent_Z
#define FL_AGENT_BREAKABLE ((U32)1 << 27)              // 0x8000000 - Breakable_Z
#define FL_AGENT_UPDATE_BREAK ((U32)1 << 28)           // 0x10000000 - Breakable_Z Suspend/Restore
#define FL_AGENT_SKIN ((U32)1 << 29)                   // 0x20000000 - SkinAgent_Z
#define FL_AGENT_UNK_0x40000000 ((U32)1 << 30)         // 0x40000000 - Unknown
#define FL_AGENT_ENGINE_LAST ((U32)1 << 31)            // 0x80000000 - Unknown

// Agent type flags
#define FL_AGENT_TYPE_BASE ((U32)1 << 0)                   // 0x00000001 - BaseAgent_G
#define FL_AGENT_TYPE_CREATURE ((U32)1 << 1)               // 0x00000002 - Creatures_G
#define FL_AGENT_TYPE_FRIEND ((U32)1 << 2)                 // 0x00000004 - Friends_G
#define FL_AGENT_TYPE_PLAYER ((U32)1 << 3)                 // 0x00000008 - Player_G
#define FL_AGENT_TYPE_BREAKBASE_0 ((U32)1 << 4)            // 0x00000010 - IT_BREAKBASE_G
#define FL_AGENT_TYPE_GAME_LOGIC ((U32)1 << 5)             // 0x00000020 - GameLogicAgent_G
#define FL_AGENT_TYPE_MENU_LOGIC ((U32)1 << 6)             // 0x00000040 - MenuLogicAgent_G
#define FL_AGENT_TYPE_COLLECT ((U32)1 << 7)                // 0x00000080 - IT_COLLECT_G
#define FL_AGENT_TYPE_PHYSIC_OBJ ((U32)1 << 8)             // 0x00000100 - Agent associated with a PhysicObj_G
#define FL_AGENT_TYPE_BALL ((U32)1 << 9)                   // 0x00000200 - IT_BALL
#define FL_AGENT_TYPE_ENEMY ((U32)1 << 10)                 // 0x00000400 - Enemies_G
#define FL_AGENT_TYPE_TIMED_PLATFORM ((U32)1 << 11)        // 0x00000800 - IT_TIMED_PLATFORM_G
#define FL_AGENT_TYPE_UNK_12 ((U32)1 << 12)                // 0x00001000 - Unknown
#define FL_AGENT_TYPE_UNK_13 ((U32)1 << 13)                // 0x00002000 - Unknown
#define FL_AGENT_TYPE_LIGHT_VOLUME ((U32)1 << 14)          // 0x00004000 - LightVolumeAgent_G
#define FL_AGENT_TYPE_UNK_15 ((U32)1 << 15)                // 0x00008000 - Unknown
#define FL_AGENT_TYPE_CAMERA_FX_VOLUME ((U32)1 << 16)      // 0x00010000 - Disable camera FX (CameraFxVolumeAgent_G)
#define FL_AGENT_TYPE_DYNBASE ((U32)1 << 17)               // 0x00020000 - IT_DYNBASE_G
#define FL_AGENT_TYPE_OBJECT ((U32)1 << 18)                // 0x00040000 - ObjectAgent_G
#define FL_AGENT_TYPE_BREAKBASE_1 ((U32)1 << 19)           // 0x00080000 - IT_BREAKBASE_G
#define FL_AGENT_TYPE_UNK_20 ((U32)1 << 20)                // 0x00100000 - Unknown
#define FL_AGENT_TYPE_UNK_21 ((U32)1 << 21)                // 0x00200000 - Unknown
#define FL_AGENT_TYPE_DETECT_VOLUME_0 ((U32)1 << 22)       // 0x00400000 - DetectVolumeAgent_G / HumanSeenVolumeAgent_G
#define FL_AGENT_TYPE_FLEE_COLLISION_VOLUME ((U32)1 << 23) // 0x00800000 - FleeCollisionVolumeAgent_G
#define FL_AGENT_TYPE_CARRYING ((U32)1 << 24)              // 0x01000000 - IT_Carrying / throwable family
#define FL_AGENT_TYPE_UNK_25 ((U32)1 << 25)                // 0x02000000 - Unknown
#define FL_AGENT_TYPE_DETECT_VOLUME_1 ((U32)1 << 26)       // 0x04000000 - DetectVolumeAgent_G / HumanSeenVolumeAgent_G
#define FL_AGENT_TYPE_GROUP_TRIGGER_ZONE ((U32)1 << 27)    // 0x08000000 - GroupTriggerZone_G family
#define FL_AGENT_TYPE_BALL_WALK ((U32)1 << 28)             // 0x10000000 - IT_BallWalk_G
#define FL_AGENT_TYPE_MISSION_VOLUME ((U32)1 << 29)        // 0x20000000 - MissionVolumeAgent_G
#define FL_AGENT_TYPE_BUMPER ((U32)1 << 30)                // 0x40000000 - IT_Bumper
#define FL_AGENT_TYPE_UNK_31 ((U32)1 << 31)                // 0x80000000 - GangAgent_G / RepulseAnimatedAgent_G

#endif
