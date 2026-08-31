#ifndef _GAMEOBJ_Z_H_
#define _GAMEOBJ_Z_H_
#include "Node_ZHdl.h"
#include "ResourceObject_Z.h"

class GameObj_Z : public ResourceObject_Z {
public:
    virtual ~GameObj_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual Bool MarkHandles();

    static BaseObject_Z* NewObject() { return NewL_Z(26) GameObj_Z; }

    Node_ZHdl GetObjectByName(const Name_Z& i_Name, S16 i_GameObjId);

private:
    Node_ZHdlDA m_NodeHdls;
};

#endif // _GAMEOBJ_Z_H_
