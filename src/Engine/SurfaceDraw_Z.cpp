#include "Surface_Z.h"

void Surface_Z::GetQuadPatchCtrlPoint(const Patch& i_Patch, QuadCtrlPoint_Z& o_QuadCtrlPoint) {
    S32 l_Rev3 = (i_Patch.m_Flag & EDGE4) != 0;
    S32 l_Rev2 = (i_Patch.m_Flag & EDGE3) != 0;
    S32 l_Rev1 = (i_Patch.m_Flag & EDGE2) != 0;
    S32 l_Rev0 = (i_Patch.m_Flag & EDGE1) != 0;
    S32 l_Edge0 = i_Patch.m_EdgeIndices[0];
    S32 l_Edge2 = i_Patch.m_EdgeIndices[2];
    S32 l_Edge1 = i_Patch.m_EdgeIndices[1];
    S32 l_Edge3 = i_Patch.m_EdgeIndices[3];

    // m_Vertices on the corners
    o_QuadCtrlPoint.m_ControlPoints[0][0] = m_Vertices[m_EdgeTab[l_Edge0].P[l_Rev0]];
    o_QuadCtrlPoint.m_ControlPoints[0][3] = m_Vertices[m_EdgeTab[l_Edge1].P[l_Rev1]];
    o_QuadCtrlPoint.m_ControlPoints[3][3] = m_Vertices[m_EdgeTab[l_Edge2].P[l_Rev2]];
    o_QuadCtrlPoint.m_ControlPoints[3][0] = m_Vertices[m_EdgeTab[l_Edge3].P[l_Rev3]];

    // First side
    o_QuadCtrlPoint.m_ControlPoints[0][1] = m_Vertices[m_EdgeTab[l_Edge0].T[l_Rev0]];
    o_QuadCtrlPoint.m_ControlPoints[0][2] = m_Vertices[m_EdgeTab[l_Edge0].T[1 - l_Rev0]];

    // Second side
    o_QuadCtrlPoint.m_ControlPoints[1][3] = m_Vertices[m_EdgeTab[l_Edge1].T[l_Rev1]];
    o_QuadCtrlPoint.m_ControlPoints[2][3] = m_Vertices[m_EdgeTab[l_Edge1].T[1 - l_Rev1]];

    // Third side
    o_QuadCtrlPoint.m_ControlPoints[3][2] = m_Vertices[m_EdgeTab[l_Edge2].T[l_Rev2]];
    o_QuadCtrlPoint.m_ControlPoints[3][1] = m_Vertices[m_EdgeTab[l_Edge2].T[1 - l_Rev2]];

    // Fourth side
    o_QuadCtrlPoint.m_ControlPoints[2][0] = m_Vertices[m_EdgeTab[l_Edge3].T[l_Rev3]];
    o_QuadCtrlPoint.m_ControlPoints[1][0] = m_Vertices[m_EdgeTab[l_Edge3].T[1 - l_Rev3]];

    // Calculate central points from adjacent corner and side points
    o_QuadCtrlPoint.m_ControlPoints[1][1] = (o_QuadCtrlPoint.m_ControlPoints[1][0] + o_QuadCtrlPoint.m_ControlPoints[0][1]) - o_QuadCtrlPoint.m_ControlPoints[0][0];
    o_QuadCtrlPoint.m_ControlPoints[1][2] = (o_QuadCtrlPoint.m_ControlPoints[0][2] + o_QuadCtrlPoint.m_ControlPoints[1][3]) - o_QuadCtrlPoint.m_ControlPoints[0][3];
    o_QuadCtrlPoint.m_ControlPoints[2][2] = (o_QuadCtrlPoint.m_ControlPoints[2][3] + o_QuadCtrlPoint.m_ControlPoints[3][2]) - o_QuadCtrlPoint.m_ControlPoints[3][3];
    o_QuadCtrlPoint.m_ControlPoints[2][1] = (o_QuadCtrlPoint.m_ControlPoints[3][1] + o_QuadCtrlPoint.m_ControlPoints[2][0]) - o_QuadCtrlPoint.m_ControlPoints[3][0];
}
