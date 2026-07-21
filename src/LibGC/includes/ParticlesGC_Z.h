#ifndef _PARTICLESGC_Z_H_
#define _PARTICLESGC_Z_H_
#include "Particles_Z.h"
#include "DisplayList_Z.h"
#include "DataManager_Z.h"

class ParticlesGDL_Z {
public:
    ParticlesGDL_Z() {
        m_DisplayList = NULL;
    }

    ~ParticlesGDL_Z() {
        if (m_DisplayList) {
            m_DisplayList->Reset();
            Delete_Z m_DisplayList;
        }
    }

    U8 m_Unk_0x0[12];
    DisplayList_Z* m_DisplayList;
};

class GCParticlesManager_Z : public DataManager_Z<ParticlesGDL_Z> {
public:
    GCParticlesManager_Z(Char* i_Name)
        : DataManager_Z<ParticlesGDL_Z>(i_Name) { }
};

typedef GCParticlesManager_Z::Hdl ParticlesGDL_ZHdl;
typedef GCParticlesManager_Z::Item ParticlesGDL_ZEle;

class ParticlesGCData_Z : public ParticlesData_Z {
public:
    ParticlesGCData_Z();
    virtual ~ParticlesGCData_Z();
    virtual void EndLoad();
    virtual void Draw(DrawInfo_Z& i_DrawInfo);
    virtual void CloneEle(const ParticlesEle_ZDA& i_Elements, const Sphere_Z& i_BSphere);

    static BaseObject_Z* NewObject() {
        return NewL_Z(23) ParticlesGCData_Z;
    }

private:
    DynArray_Z<ParticlesGDL_ZHdl, 32, TRUE, TRUE, 4> m_GDLHandles;
    U8 m_Unk_0x68[8];
};

#endif // _PARTICLESGC_Z_H_
