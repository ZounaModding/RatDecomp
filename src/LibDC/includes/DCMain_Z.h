#ifndef _DCMAIN_Z_H_
#define _DCMAIN_Z_H_

#include "Main_Z.h"
#include "Handle_Z.h"
#include "ClassManager_Z.h"
#include <kos.h>

Extern_Z void Report_Z(const Char* i_Format, ...);

class DCGlobals;
Extern_Z DCGlobals gData;

class DCGlobals : public Globals {
public:
    virtual ~DCGlobals() { };
    virtual void Minimize();

    String_Z<ARRAY_CHAR_MAX> m_AliasPath;

    DCGlobals() {
    }
};

void InitProgram(int i_Argc, Char** i_Argv);
void ShutProgram();

Weak_Z BaseObject_ZHdl::operator BaseObject_Z*() const {
    return GETPTR(*this);
}

Weak_Z BaseObject_Z* BaseObject_ZHdl::operator->() const {
    return (BaseObject_Z*)GETPTR((BaseObject_ZHdl&)*this);
}

#endif
