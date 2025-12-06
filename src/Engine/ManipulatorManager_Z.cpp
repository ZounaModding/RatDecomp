#include "ManipulatorManager_Z.h"
#include "GCMain_Z.h"
#include "Math_Z.h"

void ManipulatorManager_Z::MarkHandles() { }

Bool ManipulatorManager_Z::Minimize() {
    m_ManipDA.Minimize();
    return TRUE;
}

void ManipulatorManager_Z::Update(Float i_DeltaTime) {
    m_IsBeingUpdated = TRUE;
    gData.ClassMgr->ForbidCheckHandles(TRUE);
    if (m_IsPaused) {
        for (m_ActiveManipGroup = 0; m_ActiveManipGroup < 4; m_ActiveManipGroup++) {
            m_ManipList[m_ActiveManipGroup].Update(i_DeltaTime);
        }
        m_ManipList[20].Update(i_DeltaTime);
        m_ManipList[22].Update(i_DeltaTime);
        m_ManipList[24].Update(i_DeltaTime);
        m_ManipList[27].Update(i_DeltaTime);
        m_ManipList[26].Update(i_DeltaTime);
    }
    else {
        for (m_ActiveManipGroup = 0; m_ActiveManipGroup < 28 && !m_IsPaused; m_ActiveManipGroup++) {
            gData.ClassMgr->ForbidCheckHandles(m_ActiveManipGroup != 2);
            m_ManipList[m_ActiveManipGroup].Update(i_DeltaTime);
        }
    }
    m_IsBeingUpdated = FALSE;
    FlushBuffer();
    gData.ClassMgr->ForbidCheckHandles(FALSE);
}

void ManipulatorManager_Z::FlushBuffer() {
    ManipulatorList_Z* l_ManipList;
    S32 i;
    Manipulator_Z* l_Manip;

    i = m_ManipDA.GetSize();
    if (m_ManipDA.GetSize() == 0) {
        return;
    }

    while (i-- != 0) {
        l_Manip = m_ManipDA[i];
        l_ManipList = &m_ManipList[l_Manip->m_ManipGroup];

        Bool l_ContainsManip = Contains(l_Manip, *l_ManipList);
        if (l_ContainsManip) {
            l_ManipList->Remove(l_Manip);
        }
        else {
            m_UnkManipList.Remove(l_Manip);
        }

        if (l_Manip->m_IsActive) {
            l_ManipList->Add(l_Manip);
        }
        else {
            m_UnkManipList.Add(l_Manip);
        }
    }

    m_ManipDA.Empty();
}

void ManipulatorManager_Z::Add(Manipulator_Z* i_Manip) {
    m_UnkManipList.Add(i_Manip);
}

void ManipulatorManager_Z::Remove(Manipulator_Z* i_Manip) {
    if (i_Manip->m_IsActive) {
        m_ManipList[i_Manip->m_ManipGroup].Remove(i_Manip);
    }
    else {
        m_UnkManipList.Remove(i_Manip);
    }
}

void ManipulatorManager_Z::Order(Manipulator_Z* i_Manip, ActivableGroup_Z i_Group) {
    if ((m_IsBeingUpdated) && (i_Manip->GetGroup() == m_ActiveManipGroup)) {
        for (int i = 0; i < m_ManipDA.GetSize(); i++) {
            if (m_ManipDA[i] == i_Manip) return;
        }
        m_ManipDA.Add(i_Manip);
    }
    else if (i_Manip->m_IsActive) {
        m_ManipList[i_Manip->m_ManipGroup].Remove(i_Manip);
        i_Manip->m_ManipGroup = i_Group;
        m_ManipList[i_Manip->m_ManipGroup].Add(i_Manip);
    }
    else {
        i_Manip->m_ManipGroup = i_Group;
    }
}

void ManipulatorManager_Z::Activate(Manipulator_Z* i_Manip) {
    for (int i = 0; i < m_ManipDA.GetSize(); i++) {
        if (m_ManipDA[i] == i_Manip) {
            return;
        }
    }
    if ((m_IsBeingUpdated) && (i_Manip->GetGroup() == m_ActiveManipGroup)) {
        m_ManipDA.Add(i_Manip);
    }
    else {
        m_UnkManipList.Remove(i_Manip);
        m_ManipList[i_Manip->GetGroup()].Add(i_Manip);
    }
}

void ManipulatorManager_Z::Deactivate(Manipulator_Z* i_Manip) {
    for (int i = 0; i < m_ManipDA.GetSize(); i++) {
        if (m_ManipDA[i] == i_Manip) {
            return;
        }
    }
    if ((m_IsBeingUpdated) && (i_Manip->GetGroup() == m_ActiveManipGroup)) {
        m_ManipDA.Add(i_Manip);
    }
    else {
        m_ManipList[i_Manip->GetGroup()].Remove(i_Manip);
        m_UnkManipList.Add(i_Manip);
    }
}

void ManipulatorList_Z::Add(Manipulator_Z* i_Manip) {
    i_Manip->m_PreviousPtr = m_ListTailPtr;
    if (m_ListTailPtr == NULL)
        m_ListHeadPtr = i_Manip;
    else
        m_ListTailPtr->m_NextPtr = i_Manip;
    m_ListTailPtr = i_Manip;
}

void ManipulatorList_Z::Remove(Manipulator_Z* i_Manip) {
    if (i_Manip == m_ListHeadPtr) {
        m_ListHeadPtr = i_Manip->m_NextPtr;
        if (m_ListHeadPtr != NULL)
            m_ListHeadPtr->m_PreviousPtr = NULL;
        else
            m_ListTailPtr = NULL;
    }
    else if (i_Manip == m_ListTailPtr) {
        if (i_Manip->m_PreviousPtr != NULL) {
            i_Manip->m_PreviousPtr->m_NextPtr = NULL;
            m_ListTailPtr = i_Manip->m_PreviousPtr;
        }
        else {
            m_ListHeadPtr = NULL;
            m_ListTailPtr = NULL;
        }
    }
    else {
        i_Manip->m_PreviousPtr->m_NextPtr = i_Manip->m_NextPtr;
        i_Manip->m_NextPtr->m_PreviousPtr = i_Manip->m_PreviousPtr;
    }
    i_Manip->m_PreviousPtr = NULL;
    i_Manip->m_NextPtr = NULL;
}

void ManipulatorList_Z::Update(Float i_DeltaTime) {
    for (Manipulator_Z* l_CurManip = m_ListHeadPtr; l_CurManip != NULL; l_CurManip = l_CurManip->m_NextPtr) {
        l_CurManip->GetHandle();
        if (l_CurManip->m_IsActive) {
            l_CurManip->Heartbeat(i_DeltaTime);
        }
    }
}
