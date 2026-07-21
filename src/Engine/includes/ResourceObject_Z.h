#ifndef _RESOURCEOBJECT_Z_H_
#define _RESOURCEOBJECT_Z_H_
#include "BaseObject_Z.h"
#include "DynArray_Z.h"

class ResourceObjectLink_Z {
public:
    ResourceObjectLink_Z() { };
    ~ResourceObjectLink_Z() { };
    void Load(void** i_Data);
    void EndLoad();
    void EndLoadLinks();
    void MarkHandles();
    void UpdateLinkFromId(BaseObject_ZHdl& o_Hdl, S32 i_ID) const;
    DynArray_Z<BaseObject_ZHdl, 32> m_LinkedResourceDA;
};

class ResourceObject_Z : public BaseObject_Z {
public:
    ResourceObject_Z() { };
    virtual ~ResourceObject_Z() { };
    virtual void Load(void** i_Data) { };

    virtual void EndLoad() {
        m_ResObjLink.EndLoad();
    };

    virtual void LoadLinks(void** i_Data) {
        BaseObject_Z::LoadLinks(i_Data);
        m_ResObjLink.Load(i_Data);
    }

    virtual void EndLoadLinks() { }

    virtual Bool MarkHandles();
    ResourceObjectLink_Z m_ResObjLink;
};

#endif
