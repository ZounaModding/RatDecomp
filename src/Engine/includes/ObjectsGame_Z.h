#ifndef _OBJECTSGAME_Z_H_
#define _OBJECTSGAME_Z_H_
#include "ObjectGame_Z.h"
#include "ObjectsGame_ZHdl.h"

class ObjectsGame_Z : public ObjectGame_Z {
public:
    virtual ~ObjectsGame_Z() { }

    virtual void Init();
    virtual void Reset();
    virtual void ActionOnActivate();
    virtual void ActionOnDeactivate();
    virtual void Update(Float a1);
    virtual void AddGameObject(const ObjectMove_ZHdl& a1);
    virtual void RemoveGameObject(const ObjectMove_ZHdl& a1);
    virtual void UnregistredFromGame(const Agent_ZHdl& a1);

    static BaseObject_Z* NewObject() { return NewL_Z(125) ObjectsGame_Z; }
};

class ObjectsGameClip_Z : public ObjectGame_Z {
public:
    virtual ~ObjectsGameClip_Z();
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

#endif // _OBJECTSGAME_Z_H_
