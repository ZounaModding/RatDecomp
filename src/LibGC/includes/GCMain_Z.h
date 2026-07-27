#ifndef _GCMAIN_Z_H_
#define _GCMAIN_Z_H_
#include "Main_Z.h"
#include "Handle_Z.h"
#include "ClassManager_Z.h"
#include <dolphin/os.h>

#define Report_Z(title, buf) OSReport(title)

void CallBack_ResetPressed();
Bool PrintMemStatus();
void InitProgram(int i_Argc, Char** i_Argv);
void ShutProgram();

class GCParticlesManager_Z;
class TextureManager_Z;
class GCGlobals;
Extern_Z GCGlobals gData;

class GCGlobals : public Globals {
public:
    virtual ~GCGlobals() { };
    virtual Bool GetMgrSize(const Name_Z& i_Name, S32& i_Size1, S32& i_Size2);
    virtual void Minimize();

    //U8 m_Pad_0x7E0[0x110];
    GCParticlesManager_Z* GCParticlesManager;
    TextureManager_Z* TextureMgr;
    String_Z<ARRAY_CHAR_MAX> m_AliasPath;

    GCGlobals() {
        GCParticlesManager = NULL;
        TextureMgr = NULL;
    }
};

Weak_Z BaseObject_ZHdl::operator BaseObject_Z*() const {
    return GETPTR(*this);
}

Weak_Z BaseObject_Z* BaseObject_ZHdl::operator->() const {
    return (BaseObject_Z*)GETPTR((BaseObject_ZHdl&)*this);
}

#endif
