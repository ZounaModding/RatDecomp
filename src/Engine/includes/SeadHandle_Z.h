#ifndef _SEADHANDLE_Z_H_
#define _SEADHANDLE_Z_H_
#include "Math_Z.h"
#include "DynArray_Z.h"

class SeadEntry_Z {
    // TODO: Define
};

typedef DynArray_Z<SeadEntry_Z, 32, FALSE> SeadEntry_ZDA;

class SeadHandle_Z {
public:
    SeadHandle_Z();

    void GetSize(Vec2f& o_Min, Vec2f& o_Max);

protected:
    Vec2f m_PMin;
    Vec2f m_PMax;
    Vec2f m_InvDiag; // 1.0f / (m_PMax - m_PMin)
    S32 m_SizeX;
    S32 m_SizeY;
    S32 m_FirstFreeIdx;
    S32 m_NbFree;
    S32DA m_GridDA;
    SeadEntry_ZDA m_EntryPoolDA;
    Bool m_Active;

public:
    virtual void Load(void** i_Data);
    virtual void EndLoad();
};

#endif // _SEADHANDLE_Z_H_
