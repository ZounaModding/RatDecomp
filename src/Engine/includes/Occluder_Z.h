#ifndef _OCCLUDER_Z_H_
#define _OCCLUDER_Z_H_
#include "SplineZone_Z.h"
#include "Camera_ZHdl.h"

#define MAX_OCCLUDED_SEGMENTS 512
#define ZONE_STACK_SIZE 1024
#define FAST_QSORT_STACK_SIZE 52

class Camera_Z;
class OccludedFrustum_Z;

struct OccludeSeg_Z {
    OccludeSeg_Z() { }

    Vec2f m_Start;           // Top down start point of the segment
    Vec2f m_End;             // Top down end point of the segment
    Float m_ProjectionStart; // 0-1 float from left to right in screen space (for start point)
    Float m_ProjectionEnd;   // 0-1 float from left to right in screen space (for end point)
};

// From Monopoly MAP
struct ZoneFace_Z {
    // $SABE: Fake name
    struct ZoneFaceInternal {
        ~ZoneFaceInternal() {
            if (m_UnkPtr_0x4) {
                Free_Z(m_UnkPtr_0x4);
                m_UnkPtr_0x4 = NULL;
            }
        }

        U32 m_Unk_0x0;
        U8* m_UnkPtr_0x4;
    };

    ZoneFaceInternal m_UnkPtr_0x0;
};

typedef DynArray_Z<ZoneFace_Z, 32, TRUE, TRUE, 16> ZoneFace_ZDA;

class Occluder_Z : public SplineZone_Z {
public:
    virtual void Load(void** i_Data);

    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data) {
        DrawZone(i_DrawInfo);
        DrawPath(i_DrawInfo);
    }

    static BaseObject_Z* NewObject() { return NewL_Z(145) Occluder_Z; }

    void InitBitArrays();

    Float Occlusion(const Camera_ZHdl& i_CameraHdl, OccludedFrustum_Z& o_OccludedFrustum, BitArray_Z& o_OccludedZones);
    Float Occlusion(
        OccludedFrustum_Z& o_OccludedFrustum,
        BitArray_Z& io_OccludedZones,
        const OccludedFrustum_Z& i_BaseCameraFrustum,
        S32 i_StartZoneId,
        const Vec3f& i_CameraDirection
    );

    Float GetOccludedFrustrum(
        OccludedFrustum_Z& o_OccludedFrustum,
        const OccludedFrustum_Z& i_BaseCameraFrustum,
        OccludeSeg_Z* io_Segments,
        S32 i_SegmentCount
    );
    void GetFrustrumOccludeSegment(const OccludedFrustum_Z& i_BaseCameraFrustum, S32 i_StartZoneId, OccludeSeg_Z** io_NextSegment);
    Bool GetPosEcran(const OccludedFrustum_Z& i_ProjectionFrustum, const Vec2f& i_Point, Float& o_HorizontalProjection);

    Bool IsZoneInFrustrum(const OccludedFrustum_Z& i_OccludedFrustum, S32 i_ZoneId);
    Bool IsSegInFrs(const OccludedFrustum_Z& i_BaseCameraFrustum, const Vec2f& i_SegmentStart, const Vec2f& i_SegmentEnd);
    Bool IsSegInFrsPtsOut(const OccludedFrustum_Z& i_BaseCameraFrustum, const Vec2f& i_SegmentStart, const Vec2f& i_SegmentEnd);
    static Bool IsPtInFrs(const OccludedFrustum_Z& i_OccludedFrustum, const Vec2f& i_Point, int& o_EdgeIndex);
    void AddPtInFrs(OccludedFrustum_Z& io_OccludedFrustum, const Vec2f& i_Point, Float& io_MaxHorizontalForwardDepth);

    static Bool IsOccludedRect(const OccludedFrustum_Z& i_OccludedFrustum, const Vec2f& i_RectMin, const Vec2f& i_RectMax);
    static Bool IsOccludedSphere(const OccludedFrustum_Z& i_OccludedFrustum, const Sphere_Z& i_Sphere);

    void DrawZone(DrawInfo_Z& i_DrawInfo);
    void DrawPath(DrawInfo_Z& i_DrawInfo);

    Bool SegInterSeg(
        Vec2f const& i_SegmentStart,
        Vec2f const& i_SegmentEnd,
        Vec2f const& i_OtherSegmentStart,
        Vec2f const& i_OtherSegmentEnd,
        Vec2f const& i_OtherSegmentDelta,
        Vec2f& o_Intersection
    );
    Bool HalfSegInterSeg(
        const Vec2f& i_HalfSegmentStart,
        const Vec2f& i_HalfSegmentEnd,
        const Vec2f& i_SegmentStart,
        const Vec2f& i_SegmentEnd,
        const Vec2f& i_SegmentDelta,
        Vec2f& o_Intersection
    );
    Float DistSegFromCam(
        const OccludeSeg_Z& i_Segment,
        const Vec2f& i_CameraPosition,
        const Vec2f& i_Ray
    );

private:
    U32DA m_NodeOccluderZoneIndices; // Indices of the occluder zones that each node in the level is in
    BitArray_Z m_VisitedZonesBA;
    BitArray_Z m_ProcessedSplinesBA;
    ZoneFace_ZDA m_ZoneFaces; // $SABE: Unused (presumably used in the MASTERBF build config)
    BitArray_Z m_TestedPointsBA;
    BitArray_Z m_PointsInFrustumBA;
    Float m_OccludedFarClip; // Max length amongst all occluded frustum points projected onto the camera direction vector
};

#endif // _OCCLUDER_Z_H_
