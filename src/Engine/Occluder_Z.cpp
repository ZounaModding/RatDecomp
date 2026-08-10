#include "Occluder_Z.h"
#include "Spline_Z.h"

void Occluder_Z::InitBitArrays() {
    m_VisitedZonesBA.SetSize(m_SplineZones.GetSize());
    m_ProcessedSplinesBA.SetSize(m_Splines.GetSize());
    m_TestedPointsBA.SetSize(m_Points.GetSize());
    m_PointsInFrustumBA.SetSize(m_Points.GetSize());
}

void Occluder_Z::Load(void** i_Data) {
    SplineZone_Z::Load(i_Data);
    InitBitArrays();
    S32 l_Nb;
    LOADDYNARRAY_Z(m_NodeOccluderZoneIndices);
    S32 l_UnkByteNb;
    LOAD_Z(l_UnkByteNb);
    SKIPBYTES_Z(l_UnkByteNb);
}

// TODO: Finish matching, document
Float Occluder_Z::Occlusion(const Camera_ZHdl& i_CameraHdl, OccludedFrustum_Z& o_OccludedFrustum, BitArray_Z& o_OccludedZones) {
    o_OccludedFrustum.m_IsOccluded = FALSE;

    Camera_Z* l_Camera = i_CameraHdl;
    m_OccludedFarClip = l_Camera->GetFarClip();

    const Vec3f& l_CameraPosition = l_Camera->GetWorldPos();
    const Vec3f& l_CameraTarget = l_Camera->GetTarget();
    Float l_HeightDelta = l_CameraTarget.y - l_CameraPosition.y;

    OccludedFrustum_Z l_BaseCameraFrustum;
    l_BaseCameraFrustum.m_PointNb = 0;
    l_BaseCameraFrustum.m_IsOccluded = FALSE;
    l_BaseCameraFrustum.m_Points[0] = Vec2f(l_CameraPosition.x, l_CameraPosition.z);
    Vec2f l_TargetTopView(l_CameraTarget.x, l_CameraTarget.z);
    l_BaseCameraFrustum.m_CamToTarget = l_TargetTopView - l_BaseCameraFrustum.m_Points[0];

    const Frustrum_Z& l_CameraFrustum = l_Camera->GetFrustrum();
    // Is the target at the same height as the camera?
    if (l_HeightDelta > -Float_Eps && l_HeightDelta < Float_Eps) {
        l_BaseCameraFrustum.m_Points[1] = Vec2f(
            l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_LEFT_FAR].x,
            l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_LEFT_FAR].z
        );
        l_BaseCameraFrustum.m_Points[2] = Vec2f(
            l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_RIGHT_FAR].x,
            l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_RIGHT_FAR].z
        );
        l_BaseCameraFrustum.m_PointNb = 3;
    }
    else {
        // Is the camera pointing up?
        if (l_HeightDelta > 0.0f) {
            l_BaseCameraFrustum.m_Points[1] = Vec2f(
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_TOP_LEFT_FAR].x,
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_TOP_LEFT_FAR].z
            );
            l_BaseCameraFrustum.m_Points[2] = Vec2f(
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_LEFT_FAR].x,
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_LEFT_FAR].z
            );
            l_BaseCameraFrustum.m_Points[3] = Vec2f(
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_RIGHT_FAR].x,
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_RIGHT_FAR].z
            );
            l_BaseCameraFrustum.m_Points[4] = Vec2f(
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_TOP_RIGHT_FAR].x,
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_TOP_RIGHT_FAR].z
            );
        }
        else {
            l_BaseCameraFrustum.m_Points[1] = Vec2f(
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_LEFT_FAR].x,
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_LEFT_FAR].z
            );
            l_BaseCameraFrustum.m_Points[2] = Vec2f(
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_TOP_LEFT_FAR].x,
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_TOP_LEFT_FAR].z
            );
            l_BaseCameraFrustum.m_Points[3] = Vec2f(
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_TOP_RIGHT_FAR].x,
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_TOP_RIGHT_FAR].z
            );
            l_BaseCameraFrustum.m_Points[4] = Vec2f(
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_RIGHT_FAR].x,
                l_CameraFrustum.m_CornerPoints[FRUST_CORNER_BOTTOM_RIGHT_FAR].z
            );
        }

        l_BaseCameraFrustum.m_PointNb = 5;
        for (S32 i = 0; i < l_BaseCameraFrustum.m_PointNb - 1; i++) {
            S32 l_NextPointIndex = i + 1;
            if (l_NextPointIndex > l_BaseCameraFrustum.m_PointNb - 1) {
                l_NextPointIndex -= l_BaseCameraFrustum.m_PointNb;
            }
            Vec2f& l_NextPoint = l_BaseCameraFrustum.m_Points[l_NextPointIndex];
            S32 l_NextNextPointIndex = i + 2;
            if (l_NextNextPointIndex > l_BaseCameraFrustum.m_PointNb - 1) {
                l_NextNextPointIndex -= l_BaseCameraFrustum.m_PointNb;
            }
            Vec2f& l_NextNextPoint = l_BaseCameraFrustum.m_Points[l_NextNextPointIndex];
            Float x = l_NextNextPoint.x - l_NextPoint.x;
            Float y = l_NextNextPoint.y - l_NextPoint.y;
            Vec2f l_SecondEdge = Vec2f(x, y);

            S32 l_NextPointIndex2 = i + 1;
            if (l_NextPointIndex2 > l_BaseCameraFrustum.m_PointNb - 1) {
                l_NextPointIndex2 -= l_BaseCameraFrustum.m_PointNb;
            }
            Vec2f& l_NextPoint2 = l_BaseCameraFrustum.m_Points[l_NextPointIndex2];
            Vec2f l_FirstEdge = l_NextPoint2 - l_BaseCameraFrustum.m_Points[i];

            if ((l_SecondEdge ^ l_FirstEdge) < 0.0f) {
                S32 l_NextPointIndex3 = i + 1;
                if (l_NextPointIndex3 > l_BaseCameraFrustum.m_PointNb - 1) {
                    l_NextPointIndex3 -= l_BaseCameraFrustum.m_PointNb;
                }
                for (S32 j = l_NextPointIndex3; j < l_BaseCameraFrustum.m_PointNb; j++) {
                    l_BaseCameraFrustum.m_Points[j] = l_BaseCameraFrustum.m_Points[j + 1];
                }
                l_BaseCameraFrustum.m_PointNb--;
                i = l_BaseCameraFrustum.m_PointNb;
            }
        }
    }

    l_BaseCameraFrustum.m_WorldPos = l_CameraPosition;
    l_BaseCameraFrustum.m_TargetWorldPos = l_CameraTarget;
    l_BaseCameraFrustum.DoVec();

    const Vec2f& l_CamPos = l_BaseCameraFrustum.m_Points[0];

    Vec2f l_RightDirection = l_BaseCameraFrustum.m_Points[1] - l_CamPos;
    Vec2f l_LeftDirection = l_BaseCameraFrustum.m_Points[l_BaseCameraFrustum.m_PointNb - 1] - l_CamPos;

    l_RightDirection.Normalize();
    l_LeftDirection.Normalize();

    l_BaseCameraFrustum.m_ProjectionLineStart = l_CamPos + l_RightDirection;
    l_BaseCameraFrustum.m_ProjectionLineDelta = l_LeftDirection - l_RightDirection;

    Float l_HorizontalProjection;
    for (S32 i = 1; i < l_BaseCameraFrustum.m_PointNb; i++) {
        GetPosEcran(
            l_BaseCameraFrustum,
            l_BaseCameraFrustum.m_Points[i],
            l_HorizontalProjection
        );
        l_BaseCameraFrustum.m_HorizontalProjection[i] = l_HorizontalProjection;
    }

    l_BaseCameraFrustum.m_HorizontalProjection[0] = 0.0f;
    l_BaseCameraFrustum.m_HorizontalProjection[1] = 0.0f;
    l_BaseCameraFrustum.m_HorizontalProjection[l_BaseCameraFrustum.m_PointNb - 1] = 1.0f;

    S32 l_CameraZoneId;
    if (!GetZone(l_CameraPosition, l_CameraZoneId)) {
        return -1.0f;
    }

    m_VisitedZonesBA.ClearAllBits();
    m_ProcessedSplinesBA.ClearAllBits();
    o_OccludedZones.SetAllBits();
    o_OccludedZones.ClearBit(0);

    Float l_OccludedFarClip = Occlusion(
        o_OccludedFrustum,
        o_OccludedZones,
        l_BaseCameraFrustum,
        l_CameraZoneId,
        l_Camera->GetDir()
    );

    o_OccludedFrustum.DoVec();

    return l_OccludedFarClip;
}

