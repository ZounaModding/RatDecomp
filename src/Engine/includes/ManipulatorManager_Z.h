#ifndef _MANIPULATORMANAGER_Z_H_
#define _MANIPULATORMANAGER_Z_H_
#include "Manipulator_Z.h"

class ManipulatorList_Z {
    friend class ManipulatorManager_Z;

public:
    void Update(Float i_DeltaTime);
    void Add(Manipulator_Z* i_Manip);
    void Remove(Manipulator_Z* i_Manip);

    Manipulator_Z* GetHead() { return m_ListHeadPtr; };

private:
    Manipulator_Z* m_ListHeadPtr;
    Manipulator_Z* m_ListTailPtr;
};

class ManipulatorManager_Z {
public:
    void MarkHandles();
    Bool Minimize();
    void Update(Float i_DeltaTime);
    void FlushBuffer();
    void Add(Manipulator_Z* i_Manip);
    void Remove(Manipulator_Z* i_Manip);
    void Activate(Manipulator_Z* i_Manip);
    void Deactivate(Manipulator_Z* i_Manip);
    void Order(Manipulator_Z* i_Manip, ActivableGroup_Z i_Group);

    Bool Contains(Manipulator_Z* i_Manip, ManipulatorList_Z& i_ManipList) {
        for (Manipulator_Z* l_CurrManip = i_ManipList.GetHead();
             l_CurrManip != NULL;
             l_CurrManip = l_CurrManip->m_NextPtr) {
            if (l_CurrManip == i_Manip) {
                return TRUE;
            }
        }
        return FALSE;
    }

private:
    ManipulatorList_Z m_ManipList[28];
    ManipulatorList_Z m_UnkManipList;
    DynArray_Z<Manipulator_Z*, 16, FALSE, FALSE> m_ManipDA;
    Bool m_IsPaused;
    Bool m_IsBeingUpdated;
    S32 m_ActiveManipGroup;
};

#endif
