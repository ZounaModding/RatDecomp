#include "SeadHandle_Z.h"
#include "Program_Z.h"
#include "Node_Z.h"
#include "World_Z.h"
#include "HField_Z.h"

S32 SeadZone_Z::SeadZonesCount;

SeadHandle_Z::SeadHandle_Z() {
    m_FirstFreeIdx = -1;
    m_NbFree = 0;
    m_SizeY = 0;
    m_SizeX = 0;
    m_InvDiag.x = 0.0f;
    m_InvDiag.y = 0.0f;
    m_Active = FALSE;
}

// TODO: Finish matching
Bool SeadHandle_Z::GetGridRect(const Vec2f& i_BMin, const Vec2f& i_BMax, Rect_Z& o_Rect) const {
    if (!m_SizeX) {
        return FALSE;
    }

    Float l_FactorX, l_FactorY;
    l_FactorX = m_InvDiag.x;
    l_FactorX *= m_SizeX;
    l_FactorY = m_InvDiag.y;
    l_FactorY *= m_SizeY;

    Vec2f l_Min;
    Vec2f l_Max;
    l_Min.x = l_FactorX * (i_BMin.x - m_PMin.x);
    o_Rect.x1 = (S16)l_Min.x;
    l_Min.y = l_FactorY * (i_BMin.y - m_PMin.y);
    o_Rect.y1 = (S16)l_Min.y;
    l_Max.x = l_FactorX * (i_BMax.x - m_PMin.x);
    o_Rect.x2 = (S16)l_Max.x + 1;
    l_Max.y = l_FactorY * (i_BMax.y - m_PMin.y);
    o_Rect.y2 = (S16)l_Max.y + 1;

    if (o_Rect.x2 < 0 || o_Rect.y2 < 0 || o_Rect.x1 >= m_SizeX || o_Rect.y1 >= m_SizeY) {
        return FALSE;
    }
    if (o_Rect.x2 > m_SizeX) {
        o_Rect.x2 = m_SizeX;
    }
    if (o_Rect.y2 > m_SizeY) {
        o_Rect.y2 = m_SizeY;
    }
    if (o_Rect.x1 < 0) {
        o_Rect.x1 = 0;
    }
    if (o_Rect.y1 < 0) {
        o_Rect.y1 = 0;
    }
    if (o_Rect.x2 <= o_Rect.x1 || o_Rect.y2 <= o_Rect.y1) {
        return FALSE;
    }
    return TRUE;
}

S32 SeadHandle_Z::GetFreeEntry() {
    S32 l_Idx = m_FirstFreeIdx;
    m_FirstFreeIdx = m_EntryPoolDA[l_Idx].m_NextEntryOfThisObjectIdx;
    if (m_FirstFreeIdx < 0) {
        m_FirstFreeIdx = m_EntryPoolDA.GetSize();
        m_EntryPoolDA[l_Idx].m_NextEntryOfThisObjectIdx = m_FirstFreeIdx;
        m_NbFree += 16;

        S32 l_NewSize = m_FirstFreeIdx + 16;
        m_EntryPoolDA.SetSize(l_NewSize);
        l_NewSize--;
        S32 i;
        for (i = m_FirstFreeIdx; i < l_NewSize; i++) {
            SeadEntry_Z& l_Entry = m_EntryPoolDA[i];
            l_Entry.m_Node = NULL;
            l_Entry.m_NextObjectOfThisEntryIdx = -1;
            l_Entry.m_PrevObjectOfThisEntryIdx = -1;
            l_Entry.m_NextEntryOfThisObjectIdx = -1;
            l_Entry.m_GridIdx = -1;
            m_EntryPoolDA[i].m_NextEntryOfThisObjectIdx = i + 1;
        }
        SeadEntry_Z& l_Entry = m_EntryPoolDA[i];
        l_Entry.m_Node = NULL;
        l_Entry.m_NextObjectOfThisEntryIdx = -1;
        l_Entry.m_PrevObjectOfThisEntryIdx = -1;
        l_Entry.m_NextEntryOfThisObjectIdx = -1;
        l_Entry.m_GridIdx = -1;
    }
    m_NbFree--;
    return l_Idx;
}

