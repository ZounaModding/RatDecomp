#ifndef _OBJECTSGAMECLIP_Z_H_
#define _OBJECTSGAMECLIP_Z_H_
#include "ObjectGame_Z.h"
#include "ObjectsGame_ZHdl.h"
#include "ObjectsGameClip_ZHdl.h"

class ObjectsGameClip_Z : public ObjectGame_Z {
public:
    virtual ~ObjectsGameClip_Z() { }

    virtual void Init();
    virtual void Update(Float i_DeltaTime);

    static BaseObject_Z* NewObject() { return NewL_Z(31) ObjectsGameClip_Z; }

    void SetObjectsGameHdl(ObjectsGame_ZHdl& i_ObjectsGameHdl) {
        m_ObjectsGameHdl = i_ObjectsGameHdl;
    }

private:
    Bool m_UnkBool_0x2c;
    ObjectsGame_ZHdl m_ObjectsGameHdl;
};

#endif // _OBJECTSGAMECLIP_Z_H_
