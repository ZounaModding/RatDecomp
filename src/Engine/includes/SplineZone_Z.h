#ifndef _SPLINEZONE_Z_H_
#define _SPLINEZONE_Z_H_
#include "Object_Z.h"
#include "SplineZoneSead_Z.h"

#define MAX_SPLINES_ON_ZONE 7

class Bitmap_Z;

struct CornerSpline {
    U16 m_P0; // Index of point 0
    U16 m_P1; // Index of point 1
};

// Direction flags
#define FL_SPLINE_NORMAL_DIR (U16)(1 << 0) // Traverse the spline in the direction from m_P0 to m_P1

// Type flags
#define FL_SPLINE_LINKED (U16)(1 << 0)   // The spline links to another SplineZone and consumes the next SplineZoneLink entry
#define FL_SPLINE_OCCLUDER (U16)(1 << 1) // The spline is an unlinked occluder candidate

// Zone flags
#define FL_ZONE_SPLINE_NO_MARK (U16)(1 << 0)   // The zone's splines are not marked as processed in the spline bit array, allowing them to be processed again from another zone/side
#define FL_ZONE_SPLINE_OCCLUDER (U16)(1 << 1)  // A spline leading into this zone may also be processed as an occluding segment
#define FL_ZONE_SPLINE_NO_EXPAND (U16)(1 << 2) // Tthe zone is not expanded by OccludeTrack

struct SplineZone {
    Float m_Y;                 // Average Y position of the zone
    S16 m_SplineIdsNb;         // Number of entries belonging to this zone in m_SplineIndices
    U16 m_SplineIdsIdx;        // Index of the first entry in m_SplineIndices
    U16 m_SplineZoneLinkIdx;   // Index of this zone's first entry in m_SplineZoneLinks
    U16 m_SplineDirectionBits; // See direction flags
    U16 m_SplineTypeBits;      // See type flags
    U16 m_Flag;                // See zone flags
};

struct SplineZoneLink {
    Vec2f m_PortalPosition; // Top-down XZ waypoint associated with the connection between two zones
    U16 m_LinkedZoneId;     // Index of the zone reached through this boundary
};

typedef DynArray_Z<CornerSpline, 32, FALSE, FALSE> CornerSplineDA;
typedef DynArray_Z<SplineZone, 32, FALSE, TRUE> SplineZoneDA;
typedef DynArray_Z<SplineZoneLink, 32, FALSE, FALSE> SplineZoneLinkDA;

class SplineZone_Z : public Object_Z {
public:
    SplineZone_Z();

    virtual ~SplineZone_Z() { }

    virtual void Load(void** i_Data);
    virtual void Clean();
    virtual void Draw(DrawInfo_Z& i_DrawInfo, ObjectDatas_Z* i_Data);
    virtual void Reset();
    virtual void DrawBitmap(Bitmap_Z* i_Bitmap, const Sphere_Z& i_ClipSphere, const Vec2f& i_Pos);

    Bool GetZone(const Vec3f& i_CamPos, S32& o_ZoneId);

    S32 GetNbSplines() const { return m_Splines.GetSize(); }

    S32 GetNbZone() const { return m_SplineZones.GetSize(); }

    static BaseObject_Z* NewObject() { return NewL_Z(101) SplineZone_Z; }

protected:
    Vec4f m_UnkVec4f_0x80;
    Vec3fDA m_Points;
    CornerSplineDA m_Splines;
    SplineZoneDA m_SplineZones;
    SplineZoneSead_Z m_SplineZoneSead;
    U16DA m_SplineIndices; // Indices into m_Splines (runs for each zone)
    SplineZoneLinkDA m_SplineZoneLinks;
};

#endif // _SPLINEZONE_Z_H_
