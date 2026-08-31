#include "OccludedSeadHandle_Z.h"
#include "Camera_Z.h"
#include "Node_Z.h"
#include "Occluder_Z.h"
#include "Omni_Z.h"
#include "Viewport_Z.h"

// TODO: Finish matching
U32 OccludedSeadHandle_Z::SetFrustrum(StaticArray_Z<OmniFrust_Z, 64, FALSE, FALSE>& o_OmniFrusts, SeadZone_Z& o_Zone1, SeadZone_Z& o_Zone2, DrawInfo_Z& io_DrawInfo, U32 i_Flag, U32 i_NoFlag) {
    Frustrum_Z& l_Frustrum = io_DrawInfo.m_Camera->GetFrustrum();
    Float l_FrustumMaxY;
    Float l_FrustumMaxX;
    Float l_MinX;
    Float l_MinY;
    Float l_FrustumMinY;
    Float l_MaxX;
    Float l_MaxY;
    l_FrustumMinY = l_Frustrum.m_TopViewBoundsMin.y;
    l_FrustumMaxX = l_Frustrum.m_TopViewBoundsMax.x;
    l_FrustumMaxY = l_Frustrum.m_TopViewBoundsMax.y;
    l_MinX = Max(l_Frustrum.m_TopViewBoundsMin.x, m_PMin.x);
    l_MinY = Max(l_FrustumMinY, m_PMin.y);
    l_MaxX = Min(l_FrustumMaxX, m_PMax.x);
    l_MaxY = Min(l_FrustumMaxY, m_PMax.y);

    Float l_GridScaleX = m_InvDiag.x * (Float)m_SizeX;
    Float l_GridScaleY = m_InvDiag.y * (Float)m_SizeY;
    S32 l_GridMinX = l_GridScaleX * (l_MinX - m_PMin.x);
    Float l_GridMaxXFloat = l_GridScaleX * (l_MaxX - m_PMin.x);
    S32 l_GridMaxX = 1.0f + l_GridMaxXFloat;
    S32 l_GridMinY = l_GridScaleY * (l_MinY - m_PMin.y);
    Float l_GridMaxYFloat = l_GridScaleY * (l_MaxY - m_PMin.y);
    S32 l_GridMaxY = 1.0f + l_GridMaxYFloat;

    if (l_GridMaxX < 0) {
        return 0;
    }
    if (l_GridMaxX > m_SizeX) {
        l_GridMaxX = m_SizeX;
    }
    if (l_GridMaxY < 0) {
        return 0;
    }
    if (l_GridMaxY > m_SizeY) {
        l_GridMaxY = m_SizeY;
    }
    if (l_GridMinX >= m_SizeX) {
        return 0;
    }
    if (l_GridMinX < 0) {
        l_GridMinX = 0;
    }
    if (l_GridMinY >= m_SizeY) {
        return 0;
    }
    if (l_GridMinY < 0) {
        l_GridMinY = 0;
    }

    Float l_StartX;
    Float l_StartY;
    Float l_CellWidth;
    Float l_CellHeight;
    l_CellHeight = 1.0f / l_GridScaleY;
    S32 l_GridIdx = l_GridMinX + l_GridMinY * m_SizeX;
    S32 l_GridHeight = l_GridMaxY - l_GridMinY;
    l_CellWidth = 1.0f / l_GridScaleX;
    S32 l_GridX = l_GridMinX;
    l_StartY = l_GridMinY * l_CellHeight + m_PMin.y;
    i_NoFlag |= o_Zone1.m_SeadFlag;
    S32 l_GridWidth = l_GridMaxX - l_GridMinX;
    l_StartX = l_GridMinX * l_CellWidth + m_PMin.x;
    if (l_GridHeight == 0) {
        l_GridHeight++;
    }
    if (l_GridWidth == 0) {
        l_GridWidth++;
    }

    const OccludedFrustum_Z& l_OccludedFrustum = io_DrawInfo.m_Camera->GetOccludedFrustum();
    if (io_DrawInfo.m_IsOccluded) {
        FloatDA l_UnusedIntersections;
        FloatDA l_Intersections;
        BitArray_Z l_TestedCells;
        l_TestedCells.SetSize(l_GridWidth * l_GridHeight);
        l_TestedCells.ClearAllBits();

        S32 l_LastX = l_GridWidth - 1;
        Float l_Y = l_StartY;
        S32 l_BitRow = 0;
        S32 l_GridY = l_GridMinY;
        for (S32 y = 0; y < l_GridHeight; y++) {
            if (l_OccludedFrustum.GetPtsOnLineY(l_Y, l_Intersections)) {
                S32 l_PreviousGridY = l_GridY - 1;
                S32 l_IntersectionOffset = 0;
                for (S32 i = 0; i < l_Intersections.GetSize(); i += 2) {
                    S32 l_First = (*(Float*)((U8*)l_Intersections.GetArrayPtr() + l_IntersectionOffset) - l_StartX) / l_CellWidth;
                    S32 l_Last = (l_Intersections[i + 1] - l_StartX) / l_CellWidth;
                    if (l_First < 0) {
                        l_First = 0;
                    }
                    if (l_Last >= l_GridWidth) {
                        l_Last = l_LastX;
                    }

                    S32 l_CurrentGridX = l_GridMinX + l_First;
                    for (S32 x = l_First; x <= l_Last; x++, l_CurrentGridX++) {
                        S32 l_Bit = l_BitRow + x;
                        if (!l_TestedCells.GetBit(l_Bit)) {
                            l_TestedCells.SetBit(l_Bit);
                            S32 l_EntryIdx = m_GridDA[l_CurrentGridX + m_SizeX * l_GridY];
                            if (l_EntryIdx >= 0) {
                                SetFrustrumForSeadEntry(l_EntryIdx, o_OmniFrusts, o_Zone1, o_Zone2, io_DrawInfo, i_Flag, i_NoFlag);
                            }
                        }
                        if (y > 0 && !l_TestedCells.GetBit(l_Bit - l_GridWidth)) {
                            l_TestedCells.SetBit(l_Bit - l_GridWidth);
                            S32 l_EntryIdx = m_GridDA[l_CurrentGridX + m_SizeX * l_PreviousGridY];
                            if (l_EntryIdx >= 0) {
                                SetFrustrumForSeadEntry(l_EntryIdx, o_OmniFrusts, o_Zone1, o_Zone2, io_DrawInfo, i_Flag, i_NoFlag);
                            }
                        }
                    }
                    l_IntersectionOffset += 8;
                }
            }
            l_Y += l_CellHeight;
            l_BitRow += l_GridWidth;
            l_GridY++;
        }

        S32 l_LastY = l_GridHeight - 1;
        Float l_X = l_StartX;
        S32 l_BitColumn = 0;
        l_GridX = l_GridMinX;
        for (S32 x = 0; x < l_GridWidth; x++) {
            if (l_OccludedFrustum.GetPtsOnLineX(l_X, l_Intersections)) {
                S32 l_PreviousGridX = l_GridX - 1;
                S32 l_IntersectionOffset = 0;
                for (S32 i = 0; i < l_Intersections.GetSize(); i += 2) {
                    S32 l_First = (*(Float*)((U8*)l_Intersections.GetArrayPtr() + l_IntersectionOffset) - l_StartY) / l_CellHeight;
                    S32 l_Last = (l_Intersections[i + 1] - l_StartY) / l_CellHeight;
                    if (l_First < 0) {
                        l_First = 0;
                    }
                    if (l_Last >= l_GridHeight) {
                        l_Last = l_LastY;
                    }

                    S32 l_BitOffset = l_First * l_GridWidth;
                    S32 l_GridY = l_GridMinY + l_First;
                    for (S32 y = l_First; y <= l_Last; y++, l_BitOffset += l_GridWidth, l_GridY++) {
                        S32 l_Bit = l_BitColumn + l_BitOffset;
                        if (!l_TestedCells.GetBit(l_Bit)) {
                            l_TestedCells.SetBit(l_Bit);
                            S32 l_EntryIdx = m_GridDA[l_GridX + m_SizeX * l_GridY];
                            if (l_EntryIdx >= 0) {
                                SetFrustrumForSeadEntry(l_EntryIdx, o_OmniFrusts, o_Zone1, o_Zone2, io_DrawInfo, i_Flag, i_NoFlag);
                            }
                        }
                        if (x > 0 && !l_TestedCells.GetBit(l_Bit - 1)) {
                            l_TestedCells.SetBit(l_Bit - 1);
                            S32 l_EntryIdx = m_GridDA[l_PreviousGridX + m_SizeX * l_GridY];
                            if (l_EntryIdx >= 0) {
                                SetFrustrumForSeadEntry(l_EntryIdx, o_OmniFrusts, o_Zone1, o_Zone2, io_DrawInfo, i_Flag, i_NoFlag);
                            }
                        }
                    }
                    l_IntersectionOffset += 8;
                }
            }
            l_X += l_CellWidth;
            l_BitColumn++;
            l_GridX++;
        }

        if (l_TestedCells.FindFirstBit() >= 0) {
            return o_Zone1.m_SeadZoneDataNb;
        }
    }

    Float l_CurrentY = l_StartY;
    while (l_GridHeight--) {
        Vec2f l_RectMin;
        Vec2f l_RectMax;
        l_RectMax.y = l_CurrentY + l_CellHeight;
        l_RectMin.y = l_CurrentY;
        S32* l_EntryIdx = &m_GridDA[l_GridIdx];
        S32 l_Width = l_GridWidth;
        l_RectMax.x = l_StartX + l_CellWidth;
        l_RectMin.x = l_StartX;
        while (l_Width--) {
            S32 l_Entry = *l_EntryIdx++;
            if (l_Entry < 0) {
                l_RectMin.x += l_CellWidth;
                l_RectMax.x += l_CellWidth;
                continue;
            }
            if (io_DrawInfo.m_IsOccluded && Occluder_Z::IsOccludedRect(l_OccludedFrustum, l_RectMin, l_RectMax)) {
                l_RectMin.x += l_CellWidth;
                l_RectMax.x += l_CellWidth;
                continue;
            }
            SetFrustrumForSeadEntry(l_Entry, o_OmniFrusts, o_Zone1, o_Zone2, io_DrawInfo, i_Flag, i_NoFlag);
            l_RectMin.x += l_CellWidth;
            l_RectMax.x += l_CellWidth;
        }
        l_CurrentY += l_CellHeight;
        l_GridIdx += m_SizeX;
    }
    return o_Zone1.m_SeadZoneDataNb;
}

