#ifndef _OCCLUDEDSEADHANDLE_Z_H_
#define _OCCLUDEDSEADHANDLE_Z_H_
#include "SeadHandle_Z.h"
#include "Renderer_Z.h"

class OccludedSeadHandle_Z : public SeadHandle_Z {
public:
    U32 SetFrustrum(StaticArray_Z<OmniFrust_Z, 64, FALSE, FALSE>& o_OmniFrusts, SeadZone_Z& o_Zone1, SeadZone_Z& o_Zone2, DrawInfo_Z& io_DrawInfo, U32 i_Flag, U32 i_NoFlag);
};

#endif // _OCCLUDEDSEADHANDLE_Z_H_
