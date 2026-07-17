#ifndef _MATERIALFLAG_Z_H_
#define _MATERIALFLAG_Z_H_

// Material flags

// Render flags (and Code)

// Used for shaders and other material specific rendering logic
enum MaterialCode_Z {
    FL_MTL_CODE_DIFFUSE = 0,
    FL_MTL_CODE_ENVMAP_ALPHA = 1, // Set to use alpha mask for envmap
    FL_MTL_CODE_ENVMAP_SCREEN = 2,
    FL_MTL_CODE_UNK3 = 3,
    FL_MTL_CODE_UNK4 = 4,
    FL_MTL_CODE_EAU = 5, // Water
    FL_MTL_CODE_LAYER = 6,
    FL_MTL_CODE_EAU_DYNAMIC = 7, // Dynamic water
    FL_MTL_CODE_WATER_OCEAN = 8,
    FL_MTL_CODE_UNK9 = 9,
    FL_MTL_CODE_UNK10 = 10,
    FL_MTL_CODE_ZONLY = 11,
    FL_MTL_CODE_BLOOM = 12,
    FL_MTL_CODE_REFRACTION = 13, // Left over from CTSF (Optical camouflage)
    FL_MTL_CODE_UNK14 = 14,      // This might be heat vision from CTSF
    FL_MTL_CODE_SONAR = 15,      // Left over from CTSF (Sonar effect)
    FL_MTL_CODE_STENCIL = 16,
    FL_MTL_CODE_UNK17 = 17,
    FL_MTL_CODE_LAST = 17,
    FL_MTL_CODE_COUNT
};

#define FL_MTL_CODE_ALL (FL_MTL_CODE_ENVMAP_ALPHA | FL_MTL_CODE_ENVMAP_SCREEN | FL_MTL_CODE_UNK4 | FL_MTL_CODE_WATER_OCEAN | FL_MTL_CODE_STENCIL) // 0x1f - All bits used for code

#define FL_MTL_RDR_IS_ALPHABLENDED (1 << 5)     // 0x20 - Alpha blended material
#define FL_MTL_RDR_IS_TRANSPARENT (1 << 6)      // 0x40 - Transparent material
#define FL_MTL_RDR_INVISIBLE (1 << 7)           // 0x80 - Invisible material, not rendered at all
#define FL_MTL_RDR_TEX_ADDRESS_CLAMP_U (1 << 8) // 0x100 - Clamp U texture coordinates
#define FL_MTL_RDR_TEX_ADDRESS_CLAMP_V (1 << 9) // 0x200 - Clamp V texture coordinates
#define FL_MTL_RDR_STENCIL_OCCLUDER (1 << 10)   // 0x400 - Stencil occluder
#define FL_MTL_RDR_STENCIL_OCCLUDEE (1 << 11)   // 0x800 - Stencil occludee
#define FL_MTL_RDR_ADDITIF (1 << 12)            // 0x1000 - Additive blending
#define FL_MTL_RDR_SOUSTRACTIF (1 << 13)        // 0x2000 - Subtractive blending
#define FL_MTL_RDR_DESTADDITIF (1 << 14)        // 0x4000 - Destination additive blending
#define FL_MTL_RDR_UNK15 (1 << 15)              // 0x8000 - Unknown
#define FL_MTL_RDR_UNK16 (1 << 16)              // 0x10000 - Unknown
#define FL_MTL_RDR_TWO_SIDE (1 << 17)           // 0x20000 - Two-sided material, disables backface culling
#define FL_MTL_RDR_RESERVED_0x40000 (1 << 18)   // 0x40000 - Reserved
#define FL_MTL_RDR_RESERVED_0x80000 (1 << 19)   // 0x80000 - Reserved
#define FL_MTL_RDR_TRANSP_ATFIRST (1 << 20)     // 0x100000 - Unknown
#define FL_MTL_RDR_UNK_0x200000 (1 << 21)       // 0x200000 - Unknown
#define FL_MTL_RDR_UNK_0x400000 (1 << 22)       // 0x400000 - Unknown
#define FL_MTL_RDR_UNK_0x800000 (1 << 23)       // 0x800000 - Unknown (Default)
#define FL_MTL_RDR_UNK_0x1000000 (1 << 24)      // 0x1000000 - Unknown
#define FL_MTL_RDR_UNK_0x2000000 (1 << 25)      // 0x2000000 - Unknown
#define FL_MTL_RDR_UNK_0x4000000 (1 << 26)      // 0x4000000 - Unknown
#define FL_MTL_RDR_UNK_0x8000000 (1 << 27)      // 0x8000000 - Unknown
#define FL_MTL_RDR_UNK_0x10000000 (1 << 28)     // 0x10000000 - Unknown
#define FL_MTL_RDR_UNK_0x20000000 (1 << 29)     // 0x20000000 - Unknown

