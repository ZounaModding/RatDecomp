#ifndef _OBJECTGAME_Z_H_
#define _OBJECTGAME_Z_H_
#include "Manipulator_Z.h"
#include "ObjectMove_ZHdl.h"
#include "Agent_ZHdl.h"
#include "Game_ZHdl.h"

class ObjectGame_Z : public Manipulator_Z {
public:
    virtual ~ObjectGame_Z() { }

    virtual void Init() { }

    virtual Bool MarkHandles() {
        return FALSE;
    }

    virtual void Reset() { }

    virtual void ActionOnActivate();
    virtual void ActionOnDeactivate();

    virtual void AddGameObject(const ObjectMove_ZHdl& i_ObjectMoveHdl) { }

    virtual void RemoveGameObject(const ObjectMove_ZHdl& i_ObjectMoveHdl) { }

    virtual void UnregistredFromGame(const Agent_ZHdl& i_AgentHdl) { }

    virtual void SetGame(const Game_ZHdl& i_GameHdl) { }

    inline void SetNbVp(U32 i_NbVp) {
        m_NbVp = i_NbVp;
    }

    inline S32 GetNbVp() const {
        return m_NbVp;
    }

    inline S32 GetFirstVp() const {
        return m_FirstPlayerVpId;
    }

private:
    U32 m_NbVp;
    U32 m_FirstPlayerVpId;
    Game_ZHdl m_GameHdl;
};

#endif // _OBJECTGAME_Z_H_
