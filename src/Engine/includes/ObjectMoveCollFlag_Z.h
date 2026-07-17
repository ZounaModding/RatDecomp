#ifndef _OBJECTMOVECOLLFLAG_Z_H_
#define _OBJECTMOVECOLLFLAG_Z_H_

#define COL_BOL_NONE 0
#define INT_BOL_UNK ((U64)1 << 0)                      // 0x1 - Unknown
#define INT_BOL_UNK2 ((U64)1 << 1)                     // 0x2 - Unknown
#define INT_BOL_LODMOVE ((U64)1 << 2)                  // 0x4 - It's a LodMove_Z
#define INT_BOL_DUMMY ((U64)1 << 3)                    // 0x8 - Dummy
#define INT_BOL_SHADOW ((U64)1 << 4)                   // 0x10 - Shadow
#define INT_BOL_VREFLECT ((U64)1 << 5)                 // 0x20 - VReflect
#define INT_BOL_VOLUME ((U64)1 << 6)                   // 0x40 - Volume
#define INT_BOL_TYPE (INT_BOL_LODMOVE | INT_BOL_DUMMY) // 0xC - Type mask (LodMove_Z or Dummy)

#define COL_BOL_NODE_ORIENT ((U64)1 << 7)             // 0x80
#define COL_BOL_NO_SHADOW ((U64)1 << 8)               // 0x100
#define COL_BOL_NO_COLLIDE_NODE ((U64)1 << 9)         // 0x200
#define COL_BOL_LOD ((U64)1 << 10)                    // 0x400 - Test collision with other Lod_Z in scene / UpdateCollision()
#define COL_BOL_DECOR ((U64)1 << 11)                  // 0x800 - Test collision with scene except Lod_Z
#define COL_BOL_SPLINE ((U64)1 << 12)                 // 0x1000 - Test collision with spline collide from patch
#define COL_BOL_VOLUME ((U64)1 << 13)                 // 0x2000 - Volume
#define COL_BOL_SHADOW ((U64)1 << 14)                 // 0x4000 - Shadow
#define COL_BOL_DYNAMIC ((U64)1 << 15)                // 0x8000 - Use LOD STRENGTH sphere to apply spherical forces to nearby animated objects with FL_MTL_COL_DYNAMIC enabled on its LOD primitives
#define COL_BOL_REFLECT ((U64)1 << 16)                // 0x10000 - V/H Reflect
#define COL_BOL_UPDATE_GROUND_LIGHTING ((U64)1 << 17) // 0x20000 - Update lighting from ground
#define COL_BOL_DEPTH ((U64)1 << 18)                  // 0x40000 - Run Depth test
#define COL_BOL_SCAN_CLING ((U64)1 << 21)             // 0x200000 - Scan Cling edge
#define COL_BOL_LAND_AT_CLING ((U64)1 << 22)          // 0x400000 - Resolve cling landing at grab time. If clear, grab is unconditional and the landing is recomputed/validated on each climb-up (EndCollisionNode)
#define COL_BOL_FIT_GROUND ((U64)1 << 24)             // 0x1000000 - Collision is just a downwards raycast

// Goto
#define COL_BOL_GOTOPOS ((U64)1 << 30)        // 0x40000000 - ComputeNewPos is done by a Spline_Z or by 2 points and 2 tangents
#define COL_BOL_GOTOROT ((U64)1 << 31)        // 0x80000000 - The rotation is interpolated (when set, GotoPos direction comes from the movement)
#define COL_BOL_GOTOPOS_SPLINE ((U64)1 << 32) // 0x100000000 - GotoPos is from a Spline_Z object, other case is 2 points & 2 tangents
#define COL_BOL_GOTOREL ((U64)1 << 33)        // 0x200000000 - Spline is relative to the beginning
#define COL_BOL_GOTOREL_SMOOTH ((U64)1 << 34) // 0x400000000 - Smooth start position and Spline_Z scene position
#define COL_BOL_GOTO_ID ((U64)1 << 35)        // 0x800000000 - Then the message "msg_goto_2nd_vertex" will be sent as the 2nd point is passed
#define COL_BOL_GOTO (COL_BOL_GOTO_ID | COL_BOL_GOTOPOS | COL_BOL_GOTOROT | COL_BOL_GOTOPOS_SPLINE | COL_BOL_GOTOREL | COL_BOL_GOTOREL_SMOOTH)

#define COL_BOL_CLING ((U64)1 << 36)             // 0x1000000000 - Cling attached
#define COL_BOL_THROWN ((U64)1 << 38)            // 0x4000000000 - Collision when being thrown
#define COL_BOL_HDEPL_EPS ((U64)1 << 47)         // 0x800000000000 - Hdepl eps
#define COL_BOL_ABORTED ((U64)1 << 51)           // 0x8000000000000 - Aborted
#define COL_BOL_DEPTH_OK ((U64)1 << 54)          // 0x40000000000000 - Depth updated
#define COL_BOL_DEPTH_PENETRATION ((U64)1 << 55) // 0x80000000000000 - Is inside the collision depth (penetration)
#define COL_BOL_FORCE_ANIM ((U64)1 << 57)        // 0x200000000000000 - Force anim mapping

// Each frame clean bits (bits kept by ObjectMove_Z::ComputeNewPos)
#define COL_BOL_DEFAULT (COL_BOL_SHADOW | COL_BOL_DYNAMIC | COL_BOL_REFLECT | COL_BOL_UPDATE_GROUND_LIGHTING)
#define COL_BOL_CLEAN (U64)0x3FFFFFFFC7C

#endif