void SeadHandle_Z::Ref(S32& io_EntryIdx, Node_Z* i_Node, Rect_Z& io_Rect) {
    U32 l_OccluderZoneId;
    if (io_EntryIdx != -1 || io_Rect.x1 < 0) {
        Sphere_Z& l_Sphere = i_Node->GetBSphere();
        l_OccluderZoneId = 0;
        Vec2f l_Min;
        Vec2f l_Max;
        Float l_MaxX, l_Radius;
        l_Radius = l_Sphere.Radius;
        if (l_Radius <= 0.0f) {
            return;
        }
        Float l_X = l_Sphere.Center.x;
        l_Min.x = l_X - l_Radius;
        l_MaxX = l_X + l_Radius;
        l_Min.y = l_Sphere.Center.z - l_Radius;
        l_Max.x = l_MaxX;
        l_Max.y = l_Sphere.Center.z + l_Radius;
        Rect_Z l_Rect;
        if (!GetGridRect(l_Min, l_Max, l_Rect) || (io_EntryIdx != -1 && l_Rect.x1 == io_Rect.x1 && l_Rect.x2 == io_Rect.x2 && l_Rect.y1 == io_Rect.y1 && l_Rect.y2 == io_Rect.y2)) {
            i_Node->ClrOccluderZoneId();
            return;
        }
        UnRef(io_EntryIdx, io_Rect);
        io_Rect = l_Rect;
    }
    else {
        l_OccluderZoneId = i_Node->GetOccluderZoneId();
    }

    S32 l_GridIdx = io_Rect.x1 + io_Rect.y1 * m_SizeX;
    S16 l_SizeY = io_Rect.y2 - io_Rect.y1;
    S32 l_EntryIdx = -1;
    while (l_SizeY--) {
        S16 l_SizeX = io_Rect.x2 - io_Rect.x1;
        S32 l_CurrentGridIdx = l_GridIdx;
        while (l_SizeX--) {
            l_EntryIdx = DoRef(l_CurrentGridIdx++, l_EntryIdx, i_Node);
        }
        l_GridIdx += m_SizeX;
    }
    i_Node->SetOccluderZoneId(l_OccluderZoneId);
    io_EntryIdx = l_EntryIdx;
}

void SeadHandle_Z::UnRef(S32& io_EntryIdx, Rect_Z& o_Rect) {
    o_Rect.Set(-1, -1, -1, -1);
    S32 l_Idx = io_EntryIdx;
    io_EntryIdx = -1;
    while (l_Idx != -1) {
        S32 i = l_Idx;
        SeadEntry_Z& l_SeadEntry = m_EntryPoolDA[i];
        l_SeadEntry.m_Node->ClrOccluderZoneId();
        l_SeadEntry.m_Node = NULL;

        if (l_SeadEntry.m_NextObjectOfThisEntryIdx != -1) {
            m_EntryPoolDA[l_SeadEntry.m_NextObjectOfThisEntryIdx].m_PrevObjectOfThisEntryIdx = l_SeadEntry.m_PrevObjectOfThisEntryIdx;
        }

        if (l_SeadEntry.m_PrevObjectOfThisEntryIdx != -1) {
            m_EntryPoolDA[l_SeadEntry.m_PrevObjectOfThisEntryIdx].m_NextObjectOfThisEntryIdx = l_SeadEntry.m_NextObjectOfThisEntryIdx;
        }
        else {
            m_GridDA[l_SeadEntry.m_GridIdx] = l_SeadEntry.m_NextObjectOfThisEntryIdx;
        }

        l_Idx = l_SeadEntry.m_NextEntryOfThisObjectIdx;
        l_SeadEntry.m_NextEntryOfThisObjectIdx = m_FirstFreeIdx;
        l_SeadEntry.m_GridIdx = -1;
        m_FirstFreeIdx = i;

        m_NbFree++;
    }
}

S32 SeadHandle_Z::DoRef(S32 i_GridIdx, S32 i_EntryIdx, Node_Z* i_Node) {
    S32 l_Idx = GetFreeEntry();
    S32 l_FirstEntryIdx = m_GridDA[i_GridIdx];

    if (m_GridDA[i_GridIdx] != -1) {
        m_EntryPoolDA[l_FirstEntryIdx].m_PrevObjectOfThisEntryIdx = l_Idx;
    }

    SeadEntry_Z& l_SeadEntry = m_EntryPoolDA[l_Idx];
    l_SeadEntry.m_NextEntryOfThisObjectIdx = i_EntryIdx;
    l_SeadEntry.m_NextObjectOfThisEntryIdx = l_FirstEntryIdx;
    l_SeadEntry.m_PrevObjectOfThisEntryIdx = -1;
    l_SeadEntry.m_Node = i_Node;
    l_SeadEntry.m_GridIdx = i_GridIdx;

    m_GridDA[i_GridIdx] = l_Idx;
    i_Node->ClrOccluderZoneId();

    return l_Idx;
}

