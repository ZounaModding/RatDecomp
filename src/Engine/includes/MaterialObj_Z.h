#ifndef _MATERIALOBJ_Z_H_
#define _MATERIALOBJ_Z_H_
#include "ResourceObject_Z.h"
#include "MaterialAnim_ZHdl.h"

class MaterialObj_Z : public ResourceObject_Z {
public:
    virtual ~MaterialObj_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual Bool MarkHandles();

    static BaseObject_Z* NewObject() { return NewL_Z(20) MaterialObj_Z; }

    MaterialAnim_ZHdl GetMaterialByName(const Name_Z& i_Name);

private:
    MaterialAnim_ZHdlDA m_MaterialAnimHdls;
};

#endif // _MATERIALOBJ_Z_H_
