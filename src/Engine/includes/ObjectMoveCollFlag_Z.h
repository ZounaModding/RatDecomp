#ifndef _OBJECTMOVECOLLFLAG_Z_H_
#define _OBJECTMOVECOLLFLAG_Z_H_

#define COL_BOL_NONE 0
#define INT_BOL_OBJECTSMOVE ((U64)1 << 0)                                                                    // 0x1 - It's an ObjectsMove_Z
#define INT_BOL_OBJECTSBREAK ((U64)1 << 1)                                                                   // 0x2 - It's an ObjectsBreak_Z
#define INT_BOL_LODMOVE ((U64)1 << 2)                                                                        // 0x4 - It's a LodMove_Z
#define INT_BOL_PLAYER ((U64)1 << 3)                                                                         // 0x8 - It's the player move object
#define INT_BOL_SHADOW ((U64)1 << 4)                                                                         // 0x10 - Saved COL_BOL_SHADOW: ObjectThrow_Z::SetCarrying parks the carried object's shadow pref here and restores it on release
#define INT_BOL_VREFLECT ((U64)1 << 5)                                                                       // 0x20 - Saved COL_BOL_REFLECT while the object is carried (see SetCarrying)
#define INT_BOL_VOLUME ((U64)1 << 6)                                                                         // 0x40 - Saved COL_BOL_VOLUME while the object is carried (see SetCarrying)
#define INT_BOL_KEEP (INT_BOL_LODMOVE | INT_BOL_PLAYER)                                                      // 0xC - Identity bits kept by ObjectMove_Z::Reset (LodMove_Z or player object)
#define INT_COL_TYPE (INT_BOL_LODMOVE | INT_BOL_PLAYER | INT_BOL_SHADOW | INT_BOL_VREFLECT | INT_BOL_VOLUME) // 0x7C - All intern type bits (kept each frame)

#define COL_BOL_NODE_ORIENT ((U64)1 << 7)             // 0x80 - This frame the orientation came from the linked collide node (set by GetCollisionNodeDepl and CreaturesMove_G::Update)
#define COL_BOL_NO_SHADOW ((U64)1 << 8)               // 0x100 - Skip shadow processing; set with COL_BOL_DECOR cleared and speed zeroed when ObjectsBounce_Z::UpdateCollision puts a bounced object to rest
#define COL_BOL_NO_COLLIDE_NODE ((U64)1 << 9)         // 0x200 - Never used; only cleared each frame by ComputeNewPos
#define COL_BOL_LOD ((U64)1 << 10)                    // 0x400 - Test collision with other Lod_Z in scene / UpdateCollision()
#define COL_BOL_DECOR ((U64)1 << 11)                  // 0x800 - Test collision with scene except Lod_Z
#define COL_BOL_SPLINE ((U64)1 << 12)                 // 0x1000 - Test collision with spline collide from patch
#define COL_BOL_VOLUME ((U64)1 << 13)                 // 0x2000 - Run in/out tests against scene volumes (UpdateCollisionVolume, tracked in m_CollisionVolNodeHdls; sends the volume in/out messages)
#define COL_BOL_SHADOW ((U64)1 << 14)                 // 0x4000 - Run the drop-shadow ground raycast (UpdateShadow / UpdateCollisionShadow)
#define COL_BOL_DYNAMIC ((U64)1 << 15)                // 0x8000 - Use LOD STRENGTH sphere to apply spherical forces to nearby animated objects with FL_MTL_COL_DYNAMIC enabled on its LOD primitives
#define COL_BOL_REFLECT ((U64)1 << 16)                // 0x10000 - V/H Reflect
#define COL_BOL_UPDATE_GROUND_LIGHTING ((U64)1 << 17) // 0x20000 - Update lighting from ground
#define COL_BOL_DEPTH ((U64)1 << 18)                  // 0x40000 - Run Depth test
#define COL_BOL_COLLECTABLE ((U64)1 << 19)            // 0x80000 - Calls UpdateCollisionCollectable before collision update
#define COL_BOL_UPDATE_CARRY_COLLISION ((U64)1 << 20) // 0x100000 - Calls UpdateCollisionCarrying after collision update
#define COL_BOL_SCAN_CLING ((U64)1 << 21)             // 0x200000 - Scan Cling edge
#define COL_BOL_LAND_AT_CLING ((U64)1 << 22)          // 0x400000 - Resolve cling landing at grab time. If clear, grab is unconditional and the landing is recomputed/validated on each climb-up (EndCollisionNode)
#define COL_BOL_CARRIABLE_FRONT ((U64)1 << 23)        // 0x800000 - Carried object adopts carrier orientation instead of its local oriented rotation
#define COL_BOL_FIT_GROUND ((U64)1 << 24)             // 0x1000000 - Collision is just a downwards raycast
#define COL_BOL_BREAK_ROTATE ((U64)1 << 25)           // 0x2000000 - Rotates a breaking object from its speed
#define COL_BOL_BREAK_SCALE ((U64)1 << 26)            // 0x4000000 - Scales a breaking object down during its fade
#define COL_BOL_BREAK_FADE ((U64)1 << 27)             // 0x8000000 - Breaking object will fade
#define COL_BOL_DUR_ADD ((U64)1 << 28)                // 0x10000000 - Adds this LodMove's DUR_ADD sphere to collision tests
#define COL_BOL_DUR_ADD_CARRY ((U64)1 << 29)          // 0x20000000 - Adds the carried object's DUR sphere to collision tests

