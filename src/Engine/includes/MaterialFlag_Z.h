#ifndef _MATERIALFLAG_Z_H_
#define _MATERIALFLAG_Z_H_

// Material Codes
#define MATERIAL_CODE_UNK0 (1 << 0)
#define MATERIAL_CODE_UNK1 (1 << 1)
#define MATERIAL_CODE_UNK2 (1 << 2)
#define MATERIAL_CODE_UNK3 (1 << 3)
#define MATERIAL_CODE_UNK4 (1 << 4)

// Material flags
#define FL_IS_ALPHABLENDED (1 << 5)
#define FL_IS_TRANSPARENT (1 << 6)
#define FL_INVISIBLE (1 << 7)
#define FL_TEX_ADDRESS_CLAMP_U (1 << 8)
#define FL_TEX_ADDRESS_CLAMP_V (1 << 9)
#define FL_STENCIL_OCCLUDER (1 << 10)
#define FL_STENCIL_OCCLUDEE (1 << 11)
#define FL_ADDITIF (1 << 12)
#define FL_SOUSTRACTIF (1 << 13)
#define FL_DESTADDITIF (1 << 14)
#define FL_MAT_RDR_UNK15 (1 << 15)
#define FL_MAT_RDR_UNK16 (1 << 16)
#define FL_TWO_SIDE (1 << 17)
#define FL_MAT_RDR_UNK18 (1 << 18)
#define FL_MAT_RDR_UNK19 (1 << 19)
#define FL_MAT_RDR_UNK20 (1 << 20)
#define FL_MAT_RDR_UNK21 (1 << 21)
#define FL_MAT_RDR_UNK22 (1 << 22)
#define FL_MAT_RDR_UNK23 (1 << 23)
#define FL_MAT_RDR_UNK24 (1 << 24)
#define FL_MAT_RDR_UNK25 (1 << 25)
#define FL_MAT_RDR_UNK26 (1 << 26)
#define FL_MAT_RDR_UNK27 (1 << 27)
#define FL_MAT_RDR_UNK28 (1 << 28)
#define FL_MAT_RDR_UNK29 (1 << 29)

#define FL_MAT_COL_UNK0 (1 << 0)
#define FL_MAT_COL_COLLIDABLE (1 << 1)
#define FL_MAT_COL_UNK2 (1 << 2)
#define FL_MAT_COL_UNK3 (1 << 3)
#define FL_MAT_COL_UNK4 (1 << 4)
#define FL_MAT_COL_UNK5 (1 << 5)
#define FL_MAT_COL_UNK6 (1 << 6)
#define FL_MAT_COL_UNK7 (1 << 7)
#define FL_MAT_COL_UNK8 (1 << 8)
#define FL_MAT_COL_UNK9 (1 << 9)
#define FL_MAT_COL_UNK10 (1 << 10)
#define FL_MAT_COL_UNK11 (1 << 11)
#define FL_MAT_COL_UNK12 (1 << 12)
#define FL_MAT_COL_UNK13 (1 << 13)
#define FL_MAT_COL_UNK14 (1 << 14)
#define FL_MAT_COL_UNK15 (1 << 15)
#define FL_MAT_COL_UNK16 (1 << 16)
#define FL_MAT_COL_UNK17 (1 << 17)
#define FL_MAT_COL_SOUND1 (1 << 18) // changes sound with 80000,100000,200000
#define FL_MAT_COL_SOUND2 (1 << 19) // changes sound with 40000,100000,200000
#define FL_MAT_COL_SOUND3 (1 << 20) // changes sound with 40000,80000,200000
#define FL_MAT_COL_SOUND4 (1 << 21) // changes sound + particles with 40000,80000,100000
#define FL_MAT_COL_SLIDE1 (1 << 22) // Slide surface 1 - slippery w/ sticky
#define FL_MAT_COL_SLIDE2 (1 << 23) // Slide surface 2
#define FL_MAT_COL_STICKY (1 << 24) // Sticky surface - slippery w/ Slide 1
#define FL_MAT_COL_UNK25 (1 << 25)
#define FL_MAT_COL_UNK26 (1 << 26)
#define FL_MAT_COL_UNK27 (1 << 27)
#define FL_MAT_COL_UNK28 (1 << 28)
#define FL_MAT_COL_UNK29 (1 << 29)

#define FL_TEX_DIFFUSE (1 << 0)    // Diffuse texture
#define FL_TEX_ENV (1 << 1)        // Environment texture
#define FL_TEX_BUMP (1 << 2)       // Bump texture
#define FL_TEX_SPECULAR (1 << 3)   // Specular texture
#define FL_MATERIAL_PAWAP (1 << 4) // Pawap material

#endif
