#ifndef _NODEFLAG_Z_H_
#define _NODEFLAG_Z_H_

#define FL_NODE_SCAN (1 << 0)             // 0x1 - Do recursion, used if a Node_Z in the branch needs update
#define FL_NODE_UNK_0x2 (1 << 1)          // 0x2 - Unknown
#define FL_NODE_UNK_0x4 (1 << 2)          // 0x4 - Unknown
#define FL_NODE_UNK_0x8 (1 << 3)          // 0x8 - Unknown
#define FL_NODE_UPDATE (1 << 4)           // 0x10 - Matrix needs update (Supersedes FL_NODE_SCAN and FL_NODE_UPDATE_OBJECT)
#define FL_NODE_UPDATE_LIGHTING (1 << 5)  // 0x20 - Lighting needs update
#define FL_NODE_UPDATE_OBJECT (1 << 6)    // 0x40 - Object needs update (Supersedes FL_NODE_SCAN, does TRS update)
#define FL_NODE_INVALIDMAT (1 << 7)       // 0x80 - Invalid WorldMatrix (union bs pointer) (calculate inverse on next GetInverseWorldMatrix call)
#define FL_NODE_INVALIDROT (1 << 8)       // 0x100 - Invalid m_RotInWorld (calculate inverse on next GetInverseRotInWorld call)
#define FL_NODE_ANIMATED (1 << 9)         // 0x200 - Has animation
#define FL_NODE_NO_OMNI (1 << 10)         // 0x400 - Not lit by omni lights (Omni_Z)
#define FL_NODE_NO_OCCLUDER (1 << 11)     // 0x800 - Not occluded by occluder (Occluder_Z)
#define FL_NODE_NO_AGENT (1 << 12)        // 0x1000 - No agent created if set
#define FL_NODE_SEQUENCED (1 << 13)       // 0x2000 - LodAgent_Z if set, AnimatedAgent_Z if not set. Only used by nodes with Lod_Z as object
#define FL_NODE_SKINNED (1 << 14)         // 0x4000 - Is skinned (lod/skin)
#define FL_NODE_UNCOLLIDED (1 << 15)      // 0x8000 - Collision got disabled
#define FL_NODE_NO_SEADCOLLIDE (1 << 16)  // 0x10000 - No collision SEAD -> no collide at all?
#define FL_NODE_NO_SEADDISPLAY (1 << 17)  // 0x20000 - No display SEAD -> no drawing at all?
#define FL_NODE_HIDE (1 << 18)            // 0x40000 - Hidden
#define FL_NODE_USER_LOCK (1 << 19)       // 0x80000 - Assumed through usage in ObjectMove_Z::ChangeLink (if set, won't change) and asobo ACE video (UserLock)
#define FL_NODE_VP0_HIDE (1 << 20)        // 0x100000 - Hide in Viewport Id 0
#define FL_NODE_VP1_HIDE (1 << 21)        // 0x200000 - Hide in Viewport Id 1
#define FL_NODE_VP2_HIDE (1 << 22)        // 0x400000 - Hide in Viewport Id 2
#define FL_NODE_VP3_HIDE (1 << 23)        // 0x800000 - Hide in Viewport Id 3
#define FL_NODE_NO_SHARED (1 << 24)       // 0x1000000 - No shared data, doesn't use ObjectData_Z if set
#define FL_NODE_UNK_0x2000000 (1 << 25)   // 0x2000000 - Used in World_Z::DrawObjects. If it isn't set in that func, some items/collectibles stop their logic/physics
#define FL_NODE_COLLIDE (1 << 26)         // 0x4000000 - Do collision
#define FL_NODE_SHADOW (1 << 27)          // 0x8000000 - Does shadow update if set. Only used by nodes with Lod_Z as object
#define FL_NODE_SEQUENCED_ABORT (1 << 28) // 0x10000000 - Abort AnimFrame_Z sequence
#define FL_NODE_SPECIAL_VISION (1 << 29)  // 0x20000000 - Colored with the special vision color if using Smell-O vision

#endif
