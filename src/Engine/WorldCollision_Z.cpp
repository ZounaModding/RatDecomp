#include "World_Z.h"
#include "Node_Z.h"
#include "Object_Z.h"
#include "SeadHandle_Z.h"

void World_Z::GetCollisionZone(SeadZone_Z& io_Zone, const Vec2f& i_BMin, const Vec2f& i_BMax, U32 i_Flag, U32 i_NoFlag) {
}

// TODO: Finish matching
Bool World_Z::GetCollisionLines(const Segment_Z& i_Segment, ColLineResult_Z& o_Result, U64 i_Flag, U64 i_NoFlag) {
    U32 l_NoFlag = FL_NODE_HIDE;

    if (GetNbVp()) {
        l_NoFlag = (FL_NODE_1VP_HIDE << (GetNbVp() - 1)) | FL_NODE_HIDE;
    }

    SeadZoneData_Z l_SeadZoneDatas[SEAD_RECURSE_MAX_NODE];
    SeadZone_Z l_Zone(l_SeadZoneDatas, SEAD_RECURSE_MAX_NODE, FL_NODE_COLLIDE, l_NoFlag);

    l_Zone.SetWorld(this);
    l_Zone.ValidSegment(GetSeadCollide(), i_Segment);

    o_Result.m_IsIntersect = FALSE;

    S32 l_Nb = l_Zone.m_SeadZoneDataNb;
    SeadZoneData_Z* l_Data = l_Zone.m_HeadSeadZoneData;

    while (l_Nb--) {
        if (l_Data->m_IntersectsZoneBoundingCircle && (l_Data->m_Node->GetFlag() & FL_NODE_COLLIDE)) {
            l_Data->m_Object->GetCollisionLines(l_Data->m_Node, l_Data->m_ObjectDatas, i_Segment, o_Result, i_Flag, i_NoFlag);
        }

        if ((i_Flag & FL_COL_ONE_HIT) && o_Result.m_IsIntersect) {
            break;
        }

        l_Data++;
    }

    return o_Result.m_IsIntersect;
}