#define FL_MTL_RDR_BLEND_MASK (FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_IS_TRANSPARENT | FL_MTL_RDR_ADDITIF | FL_MTL_RDR_SOUSTRACTIF | FL_MTL_RDR_DESTADDITIF | FL_MTL_RDR_TWO_SIDE) // 0x27060 - Blend/cull-related material flags cached by SetRenderBlendOp
#define FL_MTL_RDR_TRANSP_RESET ~(FL_MTL_RDR_IS_ALPHABLENDED | FL_MTL_RDR_IS_TRANSPARENT)

// Collision flags

#define FL_MTL_COL_CLING (1 << 0)          // 0x1 - (ACCROCHE) Material can obstruct or serve as landing when cling (ledge grab) (Default)
#define FL_MTL_COL_COLLISION (1 << 1)      // 0x2 - Collidable (Default)
#define FL_MTL_COL_COLLISIONCAM (1 << 2)   // 0x4 - Collidable with cam (Default)
#define FL_MTL_COL_DEPTH (1 << 3)          // 0x8 - Depth (used for surface penetration test, for example with water)
#define FL_MTL_COL_FORCE_ANIM (1 << 4)     // 0x10 - Set collision Mapping regarding Material Keyframe "m_ForceAnimMappingWorldDir"
#define FL_MTL_COL_ANIMATED (1 << 5)       // 0x20 - Play anim frame on node when colliding
#define FL_MTL_COL_VREFLECT (1 << 6)       // 0x40 - Used for vertical planar reflection of LOD
#define FL_MTL_COL_HREFLECT (1 << 7)       // 0x80 - Used for horizontal planar reflection of LOD
#define FL_MTL_COL_NOSHADOW_PATCH (1 << 8) // 0x100 - Don't cast shadow on patch
#define FL_MTL_COL_NOSHADOW_LOD (1 << 9)   // 0x200 - Don't cast shadow on lod
#define FL_MTL_COL_UNK_0x400 (1 << 10)     // 0x400 - Unknown
#define FL_MTL_COL_UNK_0x800 (1 << 11)     // 0x800 - Unknown
#define FL_MTL_COL_UNK_0x1000 (1 << 12)    // 0x1000 - Unknown
#define FL_MTL_COL_UNK_0x2000 (1 << 13)    // 0x2000 - Unknown
#define FL_MTL_COL_UNK_0x4000 (1 << 14)    // 0x4000 - Unknown
#define FL_MTL_COL_COLLECT (1 << 15)       // 0x8000 - Collect hitbox (Will collect if it collides with collectable)
#define FL_MTL_COL_DYNAMIC (1 << 16)       // 0x10000 - Dynamic collision (DYN)
#define FL_MTL_COL_COLLECTABLE (1 << 17)   // 0x20000 - Collectable hitbox (Used for collectibles)
#define FL_MTL_COL_LAST (1 << 18)          // 0x40000 - Beginning of game-specific material flags

// Probably the end for engine flags, begin game collision flags (TODO: Move to game-specific header)

#define FL_MTL_COL_TERRAIN_TYPE1 (FL_MTL_COL_LAST << 0)                                                       // 0x40000 - changes terrain type with 80000,100000,200000
#define FL_MTL_COL_TERRAIN_TYPE2 (FL_MTL_COL_LAST << 1)                                                       // 0x80000 - changes terrain type with 40000,100000,200000
#define FL_MTL_COL_TERRAIN_TYPE3 (FL_MTL_COL_LAST << 2)                                                       // 0x100000 - changes terrain type with 40000,80000,20000
#define FL_MTL_COL_TERRAIN_TYPE4 (FL_MTL_COL_LAST << 3)                                                       // 0x200000 - changes terrain type + particles with 40000,80000,100000
#define FL_MTL_COL_WATER (FL_MTL_COL_TERRAIN_TYPE1)                                                           // 0x40000 - Water terrain type
#define FL_MTL_COL_DIRT (FL_MTL_COL_TERRAIN_TYPE2)                                                            // 0x80000 - Dirt terrain type
#define FL_MTL_COL_WOOD (FL_MTL_COL_TERRAIN_TYPE3)                                                            // 0x100000 - Wood terrain type
#define FL_MTL_COL_MUD (FL_MTL_COL_TERRAIN_TYPE4)                                                             // 0x200000 - Mud terrain type
#define FL_MTL_COL_GRASS (FL_MTL_COL_TERRAIN_TYPE1 | FL_MTL_COL_TERRAIN_TYPE2)                                // 0xc0000 - Grass terrain type
#define FL_MTL_COL_STONE = (FL_MTL_COL_TERRAIN_TYPE1 | FL_MTL_COL_TERRAIN_TYPE4)                              // 0x240000 - Stone terrain type
#define FL_MTL_COL_POPPING = (FL_MTL_COL_TERRAIN_TYPE1 | FL_MTL_COL_TERRAIN_TYPE2 | FL_MTL_COL_TERRAIN_TYPE3) // 0x1c0000 - Unk terrain type (popping sound)
#define FL_MTL_COL_SLIDE_JUMP (FL_MTL_COL_LAST << 4)                                                          // 0x400000 - Slide surface on which you can jump
#define FL_MTL_COL_SLIDE_NO_JUMP (FL_MTL_COL_LAST << 5)                                                       // 0x800000 - Slide surface on which you can't jump
#define FL_MTL_COL_STICKY (FL_MTL_COL_LAST << 6)                                                              // 0x1000000 - Sticky surface
#define FL_MTL_COL_SLIPPERY (FL_MTL_COL_SLIDE_JUMP | FL_MTL_COL_STICKY)                                       // 0x1400000 - Slippery surface
#define FL_MTL_COL_UNK_0x2000000 (FL_MTL_COL_LAST << 7)                                                       // 0x2000000 - Unknown
#define FL_MTL_COL_UNK_0x4000000 (FL_MTL_COL_LAST << 8)                                                       // 0x4000000 - Unknown
#define FL_MTL_COL_UNK_0x8000000 (FL_MTL_COL_LAST << 9)                                                       // 0x8000000 - Unknown
#define FL_MTL_COL_FOOTPRINT_DURING (FL_MTL_COL_LAST << 10)                                                   // 0x10000000 - Leave footprints while on surface
#define FL_MTL_COL_FOOTPRINT_AFTER (FL_MTL_COL_LAST << 11)                                                    // 0x20000000 - Leave footprints after leaving surface

