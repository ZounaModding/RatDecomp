#ifndef _RESOURCEOBJECT_Z_H_
#define _RESOURCEOBJECT_Z_H_
#include "BaseObject_Z.h"
#include "DynArray_Z.h"

class ResourceObjectLink_Z {
public:
    ResourceObjectLink_Z() { };
    ~ResourceObjectLink_Z() { };
    void Load(void** a1);
    void EndLoad();
    void EndLoadLinks();
    void MarkHandles();
    void UpdateLinkFromId(BaseObject_ZHdl& i_Hdl, S32 i_ID) const;
    DynArray_Z<BaseObject_ZHdl, 32> m_LinkedResourceDA;
};

/* sizeof(ResourceObject_Z) == 0x14 */
class ResourceObject_Z : public BaseObject_Z {
public:
    ResourceObject_Z() { };
    virtual ~ResourceObject_Z() { };
    virtual void Load(void** i_Data);

    virtual void EndLoad() {
        m_ResObjLink.EndLoad();
    };

    virtual void LoadLinks(void** a1);
    virtual void EndLoadLinks();
    virtual Bool MarkHandles();
    ResourceObjectLink_Z m_ResObjLink; /* 0x08 */
};

#endif