// Pref bits, set from the object setup / Init() / Reset()
#define COL_BOL_PREF (COL_BOL_LOD | COL_BOL_DECOR | COL_BOL_SPLINE | COL_BOL_VOLUME | COL_BOL_SHADOW | COL_BOL_DYNAMIC | COL_BOL_REFLECT | COL_BOL_UPDATE_GROUND_LIGHTING | COL_BOL_DEPTH | COL_BOL_COLLECTABLE | COL_BOL_UPDATE_CARRY_COLLISION | COL_BOL_SCAN_CLING | COL_BOL_LAND_AT_CLING | COL_BOL_CARRIABLE_FRONT | COL_BOL_FIT_GROUND | COL_BOL_BREAK_ROTATE | COL_BOL_BREAK_SCALE | COL_BOL_BREAK_FADE | COL_BOL_DUR_ADD | COL_BOL_DUR_ADD_CARRY)

// Goto
#define COL_BOL_GOTOPOS ((U64)1 << 30)        // 0x40000000 - ComputeNewPos is done by a Spline_Z or by 2 points and 2 tangents
#define COL_BOL_GOTOROT ((U64)1 << 31)        // 0x80000000 - The rotation is interpolated (when set, GotoPos direction comes from the movement)
#define COL_BOL_GOTOPOS_SPLINE ((U64)1 << 32) // 0x100000000 - GotoPos is from a Spline_Z object, other case is 2 points & 2 tangents
#define COL_BOL_GOTOREL ((U64)1 << 33)        // 0x200000000 - Spline is relative to the beginning
#define COL_BOL_GOTOREL_SMOOTH ((U64)1 << 34) // 0x400000000 - Smooth start position and Spline_Z scene position
#define COL_BOL_GOTO_ID ((U64)1 << 35)        // 0x800000000 - Then the message "msg_goto_2nd_vertex" will be sent as the 2nd point is passed
#define COL_BOL_GOTO (COL_BOL_GOTO_ID | COL_BOL_GOTOPOS | COL_BOL_GOTOROT | COL_BOL_GOTOPOS_SPLINE | COL_BOL_GOTOREL | COL_BOL_GOTOREL_SMOOTH)

#define COL_BOL_CLING ((U64)1 << 36)                       // 0x1000000000 - Cling attached
#define COL_BOL_CARRY_LINKED ((U64)1 << 37)                // 0x2000000000 - Carry-link interpolation completed; prevents resending msg 0x40
#define COL_BOL_THROWN ((U64)1 << 38)                      // 0x4000000000 - Collision when being thrown
#define COL_BOL_STUCK_TO_GROUND ((U64)1 << 39)             // 0x8000000000 - Uses m_StuckToGroundVectorLength for ground collision
#define COL_BOL_RESTORE ((U64)1 << 40)                     // 0x10000000000 - Restores collision when the collision sphere is clear
#define COL_BOL_NO_AUTO_LINK_COLLISION_NODE ((U64)1 << 41) // 0x20000000000 - Suppresses automatic InitLinkCollisionNode