// TODO: Finish matching
Bool Occluder_Z::IsZoneInFrustrum(const OccludedFrustum_Z& i_OccludedFrustum, S32 i_ZoneId) {
    U16 l_DirectionBits = m_SplineZones[i_ZoneId].m_SplineDirectionBits;
    S16 l_SplineCount = m_SplineZones[i_ZoneId].m_SplineIdsNb;
    U16* l_SplineIds = m_SplineIndices.GetArrayPtr() + m_SplineZones[i_ZoneId].m_SplineIdsIdx;

    U16 l_PointId = l_DirectionBits & FL_SPLINE_NORMAL_DIR ? m_Splines[*l_SplineIds].m_P0 : m_Splines[*l_SplineIds].m_P1;

    // If the first point has already been tested and is in the frustum, then we can return true immediately
    if (m_TestedPointsBA.GetBit(l_PointId) && m_PointsInFrustumBA.GetBit(l_PointId)) {
        return TRUE;
    }

    Vec2f l_ZonePoints[MAX_SPLINES_ON_ZONE + 1];
    int l_FrustumEdgeIndices[MAX_SPLINES_ON_ZONE + 1];
    Vec2f* l_ZonePointsBase = l_ZonePoints;
    // $SABE: Wtf is this shit
    Vec2f* l_ZonePointsY = (Vec2f*)&l_ZonePoints[0].y;

    // Set the first point of the zone polygon to the first point of the first spline in the zone
    l_ZonePoints[0].x = m_Points[l_PointId].x;
    l_ZonePoints[0].y = m_Points[l_PointId].z;

    // Mark the first point as tested
    m_TestedPointsBA.SetBit(l_PointId);

    if (IsPtInFrs(i_OccludedFrustum, *l_ZonePointsBase, l_FrustumEdgeIndices[0])) {
        // The first point is in the frustum, so mark it as such and return true
        m_PointsInFrustumBA.SetBit(l_PointId);
        return TRUE;
    }

    // This loop assumes that the second point of the first spline is the first point of the second spline, and so on
    S32 l_ZonePointCount = 1;
    for (S32 i = 0; i < l_SplineCount - 1; i++) {
        l_PointId = l_DirectionBits & FL_SPLINE_NORMAL_DIR ? m_Splines[*l_SplineIds].m_P1 : m_Splines[*l_SplineIds].m_P0;

        // If the point has already been tested and is in the frustum, then we can return true immediately
        if (m_TestedPointsBA.GetBit(l_PointId) && m_PointsInFrustumBA.GetBit(l_PointId)) {
            return TRUE;
        }

        // Add the point to the zone polygon
        l_ZonePoints[l_ZonePointCount].x = m_Points[l_PointId].x;
        l_ZonePoints[l_ZonePointCount].y = m_Points[l_PointId].z;

        // Mark the point as tested
        m_TestedPointsBA.SetBit(l_PointId);

        if (IsPtInFrs(
                i_OccludedFrustum,
                l_ZonePoints[l_ZonePointCount],
                l_FrustumEdgeIndices[l_ZonePointCount]
            )) {
            // The point is in the frustum, so mark it as such and return true
            m_PointsInFrustumBA.SetBit(l_PointId);
            return TRUE;
        }

        l_DirectionBits >>= 1;
        l_SplineIds++;
        l_ZonePointCount++;
    }

    // Close the zone polygon by adding the first point to the end of the array
    l_ZonePoints[l_ZonePointCount].x = l_ZonePoints[0].x;
    // $SABE: "Set my y pointer's x" - statements dreamed up by the utterly deranged
    l_ZonePointsY[l_ZonePointCount].x = l_ZonePoints[0].y;

    Vec2f* l_ZonePoint = l_ZonePoints;
    l_FrustumEdgeIndices[l_ZonePointCount] = l_FrustumEdgeIndices[0];
    int* l_FrustumEdgeIndex = l_FrustumEdgeIndices;
    for (S32 i = 0; i < l_ZonePointCount; i++, l_FrustumEdgeIndex++, l_ZonePoint++) {
        int l_FirstFrustumEdge = l_FrustumEdgeIndex[0];
        int l_SecondFrustumEdge = l_FrustumEdgeIndex[1];

        // If either of the points of the zone edge are not in the frustum's angular span, then we
        // have to check all of the frustum edges to see if the zone edge intersects with any of them
        if (l_FirstFrustumEdge == -1 || l_SecondFrustumEdge == -1) {
            Vec2f l_ZoneEdgeDelta = l_ZonePoint[1] - l_ZonePoint[0];
            for (S32 l_FrustumEdgeIndex = 1; l_FrustumEdgeIndex < i_OccludedFrustum.m_PointNb; l_FrustumEdgeIndex++) {
                Vec2f l_CamToFrustumEdgeStart = i_OccludedFrustum.m_Points[l_FrustumEdgeIndex] - i_OccludedFrustum.m_Points[0];
                Float l_Denominator = l_CamToFrustumEdgeStart ^ l_ZoneEdgeDelta;

                // If the denominator is 0, the lines are parallel and cannot intersect at a single point
                if (fabsf(l_Denominator) > Float_Eps) {
                    Vec2f l_ZoneStartToCamera = i_OccludedFrustum.m_Points[0] - l_ZonePoint[0];
                    Float l_ZoneProjection = (l_CamToFrustumEdgeStart ^ l_ZoneStartToCamera) / l_Denominator;

                    // Continue only if the intersection is between the two endpoints of the zone edge
                    if (l_ZoneProjection >= 0.0f && l_ZoneProjection <= 1.0f) {
                        Float l_FrustumProjection = (l_ZoneEdgeDelta ^ l_ZoneStartToCamera) / l_Denominator;

                        // The segments intersect only if the same intersection
                        // also is between the camera and the frustum edge point
                        if (l_FrustumProjection <= 1.0f + Float_Eps && l_FrustumProjection >= 0.0f) {
                            return TRUE;
                        }
                    }
                }
            }
        }
        // By this point if the zone edge start and end points are in the same frustum edge,
        // then the zone edge is completely outside of the frustum and cannot intersect with it
        else if (l_FirstFrustumEdge != l_SecondFrustumEdge) {
            // Otherwise we check the frustum edges between the two edges that the zone
            // edge start and end points are in to see if any of them intersect with it
            int l_FrustumEdgeEnd = l_FirstFrustumEdge;
            int l_FrustumEdgeStart = l_SecondFrustumEdge;
            if (l_FirstFrustumEdge < l_SecondFrustumEdge) {
                l_FrustumEdgeEnd = l_SecondFrustumEdge;
                l_FrustumEdgeStart = l_FirstFrustumEdge;
            }
            Vec2f l_ZoneEdgeDelta = l_ZonePoint[1] - l_ZonePoint[0];

            for (S32 l_FrustumEdgeIndex = l_FrustumEdgeStart; l_FrustumEdgeIndex < l_FrustumEdgeEnd + 1; l_FrustumEdgeIndex++) {
                Vec2f l_CamToFrustumEdgeStart = i_OccludedFrustum.m_Points[l_FrustumEdgeIndex] - i_OccludedFrustum.m_Points[0];
                Float l_Denominator = l_CamToFrustumEdgeStart ^ l_ZoneEdgeDelta;

                // If the denominator is 0, the lines are parallel and cannot intersect at a single point
                if (fabsf(l_Denominator) > Float_Eps) {
                    Vec2f l_ZoneStartToCamera = i_OccludedFrustum.m_Points[0] - l_ZonePoint[0];
                    Float l_ZoneProjection = (l_CamToFrustumEdgeStart ^ l_ZoneStartToCamera) / l_Denominator;

                    // Continue only if the intersection is between the two endpoints of the zone edge
                    if (l_ZoneProjection >= 0.0f && l_ZoneProjection <= 1.0f) {
                        Float l_FrustumProjection = (l_ZoneEdgeDelta ^ l_ZoneStartToCamera) / l_Denominator;

                        // The segments intersect only if the same intersection
                        // also is between the camera and the frustum edge point
                        if (l_FrustumProjection <= 1.0f + Float_Eps && l_FrustumProjection >= 0.0f) {
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    const Vec2f& l_CameraPosition = i_OccludedFrustum.m_Points[0];
    U32 l_IntersectionCount = 0;

    // Count the number of times a horizontal ray from the camera position intersects with the zone polygon edges
    // This is for the case where the frustum is completely inside the zone polygon, in which case the zone is visible
    for (S32 i = 0; i < l_ZonePointCount; i++, l_ZonePointsBase++) {
        const Vec2f& l_Start = l_ZonePointsBase[0];
        const Vec2f& l_End = l_ZonePointsBase[1];

        if ((l_Start.y < l_CameraPosition.y && l_CameraPosition.y <= l_End.y) || (l_CameraPosition.y <= l_Start.y && l_End.y < l_CameraPosition.y)) {
            if (l_CameraPosition.x <= l_End.x || l_CameraPosition.x <= l_Start.x) {
                if (l_End.x < l_CameraPosition.x || l_Start.x < l_CameraPosition.x) {
                    if (l_Start.y <= l_End.y) {
                        if (((l_End.x - l_Start.x) * (l_CameraPosition.y - l_Start.y) - (l_End.y - l_Start.y) * (l_CameraPosition.x - l_Start.x)) < 0.0f) {
                            l_IntersectionCount++;
                        }
                    }
                    else if (((l_Start.x - l_End.x) * (l_CameraPosition.y - l_End.y) - (l_Start.y - l_End.y) * (l_CameraPosition.x - l_End.x)) < 0.0f) {
                        l_IntersectionCount++;
                    }
                }
            }
            else {
                l_IntersectionCount++;
            }
        }
    }

    // If the number of intersections is odd, then the camera
    // position is inside the zone polygon, and the zone is visible
    return l_IntersectionCount & 1;
}

// TODO: Finish matching, document
Float Occluder_Z::Occlusion(
    OccludedFrustum_Z& o_OccludedFrustum,
    BitArray_Z& io_OccludedZones,
    const OccludedFrustum_Z& i_BaseCameraFrustum,
    S32 i_StartZoneId,
    const Vec3f& i_CameraDirection
) {
    OccludeSeg_Z l_Segments[MAX_OCCLUDED_SEGMENTS];
    OccludeSeg_Z* l_NextSegment;
    o_OccludedFrustum.m_PointNb = 0;

    Vec2f l_RightEdge = i_BaseCameraFrustum.m_Points[1] - i_BaseCameraFrustum.m_Points[0];
    Vec2f l_LeftEdge = i_BaseCameraFrustum.m_Points[i_BaseCameraFrustum.m_PointNb - 1] - i_BaseCameraFrustum.m_Points[0];

    if ((l_RightEdge ^ l_LeftEdge) >= 0.0f) {
        io_OccludedZones.ClearAllBits();
        return 0.0f;
    }

    o_OccludedFrustum.m_IsOccluded = TRUE;
    l_NextSegment = l_Segments;
    GetFrustrumOccludeSegment(
        i_BaseCameraFrustum,
        i_StartZoneId,
        &l_NextSegment
    );
    Float l_OccludedFarClip;
    S32 l_SegmentCount = ((U32)l_NextSegment - (U32)l_Segments) / sizeof(OccludeSeg_Z);
    if (l_SegmentCount == 0 || l_SegmentCount >= MAX_OCCLUDED_SEGMENTS) {
        S32 l_ZoneId = m_VisitedZonesBA.FindFirstBit(TRUE, 0);

        o_OccludedFrustum = i_BaseCameraFrustum;

        while (l_ZoneId >= 0) {
            io_OccludedZones.ClearBit(l_ZoneId + 2);
            l_ZoneId = m_VisitedZonesBA.FindFirstBit(TRUE, l_ZoneId + 1);
        }

        l_OccludedFarClip = m_OccludedFarClip;
    }
    else {
        l_OccludedFarClip = GetOccludedFrustrum(
            o_OccludedFrustum,
            i_BaseCameraFrustum,
            l_Segments,
            l_SegmentCount
        );
        Float l_HorizontalDirectionNorm = Sqrt(1.0f - i_CameraDirection.y * i_CameraDirection.y);
        l_OccludedFarClip /= l_HorizontalDirectionNorm;

        S32 l_ZoneId = m_VisitedZonesBA.FindFirstBit(TRUE, 0);
        m_TestedPointsBA.ClearAllBits();
        m_PointsInFrustumBA.ClearAllBits();

        while (l_ZoneId >= 0) {
            if (IsZoneInFrustrum(o_OccludedFrustum, l_ZoneId)) {
                io_OccludedZones.ClearBit(l_ZoneId + 2);
            }
            l_ZoneId = m_VisitedZonesBA.FindFirstBit(TRUE, l_ZoneId + 1);
        }
    }

    return l_OccludedFarClip;
}

// TODO: Finish matching, document
void Occluder_Z::GetFrustrumOccludeSegment(
    const OccludedFrustum_Z& i_BaseCameraFrustum,
    S32 i_StartZoneId,
    OccludeSeg_Z** io_NextSegment
) {
    S32 l_ZoneStack[ZONE_STACK_SIZE];
    int l_FrustumEdgeIndex;
    S32 l_ZoneStackSize = 1;
    l_ZoneStack[0] = i_StartZoneId;

    m_TestedPointsBA.ClearAllBits();
    m_PointsInFrustumBA.ClearAllBits();
    m_VisitedZonesBA.SetBit(i_StartZoneId);

    while (l_ZoneStackSize) {
        S32 l_ZoneId = l_ZoneStack[l_ZoneStackSize - 1];
        l_ZoneStackSize--;

        SplineZone* l_SplineZones = m_SplineZones.GetArrayPtr();
        U16* l_AllSplineIds = m_SplineIndices.GetArrayPtr();
        SplineZoneLink* l_SplineZoneLinks = m_SplineZoneLinks.GetArrayPtr();
        SplineZone* l_Zone = l_SplineZones + l_ZoneId;

        Vec3f* l_FirstPoint;
        S32 l_FirstPointInFrustum;
        S16 l_SplineCount = l_Zone->m_SplineIdsNb;
        U16 l_TypeBits = l_Zone->m_SplineTypeBits;
        U16 l_DirectionBits = l_Zone->m_SplineDirectionBits;
        U16* l_SplineIds = l_AllSplineIds + l_Zone->m_SplineIdsIdx;
        SplineZoneLink* l_ZoneLink = l_SplineZoneLinks + l_Zone->m_SplineZoneLinkIdx;
        const U16 l_ZoneFlags = l_Zone->m_Flag;

        CornerSpline* l_FirstSpline = m_Splines.GetArrayPtr() + *l_SplineIds;
        U16 l_FirstPointId;
        if (l_DirectionBits & FL_SPLINE_NORMAL_DIR) {
            l_FirstPointId = l_FirstSpline->m_P0;
        }
        else {
            l_FirstPointId = l_FirstSpline->m_P1;
        }
        l_FirstPoint = &m_Points[l_FirstPointId];

        Vec2f l_FirstTopViewPoint;
        if (m_TestedPointsBA.GetBit(l_FirstPointId) == 0) {
            m_TestedPointsBA.SetBit(l_FirstPointId);
            l_FirstTopViewPoint = Vec2f(
                l_FirstPoint->x,
                l_FirstPoint->z
            );
            if (IsPtInFrs(
                    i_BaseCameraFrustum,
                    l_FirstTopViewPoint,
                    l_FrustumEdgeIndex
                )) {
                l_FirstPointInFrustum = 1;
                m_PointsInFrustumBA.SetBit(l_FirstPointId);
            }
            else {
                l_FirstPointInFrustum = 0;
            }
        }
        else {
            l_FirstPointInFrustum = m_PointsInFrustumBA.GetBit(l_FirstPointId);
        }

        while (l_SplineCount--) {
            U16 l_SplineId = *l_SplineIds++;
            CornerSpline* l_Spline = m_Splines.GetArrayPtr() + l_SplineId;
            U16 l_SecondPointId;
            if (l_DirectionBits & FL_SPLINE_NORMAL_DIR) {
                l_SecondPointId = l_Spline->m_P1;
            }
            else {
                l_SecondPointId = l_Spline->m_P0;
            }
            Vec3f* l_SecondPoint = &m_Points[l_SecondPointId];

            Vec2f l_SecondTopViewPoint;
            S32 l_SecondPointInFrustum;
            if (m_TestedPointsBA.GetBit(l_SecondPointId) == 0) {
                m_TestedPointsBA.SetBit(l_SecondPointId);
                l_SecondTopViewPoint = Vec2f(
                    l_SecondPoint->x,
                    l_SecondPoint->z
                );
                if (IsPtInFrs(
                        i_BaseCameraFrustum,
                        l_SecondTopViewPoint,
                        l_FrustumEdgeIndex
                    )) {
                    l_SecondPointInFrustum = 1;
                    m_PointsInFrustumBA.SetBit(l_SecondPointId);
                }
                else {
                    l_SecondPointInFrustum = 0;
                }
            }
            else {
                l_SecondPointInFrustum = m_PointsInFrustumBA.GetBit(l_SecondPointId);
            }

            if (m_ProcessedSplinesBA.GetBit(l_SplineId) == 0) {
                if (!(l_ZoneFlags & FL_ZONE_SPLINE_NO_MARK)) {
                    m_ProcessedSplinesBA.SetBit(l_SplineId);
                }

                l_FirstTopViewPoint.Set(
                    l_FirstPoint->x,
                    l_FirstPoint->z
                );
                l_SecondTopViewPoint.Set(
                    l_SecondPoint->x,
                    l_SecondPoint->z
                );

                if (l_FirstPointInFrustum || l_SecondPointInFrustum || IsSegInFrsPtsOut(i_BaseCameraFrustum, l_FirstTopViewPoint, l_SecondTopViewPoint)) {
                    if (l_TypeBits & (FL_SPLINE_LINKED | FL_SPLINE_OCCLUDER)) {
                        if (l_TypeBits & FL_SPLINE_LINKED) {
                            S32 l_LinkedZoneId = l_ZoneLink->m_LinkedZoneId;

                            if (m_VisitedZonesBA.GetBit(l_LinkedZoneId) == 0) {
                                if (l_ZoneStackSize <= ZONE_STACK_SIZE - 1) {
                                    l_ZoneStack[l_ZoneStackSize++] = l_LinkedZoneId;
                                }
                                m_VisitedZonesBA.SetBit(l_LinkedZoneId);
                            }

                            if (!(m_SplineZones[l_LinkedZoneId].m_Flag & FL_ZONE_SPLINE_OCCLUDER)) {
                                if (l_TypeBits & FL_SPLINE_LINKED) {
                                    l_ZoneLink++;
                                }
                                l_DirectionBits >>= 1;
                                l_TypeBits >>= 2;
                                l_FirstPointInFrustum = l_SecondPointInFrustum;
                                l_FirstPoint = l_SecondPoint;
                                continue;
                            }
                        }

                        Vec2f l_SegmentDelta = l_SecondTopViewPoint - l_FirstTopViewPoint;
                        Vec2f l_EndToCamera = i_BaseCameraFrustum.m_Points[0] - l_SecondTopViewPoint;

                        if ((l_EndToCamera ^ l_SegmentDelta) > 0.0f) {
                            Float l_MinHeight = Min(l_FirstPoint->y, l_SecondPoint->y);
                            Float l_HeightDelta = l_MinHeight - i_BaseCameraFrustum.m_WorldPos.y;
                            Vec2f l_MidpointOffset;
                            Vec2f l_SegmentMidpoint = l_FirstTopViewPoint + l_SecondTopViewPoint;
                            l_MidpointOffset = l_SegmentMidpoint * 0.5f - i_BaseCameraFrustum.m_Points[0];
                            Float l_Distance = l_MidpointOffset.GetNorm();

                            if (l_Distance < 1.0f) {
                                l_Distance = 1.0f;
                            }

                            if (l_Distance > 50.0f || l_HeightDelta / l_Distance > 0.1f) {
                                OccludeSeg_Z* l_NextSegment = *io_NextSegment;
                                l_NextSegment->m_Start = l_SecondTopViewPoint;
                                l_NextSegment->m_End = l_FirstTopViewPoint;
                                *io_NextSegment = l_NextSegment + 1;
                            }
                        }
                    }
                }
            }

            if (l_TypeBits & FL_SPLINE_LINKED) {
                l_ZoneLink++;
            }

            l_DirectionBits >>= 1;
            l_TypeBits >>= 2;
            l_FirstPointInFrustum = l_SecondPointInFrustum;
            l_FirstPoint = l_SecondPoint;
        }
    }
}

// TODO: Implement (it's not in rat but it exists in Monopoly MAP)
Bool SegInterSeg(
    Vec2f const& i_SegmentStart,
    Vec2f const& i_SegmentEnd,
    Vec2f const& i_OtherSegmentStart,
    Vec2f const& i_OtherSegmentEnd,
    Vec2f const& i_OtherSegmentDelta,
    Vec2f& o_Intersection
) {
    return FALSE;
}

// TODO: Document (match if needed, it's fully inlined)
Bool Occluder_Z::HalfSegInterSeg(
    const Vec2f& i_HalfSegmentStart,
    const Vec2f& i_HalfSegmentEnd,
    const Vec2f& i_SegmentStart,
    const Vec2f& i_SegmentEnd,
    const Vec2f& i_SegmentDelta,
    Vec2f& o_Intersection
) {
    Vec2f l_HalfSegmentDelta = i_HalfSegmentEnd - i_HalfSegmentStart;
    Float l_SegmentYHalfSegmentX = i_SegmentDelta.y * l_HalfSegmentDelta.x;
    Float l_SegmentXHalfSegmentY = i_SegmentDelta.x * l_HalfSegmentDelta.y;

    if (l_SegmentYHalfSegmentX == 0.0f && l_SegmentXHalfSegmentY == 0.0f) {
        return FALSE;
    }

    Float l_InvDenominator = 1.0f / (l_SegmentYHalfSegmentX - l_SegmentXHalfSegmentY);
    Vec2f l_SegmentStartDelta = i_HalfSegmentStart - i_SegmentStart;
    Float l_HalfSegmentProjection = l_InvDenominator * (i_SegmentDelta ^ l_SegmentStartDelta);

    if (l_HalfSegmentProjection < Float_Eps || l_HalfSegmentProjection > 1.0f - Float_Eps) {
        return FALSE;
    }

    Float l_SegmentProjection = l_InvDenominator * (l_HalfSegmentDelta ^ l_SegmentStartDelta);
    if (l_SegmentProjection < Float_Eps) {
        return FALSE;
    }

    o_Intersection = i_HalfSegmentStart + l_HalfSegmentDelta * l_HalfSegmentProjection;
    return TRUE;
}

// TODO: Document (match if needed, it's fully inlined)
Float Occluder_Z::DistSegFromCam(const OccludeSeg_Z& i_Segment, const Vec2f& i_CameraPosition, const Vec2f& i_Ray) {
    Vec2f l_SegmentDelta = i_Segment.m_End - i_Segment.m_Start;

    Float l_Denominator = i_Ray ^ l_SegmentDelta;

    if (fabsf(l_Denominator) > Float_Eps) {
        return (l_SegmentDelta ^ (i_CameraPosition - i_Segment.m_Start)) / l_Denominator;
    }

    return 10000000000.0f;
}

// TODO: Document (it's not in rat but it exists in Monopoly MAP)
Bool Occluder_Z::IsSegInFrs(
    const OccludedFrustum_Z& i_BaseCameraFrustum,
    const Vec2f& i_SegmentStart,
    const Vec2f& i_SegmentEnd
) {
    S32 l_PointNb = i_BaseCameraFrustum.m_PointNb;
    Vec2f l_SegmentDelta = i_SegmentEnd - i_SegmentStart;
    const Vec2f* l_Point = i_BaseCameraFrustum.m_Points;

    for (; l_PointNb > 1; l_PointNb--) {
        Vec2f l_FrustumSegmentDelta = l_Point[1] - i_BaseCameraFrustum.m_Points[0];
        Float l_Denominator = l_FrustumSegmentDelta ^ l_SegmentDelta;

        if (fabsf(l_Denominator) > Float_Eps) {
            Vec2f l_SegmentStartToCamera = i_BaseCameraFrustum.m_Points[0] - i_SegmentStart;
            Float l_SegmentProjection = (l_FrustumSegmentDelta ^ l_SegmentStartToCamera) / l_Denominator;

            if (l_SegmentProjection >= 0.0f && l_SegmentProjection <= 1.0f) {
                Float l_FrustumProjection = (l_SegmentDelta ^ l_SegmentStartToCamera) / l_Denominator;

                if (l_FrustumProjection <= 1.0f + Float_Eps && l_FrustumProjection >= 0.0f) {
                    return TRUE;
                }
            }
        }

        l_Point++;
    }

    return FALSE;
}

// TODO: Finish matching, document
Bool Occluder_Z::IsSegInFrsPtsOut(
    const OccludedFrustum_Z& i_BaseCameraFrustum,
    const Vec2f& i_SegmentStart,
    const Vec2f& i_SegmentEnd
) {
    S32 l_PointNb = i_BaseCameraFrustum.m_PointNb;
    Vec2f l_SegmentDelta = i_SegmentEnd - i_SegmentStart;
    const Vec2f* l_Point = i_BaseCameraFrustum.m_Points;

    for (; l_PointNb > 1; l_PointNb--) {
        Vec2f l_FrustumSegmentDelta = l_Point[1] - i_BaseCameraFrustum.m_Points[0];
        Float l_Denominator = l_FrustumSegmentDelta ^ l_SegmentDelta;

        if (fabsf(l_Denominator) > Float_Eps) {
            Vec2f l_SegmentStartToCamera = i_BaseCameraFrustum.m_Points[0] - i_SegmentStart;
            Float l_SegmentProjection = (l_FrustumSegmentDelta ^ l_SegmentStartToCamera) / l_Denominator;

            if (l_SegmentProjection >= 0.0f && l_SegmentProjection <= 1.0f) {
                Float l_FrustumProjection = (l_SegmentDelta ^ l_SegmentStartToCamera) / l_Denominator;

                if (l_FrustumProjection <= 1.0f + Float_Eps && l_FrustumProjection >= 0.0f) {
                    return TRUE;
                }
            }
        }

        l_Point++;
    }

    return FALSE;
}

// TODO: Finish matching, document
Bool Occluder_Z::IsPtInFrs(const OccludedFrustum_Z& i_OccludedFrustum, const Vec2f& i_Point, int& o_EdgeIndex) {
    S32 l_PointNb = i_OccludedFrustum.m_PointNb;
    if (l_PointNb == 0) {
        return FALSE;
    }

    const Vec2f& l_CameraPos = i_OccludedFrustum.m_Points[0];
    Vec2f l_CameraToPoint = i_Point - l_CameraPos;

    Float l_ProjectionDenominator = l_CameraToPoint ^ i_OccludedFrustum.m_ProjectionLineDelta;

    o_EdgeIndex = -1;

    if (l_ProjectionDenominator < -Float_Eps) {
        Vec2f l_ProjectionStartToCamera = l_CameraPos - i_OccludedFrustum.m_ProjectionLineStart;

        Float l_ProjectionNumerator = l_CameraToPoint ^ l_ProjectionStartToCamera;

        if (l_ProjectionNumerator > 0.0f || l_ProjectionNumerator < l_ProjectionDenominator) {
            return FALSE;
        }

        Float l_Projection = l_ProjectionNumerator / l_ProjectionDenominator;

        S32 l_EdgeIndex;
        S32 l_LeftEdgeIndex;
        S32 l_RightEdgeIndex;

        l_RightEdgeIndex = l_PointNb - 1;
        l_LeftEdgeIndex = 1;
        l_EdgeIndex = (S32)(l_Projection * (Float)(l_PointNb - 2)) + 1;

        if (l_EdgeIndex == l_RightEdgeIndex) {
            l_EdgeIndex = l_RightEdgeIndex - 1;
        }

        const Float* l_HorizontalProjections = i_OccludedFrustum.m_HorizontalProjection;

        const Vec2f* l_Points = i_OccludedFrustum.m_Points;

        Float l_EdgeProjection;

        for (;;) {
            l_EdgeProjection = l_HorizontalProjections[l_EdgeIndex];

            if (l_EdgeProjection > l_Projection) {
                l_RightEdgeIndex = l_EdgeIndex + 1;
                l_EdgeIndex = (l_LeftEdgeIndex + l_EdgeIndex) >> 1;
                continue;
            }

            if (l_HorizontalProjections[l_EdgeIndex + 1] < l_Projection) {
                l_LeftEdgeIndex = l_EdgeIndex;
                l_EdgeIndex = (l_RightEdgeIndex + l_EdgeIndex) >> 1;
                continue;
            }

            break;
        }

        o_EdgeIndex = l_EdgeIndex;

        const Vec2f& l_EdgeDelta = i_OccludedFrustum.m_EdgeVectors[l_EdgeIndex];

        Float l_RayEdgeCross = l_CameraToPoint ^ l_EdgeDelta;

        Float l_EdgeOffsetCross = l_EdgeDelta ^ (l_Points[0] - l_Points[l_EdgeIndex]);

        if (l_RayEdgeCross < 0.0f) {
            if (l_EdgeOffsetCross <= l_RayEdgeCross) {
                return TRUE;
            }
        }
        else if (l_EdgeOffsetCross >= l_RayEdgeCross) {
            return TRUE;
        }

        return FALSE;
    }

    return FALSE;
}

// TODO: Document
Bool Occluder_Z::GetPosEcran(const OccludedFrustum_Z& i_ProjectionFrustum, const Vec2f& i_Point, Float& o_HorizontalProjection) {
    Vec2f l_CameraToPoint = i_Point - i_ProjectionFrustum.m_Points[0];
    Float l_ProjectionDenominator = l_CameraToPoint ^ i_ProjectionFrustum.m_ProjectionLineDelta;
    Vec2f l_ProjectionStartToCamera = i_ProjectionFrustum.m_Points[0] - i_ProjectionFrustum.m_ProjectionLineStart;
    Float l_ProjectionNumerator = l_CameraToPoint ^ l_ProjectionStartToCamera;

    if (l_ProjectionDenominator < -Float_Eps) {
        o_HorizontalProjection = l_ProjectionNumerator / l_ProjectionDenominator;
        return TRUE;
    }

    return FALSE;
}

// TODO: Finish matching, document
void Occluder_Z::AddPtInFrs(OccludedFrustum_Z& io_OccludedFrustum, const Vec2f& i_Point, Float& io_MaxHorizontalForwardDepth) {
    Float l_ForwardDepth = io_OccludedFrustum.m_CamToTarget * (i_Point - io_OccludedFrustum.m_Points[0]);

    if (l_ForwardDepth > io_MaxHorizontalForwardDepth) {
        io_MaxHorizontalForwardDepth = l_ForwardDepth;
    }

    if (io_OccludedFrustum.m_PointNb <= MAX_OCCLUDED_FRUSTUM_EDGES - 1) {

        if (io_OccludedFrustum.m_PointNb > 1) {
            const Vec2f& l_LastPoint = io_OccludedFrustum.m_Points[io_OccludedFrustum.m_PointNb - 1];

            if ((l_LastPoint - i_Point).GetNorm2() < Float_Eps_2) {
                return;
            }
        }

        S32 l_NewPointIndex = io_OccludedFrustum.m_PointNb++;
        Vec2f* l_NewPoint = &io_OccludedFrustum.m_Points[l_NewPointIndex];
        *l_NewPoint = i_Point;

        S32 l_PointNb = io_OccludedFrustum.m_PointNb;
        if (l_PointNb > 2) {
            Vec2f& l_PreviousPoint = io_OccludedFrustum.m_Points[l_PointNb - 2];
            Vec2f& l_LastPoint = io_OccludedFrustum.m_Points[l_PointNb - 1];
            Vec2f& l_PreviousPreviousPoint = io_OccludedFrustum.m_Points[l_PointNb - 3];
            Vec2f l_LastSegment = l_LastPoint - l_PreviousPoint;
            Vec2f l_PreviousSegment = l_PreviousPoint - l_PreviousPreviousPoint;

            l_LastSegment.Normalize();
            l_PreviousSegment.Normalize();

            if ((l_LastSegment * l_PreviousSegment) > 0.99f) {
                l_PreviousPoint = l_LastPoint;
                io_OccludedFrustum.m_PointNb--;
            }
        }
    }
}

// TODO: Finish matching, document
void Fast_QSort(int i_Count, OccludeSeg_Z** io_SegmentPointers) {
    S32 l_Stack[FAST_QSORT_STACK_SIZE];
    S32 l_StackSize = 2;
    l_Stack[0] = 0;
    l_Stack[1] = i_Count - 1;

    while (l_StackSize != 0) {
        S32 l_Left = l_Stack[l_StackSize - 2];
        S32 l_Right = l_Stack[l_StackSize - 1];
        l_StackSize -= 2;

        if (l_Right - l_Left < SPLINE_NBSEGMENT - 1) {
            l_Left++;
            OccludeSeg_Z** l_Current = io_SegmentPointers + l_Left;

            while (l_Left <= l_Right) {
                S32 l_InsertIndex = l_Left - 1;
                OccludeSeg_Z* l_Segment = *l_Current;
                Float l_Projection = l_Segment->m_ProjectionStart;

                for (; l_InsertIndex >= 0; l_InsertIndex--) {
                    if (io_SegmentPointers[l_InsertIndex]->m_ProjectionStart <= l_Projection) {
                        break;
                    }

                    io_SegmentPointers[l_InsertIndex + 1] = io_SegmentPointers[l_InsertIndex];
                }

                io_SegmentPointers[l_InsertIndex + 1] = l_Segment;
                l_Left++;
                l_Current++;
            }
        }
        else {
            S32 l_PivotIndex = l_Right;
            S32 l_LeftIndex = l_Left;
            S32 l_RightIndex = l_Right - 1;
            Float l_Pivot = io_SegmentPointers[l_PivotIndex]->m_ProjectionStart;

            for (;;) {
                while (io_SegmentPointers[l_LeftIndex]->m_ProjectionStart < l_Pivot) {
                    l_LeftIndex++;
                }

                while (l_LeftIndex < l_RightIndex && io_SegmentPointers[l_RightIndex]->m_ProjectionStart >= l_Pivot) {
                    l_RightIndex--;
                }

                if (l_LeftIndex >= l_RightIndex || l_RightIndex < 0) {
                    break;
                }

                OccludeSeg_Z* l_Temp = io_SegmentPointers[l_LeftIndex];
                io_SegmentPointers[l_LeftIndex] = io_SegmentPointers[l_RightIndex];
                io_SegmentPointers[l_RightIndex] = l_Temp;
            }

            if (l_LeftIndex != l_PivotIndex) {
                OccludeSeg_Z* l_Temp = io_SegmentPointers[l_LeftIndex];
                io_SegmentPointers[l_LeftIndex] = io_SegmentPointers[l_PivotIndex];
                io_SegmentPointers[l_PivotIndex] = l_Temp;
            }

            if (l_Left < l_LeftIndex - 1) {
                l_Stack[l_StackSize++] = l_Left;
                l_Stack[l_StackSize++] = l_LeftIndex - 1;
            }

            if (l_LeftIndex + 1 < l_Right) {
                l_Stack[l_StackSize++] = l_LeftIndex + 1;
                l_Stack[l_StackSize++] = l_Right;
            }
        }
    }
}

// TODO: Finish matching, document
Float Occluder_Z::GetOccludedFrustrum(
    OccludedFrustum_Z& o_OccludedFrustum,
    const OccludedFrustum_Z& i_BaseCameraFrustum,
    OccludeSeg_Z* io_Segments,
    S32 i_SegmentCount
) {
    Float l_MaxHorizontalForwardDepth = 0.0f;
    const Vec2f l_CameraPosition = i_BaseCameraFrustum.m_Points[0];

    Vec2f l_RightDirection = i_BaseCameraFrustum.m_Points[1] - l_CameraPosition;
    l_RightDirection.Normalize();

    Vec2f l_LeftDirection = i_BaseCameraFrustum.m_Points[i_BaseCameraFrustum.m_PointNb - 1] - l_CameraPosition;
    l_LeftDirection.Normalize();

    o_OccludedFrustum.m_CamToTarget = i_BaseCameraFrustum.m_CamToTarget;
    o_OccludedFrustum.m_CamToTarget.Normalize();
    o_OccludedFrustum.m_PointNb = 0;
    o_OccludedFrustum.m_Points[o_OccludedFrustum.m_PointNb++] = l_CameraPosition;
    o_OccludedFrustum.m_ProjectionLineStart = l_CameraPosition + l_RightDirection;
    const Vec2f l_ProjectionLineEnd = l_CameraPosition + l_LeftDirection;
    o_OccludedFrustum.m_ProjectionLineDelta = l_ProjectionLineEnd - o_OccludedFrustum.m_ProjectionLineStart;

    S32 l_BasePointNb = i_BaseCameraFrustum.m_PointNb;
    for (S32 l_EdgeIndex = 1; l_EdgeIndex < l_BasePointNb - 1; l_EdgeIndex++) {
        const Vec2f& l_EdgeStart = i_BaseCameraFrustum.m_Points[l_EdgeIndex];
        Vec2f l_EdgeDelta = i_BaseCameraFrustum.m_Points[l_EdgeIndex + 1] - l_EdgeStart;

        OccludeSeg_Z* l_Segment = io_Segments;
        for (S32 l_SegmentIndex = 0; l_SegmentIndex < i_SegmentCount; l_SegmentIndex++, l_Segment++) {
            Vec2f l_SegmentDelta = l_Segment->m_End - l_Segment->m_Start;
            Float l_EdgeYSegmentX = l_EdgeDelta.y * l_SegmentDelta.x;
            Float l_EdgeXSegmentY = l_EdgeDelta.x * l_SegmentDelta.y;

            if (l_EdgeYSegmentX != 0.0f || l_EdgeXSegmentY != 0.0f) {
                Float l_InvDenominator = 1.0f / (l_EdgeYSegmentX - l_EdgeXSegmentY);
                Vec2f l_EdgeStartToSegmentStart = l_Segment->m_Start - l_EdgeStart;
                Float l_SegmentProjection = l_InvDenominator * (l_EdgeDelta ^ l_EdgeStartToSegmentStart);

                if (l_SegmentProjection < 0.0f || l_SegmentProjection > 1.0f) {
                    continue;
                }

                Float l_EdgeProjection = l_InvDenominator * (l_SegmentDelta ^ l_EdgeStartToSegmentStart);

                if (l_EdgeProjection < 0.0f || l_EdgeProjection > 1.0f) {
                    continue;
                }

                Vec2f l_Intersection = l_Segment->m_Start + l_SegmentDelta * l_SegmentProjection;
                int l_FrustumEdge;
                if (IsPtInFrs(i_BaseCameraFrustum, l_Segment->m_Start, l_FrustumEdge)) {
                    l_Segment->m_End = l_Intersection;
                }
                else {
                    l_Segment->m_Start = l_Intersection;
                }
            }
        }
    }

    OccludeSeg_Z* l_NewSegment = io_Segments + i_SegmentCount;
    for (S32 l_BasePointIndex = 1; l_BasePointIndex < i_BaseCameraFrustum.m_PointNb - 1; l_BasePointIndex++) {
        l_NewSegment->m_Start = i_BaseCameraFrustum.m_Points[l_BasePointIndex];
        l_NewSegment->m_End = i_BaseCameraFrustum.m_Points[l_BasePointIndex + 1];
        l_NewSegment++;
        i_SegmentCount++;
    }

    Float l_StartProjection;
    Float l_EndProjection;
    OccludeSeg_Z* l_Segment = io_Segments;
    S32 l_LastSegmentIndex = i_SegmentCount - 1;
    S32 l_ValidSegmentCount = 0;
    OccludeSeg_Z* l_SortedSegments[MAX_OCCLUDED_SEGMENTS];

    for (S32 l_SegmentIndex = 0; l_SegmentIndex < i_SegmentCount; l_SegmentIndex++, l_Segment++) {
        if (!GetPosEcran(
                o_OccludedFrustum,
                l_Segment->m_Start,
                l_StartProjection
            )) {
            Vec2f l_Intersection;
            Bool l_IntersectsFirstEdge = HalfSegInterSeg(
                l_Segment->m_Start,
                l_Segment->m_End,
                i_BaseCameraFrustum.m_Points[0],
                i_BaseCameraFrustum.m_Points[1],
                i_BaseCameraFrustum.m_EdgeVectors[0],
                l_Intersection
            );

            l_StartProjection = l_IntersectsFirstEdge ? 0.0f : 1.0f;
        }

        if (!GetPosEcran(
                o_OccludedFrustum,
                l_Segment->m_End,
                l_EndProjection
            )) {
            Vec2f l_Intersection;
            Bool l_IntersectsFirstEdge = HalfSegInterSeg(
                l_Segment->m_Start,
                l_Segment->m_End,
                i_BaseCameraFrustum.m_Points[0],
                i_BaseCameraFrustum.m_Points[1],
                i_BaseCameraFrustum.m_EdgeVectors[0],
                l_Intersection
            );

            l_EndProjection = l_IntersectsFirstEdge ? 0.0f : 1.0f;
        }

        if (l_SegmentIndex == i_SegmentCount - (i_BaseCameraFrustum.m_PointNb - 2)) {
            l_StartProjection = 0.0f;
        }
        if (l_SegmentIndex == l_LastSegmentIndex) {
            l_EndProjection = 1.0f;
        }

        if (l_StartProjection < l_EndProjection) {
            l_Segment->m_ProjectionStart = l_StartProjection;
            l_Segment->m_ProjectionEnd = l_EndProjection;
        }
        else {
            l_Segment->m_ProjectionStart = l_EndProjection;
            l_Segment->m_ProjectionEnd = l_StartProjection;
        }

        if (l_StartProjection != l_EndProjection) {
            l_SortedSegments[l_ValidSegmentCount++] = l_Segment;
        }
    }

    Fast_QSort(l_ValidSegmentCount, l_SortedSegments);

    Float l_FurthestProjection = 0.0f;
    for (OccludeSeg_Z** l_CurrentPointer = l_SortedSegments; l_CurrentPointer - l_SortedSegments < l_ValidSegmentCount; l_CurrentPointer++) {
        OccludeSeg_Z* l_Current = *l_CurrentPointer;
        if (l_Current->m_ProjectionStart > l_FurthestProjection) {
            l_Current->m_ProjectionStart = l_FurthestProjection;
        }
        if (l_Current->m_ProjectionEnd > l_FurthestProjection) {
            l_FurthestProjection = l_Current->m_ProjectionEnd;
        }
    }

    Float l_CurrentProjection = 0.0f;
    OccludeSeg_Z* l_PreviousSegment = NULL;
    l_StartProjection = -1.0f;
    l_EndProjection = 1.0f;
    Char l_DebugText[256];

    while (l_CurrentProjection < l_EndProjection) {
        if (l_StartProjection == l_CurrentProjection) {
            for (OccludeSeg_Z** l_DebugSegment = l_SortedSegments; l_DebugSegment - l_SortedSegments < l_ValidSegmentCount; l_DebugSegment++) {
                if ((*l_DebugSegment)->m_ProjectionEnd == l_CurrentProjection || (*l_DebugSegment)->m_ProjectionStart == l_CurrentProjection) {
                    sprintf(l_DebugText, "pt0 x:%f z:%f", (*l_DebugSegment)->m_Start.x, (*l_DebugSegment)->m_Start.y);
                    sprintf(l_DebugText, "pt1 x:%f z:%f", (*l_DebugSegment)->m_End.x, (*l_DebugSegment)->m_End.y);
                }
            }
            o_OccludedFrustum = i_BaseCameraFrustum;
            break;
        }

        l_StartProjection = l_CurrentProjection;

        Vec2f l_Ray = o_OccludedFrustum.m_ProjectionLineStart + o_OccludedFrustum.m_ProjectionLineDelta * l_CurrentProjection;
        l_Ray -= l_CameraPosition;
        l_Ray.Normalize();

        Float l_NearestDistance = 10000000000.0f;
        OccludeSeg_Z* l_SelectedSegment = NULL;
        OccludeSeg_Z** l_SelectedPointer;

        for (OccludeSeg_Z** l_CurrentPointer = l_SortedSegments; l_CurrentPointer - l_SortedSegments < l_ValidSegmentCount && (*l_CurrentPointer)->m_ProjectionStart <= l_CurrentProjection; l_CurrentPointer++) {
            OccludeSeg_Z* l_Current = *l_CurrentPointer;

            if (l_Current->m_ProjectionEnd > l_CurrentProjection && l_PreviousSegment != l_Current) {
                Float l_Distance = DistSegFromCam(*l_Current, l_CameraPosition, l_Ray);

                if (l_Distance < l_NearestDistance) {
                    l_SelectedSegment = l_Current;
                    l_SelectedPointer = l_CurrentPointer;
                    l_NearestDistance = l_Distance;
                }
            }
        }

        Vec2f l_Intersection = l_CameraPosition + l_Ray * l_NearestDistance;
        AddPtInFrs(o_OccludedFrustum, l_Intersection, l_MaxHorizontalForwardDepth);

        if (l_SelectedSegment != NULL) {
            Float l_SelectedEnd = l_SelectedSegment->m_ProjectionEnd;
            OccludeSeg_Z* l_NextSegment = NULL;
            Float l_SelectedDistance = 10000000000.0f;
            OccludeSeg_Z** l_NextPointer = l_SelectedPointer;

            while (++l_NextPointer - l_SortedSegments < l_ValidSegmentCount && (*l_NextPointer)->m_ProjectionStart <= l_SelectedEnd) {
                OccludeSeg_Z* l_Candidate = *l_NextPointer;
                if (l_Candidate->m_ProjectionStart >= l_CurrentProjection) {
                    Float l_CandidateProjection = l_Candidate->m_ProjectionStart;
                    Vec2f l_CandidateRay = o_OccludedFrustum.m_ProjectionLineStart + o_OccludedFrustum.m_ProjectionLineDelta * l_CandidateProjection;
                    l_CandidateRay -= l_CameraPosition;
                    l_CandidateRay.Normalize();

                    l_SelectedDistance = DistSegFromCam(
                        *l_SelectedSegment,
                        l_CameraPosition,
                        l_CandidateRay
                    );

                    Float l_CandidateDistance = DistSegFromCam(
                        *l_Candidate,
                        l_CameraPosition,
                        l_CandidateRay
                    );

                    if (l_CandidateDistance < l_SelectedDistance) {
                        l_NextSegment = l_Candidate;
                        break;
                    }
                }
            }

            l_PreviousSegment = l_SelectedSegment;

            if (l_NextSegment != NULL) {
                l_Intersection = l_CameraPosition + l_Ray * l_SelectedDistance;
                AddPtInFrs(o_OccludedFrustum, l_Intersection, l_MaxHorizontalForwardDepth);
                l_CurrentProjection = l_NextSegment->m_ProjectionStart;
            }
            else {
                l_CurrentProjection = l_SelectedSegment->m_ProjectionEnd;
                if (l_CurrentProjection > l_EndProjection) {
                    l_CurrentProjection = l_EndProjection;
                }

                Vec2f l_EndRay = o_OccludedFrustum.m_ProjectionLineStart + o_OccludedFrustum.m_ProjectionLineDelta * l_CurrentProjection;
                l_EndRay -= l_CameraPosition;
                l_EndRay.Normalize();

                Float l_Distance = DistSegFromCam(
                    *l_SelectedSegment,
                    l_CameraPosition,
                    l_EndRay
                );

                l_Intersection = l_CameraPosition + l_EndRay * l_Distance;
                AddPtInFrs(o_OccludedFrustum, l_Intersection, l_MaxHorizontalForwardDepth);
            }
        }
        else {
            o_OccludedFrustum = i_BaseCameraFrustum;
            break;
        }
    }

    o_OccludedFrustum.m_Points[o_OccludedFrustum.m_PointNb] = l_CameraPosition;
    o_OccludedFrustum.m_HorizontalProjection[1] = 0.0f;
    o_OccludedFrustum.m_HorizontalProjection[o_OccludedFrustum.m_PointNb - 1] = l_EndProjection;

    for (S32 l_PointIndex = 2; l_PointIndex < o_OccludedFrustum.m_PointNb - 1; l_PointIndex++) {
        Vec2f l_CameraToPoint = o_OccludedFrustum.m_Points[l_PointIndex] - o_OccludedFrustum.m_Points[0];
        Vec2f l_ProjectionStartToCamera = o_OccludedFrustum.m_Points[0] - o_OccludedFrustum.m_ProjectionLineStart;
        o_OccludedFrustum.m_HorizontalProjection[l_PointIndex] = (l_CameraToPoint ^ l_ProjectionStartToCamera) / (l_CameraToPoint ^ o_OccludedFrustum.m_ProjectionLineDelta);
    }

    o_OccludedFrustum.DoVec();

    return 1.0f + l_MaxHorizontalForwardDepth;
}

Bool Occluder_Z::IsOccludedRect(const OccludedFrustum_Z& i_OccludedFrustum, const Vec2f& i_RectMin, const Vec2f& i_RectMax) {
    // TODO: Match
    return FALSE;
}

Bool Occluder_Z::IsOccludedSphere(const OccludedFrustum_Z& i_OccludedFrustum, const Sphere_Z& i_Sphere) {
    // TODO: Match
    return FALSE;
}
