#ifndef _SEADHANDLE_Z_H_
#define _SEADHANDLE_Z_H_
#include "Math_Z.h"
#include "DynArray_Z.h"
#include "NodeFlag_Z.h"
#include "SystemObject_Z.h"

#define SEAD_RECURSE_MAX_NODE 1024

class Node_Z;
class SeadZone_Z;
class SeadHandle_Z;
class Object_Z;
class ObjectDatas_Z;
class World_Z;
class OccludedSeadHandle_Z;

struct SeadZoneData_Z {
    U32 m_IntersectsZoneBoundingCircle;
    ObjectDatas_Z* m_ObjectDatas;
    Node_Z* m_Node;
    Object_Z* m_Object;
};

typedef DynArray_Z<SeadZoneData_Z, 32, FALSE, FALSE> SeadZoneData_ZDA;

class SeadZone_Z {
    friend class SeadHandle_Z;
    friend class OccludedSeadHandle_Z;
    friend class World_Z;

public:
    static S32 SeadZonesCount;

    SeadZone_Z() {
        ASSERTLE_Z(SeadZonesCount <= 1, "Trop de SeadZones", 31, "SeadZonesCount<=1");
        m_SeadZoneDataReserve = 0;
        m_SeadZoneDataNb = 0;
        m_World = NULL;
        m_SeadFlag = FL_NODE_SEAD;
        SeadZonesCount++;
    }

    SeadZone_Z(SeadZoneData_Z* i_SeadZoneData, S32 i_SeadZoneDataReserve = SEAD_RECURSE_MAX_NODE, U32 i_Flag = FL_NODE_COLLIDE, U32 i_NoFlag = FL_NODE_HIDE) {
        ASSERTLE_Z(SeadZonesCount <= 1, "Trop de SeadZones", 40, "SeadZonesCount<=1");
        m_SeadZoneDataReserve = 0;
        m_SeadZoneDataNb = 0;
        m_World = NULL;
        m_SeadFlag = FL_NODE_SEAD;
        Flush();
        m_HeadSeadZoneData = i_SeadZoneData;
        m_TailSeadZoneData = i_SeadZoneData;
        m_SeadZoneDataNb = 0;
        m_SeadZoneDataReserve = i_SeadZoneDataReserve;
        m_Flag = i_Flag;
        m_NoFlag = i_NoFlag;
        m_BoundsInitialized = FALSE;
        m_SeadFlag = FL_NODE_SEAD;
        SeadZonesCount++;
    }

    ~SeadZone_Z() {
        Flush();
        SeadZonesCount--;
    }

    void Init(SeadZoneData_Z* i_SeadZoneData, S32 i_SeadZoneDataReserve = SEAD_RECURSE_MAX_NODE, U32 i_Flag = FL_NODE_COLLIDE, U32 i_NoFlag = FL_NODE_HIDE) {
        Flush();
        m_HeadSeadZoneData = i_SeadZoneData;
        m_TailSeadZoneData = i_SeadZoneData;
        m_SeadZoneDataNb = 0;
        m_SeadZoneDataReserve = i_SeadZoneDataReserve;
        m_Flag = i_Flag;
        m_NoFlag = i_NoFlag;
        m_BoundsInitialized = FALSE;
        m_SeadFlag = FL_NODE_SEAD;
    }

    void Flush();
    void SetZone(const Vec2f& i_BMin, const Vec2f& i_BMax);
    void ValidZone(SeadHandle_Z& i_SeadHandle, const Vec2f& i_BMin, const Vec2f& i_BMax);
    void ValidSegment(SeadHandle_Z& i_SeadHandle, const Segment_Z& i_Seg);
    SeadZoneData_Z* AddNode(Node_Z* i_Node);

    inline void SetWorld(World_Z* i_World) { m_World = i_World; }

    inline World_Z* GetWorld() const { return m_World; }

    inline void SetSeadFlag(U32 i_Flag) { m_SeadFlag = i_Flag; }

    inline U32 GetSeadFlag() const { return m_SeadFlag; }

private:
    Vec2f m_BMin;              // Bounds min point
    Vec2f m_BMax;              // Bounds max point
    U32 m_Flag;                // Flag that nodes need to have to pass the test
    U32 m_NoFlag;              // Flag that nodes must not have to pass the test
    U32 m_SeadFlag;            // Flag that gets appended to nodes that pass the test (used for marking nodes that have been processed)
    S32 m_SeadZoneDataNb;      // Current number of datas
    S32 m_SeadZoneDataReserve; // Max number of datas
    SeadZoneData_Z* m_HeadSeadZoneData;
    SeadZoneData_Z* m_TailSeadZoneData;
    World_Z* m_World;
    Bool m_BoundsInitialized;
};

struct SeadEntry_Z {
    S32 m_NextObjectOfThisEntryIdx; // Next entry of this grid cell, -1 if last
    S32 m_PrevObjectOfThisEntryIdx; // Previous entry of this grid cell, -1 if first
    S32 m_NextEntryOfThisObjectIdx; // Next entry of this object (in another grid cell), -1 if last
    S32 m_GridIdx;
    Node_Z* m_Node;
};

typedef DynArray_Z<SeadEntry_Z, 32, FALSE> SeadEntry_ZDA;

class SeadHandle_Z {
public:
    SeadHandle_Z();

    Bool GetGridRect(const Vec2f& i_BMin, const Vec2f& i_BMax, Rect_Z& o_Rect) const;
    S32 GetFreeEntry();
    void Ref(S32& io_EntryIdx, Node_Z* i_Node, Rect_Z& io_Rect);
    void UnRef(S32& io_EntryIdx, Rect_Z& o_Rect);
    S32 DoRef(S32 i_GridIdx, S32 i_EntryIdx, Node_Z* i_Node);
    S32 SetZone(SeadZone_Z& io_Zone, U32 i_Flag, U32 i_NoFlag) const;
    void GetSize(Vec2f& o_Min, Vec2f& o_Max);

    inline void SetActive(Bool i_Active) {
        m_Active = i_Active;
    }

protected:
    Vec2f m_PMin;
    Vec2f m_PMax;
    Vec2f m_InvDiag; // 1.0f / (m_PMax - m_PMin)
    S32 m_SizeX;
    S32 m_SizeY;
    S32 m_FirstFreeIdx; // Index of the first free entry in m_EntryPoolDA
    S32 m_NbFree;       // Number of free entries in m_EntryPoolDA
    S32DA m_GridDA;     // First entry of each grid cell, -1 if empty (size = m_SizeX * m_SizeY)
    SeadEntry_ZDA m_EntryPoolDA;
    Bool m_Active;

public:
    virtual void Load(void** i_Data);
    virtual void EndLoad();
};

#endif // _SEADHANDLE_Z_H_