// Collision flags for object filtering, bits 32 through 63.

#define FL_COL_SURFACE ((U64)1 << 32)   // 0x0000000100000000 - Enable collision against surface patches
#define FL_COL_SPLINE ((U64)1 << 33)    // 0x0000000200000000 - Enable collision against surface spline/edge geometry
#define FL_COL_VOLUME ((U64)1 << 34)    // 0x0000000400000000 - Enable collision against volume objects
#define FL_COL_ROTSHAPE ((U64)1 << 35)  // 0x0000000800000000 - Enable collision against rotshape objects
#define FL_COL_PARTICLES ((U64)1 << 36) // 0x0000001000000000 -  Enable collision against particle emitter objects

#define FL_COL_MESH_CYLINDRE_PRIM ((U64)1 << 37)                                                 // 0x0000002000000000 - Enable Mesh_Z cylinder primitive collision
#define FL_COL_MESH_BOX_PRIM ((U64)1 << 38)                                                      // 0x0000004000000000 - Enable Mesh_Z box primitive collision
#define FL_COL_MESH_SPHERE_PRIM ((U64)1 << 39)                                                   // 0x0000008000000000 - Enable Mesh_Z sphere primitive collision
#define FL_COL_MESH_PRIM ((U64)(FL_MESH_CYLINDRE_PRIM | FL_MESH_BOX_PRIM | FL_MESH_SPHERE_PRIM)) // 0x000000E000000000 - All Mesh_Z primitive collision types

#define FL_COL_LOD_CYLINDRE_PRIM ((U64)1 << 40)                                              // 0x0000010000000000 - Enable Lod_Z cylinder primitive collision
#define FL_COL_LOD_BOX_PRIM ((U64)1 << 41)                                                   // 0x0000020000000000 - Enable Lod_Z box primitive collision
#define FL_COL_LOD_SPHERE_PRIM ((U64)1 << 42)                                                // 0x0000040000000000 - Enable Lod_Z sphere primitive collision
#define FL_COL_LOD_PRIM ((U64)(FL_LOD_CYLINDRE_PRIM | FL_LOD_BOX_PRIM | FL_LOD_SPHERE_PRIM)) // 0x0000070000000000 - All Lod_Z primitive collision types

#define FL_COL_LOD_COLLECT ((U64)1 << 43)     // 0x0000080000000000 - Select the LOD collect collision representation
#define FL_COL_LOD_DYNAMIC ((U64)1 << 44)     // 0x0000100000000000 - Select the LOD dynamic collision representation
#define FL_COL_LOD_COLLECTABLE ((U64)1 << 45) // 0x0000200000000000 - Select the LOD collectable collision representation

#define FL_COL_ELEMENT_UV ((U64)1 << 46)    // 0x0000400000000000 - Request collision element UV coordinates in the result
#define FL_COL_SPHERE_FILTER ((U64)1 << 47) // 0x0000800000000000 - Post-process sphere results to remove/filter unwanted collisions
#define FL_COL_ONE_HIT ((U64)1 << 48)       // 0x0001000000000000 - Stop world collision traversal after the first node that produces a hit

// Texture flags

#define FL_TEX_DIFFUSE (U8)(1 << 0)                                              // 0x1 - Diffuse texture
#define FL_TEX_ENV (U8)(1 << 1)                                                  // 0x2 - Environment texture
#define FL_TEX_BUMP (U8)(1 << 2)                                                 // 0x4 - Bump texture
#define FL_TEX_SPECULAR (U8)(1 << 3)                                             // 0x8 - Specular texture
#define FL_MATERIAL_PAWAP (U8)(1 << 4)                                           // 0x10 - Pawap material
#define FL_TEX_ALL (FL_TEX_DIFFUSE | FL_TEX_ENV | FL_TEX_BUMP | FL_TEX_SPECULAR) // 0xF - All bits used for textures
#define FL_TEX_RESET (U8) ~(FL_TEX_ALL)

#endif