// TODO: Finish matching
void OccludedSeadHandle_Z::SetFrustrumForSeadEntry(S32 i_EntryIdx, StaticArray_Z<OmniFrust_Z, 64, FALSE, FALSE>& o_OmniFrusts, SeadZone_Z& o_Zone1, SeadZone_Z& o_Zone2, DrawInfo_Z& io_DrawInfo, U32 i_Flag, U32 i_NoFlag) {
    Occluder_Z* l_Occluder = io_DrawInfo.m_Occluder;
    Camera_Z* l_Camera = io_DrawInfo.m_Camera;
    Frustrum_Z* l_Frustrum = &l_Camera->GetFrustrum();
    U32* l_OccluderZoneIndices = l_Occluder ? l_Occluder->m_NodeOccluderZoneIndices.GetArrayPtr() : NULL;
    U32 l_SeadFlag = o_Zone1.m_SeadFlag;

    while (i_EntryIdx >= 0) {
        SeadEntry_Z* l_Entry = &m_EntryPoolDA[i_EntryIdx];
        Node_Z* l_Node = l_Entry->m_Node;
        if (!l_Node->IsFlagEnable(i_NoFlag) && !(i_Flag & ~l_Node->GetFlag())) {
            l_Node->EnableFlag(l_SeadFlag);

            Bool l_IsOccluded;
            if (!l_Node->GetOccluderZoneId() || !l_OccluderZoneIndices) {
                l_IsOccluded = FALSE;
            }
            else {
                U32* l_ZoneIdx = l_OccluderZoneIndices + l_Node->GetOccluderZoneId();
                U32 l_ZoneCount = *l_ZoneIdx++;
                l_IsOccluded = TRUE;
                while (l_ZoneCount--) {
                    if (!l_Camera->GetOccludedZones().GetBit(*l_ZoneIdx++)) {
                        l_IsOccluded = FALSE;
                        break;
                    }
                }
            }

            if (l_IsOccluded) {
                o_Zone2.AddNode(l_Node);
            }
            else {
                if (l_Node->GetObject()->GetGeometryType() == OMNI_Z) {
                    o_Zone2.AddNode(l_Node);
                    Omni_Z* l_Omni = (Omni_Z*)l_Node->GetObject();
                    if (l_Omni->IsActive()) {
                        Mat4x4 l_LocalToCam;
                        l_LocalToCam = io_DrawInfo.m_World2Cam * l_Node->GetWorldMatrix();
                        ClipSphere_Z l_ClipSphere;
                        if (SphereVsFrustrum(*l_Frustrum, l_LocalToCam, l_Omni->GetBSphere(), l_ClipSphere, l_Node->GetUniformScale())) {
                            S32 l_Index = o_OmniFrusts.Add();
                            Omni_Z::InFrustrum(io_DrawInfo, l_ClipSphere.Sph, l_Node, l_Omni, o_OmniFrusts[l_Index], l_Index);
                        }
                    }
                }
                else {
                    o_Zone1.AddNode(l_Node);
                }
            }
        }
        i_EntryIdx = l_Entry->m_NextObjectOfThisEntryIdx;
    }
}
