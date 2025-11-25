#ifndef _SKEL_Z_H_
#define _SKEL_Z_H_
#include "Object_Z.h"
#include "Types_Z.h"

class Skel_Z : public ObjectDatas_Z {
public:
    S32 HideObjectNamed(const Name_Z& i_ObjName, Bool i_AlsoSomethingElse);
    S32 UnhideObjectNamed(const Name_Z& i_ObjName, Bool i_AlsoSomethingElse);
};

#endif // _SKEL_Z_H_