// TODO: Finish matching
S32 SeadHandle_Z::SetZone(SeadZone_Z& io_Zone, U32 i_Flag, U32 i_NoFlag) const {
    Rect_Z l_Rect;
    Vec2f& l_BMax = io_Zone.m_BMax;
    if (!GetGridRect(io_Zone.m_BMin, l_BMax, l_Rect)) {
        return 0;
    }

    S32 l_GridIdx = l_Rect.x1;
    i_NoFlag |= io_Zone.m_SeadFlag;
    S32 l_SizeX = l_Rect.x2 - l_GridIdx;
    S32 l_SizeY = l_Rect.y2 - l_Rect.y1;
    l_Rect.x2 = l_SizeX;
    l_Rect.x1 = l_SizeX;
    l_Rect.y2 = l_SizeY;
    l_GridIdx += l_Rect.y1 * m_SizeX;

    Float l_CenterX, l_CenterY;
    l_CenterY = 0.5f * (io_Zone.m_BMin.y + l_BMax.y);
    l_CenterX = 0.5f * (io_Zone.m_BMin.x + l_BMax.x);
    Vec2f l_Delta;
    l_Delta.y = l_BMax.y - l_CenterY;
    l_Delta.x = l_BMax.x - l_CenterX;
    Float l_ZoneRadius = l_Delta.GetNorm();
    SeadEntry_Z* l_EntryPool;
    S32* l_GridPool;
    SeadEntry_Z* l_Entry;
    S32* l_GridEntry;
    l_EntryPool = m_EntryPoolDA.GetArrayPtr();
    l_GridPool = m_GridDA.GetArrayPtr();

    while (l_Rect.y2--) {
        l_GridEntry = l_GridPool + l_GridIdx;
        while (l_Rect.x2--) {
            S32 l_EntryIdx = *l_GridEntry++;
            while (l_EntryIdx >= 0) {
                l_Entry = &l_EntryPool[l_EntryIdx];
                U32 l_NodeFlag = l_Entry->m_Node->GetFlag();
                if (!(l_NodeFlag & i_NoFlag) && !(i_Flag & ~l_NodeFlag)) {
                    io_Zone.AddNode(l_Entry->m_Node);
                }
                l_EntryIdx = l_Entry->m_NextObjectOfThisEntryIdx;
            }
        }
        l_GridIdx += m_SizeX;
        l_Rect.x2 = l_Rect.x1;
    }

    SeadZoneData_Z* l_ZoneData = io_Zone.m_HeadSeadZoneData;
    S32 l_ZoneDataNb = io_Zone.m_SeadZoneDataNb;
    while (l_ZoneDataNb--) {
        Sphere_Z& l_Sphere = l_ZoneData->m_Node->GetBSphere();
        Float l_Distance = Vec4_HDist2(*(const Vec4f*)&l_Sphere, Vec4f(l_CenterX, 0.0f, l_CenterY, 1.0f));
        Float l_Radius = l_Sphere.Radius + l_ZoneRadius;
        if (l_Distance < l_Radius * l_Radius) {
            l_ZoneData->m_IntersectsZoneBoundingCircle = TRUE;
        }
        else {
            l_ZoneData->m_IntersectsZoneBoundingCircle = FALSE;
        }
        l_ZoneData++;
    }
    return io_Zone.m_SeadZoneDataNb;
}

void SeadHandle_Z::GetSize(Vec2f& o_Min, Vec2f& o_Max) {
    o_Min = m_PMin;
    o_Max = m_PMax;
}

void SeadHandle_Z::Load(void** i_Data) {
    S32 l_Nb;
    LOADRANGE_Z(m_PMin, m_NbFree);
    LOADDYNARRAY_Z(m_GridDA);
    LOADDYNARRAYSIZE_Z(m_EntryPoolDA);
    for (S32 i = 0; i < l_Nb; i++) {
        m_EntryPoolDA[i].m_Node = NULL;
        LOADRANGE_Z(m_EntryPoolDA[i].m_NextObjectOfThisEntryIdx, m_EntryPoolDA[i].m_GridIdx);
        gData.ClassMgr->LoadLinkId(&m_EntryPoolDA[i].m_Node, i_Data);
    }
}