// Bits set during the main loop
#define COL_BOL_SET (COL_BOL_GOTO | COL_BOL_CLING | COL_BOL_CARRY_LINKED | COL_BOL_THROWN | COL_BOL_STUCK_TO_GROUND | COL_BOL_RESTORE | COL_BOL_NO_AUTO_LINK_COLLISION_NODE)

#define COL_BOL_STUCK_TO_GROUND_OK ((U64)1 << 42)   // 0x40000000000 - This frame found ground while STUCK_TO_GROUND was set
#define COL_BOL_WITH_GROUND ((U64)1 << 43)          // 0x80000000000 - This frame has an accepted ground/floor collision
#define COL_BOL_CEILING_COLLISION ((U64)1 << 44)    // 0x100000000000 - This frame collided with an underside/ceiling
#define COL_BOL_WALL_COLLISION ((U64)1 << 45)       // 0x200000000000 - This frame collided with a near-vertical surface
#define COL_BOL_FLOOR_COLLISION ((U64)1 << 46)      // 0x400000000000 - Set with WITH_GROUND for an accepted floor collision
#define COL_BOL_HDEPL_EPS ((U64)1 << 47)            // 0x800000000000 - Horizontal displacement this frame was below epsilon (barely moved); set by UpdateCollision, checked by GetCollisionNodeDepl
#define COL_BOL_REFLEX_COLLISION ((U64)1 << 48)     // 0x1000000000000 - Passes GetReflexLimitAngle; computes m_HorizontalColDir
#define COL_BOL_SLOPE ((U64)1 << 49)                // 0x2000000000000 - Set when collision normal y is below GetSLidingSlopeAngle()
#define COL_BOL_ABORTIVE ((U64)1 << 50)             // 0x4000000000000 - Never used; only cleared each frame by ComputeNewPos
#define COL_BOL_ABORTED ((U64)1 << 51)              // 0x8000000000000 - Never set; only tested by ObjectsGame_Z::UpdateObject, which would run ComputeNewPos with a zero delta time (frozen), so that branch is dead
#define COL_BOL_HAS_ABORTED ((U64)1 << 52)          // 0x10000000000000 - Never used; only cleared each frame by ComputeNewPos
#define COL_BOL_MOVING_SPHERE_JAMMED ((U64)1 << 53) // 0x20000000000000 - Set-only diagnostic: LodMove_Z::UpdateCollisionMovingSphere failed to resolve the push-out within 20 iterations (jammed between moving spheres). Never tested
#define COL_BOL_DEPTH_OK ((U64)1 << 54)             // 0x40000000000000 - This frame's depth probe found a depth surface (set/tested by UpdateDepth)
#define COL_BOL_DEPTH_PENETRATION ((U64)1 << 55)    // 0x80000000000000 - Is inside the collision depth (penetration)
#define COL_BOL_FORCE ((U64)1 << 56)                // 0x100000000000000 - Vestigial boost-pad feature, no material bit, no reference in Rat
#define COL_BOL_FORCE_ANIM ((U64)1 << 57)           // 0x200000000000000 - Vestigial conveyor feature: consumer in CreaturesMove_G::Update_Master remains, but no code translates an FL_MTL_COL_FORCE_ANIM hit into this

// Default pref bits, set in Init() and Reset()
#define COL_BOL_DEFAULT (COL_BOL_SHADOW | COL_BOL_DYNAMIC | COL_BOL_REFLECT | COL_BOL_UPDATE_GROUND_LIGHTING)

// Bits kept each frame by ObjectMove_Z::ComputeNewPos
#define COL_BOL_CLEAN (INT_COL_TYPE | COL_BOL_PREF | COL_BOL_SET)

#endif
