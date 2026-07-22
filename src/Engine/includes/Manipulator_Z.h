#ifndef _MANIPULATOR_Z_H_
#define _MANIPULATOR_Z_H_
#include "ActivableGroups_Z.h"
#include "BaseObject_Z.h"
#include "Types_Z.h"

class ManipulatorManager_Z;
class ManipulatorList_Z;
class Viewport_Z;

class Manipulator_Z : public BaseObject_Z {
    friend class ManipulatorManager_Z;
    friend class ManipulatorList_Z;

public:
    Manipulator_Z();
    void Activate();
    void Deactivate();
    void SetGroup(ActivableGroup_Z i_Group);
    void Heartbeat(Float i_DeltaTime);
    virtual void Init();
    virtual ~Manipulator_Z();
    virtual void Reset();
    virtual void ActionOnActivate();
    virtual void ActionOnDeactivate();
    virtual void Update(Float i_DeltaTime) = 0;

    inline ActivableGroup_Z GetGroup() const { return m_ManipGroup; }

    inline Bool IsActive() const { return m_IsActive; }

protected:
    Manipulator_Z* m_PreviousPtr;
    Manipulator_Z* m_NextPtr;
    ActivableGroup_Z m_ManipGroup; //TODO: Define enum for this
    Bool m_IsActive;
    Float m_TimeSpentRunning;
};

class ManipulatorDraw_Z : public Manipulator_Z {
public:
    ManipulatorDraw_Z();
    virtual void Init();

    virtual ~ManipulatorDraw_Z() { }

    virtual void Update(Float i_DeltaTime);

    virtual void Draw(const Viewport_Z* i_Vp) { }

    virtual void Draw(const DrawInfo_Z& i_DrawInfo);

    void VpRegister();
    void VpRegister(S32 i_VpId);
    void VpUnRegister();

protected:
    S32 m_ViewportId;
};

class ManipulatorSceneDraw_Z : public Manipulator_Z {
public:
    ManipulatorSceneDraw_Z();
    virtual void Init();

    virtual ~ManipulatorSceneDraw_Z() { }

    virtual void Update(Float i_DeltaTime);
    virtual void BeforeDraw(const DrawInfo_Z& i_DrawInfo);
    virtual void Draw(const DrawInfo_Z& i_DrawInfo);
};

#endif