void SeadHandle_Z::EndLoad() {
    for (S32 i = 0; i < m_EntryPoolDA.GetSize(); i++) {
        if (m_EntryPoolDA[i].m_Node) {
            gData.ClassMgr->UpdateLinkPtr(&m_EntryPoolDA[i].m_Node);
        }
    }
}

void SeadZone_Z::Flush() {
    SeadZoneData_Z* l_SeadZoneData = m_HeadSeadZoneData;
    while (m_SeadZoneDataNb--) {
        l_SeadZoneData->m_Node->DisableFlag(m_SeadFlag | FL_NODE_SEAD_UNUSED);
        l_SeadZoneData++;
    }
    m_SeadZoneDataNb = 0;
}

void SeadZone_Z::SetZone(const Vec2f& i_BMin, const Vec2f& i_BMax) {
    m_BMin = i_BMin;
    m_BMax = i_BMax;
    m_BoundsInitialized = TRUE;
    if (m_World->GetHField()) {
        SeadZoneData_Z* l_ZoneData = AddNode(m_World->GetHField()->GetNode());
        l_ZoneData->m_IntersectsZoneBoundingCircle = TRUE;
    }
}

void SeadZone_Z::ValidZone(SeadHandle_Z& i_SeadHandle, const Vec2f& i_BMin, const Vec2f& i_BMax) {
    if (m_BoundsInitialized) {
        Bool l_Changed = FALSE;
        if (i_BMin.x < m_BMin.x) {
            m_BMin.x = i_BMin.x;
            l_Changed = TRUE;
        }
        if (i_BMin.y < m_BMin.y) {
            m_BMin.y = i_BMin.y;
            l_Changed = TRUE;
        }
        if (i_BMax.x > m_BMax.x) {
            m_BMax.x = i_BMax.x;
            l_Changed = TRUE;
        }
        if (i_BMax.y > m_BMax.y) {
            m_BMax.y = i_BMax.y;
            l_Changed = TRUE;
        }
        if (l_Changed) {
            i_SeadHandle.SetZone(*this, m_Flag, m_NoFlag);
        }
    }
    else {
        SetZone(i_BMin, i_BMax);
        i_SeadHandle.SetZone(*this, m_Flag, m_NoFlag);
    }
}

void SeadZone_Z::ValidSegment(SeadHandle_Z& i_SeadHandle, const Segment_Z& i_Seg) {
    Float l_BMaxX;
    Float l_BMinX;
    Float l_BMaxY;
    Float l_BMinY;
    Float l_Direction = i_Seg.Direction.x;
    if (l_Direction < 0.0f) {
        l_BMaxX = i_Seg.Origin.x;
        l_BMinX = l_Direction * i_Seg.Length + l_BMaxX;
    }
    else {
        l_BMinX = i_Seg.Origin.x;
        l_BMaxX = l_Direction * i_Seg.Length + l_BMinX;
    }
    l_Direction = i_Seg.Direction.z;
    if (l_Direction < 0.0f) {
        l_BMaxY = i_Seg.Origin.z;
        l_BMinY = l_Direction * i_Seg.Length + l_BMaxY;
    }
    else {
        l_BMinY = i_Seg.Origin.z;
        l_BMaxY = l_Direction * i_Seg.Length + l_BMinY;
    }

    Vec2f l_BMin(l_BMinX, l_BMinY);
    Vec2f l_BMax(l_BMaxX, l_BMaxY);

    if (!m_BoundsInitialized) {
        SetZone(l_BMin, l_BMax);
    }
    else {
        if (l_BMin.x < m_BMin.x) {
            m_BMin.x = l_BMin.x;
        }
        if (l_BMin.y < m_BMin.y) {
            m_BMin.y = l_BMin.y;
        }
        if (l_BMax.x > m_BMax.x) {
            m_BMax.x = l_BMax.x;
        }
        if (l_BMax.y > m_BMax.y) {
            m_BMax.y = l_BMax.y;
        }
    }
    i_SeadHandle.SetZone(*this, m_Flag, m_NoFlag);
}

SeadZoneData_Z* SeadZone_Z::AddNode(Node_Z* i_Node) {
    if (m_SeadZoneDataNb >= m_SeadZoneDataReserve) {
        return NULL;
    }
    i_Node->EnableFlag(m_SeadFlag);
    m_SeadZoneDataNb++;
    m_TailSeadZoneData->m_ObjectDatas = i_Node->GetObjectDatas();
    m_TailSeadZoneData->m_Node = i_Node;
    m_TailSeadZoneData->m_Object = i_Node->GetObject(TRUE);
    return m_TailSeadZoneData++;
}
