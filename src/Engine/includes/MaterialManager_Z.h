#ifndef _MATERIALMANAGER_Z_H_
#define _MATERIALMANAGER_Z_H_
#include "MaterialAnim_Z.h"
#include "MaterialAnim_ZHdl.h"
#include "Material_Z.h"
#include "StaticArray_Z.h"
#include "DynArray_Z.h"

#define MAX_MATERIAL_ANIMS 1024
#define MAX_MATERIAL_USERS 16

class MaterialManager_Z {
private:
    StaticArray_Z<MaterialAnim_ZHdl, MAX_MATERIAL_ANIMS> m_MaterialAnimHdls;
    StaticArray_Z<MaterialAnim_ZHdl, MAX_MATERIAL_ANIMS> m_MaterialAnimHdlsReallyAnimated;
    DynArray_Z<MaterialUserDesc_Z, MAX_MATERIAL_USERS> m_MaterialUsers;
    Bool m_IsPaused;

public:
    MaterialManager_Z();
    virtual ~MaterialManager_Z();

    void AddMaterialAnim(const MaterialAnim_ZHdl& i_MaterialAnimHdl);
    void GetMaterialByName(const Name_Z& i_Name, MaterialAnim_ZHdl& o_MaterialAnimHdl);

    const MaterialAnim_ZHdl& GetMaterialAnimFromMaterial(const Material_ZHdl& i_MaterialHdl);
    void RegisterMaterialUser(S32 i_MaterialCode, NewMaterialUserProc i_NewMaterialUserProc);
    MaterialUser_Z* GetMaterialUser(S32 i_MaterialCode);

    void Update(Float i_DeltaTime);
    void MarkHandles();

    inline void EndPause() {
        m_IsPaused = FALSE;
    }
};

#endif // _MATERIALMANAGER_Z_H_
